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


/* ── Busca de bloco ───────────────────────────────────────────────────────── */

/*
 * Procura primeiro na L2 Se não está na L2: conta miss, busca na RAM, preenche L2.
 * Antes de ocupar o slot L1, chama l1_to_l2 (write-back).
 */
static void fetch_block_to_l1(MemorySystem *sys, uint32_t address, uint32_t l1_index, uint32_t l1_tag) {
    uint32_t l2_index = get_index(address, L2_SETS);
    uint32_t l2_tag   = get_tag(address, L2_SETS);

    if (sys->l2[l2_index].valid && sys->l2[l2_index].tag == l2_tag) {
        /* ── L2 HIT ── */
        sys->stats_L2.hits++;
    } else {
        /* ── L2 MISS ── busca na RAM */
        sys->stats_L2.misses++;

        sys->l2[l2_index].valid = true;
        sys->l2[l2_index].tag   = l2_tag;
        sys->l2[l2_index].dirty = false;

        uint32_t base = address & ~((uint32_t)(WORDS_PER_LINE - 1));
        for (int i = 0; i < WORDS_PER_LINE; i++)
            sys->l2[l2_index].words[i] =
                (base + i < RAM_SIZE) ? sys->ram[base + i] : 0;
    }

    sys->l1[l1_index].valid = true;
    sys->l1[l1_index].tag   = l1_tag;
    sys->l1[l1_index].dirty = false;
    for (int i = 0; i < WORDS_PER_LINE; i++)
        sys->l1[l1_index].words[i] = sys->l2[l2_index].words[i];
}

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
