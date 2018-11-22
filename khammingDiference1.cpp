/**
   Autor: Lucas Barbosa Rocha
   Orientador: Francisco Elói
   Ano: 2018
   FACOM: Mestrado em Ciência da Computação

   Este projeto implementa o algoritmo para encontrar segmentos específicos
   utilizando a distância de Hamming como função objetivo

   khammingDiference1 é uma versão cúbica para resolução do problema
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <time.h>

using namespace std;

/* Função que calcula as diferenças para cada posição i de u com cada posição j de v */
int *diferencas(char *u, int tamU, char *v, int tamV, int k);
/* Função que recebe um tempo em segundos e imprime em tempo de horas */
void formataTempo(int tempo);
int MAX(int a, int b);

int main(void)
{
	int tamU, tamV, posV, posU, posCand, j, l, *err, k, cand, tamCand, tam, val, i, h, candV;

    /* Leitura dos dados */
    string tmpU;
    string tmpV;

    cout << "Versão cúbica...\n";

    cout << "Sequencia U: ";
    cin >> tmpU;
    cout << "Sequencia V: ";
    cin >> tmpV;
    cout << "k: ";
    cin >> k;
    tamU = tmpU.length();
    tamV = tmpV.length();

    char *u = new char[tamU];
    char *v = new char[tamV];

    strcpy(u, tmpU.c_str());
    strcpy(v, tmpV.c_str()); 
    /* Fim leitura */

    time_t inicio, fim;

    time(&inicio);
    err = diferencas(u, tamU, v, tamV, k);
    time(&fim);

    formataTempo(difftime(fim,inicio));

    string nomeArquivo = "Saidas/khammingDiference1_a"+to_string(tamU)+"_b"+to_string(tamV)+"_k"+to_string(k);
    fstream out;
    try
    {
        out.open(nomeArquivo, ios::out | ios::trunc);
        out<< "j|tamanho\n";
        for (i = 0; i < tamU; i++)
        {
            out<< i<<"|"<<err[i]<<"\n";
        }
        out.close();

    }catch(std::ifstream::failure e){
        cout<<"Deu ruim\n";
    }
    cout<<"Salvo em: ";
    cout<<nomeArquivo<<endl;
    return 0;
}

int *diferencas(char *u, int tamU, char *v, int tamV, int k)
{
    int posV, posU, h, err, j = 0, i, tamCand, tam;
    int *r = new int[tamU];

    cout << "\nAlgoritmo rodando...\n";

    for(j = 0; j < tamU; j++)
        r[j] = k;

    j = 0;

    for (posU = 0; posU < tamU; posU++)
    {
        for (posV = 0; posV < tamV; posV++)
        {
            h = 0;
            err = 0;
            tamCand = tam = 0;

            while(err < k && posV + h < tamV && posU + h < tamU)
            {
                if (v[posV+h] != u[posU+h])
                {
                    err++;
                }
                if (err < k)
                    tam++;
                if (err <= k)
                    tamCand++;
                h++;
            }

            if (err >= k && r[posU] <= tamCand)
            {
                r[posU] = MAX(r[posU], tamCand);
            }else
            if (err < k && tamCand >= r[posU])
            {
                r[posU] = tamCand + 1;
            }

        }    
    }

    /* Analise nao condidatos */
    for(i = 0; i < tamV; i++)
        if(r[i] > tamV-i)
            r[i] = -1; 

    if (tamU < tamV) /* Varredura final, se |U| < |V| elementos em v não tem solução */
    {
        for(i = 0; i < (tamV - tamU); i++)
            if (r[i] > tamU)
                r[i] = -1;
    }
    /* Fim analise nao candidatos */

    return r;
}

void formataTempo(int tempo)
{
    int horas, minutos, horas_seg = 3600, segundos;

    horas = (tempo/horas_seg); //resultado da hora
    minutos = (tempo -(horas_seg*horas))/60;
    segundos = (tempo -(horas_seg*horas)-(minutos*60));
    
    cout<<"Tempo de execucao: "<< horas << " hora(s) " << minutos << " minuto(s) " << segundos << " segundo(s)" << endl;
}

int MAX(int a, int b)
{
    if (a >= b)
        return a;
    else
        return b;
}