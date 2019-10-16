#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include "list" 
#include "sstream" 
#include "iterator" 
#include <fstream>
#include <cmath>

using namespace std;

void SSP_P(char *u, int tamU, char *v, int tamV, int k, int *err, int posicoesPlanoCartesiano[][2]);
void formataTempo(int tempo);
void contaQuantidadeSequencias(string arquivoEntrada, int *atributosAlpha);
string NumberToString (int Number);
void SSP_Uma_Posicao(char *u, int tamU, char *v, int tamV, int k, int *err, int posicao);
void SSP_H(int tamU, int tamV, int *err, int posicoesPlanoCartesiano[][2]);

int main(int argc, char **argv)
{
    //long SIZE = 10E9; // Alocar 1 GB de vetor
    int SIZE = 100000000;
    int *err, *errAux;
    int rank, size, tag, rc, i, k, continuaA, continuaB, maior, tamU, tamV, posi, atributosAlpha[2], atributosBeta[2];
    string nomeArquivoSaida = "", aux = "", nomeCorrida = "";
    char u[100000], v[100000], sequencia[100000];
    time_t inicio, fim;
    MPI_Status status;
    rc = MPI_Init(&argc, &argv);
    rc = MPI_Comm_size(MPI_COMM_WORLD, &size);
    rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    tag = 7;
    double t1, t2; // Variveis para tempo

    /* Cada processo (node) alocará 1GB de vetor */
    char *subVetorA = (char *)(malloc(SIZE * sizeof(char)));

    /* Fixando valores */
    k = atoi(argv[3]);
    /* Fim */

    /* Lendo os arquivos de entrada para buscar a quantidade e o 
    tamanho da maior sequencia. */
    cout << "Lendo os arquivos de entrada em busca do tamanho da maior sequência." << endl;
    contaQuantidadeSequencias(argv[1], atributosAlpha);
    contaQuantidadeSequencias(argv[2], atributosBeta);
    int qtdSequenciasAlpha = atributosAlpha[0], maxTamU = atributosAlpha[1];
    int qtdSequenciaBeta = atributosBeta[0], maxTamV = atributosBeta[1];
    cout << "Leitura terminada!" << endl;
    err = new int[maxTamU];
    errAux = (int *)(malloc(SIZE * sizeof(int)));
    /* Fim leitura */    

    /* Master */
    if (rank == 0)
    {
        fstream out, saida;
        t1 = MPI_Wtime(); 
        cout << "I am master: " << rank << " I have " << size - 1 << " slaves" << endl;

        /* Criando a pasta Saidas para armazenar os arquivos de saida. */
        system("if [ ! -d 'Saidas' ]; then  mkdir Saidas;  fi");       
        time_t inicio, fim;
        time(&inicio);  
        /* Abrindo o arquivo de saida */
        nomeArquivoSaida = "Saidas/khammingDiference2H3-P-outk"+NumberToString(k)+"-alpha-"+NumberToString(qtdSequenciasAlpha)+"-Tipo2.csv";
        out.open(nomeArquivoSaida.c_str(), ios::out | ios::trunc);

        /* Passo 1 - Ler arquivos e enviar para todos os processos */
        // Abrindo o arquivo A'
        ifstream arquivoA(argv[1], std::ifstream::in);
        cout << "Divisao " << qtdSequenciasAlpha << " " << size << endl;
        int divisao = qtdSequenciasAlpha / (size - 1); // Master 0 e mais size - 1 escravos
        int controleA = 0, numArquivo = 1;

        /* Aqui controla a qtd. do arquivo A */
        while (controleA < qtdSequenciasAlpha)
        {
            aux = "";
            for (int i = 1; i < size; ++i)
            {
                for (int j = 0; j < divisao; j++){ // A'
                    string line;
                    getline(arquivoA, line);
                    aux += line + "#";
                    controleA++; // Controla as sequencias no arquivo A
                }   

                strcpy(subVetorA, aux.c_str());
                maior = aux.size();
                rc = MPI_Send(&maior, 1, MPI_LONG, i, tag, MPI_COMM_WORLD);
                rc = MPI_Send(subVetorA, SIZE, MPI_CHAR, i, tag, MPI_COMM_WORLD);
                aux = "";
            }
            
            /* Passo 3 - recebendo dados dos processadores - respostas */  
            // Agora as outras respostas 

            /* Escrevendo respostas em arquivos separados */

            string nomeArquivo = "";
            for (int i = 1; i < size; i++)
            {
                nomeArquivo = "saida-arquivo-" + NumberToString(numArquivo);
                numArquivo++;
                saida.open(nomeArquivo, ios::out | ios::trunc);
    
                rc = MPI_Recv(errAux, SIZE, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
                //cout << "MESTRE " << maior << endl;
                 for (int j = 0; j < maior; j++)
                {
                    if (errAux[j] != -2)
                        saida<< errAux[j] <<" ";
                    else
                        saida << "\n";
                }
                saida.close();
            }

            /* Verificando se tem mais dados para enviar em A*/
            for (int i = 1; i < size; ++i)
            {
                rc = MPI_Recv(&continuaA, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
                //printf("%d < %d\n", controleA, qtdSequenciasAlpha);                 
                if (controleA < qtdSequenciasAlpha)
                    continuaA = 1;
                else
                    continuaA = 0;
                //printf("rank %d envia continuaA %d\n", rank, continuaA);
                rc = MPI_Send(&continuaA, 1, MPI_INT, i, tag, MPI_COMM_WORLD); 
            }            
        } /* Fim passo 1 */
        
        time(&fim);
        formataTempo(difftime(fim,inicio));
        t2 = MPI_Wtime(); 
        printf( "Tempo MPI eh %f\n", t2 - t1 ); // Tempo gasto
    }else { /* Slave */
        cout << "I am slave " << rank << " will read " << argv[2] << "\n";
        continuaB = 0;
        continuaA = 0;
        do
        {
            rc = MPI_Recv(&maior, 1, MPI_LONG, 0, tag, MPI_COMM_WORLD, &status); 
            rc = MPI_Recv(subVetorA, SIZE, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
            //cout << "Escravo recebi " << subVetorA << endl;
            //cout << "Escravo " << rank << "envia " << maior << endl;            
            posi = 0;
            
            for (int i = 0; i < maior; i++)
            {
                if (i == 0) {
                    for (int j = 0; j < maxTamU; j++)
                        err[j] = -1;
                }  
                
                string sequenciaAux = "";
                tamU = 0;
                while (subVetorA[i] != '#'){
                    sequenciaAux += subVetorA[i++];
                    tamU++;
                }
                //cout << "TAM " << tamU << endl;
                strcpy(sequencia, sequenciaAux.c_str());
                ifstream beta(argv[2], std::ifstream::in);
                
                // Variáveis para ssp uma posição   
                int pula = (int) tamU / (log(tamU) / log(2));
                int tamanhoVetor = tamU/pula;
                int posicoesPlanoCartesiano[tamanhoVetor][2]; // Lembrar de arrumar 
                string linha;
                
                // Passo 2 - processando uma sequência de A
                //com todas as sequênciasd e B 
                while (!beta.eof())
                {
                    getline(beta, linha);
                    tamV = linha.length();
                    strcpy(v, linha.c_str());

                    // Executando o sequencial para duas sequencias em algumas posicoes. 
                    //cout << "Escravo " << rank << ": " << sequencia << " " << v << endl;
                    SSP_P(sequencia, tamU, v, tamV, k, err, posicoesPlanoCartesiano);           
                    // Fim execucao 
                }
                beta.close();
                
                //Rodando a heuristica 
                SSP_H(tamU, tamV, err, posicoesPlanoCartesiano);
                // Fim Heuristica 

                // Analisando se alguma posição nao eh uma resposta valida. 
                for(int j = 0; j < tamU; j++)
                {
                    //cout << err[j] << " ";
                    if(err[j] > tamV || err[j] > tamU-j )
                         err[j] = -1;
                    //cout << "posi " << posi << " ";
                   errAux[posi++] = err[j]; 
                }
                //cout << endl;
                errAux[posi++] = -2; // Flag para saber quando uma sequencia acaba 
                // Fim  analise nao candidatos 
            
            }
            //cout << "Escravo " << rank << ": "  << maior << endl;
            /* for (int j = 0; j < maior; j++)
                cout << errAux[j] << " ";
            cout << endl;*/
            //errAux[posi] = -3; 
            /* Passo 3 - todo mundo envia para o 0*/
            
            rc = MPI_Send(errAux, SIZE, MPI_INT, 0, tag, MPI_COMM_WORLD);                
        
            /* Enviando uma requisicao para verificar se tem mais sequencias em A*/
            rc = MPI_Send(&continuaA, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);  
            rc = MPI_Recv(&continuaA, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status); 
            //printf("rank %d continuaA %d\n", rank, continuaA);
        } while(continuaA == 1);

    }
    return 0;
}

void formataTempo(int tempo)
{
    int horas, minutos, horas_seg = 3600, segundos;

    horas = (tempo/horas_seg); /* resultado da hora */
    minutos = (tempo -(horas_seg*horas))/60;
    segundos = (tempo -(horas_seg*horas)-(minutos*60));
    cout << "Tempo de execução: " << "\033[1;35m" << horas << " hora(s) " << minutos << " minuto(s) " << segundos << " segundo(s)." <<    "\033[0m\n";    
}

void SSP_P(char *u, int tamU, char *v, int tamV, int k, int *err, int posicoesPlanoCartesiano[][2])
{
    int pos = 0, posU, pula = (int) tamU / (log(tamU) / log(2));;
    for (posU = 0; posU < tamU; posU += pula)
    {  
        SSP_Uma_Posicao(u, tamU, v, tamV, k, err, posU);
        //cout << posU << ": " << err[posU] << " ";      
        posicoesPlanoCartesiano[pos][0] = posU;
        posicoesPlanoCartesiano[pos][1] = err[posU];   
        pos++;
    }
    //cout << endl;
}

void contaQuantidadeSequencias(string arquivoEntrada, int *atributosAlpha)
{
    int qtd = 0, tamanho = 0;
    string line;

    ifstream arquivo(arquivoEntrada.c_str(), std::ifstream::in);
    while (!arquivo.eof())
    {
        getline(arquivo, line);
        if(tamanho <= line.length())
            tamanho = line.length();

        if (line.length() > 1)
            qtd++;
    }
    atributosAlpha[0] = qtd;
    atributosAlpha[1] = tamanho;
}

string NumberToString (int Number)
{
    ostringstream ss;
    ss << Number;
    return ss.str();
}

void SSP_Uma_Posicao(char *u, int tamU, char *v, int tamV, int k, int *err, int posicao)
{
int i, I, j, J, max, d, erros, limite = posicao;
	char *str1, *str2;

	/* Laço externo */
	I = posicao;
	J = tamV;
	while (J >= 0)
	{
		/* Controlando o par ij*/
		/*if (J > 0)
			J = J - 1;
		else
        {
			I = I + 1;
            break;
        }*/

		d = erros = 0;
		i = I;
		j = J;

		if (tamU - I < tamV - J)
			max = tamU;
		else
			max = tamV - J + I;

		while (d + i < tamU && d + j < tamV && erros < k - 1)
		{
 			if (u[d+i] != v[j+d])
				erros += 1;
			d += 1;
		}

        while (d + i < tamU && d + j < tamV && u[d+i] == v[d+j])
            d += 1;

        if (max >= d + i)
            d += 1;

        if (err[i] < d)
            err[i] = d;
        
        J--;
	}
}

void SSP_H(int tamU, int tamV, int *err, int posicoesPlanoCartesiano[][2])
{
    /* Variáveis para ssp uma posição */    
    int j = 0, i, pula = (int) tamU / (log(tamU) / log(2));
    int tamanhoVetor = tamU/pula, pos = 0, valorInteiro;
    int Xi, Yj, Xii, Yjj, maior;
    double valor, m;          
    /* Rodando a heuristica, começando a heristica, colocar os valores que estão na reta */

    pos = 0;
    while (pos < tamanhoVetor - 1)
    {
        Xi = posicoesPlanoCartesiano[pos][0];
        Yj = posicoesPlanoCartesiano[pos][1];
        Xii = posicoesPlanoCartesiano[pos+1][0];
        Yjj = posicoesPlanoCartesiano[pos+1][1];
        /* Calculando o ângulo da reta 
        m = (Y2 - Y1)/(X2 - X1) */
        m = (Yjj - Yj) / ((Xii - Xi) * 1.0);
        /* Reta: 
        Y - Y1 = m(X - X1)
        Y - Y1 - mX + mX1 = 0  */
        
        i = Xi + 1;
        while (i < tamU && Xii < tamU && i < Xii)
        { 
            /* Vamos rodar no intervalo da reta, se der 0 para um ponto estah na reta
            Mas aqui vamos considerar que se der entre [-1:1] será um valor interessante.
            Para o ponto atual i, vamos consultar y anterior e um acima e um abaixo
            e com isso decidir pegar o ponto mais proximo de zero. */
            valor = Yj + m*i - m*Xi;
            valorInteiro = (int) valor + 0.5;
            err[i] = valorInteiro;
        i++;
        }
        pos++;
    }
    /* Fim Heuristica */    
}
