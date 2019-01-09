# Problema da Seleção das Subsequências Específicas - SSP (Specific Substring Problem)

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

Dados dois conjuntos A e B de sequências, encontrar todas as subsequências minimais 
que tenham distância k de cada subsequência em B para uma dada função de distância D.

## Começando

### Pré-requisitos

* GCC (>= 4.0)
* MPI

## Programas

### khammingDiference1 - KHD1

Esta é a versão trival para resolver o problema, realiza todos os alinhamentos possíveis na força bruta, possui complexida O(n³)

Comando para compilar:

```
g++ -std=c++0x khammingDiference1.cpp -o khammingDiference1
./khammingDiference1
```

### khammingDiference2 - KHD2

Esta é a versão melhorada do algoritmo cúbico, foi baseada no artigo **fast algorithm for the inexact characteristic string problem.** e tem complexidade O(n²).

Comando para compilar:

```
g++ -std=c++0x khammingDiference2.cpp -o khammingDiference2
./khammingDiference2
```

### khammingDiference3 - KHD3

Esta é a versão KHD2 utilizando uma árvore de súfixo para otimizar o tempo.
Para implementar a árvore: https://github.com/peterwilliams97/strings/tree/master/strmat

Comando para compilar e executar:

```
chmod 777 makeKhammingDiference3
./makeKhammingDiference3
./khammingDiference3
```

### khammingDiference2Tara - KHD4

Esta é a versão KHD2 adaptada para receber dois arquivos do tipo fasta:

```
g++ -std=c++0x khammingDiference2TARA.cpp -o khammingDiference2TARA
./khammingDiference2TARA
```

### khammingDiference2TaraP1 - KHD2-P1

Esta é a versão KHD4 paralela.

Comando para compilar e executar:

```
mpiCC khammingDiference2TARA-P1.cpp -o khammingDiference2TARA-P1
mpirun -np 3 -hostfile hostf ./khammingDiference2TARA-P1 entrada1.asta entrada2.asta k
```
###  khammingDiference2Tara-P2 - KDH2-P2

Esta é a versão KHD4 paralela, focada em otimizar a memória.


Comando para compilar e executar:

```
mpiCC khammingDiference2TARA-P1.cpp -o khammingDiference2TARA-P1
mpirun -np 3 -hostfile hostf ./khammingDiference2TARA-P1 entrada1.asta entrada2.asta k
```

## Publicações referente ao problema:

http://bsb2018.ic.uff.br/wp-content/uploads/2018/10/Proceedings-BSB-2018.pdf

## Autores/Colaboradores

* Lucas Barbosa Rocha
* Francisco Elói Soares de Araujo
* Said Sadique Adi
* Marco Aurelio Stefanes

## Agradecimentos
* Universidade Federal de Mato Grosso do Sul
* CAPES
