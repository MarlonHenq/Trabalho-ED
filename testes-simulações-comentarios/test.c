#include <stdio.h>
#include <stdbool.h>

int main(){

    int cu = 0;

    while(true){
        if (cu >= 600000000000){
            break;
        }

        printf ("%d\n", cu/60*100);

        printf("Hello World %d\n", cu);
        cu++;
    }

    return 0;
}