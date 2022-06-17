#include <stdio.h>
#include <stdlib.h>
//Provável adição de um randon math

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

const char machinesFileName[20] = "Maquinas.csv"; //Arquivos para carregamento de dados
const char productsFileName[20] = "Produtos.csv";
const char simulationFileName[20] = "Simulacao.csv";
#pragma endregion

#pragma region "Structs"
typedef struct product{
    int id; //ID que receberá em ordem de carregamento do arquivo
    char productionType[2]; //NOME abreviado (C - coxinha | P - Peixe | A - Almôndega)
    float productionCust; //Custo de Produção (EM REAIS)
    float salePrice; //Preço Atacado (Preço da venda no atacado) (EM REAIS)
    int deteriorationTime; //Tempo de deteriorização (EM SEGUNDOS) 
    int productionProbability; //Probabilidade de ser produzido (%)
    struct product *next; //Ponteiro para o próximo produto
}TProduct;

typedef struct machine{
    int id; //ID que receberá em ordem de carregamento do arquivo
    char model[30]; //Modelo
    char productionType[2]; //Tipo de produto processado (T repesentando todos)
    int productionTime; //Tempo de produção quem pode variar +/- 10% (Constante productionTimeVariation)
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


#pragma endregion

#pragma region "Funções Menu" //Submenus
void creditsMenu();
void createSimulationFileMenu();
#pragma endregion

#pragma region "Funções Internas" //Alocação de memória
TProduct *alocateProductMemorie(int id, char productionType[2], float productionCust, float salePrice, int deteriorationTime, int productionProbability){
    TProduct *newProduct = (TProduct *)malloc(sizeof(TProduct)); //Alocação de memória para o novo produto

    if (newProduct == NULL){ //Verificação de erro de alocação de memória
        printf(errorColor"Erro ao alocar memória para o produto!\n"resetColor);
        return NULL;
    }

    //Atribuição de valores
    newProduct->id = id;
    strcpy(newProduct->productionType, productionType);
    newProduct->productionCust = productionCust;
    newProduct->salePrice = salePrice;
    newProduct->deteriorationTime = deteriorationTime;
    newProduct->productionProbability = productionProbability;
    newProduct->next = NULL;

    return newProduct;
}

TMachine *alocateMachineMemorie(int id, char model[30], char productionType[2], int productionTime, int consumption, int price){
    TMachine *newMachine = (TMachine *)malloc(sizeof(TMachine)); //Alocação de memória para a nova maquina
    
    if (newMachine == NULL){ //Verificação de erro de alocação de memória
        printf(errorColor"Erro ao alocar memória para a máquina!\n"resetColor);
        return NULL;
    }

    //Atribuição de valores
    newMachine->id = id;
    strcpy(newMachine->model, model);
    strcpy(newMachine->productionType, productionType);
    newMachine->productionTime = productionTime;
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

#pragma endregion

#pragma region "Funções de Leitura e Print" //Carregamento de arquivos
void loadProducts(THeadProduct **headProduct){
    FILE *file = fopen(productsFileName, "r"); //Abertura do arquivo
    
    if (file == NULL){ //Verificação de erro de abertura do arquivo
        printf(errorColor"Erro ao abrir o arquivo de produtos!\n"resetColor);
        return;
    }

    int id = 1;

    //Variáveis de atribuição
    char productionType[2];
    float productionCust;
    float salePrice;
    int deteriorationTime;
    int productionProbability;

    //Carregamento de dados
    while (fscanf(file, "%s %f %f %d %d", productionType, &productionCust, &salePrice, &deteriorationTime, &productionProbability) != EOF){
        fscanf(file, "%d %s %f %f %d %d", &id, &productionType, &productionCust, &salePrice, &deteriorationTime, &productionProbability);
        TProduct *newProduct = alocateProductMemorie(id, productionType, productionCust, salePrice, deteriorationTime, productionProbability);

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
        printf("%d   |   %s   |   %f   |   %f   |   %d   |   %d   |\n", aux->id, aux->productionType, aux->productionCust, aux->salePrice, aux->deteriorationTime, aux->productionProbability);
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
    int productionTime;
    int consumption;
    int price;
    
    //Carregamento de dados
    while (fscanf(file, "%s %s %d %d %d", model, productionType, &productionTime, &consumption, &price) != EOF){
        fscanf(file, "%d %s %s %d %d %d", &id, model, productionType, &productionTime, &consumption, &price);
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

        printf("ID   | Tipo  | Tempo  | Consumo |   Preço    |  Modelo\n");
        printf("-----|-------|--------|---------|------------|------------\n");

    while (aux != NULL){
        //Imprime uma tabela com os dados das máquinas
        printf("%d    |   %s   |   %d   |   %d    |   %d   |   %s\n", aux->id, aux->productionType, aux->productionTime, aux->consumption, aux->price, aux->model);
        aux = aux->next;
    }
}

#pragma endregion

#pragma region "Funnções de Criação de Arquivos" //Criação de arquivos

void createSimulationFile(){
    FILE *file = fopen(simulationFileName, "w"); //Abertura do arquivo
    if (file == NULL){ //Verificação de erro de abertura do arquivo
        printf(errorColor"Erro ao abrir o arquivo de simulação!\n"resetColor);
        return;
    }

    int machineID = NULL;
    int numberOfMachines = NULL;

    while (1==1){
        printf("Digite o ID da máquina que deseja adicionar (Para parar de adicionar digite '0'): ");
        scanf("%d", &machineID);
        if (machineID != 0){
            fprintf(file, "%d ", machineID);
        }
        else{
            break;
        }

        printf("Digite o número de máquinas que deseja adicionar: ");
        scanf("%d", &numberOfMachines);
        fprintf(file, "%d\n", numberOfMachines);
    }

    fclose(file); //Fechando o arquivo
}

#pragma endregion

#pragma region "Funções de Simulação" //Simulação
#pragma endregion
int main(){
    THeadProduct *listProducts = alocateHeadProductMemorie(); //Alocação de memória para a cabeça da lista de produtos
    THeadMachine *listMachines = alocateHeadMachineMemorie(); //Alocação de memória para a cabeça da lista de máquinas

    int userOp = NULL;

    while (userOp!=5){
        printf(contrastColor"=======================MENU PRINCIPAL=======================\n" resetColor);
        printf(contrastColor "1" resetColor " - Carregar Maquinas e Produtos disponíveis\n");
        printf(contrastColor "2" resetColor " - Imprimir Maquinas e Produtos disponíveis\n");
        printf(contrastColor "3" resetColor " - Gerar um arquivo de uma nova simulação (%s)\n", simulationFileName);
        printf(contrastColor "4" resetColor " - Carregar e rodar uma nova simulação (%s))\n", simulationFileName);
        printf("\n");
        printf("Escolha uma opção: ");
        scanf("%d", &userOp);
        getchar();

        switch (userOp){
            case 1:
                system("@cls||clear"); //Limpa Tela

                loadProducts(&listProducts);
                printProducts(listProducts);
                printf("\n");
                loadMachines(&listMachines);
                printMachines(listMachines);
                printf("\n");
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
                createSimulationFile();
            break;

            case 4:
                system("@cls||clear"); //Limpa Tela

            break;

            case 5:

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