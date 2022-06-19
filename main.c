#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>  

#pragma region "Cores"
//Cores especiais
#define errorColor     "\x1b[31m"
#define listColor    "\x1b[34m"
#define alertColor  "\x1b[33m"
#define contrastColor    "\x1b[36m"

#define resetColor   "\x1b[0m"

#pragma endregion

#pragma region "Constantes"
//Valores HardCoded 
const productionTimeVariation = 10; // 10%
const KWHCust = 1; // 1 real o Kilo Watt Hora

const hourInSeconds = 3600; //Total de segundos que tem em uma hora
const twoYearsInSeconds = 63072000; //Total de segundos que tem em 2 anos

const char machinesFileName[20] = "Maquinas.csv"; //Arquivos para carregamento de dados
const char productsFileName[20] = "Produtos.csv";
const char simulationFileName[20] = "Simulacao.csv";


#pragma endregion

#pragma region "Structs"
typedef struct product{
    int id; //ID que receberá em ordem de carregamento do arquivo
    char productionType[2]; //NOME abreviado (C - coxinha | P - Peixe | A - Almôndega)
    float productionCost; //Custo de Produção (EM REAIS)
    float salePrice; //Preço Atacado (Preço da venda no atacado) (EM REAIS)
    int deteriorationTime; //Tempo de deteriorização (EM SEGUNDOS) 
    int productionProbability; //Probabilidade de ser produzido (%)
    struct product *next; //Ponteiro para o próximo produto
}TProduct;

typedef struct machine{
    int id; //ID que receberá em ordem de carregamento do arquivo
    char model[30]; //Modelo
    char productionType[2]; //Tipo de produto processado (T repesentando todos)
    int productionTime[3]; //Tempo de produção quem pode variar +/- 10% (Constante productionTimeVariation)
    int consumption; //Consumo em KWH
    int price; //Preço em reais da compra da maquina
    struct machine *next; //Ponteiro para o próximo produto
}TMachine;

typedef struct headProduct{
    int idCount; //ID que recebe quantos produtos foram carregados
    TProduct *first; //Ponteiro para o primeiro produto
}THeadProduct;

typedef struct headMachine{
    int idCount; //ID que recebe quantas máquinas foram carregadas
    TMachine *first; //Ponteiro para o primeiro produto
}THeadMachine;

typedef struct simulation{
    int idMachine; //ID que recebe em ordem de carregamento do arquivo
    int numberOfMachines; //Quantidade de máquinas
    struct simulation *next; //Ponteiro para o próximo produto
}TSimulation;

typedef struct line{
    //struct batch *product;
    int type;          //Tipo de Produto
    int entryTime;      //Tempo de Entrada
    struct line *next;  //Próximo da Fila
}TLine;

typedef struct machineOnProduction{
    int id; //ID
    char model[30]; //Modelo
    char productionType[2]; //Tipo de produto processado (T repesentando todos)
    int productionTime[3]; //Tempo de produção quem pode variar +/- 10% (Constante productionTimeVariation)
    int consumption; //Consumo em KWH
    int price; //Preço em reais da compra da maquina
    struct machineOnProduction *next; //Ponteiro para o próximo produto
    TLine *first; //Ponteiro para o primeiro produto da linha
    int numberOfProducts; //Quantidade de produtos na linha
    int timeOfProduction; //Tempo de produção do produto em destaque
}TMachineOnProduction;

typedef struct packaging{
    int cBatches;           //Quantidade de Lotes de Coxinha  
    int pBatches;           //Quantidade de Lotes de Peixe    
    int aBatches;           //Quantidade de Lotes de Almôndega
    int cWasted;            //Disperdiçados de Coxinha  
    int pWasted;            //Disperdiçados de Peixe    
    int aWasted;            //Disperdiçados de Almôndega
    int cProduction;        //Produção de Coxinha
    int pProduction;        //Produção de Peixe
    int aProduction;        //Produção de Almôndega

    int finalTime;         //Tempo de Fim da Simulação
    int totalCost;         //Custo Total da Simulação
    int totalGain;         //Lucro Total da Simulação
}TPackaging;

