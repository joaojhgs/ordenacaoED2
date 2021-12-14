#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "item_venda.h"
#include "buffer_entrada.h"


void leitura_registros(BufferEntrada* buffer) {
    buffer->idc_leitura = 0; // reinicia buffer

    // le determinada quantidade de bytes do arquivo de temporario do buffer
    buffer->lidos = fread(buffer->registros, TAM_ITEM, buffer->max_itens, buffer->arq);
}

BufferEntrada* BufferEntrada_Criar(FILE* entrada, long int max_itens_buffer) {

    BufferEntrada* buffer = (BufferEntrada*)malloc(sizeof(BufferEntrada));
    buffer->arq = entrada;
    buffer->max_itens = max_itens_buffer;
    buffer->registros = (ITEM_VENDA *) calloc(buffer->max_itens, TAM_ITEM);

    fseek(buffer->arq, 0, SEEK_SET);
    leitura_registros(buffer);

    return buffer;
}

ITEM_VENDA BufferEntrada_Proximo(BufferEntrada* buffer) {
    // returna o proximo item do buffer
    return buffer->registros[buffer->idc_leitura];
}

ITEM_VENDA BufferEntrada_Consumir(BufferEntrada* buffer) {

    // pega o proximo
    ITEM_VENDA item = buffer->registros[buffer->idc_leitura++];

    // se não há mais disponiveis, efetua a leitura de mais uma parcela de registros
    if (buffer->idc_leitura == buffer->max_itens) {
        leitura_registros(buffer);
    }

    return item;
}

int BufferEntrada_Vazio(BufferEntrada* buffer) {
    if(buffer == NULL)
        return 1;

    // caso tenha lido menos que o maximo disponivel, considera a quantidade lida
    int cmp = buffer->lidos < buffer->max_itens ? buffer->lidos : buffer->max_itens;

    return buffer->idc_leitura < cmp ? 0 : 1;
}

void BufferEntrada_Destruir(BufferEntrada* buffer) {
    free(buffer->registros);
    fclose(buffer->arq);
    free(buffer);
}

