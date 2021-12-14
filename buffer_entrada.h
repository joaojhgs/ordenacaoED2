#ifndef BUFFERENTRADAH
#define BUFFERENTRADAH
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "item_venda.h"

typedef struct BufferEntrada{
    ITEM_VENDA* registros;
    long int lidos;
    long int idc_leitura;
    long int max_itens;
    FILE* arq;
}BufferEntrada;

BufferEntrada* BufferEntrada_Criar(FILE*, long int);

ITEM_VENDA BufferEntrada_Proximo(BufferEntrada*);

ITEM_VENDA BufferEntrada_Consumir(BufferEntrada*);

int BufferEntrada_Vazio(BufferEntrada*);

void BufferEntrada_Destruir(BufferEntrada*);
#endif