#pragma endregion

#pragma region "Funções Menu" //Submenus
void creditsMenu();
#pragma endregion

#pragma region "Funções Internas" //Alocação de memória | veficações | calculos internos

TPackaging *alocatePackaging(int cBatches, int pBatches, int aBatches, int cWasted, int pWasted, int aWasted){
    TPackaging *package = (TPackaging *)malloc(sizeof(TPackaging));

    if(package == NULL){
        printf(errorColor"Erro ao alocar memória para o Packaging!\n"resetColor);
        return NULL;
    }

    package->cBatches = cBatches;
    package->pBatches = pBatches;
    package->aBatches = aBatches;
    package->cWasted = cWasted;
    package->pWasted = pWasted;
    package->aWasted = aWasted;
    package->cProduction = 0;
    package->pProduction = 0;
    package->aProduction = 0;

    package->finalTime = 0;
    package->totalCost = 0;
    package->totalGain = 0;
    return package;
}


TProduct *alocateProductMemorie(int id, char productionType[2], float productionCost, float salePrice, int deteriorationTime, int productionProbability){
    TProduct *newProduct = (TProduct *)malloc(sizeof(TProduct)); //Alocação de memória para o novo produto

    if (newProduct == NULL){ //Verificação de erro de alocação de memória
        printf(errorColor"Erro ao alocar memória para o produto!\n"resetColor);
        return NULL;
    }

    //Atribuição de valores
    newProduct->id = id;
    strcpy(newProduct->productionType, productionType);
    newProduct->productionCost = productionCost;
    newProduct->salePrice = salePrice;
    newProduct->deteriorationTime = deteriorationTime;
    newProduct->productionProbability = productionProbability;
    newProduct->next = NULL;

    return newProduct;
}

TMachine *alocateMachineMemorie(int id, char model[30], char productionType[2], int productionTime[3], int consumption, int price){
    TMachine *newMachine = (TMachine *)malloc(sizeof(TMachine)); //Alocação de memória para a nova maquina
    
    if (newMachine == NULL){ //Verificação de erro de alocação de memória
        printf(errorColor"Erro ao alocar memória para a máquina!\n"resetColor);
        return NULL;
    }

    //Atribuição de valores
    newMachine->id = id;
    strcpy(newMachine->model, model);
    strcpy(newMachine->productionType, productionType);
    newMachine->productionTime[0] = productionTime[0];
    newMachine->productionTime[1] = productionTime[1];
    newMachine->productionTime[2] = productionTime[2];
    newMachine->consumption = consumption;
    newMachine->price = price;
    newMachine->next = NULL;

    return newMachine;
}

THeadProduct *alocateHeadProductMemorie(){
    THeadProduct *newHeadProduct = (THeadProduct *)malloc(sizeof(THeadProduct)); //Alocação de memória para a cabeça da lista de produtos
    newHeadProduct->idCount = 0; //Inicialização do ID
    newHeadProduct->first = NULL; //Inicialização do primeiro elemento

    return newHeadProduct;
}

THeadMachine *alocateHeadMachineMemorie(){
    THeadMachine *newHeadMachine = (THeadMachine *)malloc(sizeof(THeadMachine)); //Alocação de memória para a cabeça da lista de máquinas
    newHeadMachine->idCount = 0; //Inicialização do ID
    newHeadMachine->first = NULL; //Inicialização do primeiro elemento

    return newHeadMachine;
}

TSimulation *alocateSimulationMemorie(int idMachine, int numberOfMachines){
    TSimulation *newSimulation = (TSimulation *)malloc(sizeof(TSimulation)); //Alocação de memória para a nova simulação
    
    if (newSimulation == NULL){ //Verificação de erro de alocação de memória
        printf(errorColor"Erro ao alocar memória para a simulação!\n"resetColor);
        return NULL;
    }

    //Atribuição de valores
    newSimulation->idMachine = idMachine;
    newSimulation->numberOfMachines = numberOfMachines;
    newSimulation->next = NULL;
    
    return newSimulation;
}

