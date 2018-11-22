/**
   Autor: Lucas Barbosa Rocha
   Orientador: Francisco Elói
   Ano: 2018
   FACOM: Mestrado em Ciência da Computação
 
   Este projeto implementa o algoritmo para encontrar segmentos específicos
   utilizando a distância de Hamming como função objetivo
 
   khammingDiference2 é uma versão quadrática para resolução do problema
*/
#include <ctype.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <time.h>
#include <sstream>
 
#define maxError 300
 
using namespace std;
 
void diferencas(char *u, int tamU, const char *v, int tamV, int k, int *err);
/* Função que recebe um tempo em segundos e imprime em tempo de horas */
void formataTempo(int tempo);
string lerArquivo(const char *local);
 
int main(int argc, char** argv)
{
    int k, *err, *errAux, *r, tamV, tamU, i, status = 0;
    string nomeCorrida = "ERR599040";
 
    /* Leitura dos dados */
    string tmpU;
    string tmpV;
 
    if(argc != 4){
        cout << "Use: khammingDiference2TARA 'alpha' 'beta' k\n";
        return 0;
    }
 
    /* Abrindo o arquivo para ler as entradas */
    k = atoi(argv[3]);
    time_t inicio, fim;
 
    // Abrindo o(s) arquivo(s) alpha e beta
    ifstream alpha(argv[1], std::ifstream::in);
 	int nSequence = 1;
     
    string nomeArquivo = "Saidas/"+nomeCorrida;
    fstream out;
    out.open(nomeArquivo, ios::out | ios::trunc);   
 
    char *u;
    errAux = new int[maxError];
    err = new int[maxError];
    time(&inicio);
    for (int i = 0; i < 20; i++){ // alpha
        string line;
        getline(alpha, line);
        if (line[0] != '>') {
            tamU = line.length();

            u = new char[tamU];
            strcpy(u, line.c_str());
 
            //err = new int[tamU];
            for (int l = 0; l < tamU; l++) // Inicializando o vetor principal de erros
                err[l] = -1;

            ifstream beta(argv[2], std::ifstream::in);
            //for (int l = 0; l < 10000; l++){ // beta
            //string line;
            while (!beta.eof()) {
                getline(beta, line);
                if (line[0] != '>') {
                    tamV = line.length();
                    //errAux = new int[tamV];
                    //memset(errAux, 0, tamV);
                    for (int j = 0; j < tamU; j++)
                        errAux[j] = -1;

                    if (status == 1) {
                        cout<<"alpha " << u<<"\n";
                        cout<<"tamU "<<tamU<<"\n";
                        cout<<"beta "<< line.c_str()<<"\n";
                        cout<<"tamV " << tamV<<"\n";
                    }
 
                    /* Executar o SSP */
                    diferencas(u, tamU, line.c_str(), tamV, k, errAux);
 
                    for (int j = 0; j < tamU; j++) { 
                        if (errAux[j] > err[j])
                            err[j] = errAux[j];
                    }
                }
            }  
            // Liberar memoria 
            delete u;          

            beta.close();
    
            /* Escrevendo a resposta no arquivo de saida */
            try
            {
		        out<< "Sequência "<< nSequence++ << "\nj|tamanho\n";
		        for (int j = 0; j < tamU; j++)
		        {
		            out<< j<<"|"<<err[j]<<"\n";
		        }
         
            }catch(std::ifstream::failure e){
                cout<<"Deu ruim\n";
            }
        }
    }
    time(&fim);
    /* Fim leitura */
 
    // Fechando os arquivos alpha e out
    alpha.close();
    out.close();
    cout<<"Salvo em: ";
    cout<<nomeArquivo<<endl;
 
 
    formataTempo(difftime(fim,inicio));
 
 
    return 0;
}
 
void diferencas(char *u, int tamU, const char *v, int tamV, int k, int *err)
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
 
 
    //return err;
}
 
void formataTempo(int tempo)
{
    int horas, minutos, horas_seg = 3600, segundos;
 
    horas = (tempo/horas_seg); //resultado da hora
    minutos = (tempo -(horas_seg*horas))/60;
    segundos = (tempo -(horas_seg*horas)-(minutos*60));
     
    cout<<"Tempo de execucao: "<< horas << " hora(s) " << minutos << " minuto(s) " << segundos << " segundo(s)" << endl;
}
 
string lerArquivo(const char *local){
 
    ifstream arquivo(local, std::ifstream::in);
 
    if(!arquivo.good()) {
        arquivo.close();
        return local;
    }
    cout<<"Abrindo arquivo "<<local<<"...\n";
 
    std::ostringstream contents;
 
    arquivo.close();
 
    return(contents.str());
}
 
/*
 
  for (int i=0;i<4;i++)
    {
        string line;
        getline(arquivo, line);
        cout<<line<<"\n";
    }
*/