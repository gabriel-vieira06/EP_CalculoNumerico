#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

#define EPSILON 0.00000001	//10^-8

void abreMenu();
void conversaoNumerica();
void recebeSistemaLinear();
void lerEquacaoAlgebrica();

void main()
{
	
	abreMenu();
	
}	// Fim main

void abreMenu(){
	
	/*
		Funcao que imprime ao usuario um menu com 4 opcoes:
		�C� � Conversao, �S� � Sistema Linear, �E� � Equacao Algebrica e �F� � Finalizar.
		E entao realiza a chamada do metodo solicitado por ele.
	*/

	char operacao;

	void gotoxy(int x, int y)
	{
		/*
			Fun��o auxiliar que manipula a localizacao do cursor no console
			a fim de tornar o menu principal mais pratico e intuitivo.
			Recebe como parametro a coluna x e a linha y onde o cursor deve se mover.
		*/
		
	  	COORD coordenada;
	  	coordenada.X = x;
	  	coordenada.Y = y;
	  	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordenada);
	}	// Fim gotoxy
	
	do {
		printf("\n\n\t");
        system("pause");
        system("cls");
        gotoxy(5,  1); printf("MENU PRINCIPAL - Exercicio-programa");
        gotoxy(5,  3); printf("[ C ] Conversao");
        gotoxy(5,  4); printf("[ S ] Sistema Linear");
        gotoxy(5,  5); printf("[ E ] Equacao Algebrica");
        gotoxy(5,  6); printf("[ F ] Finalizar");
        gotoxy(5,  7); printf("[ ? ] Opcao Desejada (Case Sensitive)");
    
        do {
        	gotoxy(5,  7); printf("[ ? ] Opcao Desejada (Case Sensitive)");
        	gotoxy(7, 7); scanf("%c", &operacao);
        } while (operacao != 'C' && operacao != 'S' && operacao != 'E' && operacao != 'F');
    
    	switch (operacao) {
    		case 'C': conversaoNumerica(); break;
    		case 'S': recebeSistemaLinear(); break;
    		case 'E': lerEquacaoAlgebrica(); break;
		}
    
	}while(operacao!='F');
	
}	// Fim abreMenu

