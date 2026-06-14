/*
 * Cache.c — Implementação do simulador de hierarquia de memória
 *
 * Fluxo de um acesso:
 *   1. access_cache() decompõe o endereço e verifica a L1.
 *   2. L1-hit  → lê/escreve diretamente.
 *   3. L1-miss → fetch_block_to_l1():
 *       a. Verifica L2. L2-hit: copia bloco para L1.
 *       b. L2-miss: busca bloco na RAM → preenche L2 → preenche L1.
 *       c. Antes de sobrescrever um slot L1 sujo → l1_to_l2().
 *       d. Antes de sobrescrever um slot L2 sujo → l2_to_ram().
 */

#include "Cache.h"
#include <string.h>

/* ── Funções auxiliares de decomposição de endereço ──────────────────────── */

/* Retorna os OFFSET_BITS menos significativos (palavra dentro do bloco) */
static uint32_t get_offset(uint32_t address) {
    return address & ((1 << OFFSET_BITS) - 1);
}

/* Retorna os bits de índice para um cache com 'num_sets' conjuntos */
static uint32_t get_index(uint32_t address, int num_sets) {
    return (address >> OFFSET_BITS) & (num_sets - 1);
}

/* Retorna a tag para um cache com 'num_sets' conjuntos.
 * Calcula dinamicamente quantos bits de índice são necessários (log2). */
static uint32_t get_tag(uint32_t address, int num_sets) {
    int index_bits = 0;
    int temp = num_sets;
    while (temp >>= 1) index_bits++;
    return address >> (OFFSET_BITS + index_bits);
}

/* ── Funções de transferência entre níveis de cache ─────────────────────────── */

/*
 * Copia os dados de volta para a RAM.
 */
static void l2_to_ram(MemorySystem *sys, uint32_t l2_index) {
    if (!sys->l2[l2_index].valid || !sys->l2[l2_index].dirty) return;

    uint32_t base_address =
        (sys->l2[l2_index].tag << (OFFSET_BITS + 6)) | (l2_index << OFFSET_BITS);

    for (int i = 0; i < WORDS_PER_LINE; i++) {
        if (base_address + i < RAM_SIZE)
            sys->ram[base_address + i] = sys->l2[l2_index].words[i];
    }
    sys->l2[l2_index].dirty = false;
}

/*
 * Copia o bloco da L1 para a L2 
 */
static void l1_to_l2(MemorySystem *sys, uint32_t l1_index) {
    if (!sys->l1[l1_index].valid || !sys->l1[l1_index].dirty) return;

    uint32_t old_address =
        (sys->l1[l1_index].tag << (OFFSET_BITS + 4)) | (l1_index << OFFSET_BITS);

    uint32_t l2_index = get_index(old_address, L2_SETS);
    uint32_t l2_tag   = get_tag(old_address, L2_SETS);

    /* Conflito na L2? Despeja antes de sobrescrever */
    if (sys->l2[l2_index].valid && sys->l2[l2_index].tag != l2_tag)
        l2_to_ram(sys, l2_index);

    sys->l2[l2_index].valid = true;
    sys->l2[l2_index].dirty = true;  /* herda o dirty da L1 */
    sys->l2[l2_index].tag   = l2_tag;
    for (int i = 0; i < WORDS_PER_LINE; i++)
        sys->l2[l2_index].words[i] = sys->l1[l1_index].words[i];
}

/* ── Busca de bloco ───────────────────────────────────────────────────────── */

/*
 * Procura primeiro na L2 Se não está na L2: conta miss, busca na RAM, preenche L2.
 * Antes de ocupar o slot L1, chama l1_to_l2 (write-back).
 */
static void fetch_block_to_l1(MemorySystem *sys, uint32_t address,
                               uint32_t l1_index, uint32_t l1_tag) {
    uint32_t l2_index = get_index(address, L2_SETS);
    uint32_t l2_tag   = get_tag(address, L2_SETS);

    if (sys->l2[l2_index].valid && sys->l2[l2_index].tag == l2_tag) {
        /* ── L2 HIT ── */
        sys->stats_L2.hits++;
    } else {
        /* ── L2 MISS ── busca na RAM */
        sys->stats_L2.misses++;

        l2_to_ram(sys, l2_index);          /* libera slot L2 se sujo  */

        sys->l2[l2_index].valid = true;
        sys->l2[l2_index].tag   = l2_tag;
        sys->l2[l2_index].dirty = false;

        uint32_t base = address & ~((uint32_t)(WORDS_PER_LINE - 1));
        for (int i = 0; i < WORDS_PER_LINE; i++)
            sys->l2[l2_index].words[i] =
                (base + i < RAM_SIZE) ? sys->ram[base + i] : 0;
    }

    /* Libera slot L1 (write-back se sujo) e instala o novo bloco */
    l1_to_l2(sys, l1_index);

    sys->l1[l1_index].valid = true;
    sys->l1[l1_index].tag   = l1_tag;
    sys->l1[l1_index].dirty = false;
    for (int i = 0; i < WORDS_PER_LINE; i++)
        sys->l1[l1_index].words[i] = sys->l2[l2_index].words[i];
}

/* ── Núcleo do acesso ─────────────────────────────────────────────────────── */

/*
 * Acessa a cache iniciando o processo de busca.
 */
static bool access_cache(MemorySystem *sys, uint32_t address, int *data, bool is_write) {
    uint32_t offset   = get_offset(address);
    uint32_t l1_index = get_index(address, L1_SETS);
    uint32_t l1_tag   = get_tag(address, L1_SETS);
    bool hit = true;

    if (!(sys->l1[l1_index].valid && sys->l1[l1_index].tag == l1_tag)) {
        sys->stats_L1.misses++;
        fetch_block_to_l1(sys, address, l1_index, l1_tag);
        hit = false;
    } else {
        sys->stats_L1.hits++;
    }

    if (is_write) {
        sys->l1[l1_index].words[offset] = *data;
        sys->l1[l1_index].dirty = true;
    } else {
        *data = sys->l1[l1_index].words[offset];
    }
    return hit;
}

/* ── Funções Pública ──────────────────────────────────────────────────────────── */

void init_memory(MemorySystem *sys) {
    memset(sys, 0, sizeof(MemorySystem));
}

bool read_memory(MemorySystem *sys, uint32_t address, int *data) {
    return access_cache(sys, address, data, false);
}

bool write_memory(MemorySystem *sys, uint32_t address, int data) {
    return access_cache(sys, address, &data, true);
}

/* Zera apenas os contadores */
void reset_stats(MemorySystem *sys) {
    sys->stats_L1.hits   = 0;
    sys->stats_L1.misses = 0;
    sys->stats_L2.hits   = 0;
    sys->stats_L2.misses = 0;
}

/*
 * warmup_cache lê um intervalo de endereços para pré-carregar a cache sem contar nos stats.
 */
void warmup_cache(MemorySystem *sys, uint32_t start_addr, uint32_t size) {
    int dummy;
    for (uint32_t i = 0; i < size; i++)
        read_memory(sys, start_addr + i, &dummy);
    reset_stats(sys);   /* descarta contagens do aquecimento */
}