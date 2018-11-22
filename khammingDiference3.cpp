/**
   Autor: Lucas Barbosa Rocha
   Orientador: Francisco Elói
   Ano: 2018
   FACOM: Mestrado em Ciência da Computação

   Este projeto implementa o algoritmo para encontrar segmentos específicos
   utilizando a distância de Hamming como função objetivo

   khammingDiference3 é uma versão quadrática para resolução do problema que
   utiliza uma árvore de sufixo para encontrar trechos iguais
*/

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <iomanip>
#include <time.h>
#include <fstream>
#include <list>

//biblioteca arvore sufixo
#include "strmat/strmat.h"
#include "strmat/stree_ukkonen.h"
#include "strmat/strmat_stubs2.h"
#include "strmat/strmat_print.h"
#include "strmat/peter_longest_common_extension.h"
#include "strmat/strmat_util.h"
#include "strmat/stree_lca.h"

using namespace std;
LCE *lce; /* Árvore de Sufixo */

/* Função que recebe as sequências u e v e constrói uma árvore de sufixo */
void constroiArvore(const char *u, int tamU, char *v, int tamV);
/* Função que calcula as diferenças para cada posição i de u com cada posição j de v */
int *diferencas(const char *u, int tamU, char *v, int tamV, int k);
/* Função que recebe dois inteiros e computa a maior sequência para o i de u com o j de v */
int LCE(int x, int y);
/* Função que recebe um tempo em segundos e imprime em tempo de horas */
void formataTempo(int tempo);

int main(void)
{
	int k, *err, *r, tamV, tamU, i;

    /* Leitura dos dados */
	string tmpU;
	string tmpV;

    cout << "Versão quadrática usando Árvore de Súfixo...\n";
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
  	constroiArvore(u, tamU, v, tamV);
    time(&fim);
    cout << "\nC.Árvore: ";
    formataTempo(difftime(fim,inicio));

    time(&inicio);
    err = diferencas(u, tamU, v, tamV, k);
    time(&fim);
    cout << "Algoritmo: ";
    formataTempo(difftime(fim,inicio));

    string nomeArquivo = "Saidas/khammingDiference3_a"+to_string(tamU)+"_b"+to_string(tamV)+"_k"+to_string(k);
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

void constroiArvore(const char *u, int tamU, char *v, int tamV)
{
	int i, I, j, J, max, d, erros;
	const STRING *str1, *str2;
	char *auxV;

    /* time_t inicio, fim;
    inicio = clock(); */

    CHAR_TYPE *sq1 = new CHAR_TYPE[tamU];
	CHAR_TYPE *sq2 = new CHAR_TYPE[tamV+2];

	auxV = new char[tamV+2];
	auxV = v;
	auxV[tamV] = '$';
	auxV[tamV+1] = '\0';

    str1 = make_seqn("alpha", str_to_sequence(u, sq1,tamU+1), tamU+1, false);
    str2 = make_seqn("beta", str_to_sequence(auxV, sq2,tamV+2), tamV+2, false);

	lce = prepare_longest_common_extension(str1, str2, false);
    
    /*fim = clock();
	cout<<"\nC.Árvore: (Tempo: "<<((fim - inicio) / (CLOCKS_PER_SEC / 1000))<<")\n";*/
}

int *diferencas(const char *u, int tamU, char *v, int tamV, int k)
{
	int *err = new int[tamU], i, I, j, J, max, d, erros;
	const STRING *str1, *str2;
	char *auxV;

    cout << "Algoritmo rodando...\n";

    /*time_t inicio, fim;
	inicio = clock(); */
	
	for(i = 0; i < tamU; i++){
		err[i] = k;
	}

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

		/*while (d + i < max && erros < k - 1)
		{
			if (u[d+i] != v[j+d])
				erros += 1;
			d += 1;
		}*/

		d += LCE(d + i, tamU + d + j + 1); // O(1)
		while(d + i < max && erros < k - 1)
		{
			erros += 1;
			d += LCE(d + i, tamU + d + j + 1); // O(1)
			d += 1;
		}

		while (i < max)
		{
			/* LCE */
			if (d + i < max)
				d += LCE(d + i, tamU + d + j + 1); // O(1)
			/*while (d + i < max && u[d+i] == v[d+j])
				d += 1;*/
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

    /*fim = clock();
   	cout<<"Execução: (Tempo: "<<((fim - inicio) / (CLOCKS_PER_SEC / 1000))<<")\n"; */


    /* Analise nao condidatos */
    for(i = 0; i < tamU; i++){
        if(err[i] > tamV || err[i] > tamU-i ){
            err[i] = -1; 
        }
    }
    /* Fim analise nao candidatos */   	

	return err;
}

int LCE(int x, int y)
{
   return lookup(lce, x, y);
}

void formataTempo(int tempo)
{
    int horas, minutos, horas_seg = 3600, segundos;

    horas = (tempo/horas_seg); //resultado da hora
    minutos = (tempo -(horas_seg*horas))/60;
    segundos = (tempo -(horas_seg*horas)-(minutos*60));
    
    cout<<"Tempo de execucao: "<< horas << " hora(s) " << minutos << " minuto(s) " << segundos << " segundo(s)" << endl;
}