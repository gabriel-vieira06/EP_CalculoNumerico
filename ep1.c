/*
	Integrantes da equipe:
	
	- Gabriel Vieira Cavalcante
	- Luiz Matheus Sena Macedo
	- Mateus Rodrigues Alves
	- Gilberto Magno Souza Vieira
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 0.00000001	//10^-8

// Funcoes primarias

void abreMenu();
void conversaoNumerica();
void recebeSistemaLinear();
void lerEquacaoAlgebrica();

// Funcoes auxiliares

void converteDecimal(int base, char* valorConvertido, double decimal);
double** criaMatriz (int l, int c);
void metodoGaussSeidel(int numeroVariaveis, double* solucoes, double** m);
void teoremaLagrange(double n, double an, double* coeficientes, double* limites, int contLimites);
void metodoNewton(double* limites, int grauEquacao, double* coeficientes);
void swap(double *a, double *b);

void main()
{
	
	abreMenu();
	
}	// Fim main

void abreMenu()
{	
	/*
		Funcao que imprime ao usuario um menu com 4 opcoes:
		‘C’ – Conversao, ‘S’ – Sistema Linear, ‘E’ – Equacao Algebrica e ‘F’ – Finalizar.
		E entao realiza a chamada do metodo solicitado por ele.
	*/

	char operacao;
	
	do {
		printf("\n\n\tMENU PRINCIPAL - Exercicio-programa");
        printf("\n\t[ C ] Conversao");
        printf("\n\t[ S ] Sistema Linear");
        printf("\n\t[ E ] Equacao Algebrica");
        printf("\n\t[ F ] Finalizar");
        printf("\n\tOpcao Desejada (Case Sensitive): ");
    
        do {
        	operacao = getchar();
        } while (operacao != 'C' && operacao != 'S' && operacao != 'E' && operacao != 'F');
    
    	switch (operacao) {
    		case 'C': conversaoNumerica(); break;
    		case 'S': recebeSistemaLinear(); break;
    		case 'E': lerEquacaoAlgebrica(); break;
		}
    
	} while(operacao!='F');
	
}	// Fim abreMenu

void conversaoNumerica()
{	
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
	
	printf("\n\tDigite um numero decimal: ");
	scanf("%lf%*c", &decimal);
	
	converteDecimal(2, binario, decimal);
	converteDecimal(8, octal, decimal);
	converteDecimal(16, hexadecimal, decimal);
	
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
	
	printf("\n\tDigite o nome do arquivo a ser lido: ");
	scanf("%s%*c", nomeArquivo);
	
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
		metodoGaussSeidel(numeroVariaveis, solucoes, m);
		for(i = 0; i< numeroVariaveis; i++) printf("\n\tx[%d] = %lf", i+1, solucoes[i]);
	}
	
	for (i = 0; i < numeroVariaveis; i++) free(m[i]);
	free (m);
	free(solucoes);
	free(somatorioLinhas);
	free(somatorioColunas);
	
	fclose(arquivo);
	
}	// Fim recebeSistemaLinear

void lerEquacaoAlgebrica()
{
	/*
		Le o grau de uma equacao algebrica, em seguida
		le os coeficientes a[n] ate a[0], printa erro caso a[n] <= 0 ou a[0] = 0.
		
		A funcao utiliza o Teorema de Lagrange para calcular os limites das raizes da equacao
		e tambem determina uma aproximacao de uma raiz utilizando o Metodo de Newton.
	*/
	
	int grauEquacao, i, contLimites = 0;
	double *coeficientes, limites[4];
	
	printf("\n\tDigite o grau da equacao: ");
	scanf("%d%*c", &grauEquacao);
	
	coeficientes = malloc(sizeof(double) * grauEquacao + 1);
	
	if(coeficientes == NULL)
	{
		printf("\n\n\tFaltou memoria.\n");
		return;
	}
	
	for(i = 0; i <= grauEquacao; i++)
	{
		printf("\n\tDigite a[%d]: ", grauEquacao - i);
		scanf("%lf%*c", &coeficientes[i]);
	}

	if(coeficientes[0] <= 0)
	{
		printf("\n\tErro: a[%d] <= 0, encerrando operacao.", grauEquacao);
		return;
	}
	
	if(coeficientes[grauEquacao] == 0) 
	{
		printf("\n\tErro: a[0] = 0, encerrando operacao.");
		return;
	}
	
	teoremaLagrange(grauEquacao, coeficientes[0], coeficientes, limites, contLimites);	// Calcula L para limite superior das raizes positivas.
	contLimites++;
	
	for(i = 0; i < (grauEquacao+1)/2; i++)
	{
		swap(&coeficientes[i], &coeficientes[grauEquacao-i]);	// Invertemos a ordem dos coeficientes para obter X^n . p(1/x)
	}
	
	teoremaLagrange(grauEquacao, coeficientes[0], coeficientes, limites, contLimites);	// Calcula L1 para o Limite inferior das raizes positivas.
	contLimites++;
	
	for(i = 0; i <= grauEquacao; i++)
	{
		if((grauEquacao-i) % 2) coeficientes[i] *= -1;	//	Trocamos o sinal dos coeficientes de indice impar para obter X^n . p(-1/x)
	}
	
	teoremaLagrange(grauEquacao, coeficientes[0], coeficientes, limites, contLimites);	// Calcula L3 para o limite superior das raizes negativas
	contLimites++;
	
	for(i = 0; i < (grauEquacao+1)/2; i++)				//	Invertemos a ordem dos coeficientes para obter p(-x)
	{											
		swap(&coeficientes[i], &coeficientes[grauEquacao-i]);
	}
	
	teoremaLagrange(grauEquacao, coeficientes[0], coeficientes, limites, contLimites);	// Calcula L2 para o limite inferior das raizes negativas
	
	printf("\n\tLimites das raizes positivas: ");
	printf("\n\t%lf <= x+ <= %lf", 1/limites[1], limites[0]);
	printf("\n\tLimites das raizes negativas: ");
	printf("\n\t%lf <= x- <= %lf", -limites[3], -1/limites[2]);
	
	metodoNewton(limites, grauEquacao, coeficientes);
	
	free(coeficientes);
	
}	// Fim lerEquacaoAlgebrica

