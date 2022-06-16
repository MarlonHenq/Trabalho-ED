#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Cores especiais
#define errorColor     "\x1b[31m"
#define listColor    "\x1b[34m"
#define alertColor  "\x1b[33m"
#define contrastColor    "\x1b[36m"

#define resetColor   "\x1b[0m"

#define productPath "./Produtos.csv"
#define machinePath "./Maquinas.csv"

typedef struct batch{
    char type;
    float cost;
    float profit;
    int deterioration;
    float productionProb;
    struct batch *next;
}s_Batch;

typedef struct line{
    //struct batch *product;
    char type;          //Tipo de Produto
    int entryTime;      //Tempo de Entrada
    struct line *next;  //Próximo da Fila
}s_Line;

typedef struct machine{
    char type[10];          //Tipo da Máquina
    char canProcess[4];     //Items Processáveis
    int timeToProcess[3];   //Tempo de Processo
    int size;               //Tamanho da Linha
    int price;              //Preço da Máquina
    float cost;             //Custo Operacional da Máquina
    int powerConsumption;   //Consumo de Energia
    struct line *line;      //Linha
    struct machine *next;   //Próxima Máquina
}s_Machine;

typedef struct packaging{
    struct machine *machine;//Máquina
    int cBatches;           //Quantidade de Lotes de Coxinha  
    int pBatches;           //Quantidade de Lotes de Peixe    
    int aBatches;           //Quantidade de Lotes de Almôndega
    int cWasted;            //Disperdiçados de Coxinha  
    int pWasted;            //Disperdiçados de Peixe    
    int aWasted;            //Disperdiçados de Almôndega
    int clock;              //Relógio
}s_Packaging;


void helpMenu();

void infoScreen(s_Machine *machines,s_Batch *batches);

void importProducts(s_Batch **products);

void chooseMachines();

void addMachines(s_Packaging **machines,char type[10],int clock);

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
    printf(contrastColor "Gustavo Henrique Cardoso de Araujo - RA: 2386712 \n" resetColor);
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

void infoScreen(s_Machine *machines,s_Batch *batches){
    int i =0;
    //system("@cls||clear"); //Limpa Tela
    printf(contrastColor "===================Info==================\n" resetColor);
    printf(listColor "=== Lotes Empacotados ===\n" resetColor);
    printf("Coxinha: %d  | Lucro: %.2f\n");
    printf("Peixe: %d  | Lucro: %.2f\n");
    printf("Almondega: %d  | Lucro: %.2f\n");
    printf("Totais: %d  | Lucro: %.2f\n");
    printf(listColor "=== Lotes Disperdicados ===\n" resetColor);
    printf("Coxinha: %d  | Custo: %.2f\n");
    printf("Peixe: %d  | Custo: %.2f\n");
    printf("Almondega: %d  | Custo: %.2f\n");
    printf("Totais: %d  | Custo: %.2f\n");
    printf(listColor "=== Máquinas ===\n" resetColor);
    for(i = 0; i< (sizeof(machines)/sizeof(s_Machine));i++){
        printf("%s: %.2f\n",machines[i].type);
    }
    printf(contrastColor "Tempo Total de Pagamento:");
}

void chooseMachines(s_Packaging **machines){
    int op = -1;
    while(op!=0){
        printf("Escolha a máquina:\n");
        printf("1 - FishPak");
        printf("2 - ChickenPak");
        printf("3 - AllPak");
        printf("4 - Plastific");
        printf("5 - EnSacAll");
        printf("6 - Universal");
        printf("0 - Sair");
        scanf("%d",&op);
        getchar();

    }
}

void importProducts(s_Batch **products){
    if(*products){
        s_Batch *p_aux = *products;
        while(*products!=NULL){
            p_aux = *products;
            *products = (*products)->next;
            free(p_aux);
        }
    }
    FILE *file = fopen(productPath,"r");
    while(!feof){
        s_Batch *product = (s_Batch *) malloc(sizeof(s_Batch));
        fscanf(file,"%c %f %f %d %d",product->type,product->cost,product->profit,product->deterioration,product->productionProb);
        product->next = *products;
        *products = product;
    }
    fclose(file);
}

void addMachines(s_Packaging **machines,char type[10]){
    if(!*machines){
        s_Packaging *packaging = (s_Packaging *) malloc(sizeof(s_Packaging));
        if(!packaging) return;
        packaging->clock = 0;
        packaging->machine = NULL;
    }
    FILE *file = fopen(machinePath,"r");
    s_Machine *machine = NULL;
    while(!feof){
        int i = 0;
        if((*machines)->machine->type == type){
            i++;
            machine = (*machines)->machine;
        }
        machine = (s_Machine *) malloc(sizeof(s_Machine));
        if(!machine) return;
        fscanf(file,"%s %c %d %d %d",machine->type,machine->canProcess[i],machine->timeToProcess[i],machine->powerConsumption,machine->price);
        machine->line = NULL;
    }
    if(!(*machines)->machine){
        machine->next = machine;
        (*machines)->machine = machine;
        return;
    }
    if((*machines)->machine->type == machine->type){

    }
    machine->next = (*machines)->machine->next;
    (*machines)->machine->next = machine;
}

