#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "item_venda.h"
#include "buffer_saida.h"

BufferSaida* BufferSaida_Criar(char nome_arquivo[], long int max_registros) {

    BufferSaida* buffer = (BufferSaida* ) malloc(sizeof(BufferSaida));
    buffer->arq = fopen(nome_arquivo, "wb");
    buffer->max_registros = max_registros;
    buffer->idc_escrita = 0;
    buffer->registros = (ITEM_VENDA*)calloc(TAM_ITEM, max_registros);

    return buffer;
}

void BufferSaida_Destruir(BufferSaida* buffer) {
    fclose(buffer->arq);
    free(buffer->registros);
    free(buffer);
};

void BufferSaida_Inserir(BufferSaida* buffer, ITEM_VENDA item) {

    // se todos os espaços forem ocupados
    if (buffer->idc_escrita == buffer->max_registros) {
        BufferSaida_Despejar(buffer); // salva o buffer no arquivo de saida
        buffer->idc_escrita = 0; // reinicia o buffer
    }

    // copia o item para o buffer
    memcpy(&buffer->registros[buffer->idc_escrita++], &item, TAM_ITEM);
}

void BufferSaida_Despejar(BufferSaida* buffer) {
    // escreve os itens no arquivo de saida
    fwrite(buffer->registros, TAM_ITEM, buffer->idc_escrita, buffer->arq);    
}