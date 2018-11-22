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



using namespace std;

int *diferencas(char *u, int tamU, char *v, int tamV, int k);
/* Função que recebe um tempo em segundos e imprime em tempo de horas */
void formataTempo(int tempo);
string lerArquivo(const char *local);


int main(int argc, char** argv)
{
	int k, *err, *r, tamV, tamU, i;

    /* Leitura dos dados */
	string tmpU;
	string tmpV;

    if(argc != 4){
    	cout << "Use: khammingDiference2 'alpha' 'beta' k\n";
    	return 0;
    }


    /*cout << "Versão quadrática...\n";
    cout << "Sequencia U: ";
  	cin >> tmpU;
	cout << "Sequencia V: ";
  	cin >> tmpV;
	cout << "k: ";
  	cin >> k;*/
  	
  	//tmpU = argv[1];
  	//tmpV = argv[2];
  	/* Abrindo o arquivo para ler as entradas */
  	tmpU = lerArquivo(argv[1]);
  	tmpV = lerArquivo(argv[2]);

  	k = atoi(argv[3]);
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
   
    string nomeArquivo = "Saidas/khammingDiference2_a"+to_string(tamU)+"_b"+to_string(tamV)+"_k"+to_string(k);
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
	int *err, i, I, j, J, max, d, erros;
	char *str1, *str2;
	err = (int *)(malloc(tamU * sizeof(int)));

    cout << "Algoritmo rodando...\n";


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
            err[i] = -1; 
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

string lerArquivo(const char *local){

  ifstream arquivo(local, std::ifstream::in);

  if(!arquivo.good()) {
    arquivo.close();

    return local;
  }
  cout<<"Abrindo arquivo "<<local<<"...\n";

  std::ostringstream contents;
  contents << arquivo.rdbuf();

  arquivo.close();

  return(contents.str());
}