bool fileExists(char fileName[20])
{
    FILE *file = fopen(fileName, "r");
    if (file)
    {
        fclose(file);
        return true;
    }
    return false;
}

TMachineOnProduction *alocateMachineOnProductionMemorie(int id, char model[30], char productionType[2], int productionTime[3], int consumption, int price){
    TMachineOnProduction *newMachineOnProduction = (TMachineOnProduction *)malloc(sizeof(TMachineOnProduction)); //Alocação de memória para a nova maquina

    if (newMachineOnProduction == NULL){ //Verificação de erro de alocação de memória
        printf(errorColor"Erro ao alocar memória para a máquina!\n"resetColor);
        return NULL;
    }

    //Atribuição de valores
    newMachineOnProduction->id = id;
    strcpy(newMachineOnProduction->model, model);
    strcpy(newMachineOnProduction->productionType, productionType);
    newMachineOnProduction->productionTime[0] = productionTime[0];
    newMachineOnProduction->productionTime[1] = productionTime[1];
    newMachineOnProduction->productionTime[2] = productionTime[2];
    newMachineOnProduction->consumption = consumption;
    newMachineOnProduction->price = price;
    newMachineOnProduction->next = NULL;
    newMachineOnProduction->first = NULL;
    newMachineOnProduction->numberOfProducts = 0;
    newMachineOnProduction->timeOfProduction = 0;

    return newMachineOnProduction;
}

int randomProduct(){
    int random = rand() % 10;
    //printf("%d\n", random);
    switch(random){
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            return 1; //Coxinha
        break;

        case 5:
        case 6:
        case 7:
            return 2; //Peixe
        break;

        case 8:
        case 9:
            return 3; //Almondega
        break;
    }
}

#pragma endregion

