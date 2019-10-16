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
   
   Este codigo eh uma variação do khammingDiference1H2
   Heurisica: Dadas 2 probabilidades, escolhar a maior delas
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
#include <math.h>
#include <vector>

/* Flags */
#define flagMostraTabelaProbabilidade 0
#define flagEsperancaVariancia 0
#define flagDetalhes 0
#define limiteTabela 500

using namespace std;

/* Esta funcao calcula fatorial n! */
double fat(int m);

/* Esta funcao calcula a binomial: m escolhe k */
double binomial(int m, int k);

/* Esta funcao calcula probabilidade de uma sequencia
   de comprimento m possuir k diferencas. */
double probabilidadeComK(int m, int k);

/* Esta funcao calcula probabilidade de uma sequencia
   de comprimento n possuir k ou mais diferencas. */
double probabilidadeComPelosK(int n, int m, int k);

/* Esta funcao insere a probabilidade dos tamanhos 1 até maxTamU 
   terem pelo menos k diferencas em uma matriz de maxTamU linhas
   e duas colunas, sendo a primeira coluna (coluna 0) com a probabilidade
   do tamanho ter exatamente k dferencas, e a segunda coluna (coluna 1)
   com a probabilidade do tamanho ter pelo menos k diferencas. */
void insereDadosTabelaProbabilidades(double tabelaProbabilidades[][2], int maxTamU, int k, int *quantidade);

/* Esta funcao percorre o arquivo de entrada contando a quantidade de linhas e busca
   o tamanho da maior sequencia.  */
void contaQuantidadeSequencias(string arquivoEntrada, int *atributosAlpha);

/* Função que calcula as diferenças para uma posição i de u com cada posição j de v */
void SSP_Uma_Posicao(char *u, int tamU, char *v, int tamV, int k, int *err, int posicao);

/* Esta funcao calcula as diferenças para cada posição i de u com cada posição j de v */
void SSP_Heuristico(char *u, int tamU, char *v, int tamV, int k, int *err, double tabelaProbabilidades[][2], int esperanca, int variancia);

/* Esta funcao calcula o maior valor entre A, B e C
   Com um peso, devolve o maior valor entre A, B e C */
int MAX(double A, double B, double C, int dadoA, int dadoB, int dadoC);

/* Esta funcao calcula o maior valor entre A e B
   Se A >= B deolve A, caso contrario devolve B. */
int MAX2(int A, int B);

/* Esta funcao recebe um tempo em segundos e 
   imprime em tempo de horas. */
void formataTempo(int tempo);

/* Esta funcao calcula a esperanca e a variancia em cima de uma tabela de probabilidade */
void calculaEsperancaVarianca(double tabelaProbabilidades[][2], int maxTamU, int *esperanca, int *variancia);

/* Esta funcao executa a heuristica SSP_Heuristico em dois conjuntos. Dados dois conjuntos
   A e B , a funcao executaTodos executa cada sequência de A com todas de B */
void executaTodos(string nomeSaida, int maxTamU, int maxTamV, int k, int qtdSequenciaAlpha, int qtdSequenciasBeta, char** argv, double tabelaProbabilidades[][2], int esperanca, int variancia);

string NumberToString(int Number);

