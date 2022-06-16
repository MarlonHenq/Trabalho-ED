#include <stdio.h>
#include <stdlib.h>

//Cores especiais
#define errorColor     "\x1b[31m"
#define listColor    "\x1b[34m"
#define alertColor  "\x1b[33m"
#define contrastColor    "\x1b[36m"

#define resetColor   "\x1b[0m"

void helpMenu();

int main(){
    

    return 0;
}


void helpMenu(){
    int opcaoUsuario = 0;

    system("@cls||clear");
    printf("================Menu Help================\n");
    printf("Trabalho de Estruturas de Dados\n");
    printf("\n");
    printf("Autores: \n");
    printf(contrastColor "Marlon Henrique Sanches - RA: 2407388\n" resetColor);
    printf(contrastColor "Gustavo Henrique Cardoso de Araújo - RA: AAAAAAA \n" resetColor);
    printf("=========================================\n");
    // Autores: 
    // Marlon Henrique Sanches - RA: 2407388
    // Gustavo Henrique Cardoso de Araújo - RA: AAAAAAA

    printf("\n");

    printf("0 - Sair\n");
    scanf("%d", &opcaoUsuario);
    getchar();
    if (opcaoUsuario>=0) {
        system("@cls||clear"); //Limpa Tela
        return;
    }
}