void conversaoNumerica(){
	
	/*
		Le um valor decimal e mostra seu valor
		na base binaria, octal e hexadecimal.
		
		OBS: O ideal seria que a variavel decimal fosse do tipo
		long double, mas ao faze-lo ocorrem erros de execucao,
		e mesmo depois de adaptar o algoritmo, os erros persistem.
		Tendo isso em mente, ha a possibilidade de valores com mais de
		10 casas decimais, apresentarem conversoes inadequadas.
	*/
	
	double decimal;
	char hexadecimal[100], octal[100], binario[100];
	
	void converteDecimal(int base, char* valorConvertido)
	{
		/*
			Converte o valor decimal dado pelo usuario.
			A base da conversao e a string onde a conversao sera armazenada
			sao passados como parametros.
			
			ex: converteDecimal(2, binario), armazena na string binario a conversao
			do valor decimal para um sistema numerico posicional de base 2
		*/
		
		int parte_inteira = (int) decimal;
		double parte_fracionaria = decimal - parte_inteira;
		int posicao = 1, valor_temp, i;
		char aux;
		
		if(parte_inteira < 0)				//	Verifica o sinal do decimal e garante que a conversao
		{									//	tera o mesmo sinal
			valorConvertido[0] = '-';
			parte_inteira *= -1;
		} else valorConvertido[0] = ' ';
		
		if(parte_fracionaria < 0) parte_fracionaria *= -1;
		
		while(parte_inteira != 0)					//	Convertendo a parte inteira, realiza-se divisoes inteiras
		{											//	e armazenamos o valor do resto
			valor_temp = parte_inteira % base;
			if(valor_temp < 10) valor_temp += 48;	//	Se o valor for menor que 10, guarda-se o caractere do numero equivalente
			else valor_temp += 55;					//	Senao, guarda-se o caractere da letra equivalente
			valorConvertido[posicao] = valor_temp;
			posicao++;
			parte_inteira /= base;
		}
		
		for(i = 0; i < posicao/2; i++)				//	Swap dos valores da parte inteira, do contrario
		{											//	os valores da parte inteira apareceriam invertidos
			aux = valorConvertido[i+1];
			valorConvertido[i+1] = valorConvertido[posicao-i-1];
			valorConvertido[posicao-i-1] = aux;
		}
		
		if(parte_fracionaria != 0){						// Se existir parte fracionaria, separa por um ponto
			valorConvertido[posicao] = '.';
			posicao++;
		}
		
		while(parte_fracionaria != 0)					//	Convertendo a parte fracionaria, multiplicamos pela base
		{												//	e armazenamos o valor da parte inteira
			valor_temp = parte_fracionaria * base;
			if(valor_temp < 10) valor_temp += 48;
			else valor_temp += 55;
			valorConvertido[posicao] = valor_temp;
			posicao++;
			parte_fracionaria *= base;
			parte_fracionaria -= (int) parte_fracionaria;	//	Garante que a parte fracionaria nao possua um valor inteiro
		}
		
		valorConvertido[posicao] = '\0';
		
	}	// Fim converteDecimal
	
	system("cls");
	printf("\n\tDigite um numero decimal: ");
	scanf("%lf", &decimal);
	
	converteDecimal(2, binario);
	converteDecimal(8, octal);
	converteDecimal(16, hexadecimal);
	
	printf("\n\tBinario: %s\n", binario);
	printf("\tOctal: %s\n", octal);
	printf("\tHexadecimal: %s", hexadecimal);
	
}	// Fim conversaoNumerica