void converteDecimal(int base, char* valorConvertido, double decimal)
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
		
	if(decimal < 0)						//	Verifica o sinal do decimal e garante que a conversao
	{									//	tera o mesmo sinal
		valorConvertido[0] = '-';
		parte_inteira *= -1;
	} else valorConvertido[0] = ' ';
	
	if(parte_inteira == 0)				// Caso nao haja parte inteira, a conversao tambem nao tera
	{
		valorConvertido[posicao] = '0';
		posicao++;
	}
	
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
	
void metodoGaussSeidel(int numeroVariaveis, double* solucoes, double** m)
{
	/*
		A partir da solucao trivial, todas as variaveis = 0, calcula solucoes aproximadas
		utilizando o Metodo de Gauss-Seidel e armazena no vetor de solucoes.
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
		
	printf("\n\n\tIteracoes: %d", contaIteracoes);
				
}	// Fim metodoGaussSeidel

void teoremaLagrange(double n, double an, double* coeficientes, double* limites, int contLimites)
{
	/*
		Aplica o Teorema de Lagrange utlizando sua equacao caracteristica
		e guarda o resultado no vetor de limites para ser exibido ao usuario.
	*/
	
	double k = 0, b = 0;		// n: grau do polinomio, k: maior indice dos coeficientes negativos do polinomio
									// an: coeficiente de indice n, b: modulo do menor coeficiente negativo
	int i;
		
	if(an < 0)											// Caso an seja negativo, k = n e teriamos uma raiz de indice 0
	{													// se multiplicarmos o polinomio por -1 evitamos esse problema
		for(i = 0; i<= n; i++) coeficientes[i] *= -1;
		an *= -1;
	}
	
	for(i = 0; i <= n; i++)
	{
		if(coeficientes[i] < 0 && k == 0) k = n-i;
			if(coeficientes[i] < 0 && coeficientes[i] < b) b = coeficientes[i];
	}
	
	b = fabs(b);	// guardamos o modulo de b
	limites[contLimites] = 1 + pow(b/an, 1.0/(n-k));
	
}	// Fim teoremaLagrange
	
void metodoNewton(double* limites, int grauEquacao, double* coeficientes)
{
	/*
		Utiliza o limite superior das raizes positivas da equacao como x[0]
		e aplica o Metodo de Newton para o calculo de uma raiz aproximada.
		
		Ao final, imprime o resultado obtido e as iteracoes realizadas quando a variacao 
		entre o modulo de duas aproximacoes for menor que 10^-8 ou quando ocorrerem 1000 iteracoes.
	*/
		
	double aux, *derivada, x = limites[0], fx[2];	// x: aproximacao resultante
	int contaIteracoes, i, j;						// fx: resultado do polinomio (e sua derivada) no ponto x
	
	derivada = malloc(sizeof(double) * grauEquacao);
	if(derivada == NULL)
	{
		printf("\n\n\tFaltou memoria.\n");
		return;
	}
	
	for(i = 0; i < grauEquacao; i++)
	{
		if((grauEquacao-i) % 2) coeficientes[i] *= -1;		//	Trocamos o sinal dos coeficientes de indice impar para obter p(x) original
		derivada[i] = coeficientes[i] * (grauEquacao - i);	//	Encontramos os coeficientes da derivada de f(x)
	}
		
	for(i = 0; i < 1000; i++)
	{
		fx[0] = 0; fx[1] = 0;								//	Resetamos os valores em fx
		for(j = 0; j <= grauEquacao; j++)
		{
			fx[0] += pow(x,grauEquacao-j)*coeficientes[j];						// Calcula f(x)
			if(j != grauEquacao) fx[1] += pow(x,grauEquacao-j-1)*derivada[j];	// Calcula f'(x)
		}
		aux = x - fx[0]/fx[1];
		if(fabs(aux - x) < EPSILON)
		{
			printf("\n\n\tIteracoes: %d\n", i);
			printf("\n\tx = %lf", x);
			free(derivada);
			return;
		}
		x = aux;
	}
	
	printf("\n\n\tIteracoes: %d\n", i);
	printf("\n\tx = %lf\n", x);
	free(derivada);
		
}	// Fim metodoNewton
	
void swap(double *a, double *b)	
{
	/*
		Funcao auxiliar simples para o swap dos valores presentes 
		em diferentes posicoes de um ponteiro double.
	*/
		
	double aux;
	
	aux = *a;
	*a = *b;
	*b = aux;

}	// Fim swap
