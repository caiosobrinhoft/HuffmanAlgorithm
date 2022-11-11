#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

#define TAM 256

int main(){
    FILE *input, *output;
    No *arvore;
    Lista lista;
    int colunas;
    char **dicionario;
    char *codificado, *decodificado;

    unsigned char *texto;
    unsigned char *string;
    unsigned int tabela[TAM];
    int pos;
    int op;

    input = fopen("texto.txt","r");
    fseek(input, 0, SEEK_END);
    pos = ftell(input);
    texto = malloc(pos*sizeof(unsigned char));
    string = malloc(pos*sizeof(unsigned char));
    fseek(input, 0, SEEK_SET);
    fread(texto, 1, pos, input);
            
    preencherTabelaFreq(tabela, texto);
    int tamanhoTab;
    tamanhoTab = tamanhoTabelaFreq(tabela);

    inicializaLista(&lista);
    preencherLista(&lista, tabela);

    arvore = criaArvore(&lista);
        
    colunas = alturaArvore(arvore)+1;
    dicionario = alocaDicionario(colunas);
    criarDicionario(dicionario, arvore, "", colunas);

    codificado = codificacao(dicionario, texto);
    decodificado = decodificacao(arvore, codificado);


    printf("\nDigite a opcao que deseja ");
    printf("\n 1 - Comprimir arquivo");
    printf("\n 2 - Descomprimir arquivo");
    printf("\n 3 - Sair\n");
    scanf("%d", &op);
    switch (op)
    {
    case 1:
        compactacao(codificado, tamanhoTab, tabela); //compacta o arquivo e ja cria o cabeçalho
        output = fopen("compactado.huf", "rb");

        printf("\nTamanho do arquivo de texto: %d", getFileSize(input));
        printf("\nTamanho do arquivo compactado: %d", getFileSize(output));
        printf("\nBytes economizados: %d\n", getFileSize(input)-getFileSize(output));
        break;
    case 2:
        descompactar(arvore);
        break;
    default:
        break;
    }

    free(texto);
    free(decodificado);
    free(codificado);
    liberaLista(&lista);
    liberaArvore(arvore);
    liberaDicionario(dicionario);
    fclose(input);
    
    return(0);
}