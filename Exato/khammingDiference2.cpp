/**
   Autor: Lucas Barbosa Rocha
   Orientador: Francisco Eloi
   Coorientador: Said Sadique
   Ano: 2018
   FACOM: Mestrado em Ciência da Computação

   Este projeto implementa o algoritmo para encontrar segmentos específicos
   utilizando a distância de Hamming como função objetivo

   khammingDiference2 é uma versão quadrática para resolução do problema 
	 baseada no algoritmo do MAASS
*/
#include <ctype.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <time.h>
#include <sstream>
#include <cmath>

/* Flags */
#define flagDetalhes 0

using namespace std;

/* Esta funcao percorre o arquivo de entrada contando a quantidade de linhas e busca
   o tamanho da maior sequencia.  */
void contaQuantidadeSequencias(string arquivoEntrada, int *atributosAlpha);

/* Função que calcula as diferenças para cada posição i de u com cada posição j de v */
void SSP(char *u, int tamU, char *v, int tamV, int k, int *err);

/* Esta funcao recebe um tempo em segundos e 
   imprime em tempo de horas. */
void formataTempo(int tempo);

int MAX(int a, int b);

string lerArquivo(const char *local);

/* Esta funcao executa a heuristica SSP_Heuristico em dois conjuntos. Dados dois conjuntos
   A e B , a funcao executaTodos executa cada sequência de A com todas de B */
void executaTodos(string nomeSaida, int maxTamU, int maxTamV, int k, int qtdSequenciaAlpha, int qtdSequenciasBeta, char** argv);

string NumberToString (int Number);

int main(int argc, char** argv)
{
    int k, atributosAlpha[2], atributosBeta[2], *err, tipo;
    string nomeArquivoSaida;
    char *u, *v;
    fstream out;

    if (argc != 4) {
        cout << "\033[1;31m Use: khammingDiference1 'alpha' 'beta' k \033[0m\n";
        cout << "\033[1;34m alpha é o conjunto A com uma ou mais sequências \033[0m\n";
        cout << "\033[1;34m beta é o conjunto B com uma ou mais sequências \033[0m\n";
        cout << "\033[1;34m k é a quantidade de diferenças \033[0m\n";
        return 0;
    }


    /* Lendo os arquivos de entrada para buscar a quantidade e o 
        tamanho da maior sequencia. */
    cout << "Lendo os arquivos de entrada em busca do tamanho da maior sequência." << endl;
    contaQuantidadeSequencias(argv[1], atributosAlpha);
    contaQuantidadeSequencias(argv[2], atributosBeta);
    int qtdSequenciasAlpha = atributosAlpha[0], maxTamU = atributosAlpha[1];
    int qtdSequenciasBeta = atributosBeta[0], maxTamV = atributosBeta[1];
    cout << "Leitura terminada!" << endl;
    /* Fim leitura */   

    /* Criando a pasta Saidas para armazenar os arquivos de saida. */
    system("if [ ! -d 'Saidas' ]; then  mkdir Saidas;  fi");

    /* Fixando alguns valores. */
    k = atoi(argv[3]);
    err = new int[maxTamU];
    u = new char[maxTamU];
    v = new char[maxTamV];
    /* Fim */  

    time_t inicio, fim;
    time(&inicio);

    nomeArquivoSaida = "Saidas/khammingDiference2-outk"+NumberToString(k)+"-alpha-"+NumberToString(qtdSequenciasAlpha/2)+".csv";
    cout << "\033[1;33mAlgoritmo rodando... \033[0m\n";
    executaTodos(nomeArquivoSaida, maxTamU, maxTamV, k, qtdSequenciasAlpha, qtdSequenciasBeta, argv);

    time(&fim);
    formataTempo(difftime(fim,inicio));
    return 0;   
}

void contaQuantidadeSequencias(string arquivoEntrada, int *atributosAlpha)
{
    int qtd = 0, tamanho = 0;
    string line;

    ifstream arquivo(arquivoEntrada, std::ifstream::in);
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

void SSP(char *u, int tamU, char *v, int tamV, int k, int *err)
{
	int i, I, j, J, max, d, erros;
	char *str1, *str2;

	/* Laço externo */
	I = 0;
	J = tamV + 1;

	while (I < tamU)
	{
		/* Controlando o par ij*/
		if (J > 0)
			J = J - 1;
		else
			I = I + 1;

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

			do
			{
				if (err[i] < d)
					err[i] = d;
				i += 1;
				j += 1;
				d -= 1;
			}while(i != max && u[i-1] == v[j-1]);
		}
	}
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

void executaTodos(string nomeSaida, int maxTamU, int maxTamV, int k, int qtdSequenciaAlpha, int qtdSequenciasBeta, char** argv)
{
    char *u = new char[maxTamU], *v = new char[maxTamV];
    int *err = new int[maxTamU], nSequenceAlpha = 1, nSequenceBeta = 1, tamU, tamV, i;
    string aux = "", linha, nomeAlpha, nomeBeta;
    fstream out;

    out.open(nomeSaida, ios::out | ios::trunc);

	/* Abrindo o(s) arquivo(s) alpha e beta. */
    ifstream alpha(argv[1], std::ifstream::in);
    
    /* Escrevendo o cabecalho */
    /*for (i = 0; i < maxTamU; i++)
        aux += NumberToString(i)+",";
    out << aux+"\n";*/
    /* Fim escrita */

    while (!alpha.eof() && nSequenceAlpha <= qtdSequenciaAlpha)
    {
        if (flagDetalhes == 1)
            cout << "Sequência " << nSequenceAlpha << ": Algoritmo rodando...\n";

        ifstream beta(argv[2], std::ifstream::in);
        nSequenceBeta = 1;

        getline(alpha, nomeAlpha);
        getline(alpha, linha);

        tamU = linha.length();
        strcpy(u, linha.c_str());

        for (i = 0; i < maxTamU; i++)
            err[i] = k;

        while (!beta.eof() && nSequenceBeta <= qtdSequenciasBeta)
        {
            if (flagDetalhes == 1)
                cout << "Sequência " << nSequenceBeta << ": Algoritmo rodando...\n";
        
            getline(beta, nomeBeta);
            getline(beta, linha);

            tamV = linha.length();
            strcpy(v, linha.c_str());

            /* Executando o SSP para duas sequencias. */
            SSP(u, tamU, v, tamV, k, err);
            /* Fim execucao */
            nSequenceBeta++;
        }
        
        /* Analisando se alguma posição nao eh uma resposta valida. */
        for(i = 0; i < maxTamU; i++)
            if(err[i] > tamV || err[i] > tamU-i )
                err[i] = -1; 
        /* Fim analise nao candidatos */       

        /* Escrevendo no arquivo de saida. */
        out << nomeAlpha << "\n";

        aux = "";
        for (i = 0; i < tamU; i++)
            aux += NumberToString(i)+",";
        out << aux+"\n";

        aux = "";
        for (i = 0; i < maxTamU; i++)
        {
            aux += NumberToString(err[i]) + ",";
        }
        out << aux <<"\n";
        /* Fim escrita. */

        beta.close();
        nSequenceAlpha++;
    }
    alpha.close();
    out.close();
    cout << "\033[1;34m" << qtdSequenciaAlpha/2 << " sequência(s) analisada(s)." <<   "\033[0m\n";
    cout << "Arquivo criado: \033[1;34m" << nomeSaida << "\033[0m\n";
}

string NumberToString (int Number)
{
    ostringstream ss;
    ss << Number;
    return ss.str();
}