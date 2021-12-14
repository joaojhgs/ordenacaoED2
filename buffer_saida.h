#ifndef BUFFERSAIDAH
#define BUFFERSAIDAH
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "item_venda.h"

typedef struct BufferSaida{
    ITEM_VENDA * registros;
    long int idc_escrita;
    long int max_registros;
    FILE* arq;
}BufferSaida;

BufferSaida* BufferSaida_Criar(char[], long int);

void BufferSaida_Inserir(BufferSaida*, ITEM_VENDA);

void BufferSaida_Despejar(BufferSaida*);

void BufferSaida_Destruir(BufferSaida*);
#endif