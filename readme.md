# Ótimo - Algoritmo exato
## khammingDiference1 - Versão Cúbica
g++ -std=c++0x khammingDiference1.cpp -o khammingDiference1

## khammingDiference2 - Versão Quadrática
g++ -std=c++0x khammingDiference2.cpp -o khammingDiference2

# Heurísticas

## Heurística 1
g++ -std=c++0x khammingDiference2H1.cpp -o khammingDiference2H1

## Heurística 2
g++ -std=c++0x khammingDiference2H2.cpp -o khammingDiference2H2

# Paralelo

### Versão paralela da Heurística 2
mpic++ khammingDiference2H2-P.cpp -o khammingDiference2H2-P
mpirun -np 3 -hostfile hostf ./khammingDiference2H2-P entrada1.asta entrada2.asta k