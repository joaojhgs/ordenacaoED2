#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "buffer_entrada.h"
#include "buffer_saida.h"
#include "ordenacao.h"

void check_ordenacao(char arquivo[]) {
    FILE* file = fopen(arquivo, "rb");

    ITEM_VENDA cmp;
    fread(&cmp, TAM_ITEM, 1, file);
    while(!feof(file)) {
        ITEM_VENDA next;
        fread(&next, TAM_ITEM, 1, file);

        if (cmp.id > next.id) {
            printf("Não está ordenado\n");
            fclose(file);
            exit(1);
        }

        cmp = next;
    }
    fclose(file);
    printf("Ordenado\n");
}

int main(int argc, char** argv){

    long int B = 1024 * 1024 * 8; // 1MB;
    long int S = B / 8;

    ordenacao_externa("teste.dat", B, S, "saida.dat"); // 1MB, 32KB
    // check_ordenacao("saida.dat");

    return 0;
}
