#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>

void abreMenu();
void conversaoNumerica();
void recebeSistemaLinear();
void lerEquacaoAlgebrica();

void main(){
	
	abreMenu();
	
}

void abreMenu(){
	
	/*
		Funcao que imprime ao usuario um menu com 4 opcoes:
		‘C’ – Conversao, ‘S’ – Sistema Linear, ‘E’ – Equacao Algebrica e ‘F’ – Finalizar.
		E entao realiza a chamada do metodo solicitado por ele.
	*/

	char operacao;

	void gotoxy(int x, int y)
	{
		/*
			Função auxiliar que manipula a localizacao do cursor no console
			a fim de tornar o menu principal mais pratico e intuitivo.
			Recebe como parametro a coluna x e a linha y onde o cursor deve se mover.
		*/
		
	  	COORD coordenada;
	  	coordenada.X = x;
	  	coordenada.Y = y;
	  	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordenada);
	}
	
	do {
        system("pause");
        system("cls");
        gotoxy(5,  1); printf("MENU PRINCIPAL - Exercicio-programa");
        gotoxy(5,  3); printf("[ C ] Conversao");
        gotoxy(5,  4); printf("[ S ] Sistema Linear");
        gotoxy(5,  5); printf("[ E ] Equacao Algebrica");
        gotoxy(5,  6); printf("[ F ] Finalizar");
        gotoxy(5,  7); printf("[ ? ] Opcao Desejada (Case Sensitive)");
    
        do {
        	gotoxy(7, 7); scanf("%c", &operacao);
        } while (operacao != 'C' && operacao != 'S' && operacao != 'E' && operacao != 'F');
    
    	switch (operacao) {
    		case 'C': conversaoNumerica(); break;
    		case 'S': recebeSistemaLinear(); break;
    		case 'E': lerEquacaoAlgebrica(); break;
		}
    
	}while(operacao!='F');
	
}

void conversaoNumerica(){
	
	/*
		Le um valor decimal, converte e mostra o resultado para
		a base binaria, octal e hexadecimal.
		
		OBS: O ideal seria que a variavel decimal fosse do tipo
		long double, mas ao faze-lo ocorrem erros de execucao,
		e mesmo depois de adaptar o algoritmo, os erros persistem.
		Tendo isso em mente, ha a possibilidade de valores com mais de
		6 casas decimais, apresentarem conversoes inadequadas.
		As conversoes, no entanto mostram por volta de ate 90 casas decimais significativas.
	*/
	
	double decimal;
	char hexadecimal[100], octal[100], binario[100];
	
	void converteDecimal(int base, char* snp)
	{
		/*
			Converte o valor decimal dado pelo usuario
			em um valor de base dado como parametro e armazena
			na string passada.
			
			ex: converteDecimal(2, binario)
		
		*/
		int parte_inteira = (int) decimal;
		double parte_fracionaria = decimal - parte_inteira;
		int posicao = 1, valor_temp, i;
		char aux;
		
		if(parte_inteira < 0)				//	Verifica o sinal do decimal e garante que a conversao
		{									//	tera o mesmo sinal
			snp[0] = '-';
			parte_inteira *= -1;
		} else snp[0] = ' ';
		
		if(parte_fracionaria < 0) parte_fracionaria *= -1;
		
		while(parte_inteira != 0)
		{
			valor_temp = parte_inteira % base;
			if(valor_temp < 10) valor_temp += 48;	//	Se for menor que 10, numero da tabela ascii
			else valor_temp += 55;					//	Senao, letra da tabela ascii
			snp[posicao] = valor_temp;
			posicao++;
			parte_inteira /= base;
		}
		
		for(i = 0; i < posicao/2; i++)				//	Swap dos valores da parte inteira, do contrario
		{											//	os valores da parte inteira aparecem invertidos
			aux = snp[i+1];
			snp[i+1] = snp[posicao-i-1];
			snp[posicao-i-1] = aux;
		}
		
		if(parte_fracionaria != 0){						// Se existir parte fracionaria, separa por um ponto
			snp[posicao] = '.';
			posicao++;
		}
		
		while(parte_fracionaria != 0)
		{
			valor_temp = parte_fracionaria * base;
			if(valor_temp < 10) valor_temp += 48;
			else valor_temp += 55;
			snp[posicao] = valor_temp;
			posicao++;
			parte_fracionaria *= base;
			parte_fracionaria -= (int) parte_fracionaria;
		}
		
		snp[posicao] = '\0';
		
	}
	
	system("cls");
	printf("\n\tDigite um numero decimal: ");
	scanf("%lf", &decimal);
	
	converteDecimal(2, binario);
	converteDecimal(8, octal);
	converteDecimal(16, hexadecimal);
	
	printf("\n\tBinario: %s\n", binario);
	printf("\tOctal: %s\n", octal);
	printf("\tHexadecimal: %s\n\n", hexadecimal);
	
}

void recebeSistemaLinear(){
	
	char nomeArquivo[50], *linha;
	FILE *arquivo;
	int numeroVariaveis, i, j;
	double **coeficiente;
	
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
	}
	
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
	
	coeficiente = criaMatriz(numeroVariaveis, numeroVariaveis + 1);
	
	if(coeficiente == NULL)
	{
		printf("\n\n\tFaltou memoria.\n");
		return;
	}
	
	for(i = 0; i < numeroVariaveis; i++)
	{
		printf("\n");
		for (j = 0; j < numeroVariaveis + 1; j++)
		{
			fscanf(arquivo, "%lf", &coeficiente[i][j]);
			printf("\t%.0lf", coeficiente[i][j]);	// Linha de teste
		}
		printf("\n");
	}
	
	fclose(arquivo);
	
}
void lerEquacaoAlgebrica(){
	printf("\n\tEquacao Algebrica\n\n");
}