int main(int argc, char** argv)
{
    int k, atributosAlpha[2], atributosBeta[2], *err, esperanca, variancia, tipo, quantidade;
    double tabelaProbabilidades[limiteTabela][2], diferencas;
    string nomeArquivoSaida;
    char *u, *v;
    fstream out;

    if (argc != 4) {
        cout << "\033[1;31m Use: khammingDiference2H2-V2 'alpha' 'beta' k \033[0m\n";
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
    int qtdSequenciasBeta = atributosBeta[0], maxTamV = atributosBeta[1];
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

    nomeArquivoSaida = "Saidas/khammingDiference2H1-outk"+NumberToString(k)+"-alpha-"+NumberToString(qtdSequenciasAlpha/2)+".csv";
    cout << "\033[1;33mAlgoritmo rodando... \033[0m\n";
    executaTodos(nomeArquivoSaida, maxTamU, maxTamV, k, qtdSequenciasAlpha, qtdSequenciasBeta, argv, tabelaProbabilidades, esperanca, variancia);
    
    time(&fim);
    formataTempo(difftime(fim,inicio));
    return 0;
}

double fat(int m)
{
    if (m == 0) return 1;
    return m * fat(m-1);
}

double binomial(int m, int k)
{
    if (m < k)
        return 0;
    return fat(m)/(fat(k)*fat(m-k));
}

double probabilidadeComK(int m, int k)
{
    double numerador = binomial(m-1, k-1) * (pow(3,k-1));
    double denominador = (1.0 * pow(4,m-1));
    return (numerador / denominador) * (3.0/4);
}

double probabilidadeComPelosK(int n, int m, int k)
{
    int d = n - m + 1; 
    double multi, p = 0;
    for(int i = 2; i < m; i++)
        p += probabilidadeComK(i, k);
    multi = p;
    for(int i = 1; i < d; i++)
        multi *= p;
    return multi;
}

void insereDadosTabelaProbabilidades(double tabelaProbabilidades[][2], int maxTamU, int k, int *quantidade)
{
    double diferencas, prob = 0;
    int qtd = 0, i = 0;

    for (int j = 0; j < limiteTabela; j++)
    {
        tabelaProbabilidades[j][0] = 0;
        tabelaProbabilidades[j][1] = 0;
    }

    while (i < maxTamU && prob < 1)
    {
        qtd++;
        tabelaProbabilidades[i][0] = probabilidadeComPelosK(maxTamU, i, k);
        prob = tabelaProbabilidades[i][0];
        if (i == 1)
            tabelaProbabilidades[i][1] = tabelaProbabilidades[i][0];
        else
        {
            if ( tabelaProbabilidades[i][0] >= 1 )
                tabelaProbabilidades[i][1] = 0;
            else{
                diferencas = tabelaProbabilidades[i][0] - tabelaProbabilidades[i-1][0];
                tabelaProbabilidades[i][1] = diferencas;
            }
        }
        i++;
    }

    *quantidade = qtd;
    if (flagMostraTabelaProbabilidade == 1)
    {
        for (int i = 1; i < *quantidade; i++)
        {
            cout << i << " " << tabelaProbabilidades[i][0] << " " << tabelaProbabilidades[i][1] << endl;
        }
    }    
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

void SSP_Heuristico(char *u, int tamU, char *v, int tamV, int k, int *err, double tabelaProbabilidades[][2], int esperanca, int variancia)
{
    int posV, posU, h, j = 0, qtdErr, i, tamCand, executa = 0, dado, dadoAnterior, dadoMaior, dadoBase;
    int posVoltaEsperanca = (int) tamU * 0.25, pos = 0;
    int pula = (int) tamU / (log(tamU) / log(2));    
    int tamanhoVetor = tamU / pula;
    int posicoes[tamanhoVetor][2];
    j = 0;

    /* Executando o SSP para duas sequencias em algumas posições. */
    for (posU = 0; posU < tamU; posU += pula)
    {  
        //cout << "SSP " << posU << endl;
        SSP_Uma_Posicao(u, tamU, v, tamV, k, err, posU);
        posicoes[pos][0] = posU;
        posicoes[pos][1] = err[posU];  
        pos++;
    }
    /* Fim execucao */

    pos = 0; int posI = 0, posJ = 0;
    while (pos < tamanhoVetor - 1)
    {
        posI = posicoes[pos][0];
        posJ = posicoes[pos+1][0];
        i = posI + 1;
        dado = dadoBase = err[i-1];
        while (i < tamU && posJ < tamU && i < posJ)
        {
            if (dado <= dadoBase - variancia || dado >= dadoBase + variancia)
            {
                dado = dadoBase;
            }         

            if (dado >= limiteTabela)
                dadoMaior = MAX(0.33, 0.33, 0.33, dado + 1, dado - 1, dado);
            else{
                dadoMaior = MAX(tabelaProbabilidades[dado + 1][1], tabelaProbabilidades[dado - 1][1], tabelaProbabilidades[dado][1], dado + 1, dado - 1, dado);
            }
	        if(dadoMaior > tamV || dadoMaior > tamU-i)
            {
    	        err[i] = err[i-1] - 1;
                dado = err[i];   
            }
            else
            {
                err[i] = MAX2(err[i],dadoMaior);
                dado = dadoMaior;
            }
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

void SSP_H(int tamU, int tamV, int *err, int posicoes[][2], double tabelaProbabilidades[][2], int esperanca, int variancia)
{
    int j = 0, i, dado, dadoMaior, dadoBase, pos, pula = (int) tamU / (log(tamU) / log(2));    
    int tamanhoVetor = tamU / pula;

    /* Heuristica */
    pos = 0; int posI = 0, posJ = 0;
    while (pos < tamanhoVetor - 1)
    {
        posI = posicoes[pos][0];
        posJ = posicoes[pos+1][0];
        i = posI + 1;
        dado = dadoBase = err[i-1];
        //cout << "Entre: " << i << "(" << err[i-1] << ")" << ":" << posJ  << "(" << err[posJ] << ")" << endl;
        while (i < tamU && posJ < tamU && i < posJ)
        {
            if (dado <= dadoBase - variancia || dado >= dadoBase + variancia)
            {
                dado = dadoBase;
            }         

            /* cout << dado + 1 << ":" << tabelaProbabilidades[dado + 1][1] << " ";
            cout << dado << ":" << tabelaProbabilidades[dado][1] << " ";
            cout << dado - 1 << ":" << tabelaProbabilidades[dado +- 1][1] << " ";*/
            if (dado >= limiteTabela)
                dadoMaior = MAX(0.33, 0.33, 0.33, dado + 1, dado - 1, dado);
            else{
                dadoMaior = MAX(tabelaProbabilidades[dado + 1][1], tabelaProbabilidades[dado - 1][1], tabelaProbabilidades[dado][1], dado + 1, dado - 1, dado);
            }
            if(dadoMaior > tamV || dadoMaior > tamU-i)
            {
                err[i] = err[i-1] - 1;
                dado = err[i];   
            }
            else
            {
                err[i] = dadoMaior;
                dado = dadoMaior;
            }
            //cout << err[i] << endl;            
            i++;
        }
        pos++;
    }
    /* Fim heuristica */
}

int MAX(double A, double B, double C, int dadoA, int dadoB, int dadoC)
{
    int auxA = (int) (A*100), auxB = (int) (B*100), auxC =(int) (C*100), dado;
    int total = auxA + auxB + auxC;
    if (total > 0)
        dado = rand() % total + 1;
    else
        dado = 0;

    if (dado <= auxA)
        return dadoA;
    else if (dado > auxA && dado <= (auxA + auxB))
        return dadoB;
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

void calculaEsperancaVarianca(double tabelaProbabilidades[][2], int maxTamU, int *esperanca, int *variancia)
{
    int i;
    double auxEsperanca = 0, auxVariancia;
    /* Calculando a esperanca. */
    for(i = 0; i < maxTamU; i++)
    {
        auxEsperanca += (tabelaProbabilidades[i][1]*i);
        auxVariancia += (tabelaProbabilidades[i][1]*(pow(i,2)));
    }
    *esperanca = auxEsperanca;
    /* Esperanca calculada. */

    /* Calculando a variancia. */
    *variancia = auxVariancia - pow(auxEsperanca,2);
    /* Variancia calculada. */
}

void executaTodos(string nomeSaida, int maxTamU, int maxTamV, int k, int qtdSequenciaAlpha, int qtdSequenciasBeta, char** argv, double tabelaProbabilidades[][2], int esperanca, int variancia)
{
    char *u = new char[maxTamU], *v = new char[maxTamV];
    int *err = new int[maxTamU], nSequenceAlpha = 1, nSequenceBeta = 1, tamU, tamV, i, quantidade;
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
        ifstream beta(argv[2], std::ifstream::in);
        nSequenceBeta = 1;

        getline(alpha, nomeAlpha);
        getline(alpha, linha);
        tamU = linha.length();
        strcpy(u, linha.c_str());

        for (i = 0; i < maxTamU; i++)
            err[i] = k;

        if (flagEsperancaVariancia == 1)
            cout << "Gerando tabela de probabilidades." << endl;
        // Gerando a tabela de probabidades. 
        insereDadosTabelaProbabilidades(tabelaProbabilidades, tamU, k, &quantidade);
        // Fim geração da tabela. 
        if (flagEsperancaVariancia == 1)        
            cout << "Tabela gerada!" << endl;

        if (flagEsperancaVariancia == 1)
            cout << "Consultando a Esperança e Variância." << endl;
        // Calculando a Esperanca e a Variancia
        calculaEsperancaVarianca(tabelaProbabilidades, quantidade, &esperanca, &variancia);
        // Fim calculo
        if (flagEsperancaVariancia == 1)
        {
            cout << "Esperança: " << esperanca << endl;
            cout << "Variância: " << variancia << endl;                
        }
        /* Variáveis para o SSP uma posição */
        int pula = (int) tamU / (log(tamU) / log(2));    
        int tamanhoVetor = tamU / pula;
        int posicoes[tamanhoVetor][2];

        while (!beta.eof() && nSequenceBeta <= qtdSequenciasBeta)
        {
            if (flagDetalhes == 1)
                cout << "Sequência Alpha " << nSequenceAlpha << " Sequência Beta " << nSequenceBeta << ": Algoritmo rodando...\n";
        
            getline(beta, nomeBeta);
            getline(beta, linha);
            tamV = linha.length();
            strcpy(v, linha.c_str());

            SSP_P(u, tamU, v, tamV, k, err, posicoes);
            nSequenceBeta++;
        }

        /* Heuristica */
        SSP_H(tamU, tamV, err, posicoes, tabelaProbabilidades, esperanca, variancia);
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