#include "ordenacao.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "buffer_entrada.h"
#include "buffer_saida.h"
#include "item_venda.h"

/*
 * função para obter o tamanho do arquivo em bytes
 */
long int tamanho_arquivo(char nome_arquivo[]) {
    FILE* file = fopen(nome_arquivo, "rb");

    fseek(file, 0, SEEK_END);


    long int tamanho = ftell(file);

    fclose(file);
    return tamanho;
}

/*
 * funcão de comparação de itens
 */
int cmp(const void* a, const void* b) {
    ITEM_VENDA* item_a = (ITEM_VENDA*)a;
    ITEM_VENDA* item_b = (ITEM_VENDA*)b;

    return item_a->id - item_b->id;
}

/*
 * intercala buffers de entrada para um buffer de saida
 */
void intercalacao(BufferEntrada** buffers_entrada, long int qtde_entrada,
                  BufferSaida* buffer_saida) {
    while (1) {
        long int idc_menor = 0;

        // define o indice do primeiro buffer de entrada que não está vazio
        while (idc_menor < qtde_entrada &&
            BufferEntrada_Vazio(buffers_entrada[idc_menor])) {
            idc_menor++;
        }

        if (idc_menor == qtde_entrada)  // todos os buffers foram consumidos
          break;

        ITEM_VENDA menor_item = BufferEntrada_Proximo(buffers_entrada[idc_menor]);

        int idc = idc_menor;

        // laço para identifica o indice do menor (id) item
        do {
            if (BufferEntrada_Vazio(buffers_entrada[idc])) continue; // ignora se está vazio

            ITEM_VENDA cmp_item = BufferEntrada_Proximo(buffers_entrada[idc]);

            if (menor_item.id > cmp_item.id) { // compara com o menor ate o momento
                idc_menor = idc;
                menor_item = cmp_item;
            }

        } while (++idc < qtde_entrada);

        // consome o menor dos itens dos buffers de entrada e insere no buffer de saida
        BufferSaida_Inserir(buffer_saida,
                            BufferEntrada_Consumir(buffers_entrada[idc_menor]));
    }

    // caso haja registros, salva o restante no arquivo de saida
    BufferSaida_Despejar(buffer_saida);
}


/*
 * define quantidade de buffers de entrada
 * divide arquivo e ordena cada parte
 * salva em arquivos temporarios
 * cria buffers de entrada e o buffer de saida
 * chama a intercalação dos buffers
 */
void ordenacao_externa(char nome_arquivo[], long int memoria_max_B,
                       long int tam_buffer_saida_S, char arquivo_saida[]) {
    long int tam_arq = tamanho_arquivo(nome_arquivo);

    // define quantidade (K) de buffers de divisões no arquivo
    long int divisoes_K = (int)ceil((double)tam_arq / memoria_max_B);

    // quantidade maxima de bytes por buffer
    long int max_itens_buffer =
        floor(((memoria_max_B - tam_buffer_saida_S) / divisoes_K) / TAM_ITEM);

    BufferEntrada** buffers =
        (BufferEntrada**)calloc(sizeof(BufferEntrada*), divisoes_K);

    FILE* arq_entrada = fopen(nome_arquivo, "rb");

    // laço para a leitura e ordenação de cada parte do arquivo de entrada
    long int restante_arq = tam_arq;
    for (int i = 0; i < divisoes_K; i++) {
        int qtde_bytes =
            restante_arq > memoria_max_B ? memoria_max_B : restante_arq;
        int qtde_itens = qtde_bytes / TAM_ITEM;

        ITEM_VENDA* entrada = (ITEM_VENDA*)calloc(TAM_ITEM, qtde_itens);

        // leitura dos registros
        long int readed = fread(entrada, TAM_ITEM, qtde_itens, arq_entrada);

        // ordenação dos registros
        qsort(entrada, qtde_itens, TAM_ITEM, cmp);  // TODO: criar um metodo e alterar depois

        // criação de um arquivo temporario (será excluido automaticamente no final da execução)
        FILE* aqr_temp = tmpfile();

        // escrita dos registros ordenados no arquivo temporario
        fwrite(entrada, TAM_ITEM, qtde_itens, aqr_temp);

        // criação do buffer de entrada
        buffers[i] = BufferEntrada_Criar(aqr_temp, max_itens_buffer);

        free(entrada);
        restante_arq -= qtde_bytes;
    }

    fclose(arq_entrada);

    // criação de buffer de saida
    BufferSaida* saida = BufferSaida_Criar(arquivo_saida, tam_buffer_saida_S / TAM_ITEM);

    // chama a intercalação dos buffers
    intercalacao(buffers, divisoes_K, saida);

    // desaloca a memoria dos buffers de entrada e saida
    for (int i = 0; i < divisoes_K; ++i) {
        BufferEntrada_Destruir(buffers[i]);
    }
    free(buffers);
    BufferSaida_Destruir(saida);
}
