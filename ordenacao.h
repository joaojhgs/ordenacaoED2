#ifndef ORDENACAOH
#define ORDENACAOH

#include "buffer_entrada.h"
#include "buffer_saida.h"

/*
 * separa o arquivo de entrada, ordena cada parte, cria os buffers e intercala
 */
void ordenacao_externa(char [], long int, long int, char []);

/*
 * intercala buffers de entrada e salva no buffer de saida
 */
void intercalacao(BufferEntrada**, long int, BufferSaida*);

#endif