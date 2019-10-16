/**
   Autor: Lucas Barbosa Rocha
   Orientador: Francisco Eloi
   Coorientador: Said Sadique
   Ano: 2019
   FACOM: Mestrado em Ciência da Computação

   Este projeto implementa o algoritmo para encontrar segmentos específicos
   utilizando a distância de Hamming como função objetivo

   khammingDiference1 é uma versão cúbica para resolução do problema
   Esta versão é uma Heurística baseada na esperança e variância 
   Que quando necessário, utiliza a versão cúbica para rodar para uma 
   determinada posição. To do: Explicar melhor
   
   Heurisica:  Utiliza a equação da reta
   Roda o algoritmo ótimo para algumas posições e liga uma reta entre os pontos
   os chutes são em torno da reta formada entre dois pontos.
*/

/* Bibliotecas */
#include <ctype.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <time.h>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <vector>

/* Flags */
#define flagDetalhes 0

using namespace std;

/* Esta funcao percorre o arquivo de entrada contando a quantidade de linhas e busca
   o tamanho da maior sequencia.  */
void contaQuantidadeSequencias(string arquivoEntrada, int *atributosAlpha);

/* Função que calcula as diferenças para uma posição i de u com cada posição j de v */
void SSP_Uma_Posicao(char *u, int tamU, char *v, int tamV, int k, int *err, int posicao);

/* Esta funcao calcula as diferenças para cada posição i de u com cada posição j de v 
   A funcao aqui em questao realiza a equação geral da reta para ligar dois pontos
   e então realizar palpites de valores com base na reta entre os dois pontos. */
void SSP_Heuristico(char *u, int tamU, char *v, int tamV, int k, int *err);

void SSP_P(char *u, int tamU, char *v, int tamV, int k, int *err, int posicoesPlanoCartesiano[][2]);

void SSP_H(int tamU, int tamV, int *err, int posicoesPlanoCartesiano[][2]);

/* Esta funcao calcula o maior valor entre A e B
   Se A >= B deolve dadoA, caso contrario devolve dado B*/
int MAX(double A, double B, double C, int dadoA, int dadoB, int dadoC);

/* Esta funcao calcula o maior valor entre A e B
   Se A >= B deolve A, caso contrario devolve B. */
int MAX2(int A, int B);

/* Esta funcao recebe um tempo em segundos e 
   imprime em tempo de horas. */
void formataTempo(int tempo);

/* Esta funcao executa a heuristica SSP_Heuristico em dois conjuntos. Dados dois conjuntos
   A e B , a funcao executaTodos executa cada sequência de A com todas de B */
void executaTodos(string nomeSaida, int maxTamU, int maxTamV, int k, int qtdSequenciaAlpha, int qtdSequenciaBeta, char** argv);

string NumberToString(int Number);

int main(int argc, char** argv)
{
    int k, atributosAlpha[2], atributosBeta[2], *err, esperanca, variancia, tipo;
    double tabelaProbabilidades[500][2], diferencas;
    string nomeArquivoSaida;
    char *u, *v;
    fstream out;

    if (argc != 4) {
        cout << "\033[1;31m Use: khammingDiference2H3 'alpha' 'beta' k \033[0m\n";
        cout << "\033[1;34m alpha é o conjunto A com uma ou mais sequências \033[0m\n";
        cout << "\033[1;34m beta é o conjunto B com uma ou mais sequências \033[0m\n";
        cout << "\033[1;34m k é a quantidade de diferenças \033[0m\n";
        return 0;
    }

    /* Criando a pasta Saidas para armazenar os arquivos de saida. */
    system("if [ ! -d 'Saidas' ]; then  mkdir Saidas;  fi");

    /* Lendo os arquivos de entrada para buscar a quantidade e o 
        tamanho da maior sequencia. */
    cout << "Lendo os arquivos de entrada em busca do tamanho da maior sequência." << endl;
    contaQuantidadeSequencias(argv[1], atributosAlpha);
    contaQuantidadeSequencias(argv[2], atributosBeta);
    int qtdSequenciasAlpha = atributosAlpha[0], maxTamU = atributosAlpha[1];
    int qtdSequenciaBeta = atributosBeta[0], maxTamV = atributosBeta[1];
    cout << "Leitura terminada!" << endl;
    /* Fim leitura */

    /* Abrindo o(s) arquivo(s) de entrada e fixando
       alguns valores. */
    ifstream alpha(argv[1], std::ifstream::in);
    ifstream beta(argv[2], std::ifstream::in);
    k = atoi(argv[3]);
    err = new int[maxTamU];
    u = new char[maxTamU];
    v = new char[maxTamV];
    /* Fim */

    time_t inicio, fim;
    time(&inicio);

    nomeArquivoSaida = "Saidas/khammingDiference2H2-outk"+NumberToString(k)+"-alpha-"+NumberToString(qtdSequenciasAlpha/2)+".csv";
    cout << "\033[1;33mAlgoritmo rodando... \033[0m\n";
    executaTodos(nomeArquivoSaida, maxTamU, maxTamV, k, qtdSequenciasAlpha, qtdSequenciaBeta, argv);
    
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

void SSP_Heuristico(char *u, int tamU, char *v, int tamV, int k, int *err)
{
    int posV, posU, h, j = 0, qtdErr, i, tamCand, dado, dadoAnterior, dadoMaior, dadoBase;
    int pula = tamU * 0.25;
    int tamanhoVetor = tamU/pula;
    int posicoesPlanoCartesiano[tamanhoVetor][2], pos = 0, valorInteiro;
    double valorReta, valor, dif;

    /* Executando o SSP para duas sequencias em algumas posições. */
    for (posU = 0; posU < tamU; posU += pula)
    {  
        SSP_Uma_Posicao(u, tamU, v, tamV, k, err, posU);
        posicoesPlanoCartesiano[pos][0] = posU;
        posicoesPlanoCartesiano[pos][1] = err[posU];   
        pos++;
    }
    /* Fim execucao */


    /* Começando a heristica, colocar os valores que estão na reta (ou proximos) */
    int Xi, Yj, Xii, Yjj, maior;
    double m;
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
            valorReta = err[i-1] - Yj - m*i + m*Xi;
            valor = Yj + m*i - m*Xi;
            valorInteiro = valor;
            dif = valor - valorInteiro; 
            if (dif > 0.5)
                err[i] = ceil(valor);
            else
                err[i] = floor(valor);
        i++;
        }
        pos++;
    }
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

