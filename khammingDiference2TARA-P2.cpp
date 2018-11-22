#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include "list" 
#include "sstream" 
#include "iterator" 
#include <fstream>

#define maxQTDA 400 // Duas linhas por sequencia
#define maxQTDB 2000000
//#define qtdProcessadorA 4
#define qtdProcessadorB 1000
#define maxError 300
#define maxSubVetor 100000


using namespace std;
int *diferencas(char *u, int tamU, const char *v, int tamV, int k, int *err);
void formataTempo(int tempo);


int main(int argc, char **argv)
{
    int rank, size, tag, rc, i, k, continuaB, continuaA, nSequence = 1;
    int err[maxError], errAux[maxError], errFinal[maxError];
    time_t inicio, fim;
    MPI_Status status;
    char subVetorA[maxSubVetor], subVetorB[maxSubVetor];
    string aux, nomeCorrida = "ERR599040";;
    rc = MPI_Init(&argc, &argv);
    rc = MPI_Comm_size(MPI_COMM_WORLD, &size);
    rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    tag=7;

    k = atoi(argv[3]);

    if (rank == 0)
    {        
        if(argc != 4){
            cout << "Use: khammingDiference2TARA 'alpha' 'beta' k\n";
            return 0;
        }  
    }

    /* Master */
    if (rank == 0) 
    {
        time(&inicio);
        int controlaA = 0, controlaB = 0;
        int qtdProcessadorA = 2;
        /* Arquivo resposta */
        fstream out;
        out.open("Saidas/ERR599040-P2-A-10", ios::out | ios::trunc);  
        /* Passo 1 - Ler arquivos e enviar para todos os processos */
        // Abrindo o arquivo A'
        ifstream arquivoA(argv[1], std::ifstream::in);

        //while (!arquivoA.eof())  // Aqui controla a qtd. do arquivo A
        while(controlaA < maxQTDA)
        {
            printf("%d < %d\n", controlaA, maxQTDA);
            aux = "";
            for (int i = 1; i < maxError; i++)
                errFinal[i] = - 1;

            for (int j = 0; j < qtdProcessadorA; j++){ // A'
                string line;
                getline(arquivoA, line);
                if (line[0] != '>') {
                    aux += line + "#";
                }
                controlaA++; // Controla as sequencias no arquivo A
            }

            /*char *vetorBkp;
            vetorBkp = new char[maxSubVetor];*/
            strcpy(subVetorA, aux.c_str());

            for (int i = 1; i < size; ++i)
            {

                rc = MPI_Send(subVetorA, maxSubVetor, MPI_CHAR, i, tag, MPI_COMM_WORLD);
                aux="";
            }

            ifstream arquivoB(argv[2], std::ifstream::in);
            //while (!arquivoB.eof())  // Aqui controla a qtd. do arquivo B
            controlaB = 0;
            while(controlaB < maxQTDB)
            {
               // aux = "";
				for (int i = 1; i < size; ++i)
                {
			aux = "";
					for (int j = 0; j < qtdProcessadorB; j++){ // A'
						string line;
						getline(arquivoB, line);
						if (line[0] != '>') {
							//cout<< line << "\n";
							aux += line + "#";
						}
						controlaB++; // Controla as sequencias no arquivo B
					}
                //cout << subVetorB  << "\n";

                /*char *vetorBkp;
                vetorBkp = new char[maxSubVetor];*/
					strcpy(subVetorB, aux.c_str());

                /*for (int i = 1; i < size; ++i)
                {*/
                    rc = MPI_Send(subVetorB, maxSubVetor, MPI_CHAR, i, tag, MPI_COMM_WORLD);
                }

                /* Passo 3 - recebendo dados dos processadores - respostas */
                //fstream out;
                //out.open("Saidas/ERR599040", ios::out | ios::trunc);  
                // Agora as outras respostas 
                for (int i = 1; i < size; i++){  
                    rc = MPI_Recv(errAux, maxError, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
                    //out<< "Sequência "<< nSequence++ << "\nj|tamanho\n";
                    for (int j = 0; j < maxError && errAux[j] != -1; j++)
                    {
                        //out<< j<<"|"<<errAux[j]<<"\n";
                        errFinal[j] = max(errFinal[j], errAux[j]);
                    }                      
                }
                //out.close();

                /* Verificando se tem mais dados para enviar em B*/
                for (int i = 1; i < size; ++i)
                {
                    rc = MPI_Recv(&continuaB, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &status);                 
                    if (controlaB < maxQTDB)
                        continuaB = 1;
                    else
                        continuaB = 0;
                    //printf("rank %d envia continuaB %d\n", rank, continuaB);
                    rc = MPI_Send(&continuaB, 1, MPI_INT, i, tag, MPI_COMM_WORLD); 
                } 
            }
            arquivoB.close();

            /* Escreve no arquivo resposta */ 

            // Agora as outras respostas 
            out<< "Sequência "<< nSequence++ << "\nj|tamanho\n";

            for (int j = 0; j < maxError && errFinal[j] != -1; j++)
            {
                out<< j<<"|"<<errFinal[j]<<"\n";
                errFinal[j] = -1;
            }                
            

            /* Verificando se tem mais dados para enviar em A*/
            for (int i = 1; i < size; ++i)
            {
                rc = MPI_Recv(&continuaA, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
                //printf("%d < %d\n", controlaA, maxQTDA);                 
                if (controlaA < maxQTDA)
                    continuaA = 1;
                else
                    continuaA = 0;
                //printf("rank %d envia continuaA %d\n", rank, continuaA);
                rc = MPI_Send(&continuaA, 1, MPI_INT, i, tag, MPI_COMM_WORLD); 
            }


        }
        arquivoA.close();
        out.close(); // ARaquivo resposta
        time(&fim);
        formataTempo(difftime(fim,inicio));

    }
    else /* Slaves */
    {
        continuaB = 0;
        continuaA = 0;
        do
        {
            rc = MPI_Recv(subVetorA, maxSubVetor, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status); 
            do
            {
                rc = MPI_Recv(subVetorB, maxSubVetor, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);

                /* Pass 2 - Nodes executam o algoritmo SSP */
                //printf("node: %d A': %s B': %s\n", rank, subVetorA, subVetorB);    
                for (int i = 0; i < strlen(subVetorA); i++)
                {
                    if (i == 0) {
                        for (int j = 0; j < maxError; j++)
                            errAux[j] = -1;
                    }

                    string sequencia = "";
                    while (subVetorA[i] != '#'){
                        sequencia += subVetorA[i++];
                    }
                    char sequenciaChar[sequencia.size()];
                    strcpy(sequenciaChar, sequencia.c_str());
                    //printf("rank %d %s \n", rank, sequenciaChar);

                    diferencas(sequenciaChar, sequencia.size(), subVetorB, strlen(subVetorB), k, err);    

                    for (int j = 0; j < sequencia.size(); j++) { 
                        if (err[j] > errAux[j])
                            errAux[j] = err[j];
                    }
    
                    /* Passo 3 - todo mundo envia para o 0*/
                    rc = MPI_Send(&errAux, maxError, MPI_INT, 0, tag, MPI_COMM_WORLD);  
                }


                /* Enviando uma requisicao para verificar se tem mais sequencias em B*/
                rc = MPI_Send(&continuaB, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);  
                rc = MPI_Recv(&continuaB, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status); 
                //printf("rank %d continuaB %d\n", rank, continuaB);
            } while(continuaB);
        
            /* Enviando uma requisicao para verificar se tem mais sequencias em A*/
            rc = MPI_Send(&continuaA, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);  
            rc = MPI_Recv(&continuaA, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status); 
            //printf("rank %d continuaA %d\n", rank, continuaA);
        } while(continuaA);
    }

    rc = MPI_Finalize();
    return 0;
}

int *diferencas(char *u, int tamU, const char *v, int tamV, int k, int *err)
{
    int /**err,*/ i, I, j, J, max, d, erros;
    char *str1, *str2;
    //err = (int *)(malloc(tamU * sizeof(int)));
    //err = new int[tamU];
 
    //cout << "Algoritmo rodando...\n";
 
 
    for(i = 0; i < tamU; i++)
        err[i] = k;
 
    /* Laço externo */
    I = 0;
    J = tamV;
 
    while (I < tamU)
    {
        d = erros = 0;
        i = I;
        j = J;
 
        if (tamU - I < tamV - J)
            max = tamU;
        else
            max = tamV - J + I;
 
        while (d + i < max && erros < k - 1)
        {
            if (u[d+i] != v[j+d])
                erros += 1;
            d += 1;
        }
 
        while (i < max)
        {
            /* LCE */
            while (d + i < max && u[d+i] == v[d+j])
                d += 1;
 
            if (max >= d + i)
                d += 1;
 
            while (1)
            {
                if (err[i] < d)
                    err[i] = d;
                i += 1;
                j += 1;
                d -= 1;
                if (i == max || u[i-1] != v[j-1])
                    break;
            }
        }
        /* Controlando o par ij*/
        if (J > 0)
            J = J - 1;
        else
            I = I + 1;
    }
 
    /* Analise nao condidatos */
    for(i = 0; i < tamU; i++)
        if(err[i] > tamV || err[i] > tamU-i )
            ;//err[i] = -1; 
    /* Fim analise nao candidatos */   
 
 
    return err;
}

void formataTempo(int tempo)
{
    int horas, minutos, horas_seg = 3600, segundos;
 
    horas = (tempo/horas_seg); //resultado da hora
    minutos = (tempo -(horas_seg*horas))/60;
    segundos = (tempo -(horas_seg*horas)-(minutos*60));
     
    cout<<"Tempo de execucao: "<< horas << " hora(s) " << minutos << " minuto(s) " << segundos << " segundo(s)" << endl;
}

