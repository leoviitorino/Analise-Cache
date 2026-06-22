#ifndef CACHE_H
#define CACHE_H

#include <stdint.h>
#include <stdbool.h>

#define L1_SETS       16    // Número de conjuntos na L1                      
#define L2_SETS       64    // Número de conjuntos na L2                      
#define WORDS_PER_LINE 4    // Palavras (int) por bloco de cache              
#define OFFSET_BITS    2    // log2(WORDS_PER_LINE) = log2(4) = 2             
#define RAM_SIZE    8192    // Capacidade da RAM em inteiros                  


// Representa uma linha (bloco) de cache
typedef struct {
    bool     valid;               
    bool     dirty;              
    uint32_t tag;                 
    int      words[WORDS_PER_LINE];
} CacheLine;

// Contadores de desempenho para um nível de cache 
typedef struct {
    int hits;
    int misses;
} CacheStats;

// Estado completo do sistema de memória 
typedef struct {
    CacheLine  l1[L1_SETS];   
    CacheLine  l2[L2_SETS];   
    CacheStats stats_L1;      
    CacheStats stats_L2;      
    int        ram[RAM_SIZE]; 
} MemorySystem;

/* ── API Pública ──────────────────────────────────────────────────────────── */

/* Inicializa (zera) todo o sistema de memória */
void init_memory(MemorySystem *sys);

/* Lê 'address'; retorna true em L1-hit */
bool read_memory(MemorySystem *sys, uint32_t address, int *data);

/* Escreve 'data' em 'address'; retorna true em L1-hit */
bool write_memory(MemorySystem *sys, uint32_t address, int data);

/* Aquece a cache lendo [start_addr, start_addr+size) sem contar nos stats */
void warmup_cache(MemorySystem *sys, uint32_t start_addr, uint32_t size);

/* Zera apenas os contadores de hits/misses (mantém conteúdo da cache) */
void reset_stats(MemorySystem *sys);

#endif /* CACHE_H */
