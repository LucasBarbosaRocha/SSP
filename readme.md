# Trabalho de Mestrado

# Problema da Seleção das Subcadeias Específicas - SSP (Specific Substring Problem)

## Definições

Distância de **Hamming**: O menor número de substituições para transformar uma *string* em outra.

Exemplo, _s_ = CGACC, _t_ = CGTCG,

```
C G A C C
C G T C G
0 0 1 0 1
```

dist(s,t) = 2

## SSP

Dados dois conjuntos A e B de sequências, encontrar todas as subcadeias minimais que tenham distância _k_ de cada subcadeia em B utilizando uma função de distância D.

## Exemplo de saída do SSP

Tome os conjuntos A = {_s_} e B = {_t_} e _k_ = 1, para cada sequência no conjunto A, teremos um vetor de inteiros _r_ como saída, o vetor tem o comprimento da sequência _s_. Cada posição do vetor representa uma posição na sequência _s_ e o inteiro representa o comprimento da subcadeia com pelo menos _k_ diferenças.

No exemplo abaixo, para _k_ = 1, temos, por exemplo, _r[5]_ = 2, que representa a subcadeia  **CA** começando na posição **5** da sequência _s_. A subcadeia **CA** ao ser comparada com todas as subcadeias do conjunto B contém pelo menos _k = 1_ diferença(s).

```
i  1 2 3 4 5 6 7 8 9
r [3 2 1 3 2 1 2 1 1]
s  T G A G C A T A A
t  T G C T C T G G C
```

**Obs.** Neste tópico para melhor entendimento do vetor _r_ começamos vamos do índice 1 até _|s|_, mas computacioalmente vamos 0 até _|s| - 1_.

## Começando

### Pré-requisitos

* GCC (>= 4.0)
* MPI

## Programas

Os algoritmos exatos e as abordagens heurísticas utilizam três parâmetros como entrada: dois arquivos fasta (Conjunto A e Conjunto B) com sequências e um inteiro _k_.

### Entrada

Cada linha dos conjuntos A e B deverá conter um rótulo e uma sequência no formato fasta, entretanto as sequências não podem estar quebradas como é de costume no formato fasta. 

**Exemplo de modelo correto:**

```
> Rótulo 1
Sequence 1
> Rótulo 2
Sequence 2
```

O inteiro _k_ representa a quantidade de diferenças (pelo menos _k_ diferenças) que cada subcadeia do Conjunto A terá ao ser comparada com todas as subcadeias do Conjunto B.

### Saída

Um arquivo com a extensão Saidas/nome.csv (planilha).

Para cada sequência s do Conjunto A, teremos um vetor de inteiros _r_ (ver **Exemplo de saída do SSP**) que é preenchido no arquivo Saidas/nome.csv.

## Exato - Algoritmo exato (Done)
### khammingDiference1 (Versão Cúbica) e khammingDiference2 (Versão Quadrática)
Acessar a pasta **Exato**:
```
chmod +x Makefile
make
```

### Execução
```
./khammingDiference1 conjunto1 conjunto2 k
./khammingDiference2 conjunto1 conjunto2 k
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
./khammingDiference2H1 conjunto1 conjunto2 k
./khammingDiference2H2 conjunto1 conjunto2 k
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
mpirun -np 3 -hostfile hostf ./khammingDiference2H2-P conjunto1 conjunto2 k
```
## Publicações

http://bsb2018.ic.uff.br/wp-content/uploads/2018/10/Proceedings-BSB-2018.pdf

## Autores/Colaboradores

* Lucas Barbosa Rocha
* Francisco Eloi Soares de Araujo
* Said Sadique Adi
* Marco Aurelio Stefanes

## Agradecimentos
* Universidade Federal de Mato Grosso do Sul
* Universidade Federal do Rio Grande do Norte
* CAPES