void recebeSistemaLinear()
{
	/*	
		A funcao le o nome de um arquivo de texto passado pelo usuario
		que contenha: o numero de variaveis de um sistema linear
		seguido pelo sistema linear em forma de matriz.
		
		A funcao imprime o sistema e se ele atende ou nao aos criterios das linhas
		e das colunas. Em caso positivo (de um dos criterios ser atendido), 
		a funcao realiza o calculo de uma solucao aproximada do sistema linear
		utilizando o Metodo de Gauss-Seidel.
	*/
	
	char nomeArquivo[50];
	FILE *arquivo;
	int numeroVariaveis, i, j;
	double **m, *solucoes, *somatorioLinhas, *somatorioColunas;
	int satisfazLinhas = 1, satisfazColunas = 1;
	
	double** criaMatriz (int l, int c)
	{	
		/* 	
			Se houver memoria disponivel, cria uma matriz de double
			com l linha e c colunas e devolve um ponteiro para a matriz;
			Caso contrario, devolve um ponteiro nulo.
		*/
	
		int i, j;
		double** m;
		
		m = malloc(sizeof (double*) * l);
		
		if (m == NULL)	return NULL;	// Falta de memoria.
			
		for (i = 0; i < l; i++)
		{
			m[i] = malloc(sizeof(double) * c);
			if (m[i] == NULL)		// Falta de memoria.
			{
				for (j = 0; j < i; j++) free (m[j]);
				free (m);
				return NULL;
			}
		}
	
		return m;
		
	}	// Fim criaMatriz
	
	void metodoGaussSeidel()
	{
		/*
			A partir da solucao trivial, todas as variaveis = 0, calcula solucoes aproximadas
			utilizando o Metodo de Gauss-Seidel e armazena na variavel de solucoes.
			O algoritmo para de calcular quando a variacao nas aproximacoes seja menor que 10^-8
			ou quando alcancar 1000 iteracoes.
			Quando encerrada, a funcao imprime o numero de iteracoes que realizou.
		*/
		
		int i, j, contaIteracoes;
		double auxiliar;
		
		for(i = 0; i < numeroVariaveis; i++) solucoes[i] = 0;	// Inicializa com solucao trivial
		
		for(contaIteracoes = 0; contaIteracoes < 1000; contaIteracoes++)
		{
			for(i = 0; i < numeroVariaveis; i++)
			{
				auxiliar = m[i][numeroVariaveis];					// Termo independente da linha i
				for(j = 0; j < numeroVariaveis; j++)
				{
					if(i != j) auxiliar += - m[i][j]*solucoes[j];
				}
				if(fabs(solucoes[i] - auxiliar/m[i][i]) < EPSILON)		//	Se a variacao da aproximacao foi menor que 10^-8, encerra o metodo
				{
					printf("\n\n\tIteracoes: %d\n", contaIteracoes);
					return;
				}
				solucoes[i] = auxiliar/m[i][i];
			}
		}
		
		printf("\n\n\tIteracoes: %d\n", contaIteracoes);
				
	}	// Fim metodoGaussSeidel
	
	system("cls");
	printf("\n\tDigite o nome do arquivo a ser lido: ");
	scanf("%s", nomeArquivo);
	
	arquivo = fopen(nomeArquivo, "rt");
	
	if(arquivo == NULL)
	{
		printf("\n\n\tArquivo nao encontrado.\n");
		return;
	}
	
	fscanf(arquivo, "%d", &numeroVariaveis);
	
	m = criaMatriz(numeroVariaveis, numeroVariaveis + 1);
	solucoes = malloc(sizeof(double) * numeroVariaveis);
	somatorioLinhas = malloc(sizeof(double) * numeroVariaveis);
	somatorioColunas = malloc(sizeof(double) * numeroVariaveis);
	
	if(m == NULL || somatorioColunas == NULL || somatorioLinhas == NULL || solucoes == NULL)
	{
		printf("\n\n\tFaltou memoria.\n");
		return;
	}
	
	for(i = 0; i < numeroVariaveis; i++)			// le a matriz do sistema linear
	{
		for (j = 0; j < numeroVariaveis + 1; j++) fscanf(arquivo, "%lf", &m[i][j]);
	}
	
	somatorioLinhas[numeroVariaveis-1] = 0;		// remove lixo presente no fim do ponteiro
	somatorioColunas[numeroVariaveis-1] = 0;	// remove lixo presente no fim do ponteiro
	
	for(i = 0; i < numeroVariaveis; i++)
	{
		for(j = 0; j < numeroVariaveis; j++)
		{
			if(i != j)
			{
				somatorioLinhas[i] += fabs(m[i][j]);	//	Soma os coeficientes das linhas, exceto o da diagonal principal
				somatorioColunas[j] += fabs(m[i][j]);	//	Soma os coeficientes das colunas, exceto o da diagonal principal
			}
		}
	}
	
	for(i = 0; i < numeroVariaveis; i++)
	{
		if(fabs(m[i][i]) <= somatorioColunas[i] && satisfazColunas)		// Verifica o criterio das colunas
		{
			printf("\n\tO sistema linear nao satisfaz o criterio das colunas.");
			satisfazColunas = 0;
		}
		
		if(fabs(m[i][i]) <= somatorioLinhas[i] && satisfazLinhas)		// Verifica o criterio das linhas
		{
			printf("\n\tO sistema linear nao satisfaz o criterio das linhas.");
			satisfazLinhas = 0;
		}
	}
	
	if(satisfazLinhas || satisfazColunas)
	{
		if(satisfazLinhas) printf("\n\tO sistema linear satisfaz o criterio das linhas.");
		if(satisfazColunas) printf("\n\tO sistema linear satisfaz o criterio das colunas.");
		metodoGaussSeidel();
		for(i = 0; i< numeroVariaveis; i++) printf("\n\tx[%d] = %lf", i+1, solucoes[i]);
	}
	
	fclose(arquivo);
	
}	// Fim recebeSistemaLinear

void lerEquacaoAlgebrica(){
	printf("\n\tEquacao Algebrica\n\n");
}