#pragma region "Funções de Leitura e Print"
void loadProducts(THeadProduct **headProduct){
    FILE *file = fopen(productsFileName, "r"); //Abertura do arquivo
    
    if (file == NULL){ //Verificação de erro de abertura do arquivo
        printf(errorColor"Erro ao abrir o arquivo de produtos!\n"resetColor);
        return;
    }

    int id = 1;

    //Variáveis de atribuição
    char productionType[2];
    float productionCost;
    float salePrice;
    int deteriorationTime;
    int productionProbability;

    //Carregamento de dados
    while (fscanf(file, "%s %f %f %d %d", productionType, &productionCost, &salePrice, &deteriorationTime, &productionProbability) != EOF){
        fscanf(file, "%d %s %f %f %d %d", &id, &productionType, &productionCost, &salePrice, &deteriorationTime, &productionProbability);
        TProduct *newProduct = alocateProductMemorie(id, productionType, productionCost, salePrice, deteriorationTime, productionProbability);

        if (newProduct == NULL) return; //Verificação de erro de alocação de memória
        
        if((*headProduct)->first == NULL){ //Verificação de se é o primeiro produto
            (*headProduct)->first = newProduct;
        }
        else{
            TProduct *aux = (*headProduct)->first;
            while (aux->next != NULL){
                aux = aux->next;
            }
            aux->next = newProduct;
        }
        (*headProduct)->idCount = id;
        id = id + 1;
    }

    fclose(file); //Fechando o arquivo
}
void printProducts(THeadProduct *headProduct){

    printf(listColor "Foram carregados %d produtos:\n" resetColor, headProduct->idCount);
    TProduct *aux = headProduct->first;

        printf("ID  | Tipo  |     Custo    |     Preço    | Tempo  |  Prob. |\n");
        printf("----|-------|--------------|--------------|--------|--------|\n");

    while (aux != NULL){
        //Imprime uma tabela com os dados dos produtos
        printf("%d   |   %s   |   %f   |   %f   |   %d   |   %d   |\n", aux->id, aux->productionType, aux->productionCost, aux->salePrice, aux->deteriorationTime, aux->productionProbability);
        aux = aux->next;
    }
}
void loadMachines(THeadMachine **headMachine){
    FILE *file = fopen(machinesFileName, "r"); //Abertura do arquivo

    if (file == NULL){ //Verificação de erro de abertura do arquivo
        printf(errorColor"Erro ao abrir o arquivo de máquinas!\n"resetColor);
        return;
    }

    int id = 1;

    //Variáveis de atribuição
    char model[30];
    char productionType[2];
    int productionTime[3];
    int consumption;
    int price;
    
    //Carregamento de dados
    while (fscanf(file, "%s %s %d %d %d %d %d", &model, &productionType, &productionTime[0], &productionTime[1], &productionTime[2], &consumption, &price) != EOF){
        fscanf(file, "%d %s %s %d %d %d %d %d", &id, &model, &productionType, &productionTime[0], &productionTime[1], &productionTime[2], &consumption, &price);
        TMachine *newMachine = alocateMachineMemorie(id, model, productionType, productionTime, consumption, price);
        
        if (newMachine == NULL) return; //Verificação de erro de alocação de memória

        if((*headMachine)->first == NULL){ //Verificação de se é o primeiro produto
            (*headMachine)->first = newMachine;
        }
        else{
            TMachine *aux = (*headMachine)->first;
            while (aux->next != NULL){
                aux = aux->next;
            }
            aux->next = newMachine;
        }
        (*headMachine)->idCount = id;
        id = id + 1;
    }

    fclose(file); //Fechando o arquivo
}
void printMachines(THeadMachine *headMachine){
    printf(listColor "Foram carregadas %d máquinas:\n" resetColor, headMachine->idCount);
    TMachine *aux = headMachine->first;

        printf("ID   | Tipo  |     Tempo    | Consumo |   Preço    |  Modelo\n");
        printf("-----|-------|----|----|----|---------|------------|------------\n");

    while (aux != NULL){
        //Imprime uma tabela com os dados das máquinas
        printf("%d    |   %s   | %d | %d | %d |   %d    |   %d   |   %s\n", aux->id, aux->productionType, aux->productionTime[0], aux->productionTime[1], aux->productionTime[2], aux->consumption, aux->price, aux->model);
        aux = aux->next;
    }
}

TMachineOnProduction *loadMachinesOnProduction( THeadMachine *headMachine,int *machinesTotalCost){
    FILE *file = fopen(simulationFileName, "r"); //Abertura do arquivo
    if (file == NULL){ //Verificação de erro de abertura do arquivo
        printf(errorColor"Erro ao abrir o arquivo de simulação!\n"resetColor);
        return NULL;
    }

    int idCount = 1;

    int machineIdToSearch = 0; 
    int numberMachines = 0; 

    TMachineOnProduction *listMachineOnProduction = NULL;

    while (!feof(file)){ //Lendo o arquivo até o fim
        if (feof(file)) break;

        fscanf(file, "%d %d", &machineIdToSearch, &numberMachines);
        // printf("%d %d\n", machineIdToSearch, numberMachines); //Debug

        if (machineIdToSearch == 0 || machineIdToSearch > headMachine->idCount){ //Verificação de erro de leitura do arquivo
            printf(errorColor"O arquivo de simulação apresenta uma máquina inválida, verifique-o!\n"resetColor);
            return NULL;
        }

        //Buscando Maquina nas maquinas existentes
        TMachine *machine = headMachine->first;
        while (machine!= NULL){
            if (machine->id == machineIdToSearch){ //ACHOU NAS MAQUINAS EXISTENTES
                for (int i = 0; i < numberMachines; i++){ //DUPLICA QUANTAS VEZES FOI DITO NO ARQUIVO
                    TMachineOnProduction *newMachineOnProduction = alocateMachineOnProductionMemorie(idCount, machine->model, machine->productionType, machine->productionTime, machine->consumption, machine->price);
                    // printf("Rodei %d\n", i); //DEBUG ALTAMENTE SOFISTICADO
                    // printf("ID %d\n", idCount);
                    if (newMachineOnProduction == NULL) return;

                    if (listMachineOnProduction == NULL){ //Caso seja a primeira maquina
                        listMachineOnProduction = newMachineOnProduction;
                    }
                    else{ //Caso não seja a primeira maquina
                        TMachineOnProduction *aux = listMachineOnProduction;
                        while (aux->next != NULL){
                            aux = aux->next;
                        }
                        aux->next = newMachineOnProduction;
                    }

                    (*machinesTotalCost) += (machine->price);

                    idCount = idCount + 1;
                }
                
                break;
            }
            machine = machine->next;
        }
        
    }

    return listMachineOnProduction;
    
}

