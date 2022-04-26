#include <stdio.h>
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
	printf("\n\tConversao\n\n");
}

void recebeSistemaLinear(){
	printf("\n\tSistema Linear\n\n");
}
void lerEquacaoAlgebrica(){
	printf("\n\tEquacao Algebrica\n\n");
}