/* Terminar de fazer a função max para a heuristica */
int MAX(double A, double B, double C, int dadoA, int dadoB, int dadoC)
{   
   if (A >= B && A >= C)
   {
       return dadoA;
   }
   else if (B >= A && B >= C)
   {
       return dadoB;
   }
   else
       return dadoC;
}

int MAX2(int A, int B)
{
    if (A >= B) return A;
    else return B;
}

void formataTempo(int tempo)
{
    int horas, minutos, horas_seg = 3600, segundos;

    horas = (tempo/horas_seg); /* resultado da hora */
    minutos = (tempo -(horas_seg*horas))/60;
    segundos = (tempo -(horas_seg*horas)-(minutos*60));
    cout << "Tempo de execução: " << "\033[1;35m" << horas << " hora(s) " << minutos << " minuto(s) " << segundos << " segundo(s)." <<    "\033[0m\n";    
}

void executaTodos(string nomeSaida, int maxTamU, int maxTamV, int k, int qtdSequenciaAlpha, int qtdSequenciaBeta, char** argv)
{
    char *u = new char[maxTamU], *v = new char[maxTamV];
    int *err = new int[maxTamU], nSequenceAlpha = 1, nSequenceBeta = 1, tamU, tamV, i, tamanhoVetor;
    string aux = "", linha, nomeAlpha, nomeBeta;
    fstream out;

    out.open(nomeSaida, ios::out | ios::trunc);

	/* Abrindo o(s) arquivo(s) alpha e beta. */
    ifstream alpha(argv[1], std::ifstream::in);
    
    /* Escrevendo o cabecalho */
    /*for (i = 0; i < maxTamU; i++)
    {
        aux += NumberToString(i)+",";
    }
    out << aux+"\n";*/
    /* Fim escrita */


    while (!alpha.eof() && nSequenceAlpha <= qtdSequenciaAlpha)
    {
        ifstream beta(argv[2], std::ifstream::in);
        nSequenceBeta = 1;

        getline(alpha, nomeAlpha);
        getline(alpha, linha);
        tamU = linha.length();
        strcpy(u, linha.c_str());

        for (i = 0; i < tamU; i++)
            err[i] = k;

        /* Variáveis para ssp uma posição */    
        int pula = (int) tamU / (log(tamU) / log(2));
        tamanhoVetor = tamU/pula;
        int posicoesPlanoCartesiano[tamanhoVetor][2], pos = 0, valorInteiro;

        while (!beta.eof() && nSequenceBeta <= qtdSequenciaBeta)
        {
            if (flagDetalhes == 1)
                cout << "Sequência Alpha " << nSequenceAlpha << " Sequência Beta " << nSequenceBeta << ": Algoritmo rodando...\n";
        
        
            getline(beta, nomeBeta);
            getline(beta, linha);
            tamV = linha.length();
            strcpy(v, linha.c_str());

            /* Executando o sequencial para duas sequencias em algumas posicoes. */
            SSP_P(u, tamU, v, tamV, k, err, posicoesPlanoCartesiano);           
            /* Fim execucao */
            nSequenceBeta++;
        }

        /* Rodando a heuristica */
        SSP_H(tamU, tamV, err, posicoesPlanoCartesiano);
        /* Fim Heuristica */

        /* Analisando se alguma posição nao eh uma resposta valida. */
        for(i = 0; i < tamU; i++)
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