void informationPanel(){
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
}

void progressBarPrinter(int percent){
    int totalBars = 50;
    int filledBars = percent/2;
    int emptyBars = totalBars - filledBars;

    printf("[");
    for (int i = 0; i < filledBars; i++){
        printf("=");
    }
    for (int i = 0; i < emptyBars; i++){
        printf(" ");
    }
    printf("] %d%% ", percent);
}

void tuxPrinter(int number){ //Tux bolado, tá?
    printf(" _____\n");
    printf("< "contrastColor"%d%%"resetColor" >\n", number);
    printf(" -----\n");
    printf("   \\ \n");
    printf("    \\ \n");
    printf("        .--.\n");
    printf("       |o_o |\n");
    printf("       |:_/ |\n");
    printf("      //   \\\n");
    printf("     (|     |)\n");
    printf("    /'\\_   _/`\\\n");
    printf("    \\___)=(___/\n");
    printf("\n");
}

void progressPrint(int totalTime, double totalGain, double totalCost){
    int percentTime = (totalTime/(twoYearsInSeconds/10)*10); //Gambirra já que o número é maior que o suportando em um int normal
    int percentMoney = (int)(totalGain/totalCost*100);
    
    system("@cls||clear"); //Limpa Tela

    printf(contrastColor "Nº Loops: " resetColor "%d\n", totalTime);
    tuxPrinter(percentTime);

    printf(contrastColor"Tempo:\n"resetColor);
    printf("%d ", totalTime);
    progressBarPrinter(percentTime);
    printf("%d\n", twoYearsInSeconds);

    printf(contrastColor"Dinheiro:\n"resetColor);
    printf("%.2f ", totalGain);
    progressBarPrinter(percentMoney);
    printf("%.2f", totalCost);

    printf("\n\n");
}

#pragma endregion

#pragma region "Funnções de Criação de Arquivos" //Criação de arquivos

void createSimulationFile(int numberMaxMachines){
    FILE *file = fopen(simulationFileName, "w"); //Abertura do arquivo
    if (file == NULL){ //Verificação de erro de abertura do arquivo
        printf(errorColor"Erro ao abrir o arquivo de simulação!\n"resetColor);
        return;
    }

    int machineID = NULL;
    int numberOfMachines = NULL;
    int lnNum = 0;

    while (1==1){
        printf("Digite o ID da máquina que deseja adicionar (Para parar de adicionar digite '0'): ");
        scanf("%d", &machineID);

        if (machineID > numberMaxMachines){ //Para caso um id indisponivel seja digitado
            system("@cls||clear"); //Limpa Tela
            printf(errorColor"O ID de maquina indisponível! Digite um ID válido\n"resetColor);
            FILE *file = fopen(simulationFileName, "w");
            fclose(file);

            return;
        }

        if (machineID != 0){
            if(lnNum>0){//Impede erro de repetição por ultima linha em branco
                fprintf(file,"\n");
            }
            fprintf(file, "%d ", machineID);
        }
        else{
            break;
        }

        printf("Digite o número de máquinas que deseja adicionar: ");
        scanf("%d", &numberOfMachines);
        fprintf(file, "%d", numberOfMachines);
        lnNum=1;
    }
    system("@cls||clear"); //Limpa Tela
    printf("Arquivo de simulação criado com sucesso!\n");
    fclose(file); //Fechando o arquivo
}

#pragma endregion

