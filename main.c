#include <stdio.h>
#include <stdlib.h>
//Provável adição de um randon math

//Cores especiais
#define errorColor     "\x1b[31m"
#define listColor    "\x1b[34m"
#define alertColor  "\x1b[33m"
#define contrastColor    "\x1b[36m"

#define resetColor   "\x1b[0m"

//Valores HardCoded 
const productionTimeVariation = 10; // 10%
const KWHCust = 1; // 1 real o Kilo Watt Hora
const hourInSeconds = 3600; //Total de segundos que tem em uma hora

typedef struct produto{
    int id; //ID que receberá em ordem de carregamento do arquivo
    char productionType[2]; //NOME abreviado (C - coxinha | P - Peixe | A - Almôndega)
    float productionCust; //Custo de Produção (EM REAIS)
    float salePrice; //Preço Atacado (Preço da venda no atacado) (EM REAIS)
    int deteriorationTime; //Tempo de deteriorização (EM SEGUNDOS) 
    int productionProbability; //Probabilidade de ser produzido (%)
}Tproduto;

typedef struct maquina{
    int id; //ID que receberá em ordem de carregamento do arquivo
    char model[30]; //Modelo
    char productionType[2]; //Tipo de produto processado (T repesentando todos)
    int productionTime; //Tempo de produção quem pode variar +/- 10% (Constante productionTimeVariation)
    int consumption; //Consumo em KWH
    int price; //Preço em reais da compra da maquina
}TMaquina;

void helpMenu();

int main(){
    

    return 0;
}


void helpMenu(){
    int userOp = 0;

    system("@cls||clear"); //Limpa Tela
    printf("================Menu Help================\n");
    printf("Trabalho de Estruturas de Dados\n");
    printf("\n");
    printf("Autores: \n");
    printf(contrastColor "Marlon Henrique Sanches - RA: 2407388\n" resetColor);
    printf(contrastColor "Gustavo Henrique Cardoso de Araújo - RA: 2386712 \n" resetColor);
    printf("=========================================\n");
    // Autores: 
    // Marlon Henrique Sanches - RA: 2407388
    // Gustavo Henrique Cardoso de Araújo - RA: 2386712

    printf("\n");

    printf("0 - Sair\n");
    scanf("%d", &userOp);
    getchar();
    if (userOp>=0) {
        system("@cls||clear"); //Limpa Tela
        return;
    }
}