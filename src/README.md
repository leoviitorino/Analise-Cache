# Como rodar o projeto
## Compilação

Para compilar todos os programas de matriz e também o simulador rode:
```bash
make
```

Para apagar todos os executáveis gerados:

```bash
make clean
```


## Execução

Pós compilar execute o programa que desejar via algum desses comandos

### Multiplicação de Matrizes Padrão
```bash
# O tamanho padrão é 1024. Você pode mudar usando N=<tamanho>
make rodar_mul
make rodar_mul N=512
```

### Multiplicação de Matrizes em Blocos
```bash
make rodar_bloc
make rodar_bloc N=512
```

### Multiplicação em Blocos Invertida
```bash
make rodar_bloc_inv
make rodar_bloc_inv N=512
```

### Simulador de Cache 
```bash
make rodar_simulacao
```