#pragma region "Funções de Simulação" //Simulação


float getCostByProduct(THeadProduct *products, int id){
    TProduct *aux = products->first;
    while(aux!=NULL){
        if(aux->id == id){
            return aux->productionCost;
        }
        aux = aux->next;
    }
    return 0;
}

int getMachinesThatAcceptTypeOfProductANDHaveShortestList(TMachineOnProduction *machineOnProduction, int typeOfProduct){
    TMachineOnProduction *aux = machineOnProduction;
    int shortestListID = 0;
    int shortestListSize = 0;

    while(aux!=NULL){
        if(aux->productionType == "T" || (aux->productionType == "C" && typeOfProduct == 1) || (aux->productionType == "P" && typeOfProduct == 2) || (aux->productionType == "A" && typeOfProduct == 3)){
            if(aux->numberOfProducts < shortestListSize){
                shortestListSize = aux->numberOfProducts;
                shortestListID = aux->id;
            }
        }
        aux = aux->next;
    }

    return shortestListID; //Return 0 caso não encontre nenhuma máquina que aceite o tipo de produto
}

int getDeteriorationTimeByProductID(THeadProduct *products, int id){
    TProduct *aux = products->first;
    while(aux!=NULL){
        if(aux->id == id){
            return aux->deteriorationTime;
        }
        aux = aux->next;
    }
    return 0;
}

void addProductToMachineByID(TMachineOnProduction **machineOnProduction, THeadProduct *products, int id, int newProduct){
    if (id == 0){
        return;
    }

    TMachineOnProduction *aux = *machineOnProduction;
    while(aux!=NULL){
        if(aux->id == id){
            aux->numberOfProducts = aux->numberOfProducts + 1;
            TLine *aux2 = aux->first; 

            if(aux->first == NULL){
                aux->timeOfProduction = getDeteriorationTimeByProductID(products, newProduct);

                aux2 = (TLine*)malloc(sizeof(TLine));
                aux2->type = newProduct;
                aux2->entryTime = getDeteriorationTimeByProductID(products, newProduct);
                aux2->next = NULL;
            }
            
            while(aux2!=NULL){
                if(aux2->next == NULL){
                    aux2->next = (TLine*)malloc(sizeof(TLine));
                    aux2->next->type = newProduct;
                    aux2->next->entryTime = getDeteriorationTimeByProductID(products, newProduct);
                    aux2->next->next = NULL;
                }
                aux2 = aux2->next;
            }
            return;
        }
        aux = aux->next;
    }
}

TPackaging simulationLoop(THeadProduct *products, TMachineOnProduction *machineOnProductionm, double machinesTotalCost){
    TPackaging *packaging = alocatePackaging(0,0,0,0,0,0);
    int newProduct = NULL;
    double totalCost = machinesTotalCost;
    double totalGain = 0;
    int totalTime = 0;

    int test = 0;

    while(true){
        //CONDIÇÔES DE PARADA:
        //1. Se o tempo de simulação for maior 2 anos
        if (totalTime >= twoYearsInSeconds){
            progressPrint(totalTime, totalGain, totalCost);
            return;
        }
        //2. Se o Ganho passar a superar o custo total (Ou seja, passar a ter lucro)
        if (totalGain >= totalCost){
            progressPrint(totalTime, totalGain, totalCost);
            return;
        }


        //FUNCIONAMETO SEGUNDO POR SEGUNDO:
        //1. Adicionar um novo produto ao sistema
        if (totalTime % 2 == 0){ //GERA UM NOVO PRODUTO PARA SER ADICIONADO A LINHA a cada 2 segundos
            newProduct = randomProduct();

            //ADD PREÇO de custo
            totalCost = totalCost + getCostByProduct(products, newProduct);

            //ADD LOTE NA ESTETISTICA
            if(newProduct == 1){
                packaging->cBatches++;
            }
            else if(newProduct == 2){
                packaging->pBatches++;
            }
            else if(newProduct == 3){
                packaging->aBatches++;
            }

            //ADD NA LISTA
            int machineID = getMachinesThatAcceptTypeOfProductANDHaveShortestList(machineOnProductionm, newProduct);
            if(machineID != 0){
                addProductToMachineByID(&machineOnProductionm, products, machineID, newProduct);
            }
        }
        //2. Remover um produto da linha
            //2.1. Para as maquinas que terminaram o seu tempo de processamento
            
            //2.2. Para os produtos que pereceram na linha (Separados por tipos já que deve-se somar na estatistica)



        //PRINT:
        if(totalTime % 500000 == 0){ //Atualiza a tela a cada 500000 segundos de simulação
            progressPrint(totalTime, totalGain, totalCost);
            //scanf("%d", &test);
        }
        
        //ATUALIZAÇÃO DE VARIÁVEIS:
        //1. Atualiza o tempo de simulação
        totalTime = totalTime + 1;

        //2. Atualizar o estado das máquinas e deteorização de produtos

    }

    packaging->finalTime = totalTime;
    packaging->totalGain = totalGain;
    packaging->totalCost = totalCost;

    return *packaging;
}


