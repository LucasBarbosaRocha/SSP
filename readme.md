# Problema da Seleção das Subcadeias Específicas - SSP (Specific Substring Problem)

## Definições

Distância de **Hamming**: O menor número de substituições para transformar uma *string* em outra.

Exemplo,u=CGACC,v=CGTCG,

```
C G A C C
C G T C G
0 0 1 0 1
```

dist(u,v) = 2

## SSP

Dados dois conjuntos A e B de sequências, encontrar todas as subcadeias minimais que tenham distância k de cada subcadeia em B utilizando uma função de distância D.

## Começando

### Pré-requisitos

* GCC (>= 4.0)
* MPI

## Programas

Os algoritmos exatos e as abordagens heurísticas utilizam três parâmetros como entrada: dois arquivos (Set A e Set B) com sequências e um inteiro k.

Cada linha dos Sets A e B deverá conter uma sequência, ou seja, as sequências estão sendo separadas por linhas.

O inteiro k representa a quantidade de diferenças (pelo menos k diferenças) que cada subcadeia do Set A terá ao ser comparada com todas as subcadeias do Set B.

## Exato - Algoritmo exato (Done)
### khammingDiference1 (Versão Cúbica) e khammingDiference2 (Versão Quadrática)
Acessar a pasta **Exato**:
```
chmod +x Makefile
make
```

### Execução
```
./khammingDiference1 entrada1.asta entrada2.asta k
./khammingDiference2 entrada1.asta entrada2.asta k
```

## Heurísticas (Done)
### Heurística 1 e Heurística 2
Acessar a pasta **Heuristica**:
```
chmod +x Makefile
make
```

### Execução
```
./khammingDiference2H1 entrada1.asta entrada2.asta k
./khammingDiference2H2 entrada1.asta entrada2.asta k
```

## Paralelo (Doing)
### Versão paralela da Heurística 2
Acessar a pasta **Paralelo**:
```
chmod +x Makefile
make
```

### Execução
```
mpirun -np 3 -hostfile hostf ./khammingDiference2H2-P entrada1.asta entrada2.asta k
```