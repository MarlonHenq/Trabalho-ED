 if (id == 0){
        return;
    }

    TMachineOnProduction *aux = *machineOnProduction;
    while(aux!=NULL){
        if(aux->id == id){
            aux->numberOfProducts = aux->numberOfProducts + 1;
            TLine *aux2 = aux->first; 

            if(aux->first == NULL){//Se for o primeiro
                
                aux->timeOfProduction = getTimeOfProductionProductOnMachine(*machineOnProduction, id, newProduct);

                printf("%d", aux->timeOfProduction); //VERIFICAR AQUI 

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
                    return;
                }
                else{
                    aux2 = aux2->next;
                }
            }
            return;
        }
        aux = aux->next;
    }