void simulation(THeadProduct *headProduct, THeadMachine *headMachine){
    printf(contrastColor"Preparando simulação...\n"resetColor);
    int machinesTotalCost = 0;
    TMachineOnProduction *machineOnProduction = loadMachinesOnProduction(headMachine,&machinesTotalCost);
    if (machineOnProduction == NULL) return;

    //print machines on production
    printf(contrastColor "Máquinas em produção:\n" resetColor);
    TMachineOnProduction *aux = machineOnProduction;
    while (aux != NULL){
        printf(listColor "%d " resetColor "- %s\n", aux->id,aux->model);
        if (aux->next != NULL)
            aux = aux->next;
        else
            break;
    }
    printf(contrastColor "Custo total das máquinas: " resetColor "%d\n",machinesTotalCost);
    printf("\n");
    printf("Deseja iniciar a simulação? ("contrastColor"1"resetColor" - Sim, "contrastColor"0"resetColor" - Não): ");

    int userOp = NULL;
    scanf("%d", &userOp);
    if (userOp == 0){
        system("@cls||clear"); //Limpa Tela
        return;
    }

    printf(listColor "Iniciando simulação...\n" resetColor);
    TPackaging *results = alocatePackaging(0,0,0,0,0,0);
    *results = simulationLoop(headProduct, machineOnProduction, (float)machinesTotalCost);

    userOp = NULL;//Valor aleatório para iniciar o loop (Já que a linguagem etende NULL como 0)
    while (true){
        printf(contrastColor"===================MENU SIMULAÇÃO===================\n" resetColor);
        printf(contrastColor "1" resetColor " - Mostrar dados da simulação\n");
        printf(contrastColor "2" resetColor " - Exportar dados para arquivo\n");
        printf(contrastColor "3" resetColor " - Mostrar e exportar dados\n");
        printf(contrastColor "0" resetColor " - Voltar\n");
        printf("\n");
        printf("Escolha uma opção: ");

        scanf("%d", &userOp);
        getchar();
        
        switch (userOp){
            case 1:
                system("@cls||clear"); //Limpa Tela
                //printSimulationData(machineOnProduction);
                break;
            case 2:
                system("@cls||clear"); //Limpa Tela
                //exportSimulationData(machineOnProduction);
                break;
            case 3:
                system("@cls||clear"); //Limpa Tela
                // printSimulationData(machineOnProduction);
                // exportSimulationData(machineOnProduction);
                break;
            case 0:
                system("@cls||clear"); //Limpa Tela
                return;
                break;
            default:
                printf(errorColor "Opção inválida!\n" resetColor);
                break;
        }
    }
}
#pragma endregion

