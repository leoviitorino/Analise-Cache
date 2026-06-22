CC = gcc
CFLAGS = -O2 -Wno-unused-variable -Wno-unused-but-set-variable

N ?= 1024

BENCH_DIR = Benchmarks
SIM_DIR = SimulationCache

PROGS = $(BENCH_DIR)/matriz_mul $(BENCH_DIR)/matriz_bloc $(BENCH_DIR)/matriz_bloc_inv
SIM = $(SIM_DIR)/simulador

.PHONY: all clean rodar_mul rodar_bloc rodar_bloc_inv rodar_simulacao

all: $(PROGS) $(SIM)

$(BENCH_DIR)/matriz_mul: $(BENCH_DIR)/matriz_mul.c
	$(CC) $(CFLAGS) $< -o $@

$(BENCH_DIR)/matriz_bloc: $(BENCH_DIR)/matriz_bloc.c
	$(CC) $(CFLAGS) $< -o $@

$(BENCH_DIR)/matriz_bloc_inv: $(BENCH_DIR)/matriz_bloc_inv.c
	$(CC) $(CFLAGS) $< -o $@

$(SIM): $(SIM_DIR)/main.c $(SIM_DIR)/Cache.c $(SIM_DIR)/Cache.h
	$(CC) $(CFLAGS) $(SIM_DIR)/main.c $(SIM_DIR)/Cache.c -o $@

rodar_mul: $(BENCH_DIR)/matriz_mul
	sudo perf stat -e cache-misses,cache-references,cycles,instructions ./$(BENCH_DIR)/matriz_mul $(N)

rodar_bloc: $(BENCH_DIR)/matriz_bloc
	sudo perf stat -e cache-misses,cache-references,cycles,instructions ./$(BENCH_DIR)/matriz_bloc $(N)

rodar_bloc_inv: $(BENCH_DIR)/matriz_bloc_inv
	sudo perf stat -e cache-misses,cache-references,cycles,instructions ./$(BENCH_DIR)/matriz_bloc_inv $(N)

rodar_simulacao: $(SIM)
	./$(SIM)

clean:
	rm -f $(PROGS) $(SIM)