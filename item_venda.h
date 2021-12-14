#ifndef ITEMVENDAH
#define ITEMVENDAH
#include <stdint.h>
#include <stdio.h>

#define TAM_ITEM sizeof(ITEM_VENDA)

typedef struct ITEM_VENDA{
    uint32_t id;
    uint32_t id_venda;
    uint32_t data;
    float desconto;
    char obs[1008];
}ITEM_VENDA;

#endif