int main(){
    THeadProduct *listProducts = alocateHeadProductMemorie(); //Alocação de memória para a cabeça da lista de produtos
    THeadMachine *listMachines = alocateHeadMachineMemorie(); //Alocação de memória para a cabeça da lista de máquinas

    int userOp = NULL;

    while (true){
        printf(contrastColor"=======================MENU PRINCIPAL=======================\n" resetColor);
        printf(contrastColor "1" resetColor " - Carregar Maquinas e Produtos disponíveis\n");
        printf(contrastColor "2" resetColor " - Imprimir Maquinas e Produtos disponíveis\n");
        printf(contrastColor "3" resetColor " - Gerar um arquivo de uma nova simulação (%s)\n", simulationFileName);
        printf(contrastColor "4" resetColor " - Carregar e rodar uma nova simulação (%s))\n", simulationFileName);
        printf(contrastColor "0" resetColor " - Sair\n");
        printf("\n");
        printf("Escolha uma opção: ");
        scanf("%d", &userOp);
        getchar();

        switch (userOp){
            case 1:
                system("@cls||clear"); //Limpa Tela

                if (fileExists(machinesFileName) && fileExists(productsFileName)){
                    if(listProducts->first != NULL || listMachines->first != NULL){
                        printf(errorColor"Já existe um arquivo de Produtos ou Maquias carregado!\n"resetColor);
                        break;
                    }
                    else{
                        loadProducts(&listProducts);
                        printProducts(listProducts);
                        printf("\n");
                        loadMachines(&listMachines);
                        printMachines(listMachines);
                        printf("\n");
                    }
                }
                else{
                    printf(errorColor"Arquivo de Maquinas ou Produtos não existe!\n"resetColor);
                    break;
                }
            break;
            
            case 2:
                system("@cls||clear"); //Limpa Tela

                if (listProducts->first == NULL || listMachines->first == NULL || listProducts->idCount == 0 || listMachines->idCount == 0){
                    printf(errorColor"Não há produtos ou Maquinas carregadas\n"resetColor);
                    break;
                }
                else{
                    printProducts(listProducts);
                    printf("\n");
                    printMachines(listMachines);
                    printf("\n");
                }
            break;
            
            case 3:
                system("@cls||clear"); //Limpa Tela

                if(listProducts->first != NULL || listMachines->first != NULL){
                    createSimulationFile(listMachines->idCount);
                }
                else{
                    printf(errorColor"Não há Produtos ou Maquinas carregadas\n"resetColor);
                }

                
            break;

            case 4:
                system("@cls||clear"); //Limpa Tela
                if(listProducts->first != NULL || listMachines->first != NULL){
                    if (fileExists(simulationFileName)){
                        simulation(listProducts, listMachines);
                    }
                    else{
                        printf(errorColor"Arquivo de simulação não encontrado\n"resetColor);
                    }
                }
                else{
                    printf(errorColor"Não há Produtos ou Maquinas carregadas\n"resetColor);
                }
            break;

            case 0:
                system("@cls||clear"); //Limpa Tela
                printf(contrastColor "Bye!\n" resetColor);
                exit(1);
            break;

            case 42:
                system("@cls||clear"); //Limpa Tela
                creditsMenu();
            break;

            default:
                system("@cls||clear"); //Limpa Tela
                printf(errorColor"Opção inválida!\n"resetColor);
            break;
        }
    }
    

    return 0;
}


void creditsMenu(){
    int userOp = 0;

    system("@cls||clear"); //Limpa Tela
    printf(contrastColor "==================Menu Créditos==================\n" resetColor);
    printf("Trabalho de Estruturas de Dados\n");
    printf("\n");
    printf("Autores: \n");
    printf(contrastColor "Marlon Henrique Sanches - RA: 2407388\n" resetColor);
    printf(contrastColor "Gustavo Henrique Cardoso de Araújo - RA: 2386712 \n" resetColor);
    printf("=================================================\n");
    // Autores: 
    // Marlon Henrique Sanches - RA: 2407388
    // Gustavo Henrique Cardoso de Araújo - RA: 2386712

    printf("\n");
    printf(contrastColor "0" resetColor " - Sair\n");
    scanf("%d", &userOp);
    getchar();
    if (userOp==0) {
        system("@cls||clear"); //Limpa Tela
        return;
    }
}