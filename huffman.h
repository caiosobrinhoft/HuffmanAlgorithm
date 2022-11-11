#ifndef huffman_h
#define huffman_h
#define TAM 256

typedef struct no{
	unsigned char caracter;
	int frequencia;
	struct no *proximo;
	struct no *esquerda;
	struct no *direita;
}No;

typedef struct{
	No *inicio;
	int tam;
}Lista;


//funcoes tabela frequencia
void iniciarTabelaFreq(unsigned int *tabela);
void preencherTabelaFreq(unsigned int *tabela, unsigned char *string);
int lerTabelaFreq(FILE *arquivo);

//funcoes para lista encadeada
void inicializaLista(Lista *lista);
void inserirNaLista(No *no, Lista *lista);
void preencherLista(Lista *lista, unsigned int *tabela);
No* removerDaLista(Lista *lista);
void liberaLista(Lista* lista);

//funcoes para arvore
No* criaArvore(Lista *lista);
int alturaArvore(No *raiz);
void liberaArvore(No* raiz);

//dicionario
char** alocaDicionario(int colunas);
void criarDicionario(char **dicionario, No *raiz, char *caminho, int coluna);
void liberaDicionario(char** dicionario);


//codificacao e decodificacao
int tamanhoString(char **dicionario, unsigned char *string);
char *codificacao(char **dicionario, unsigned char *string);
char *decodificacao(No *raiz, unsigned char *str);

//compactacao
void compactacao(unsigned char *string, int tamanho, unsigned int *tabela);

//descompactacao
unsigned int verificaBits(unsigned char byte, int i);
int tamanhoCompactado(No* raiz);
void escreverNoArq(char *string);
void descompactar2(No *raiz);
int tamanhoTxt(char nome_arquivo[]);
void lerTxt(unsigned char *texto, char nome_arquivo[]);
unsigned char* alocaMemoriaTexto(char nome_arquivo[]);

//utils
void imprimirTabelaAscii(unsigned int *tabela);
void imprimirTabelaFreq(unsigned int *tabela);
void headerTabelaFreq(unsigned int *tabela, FILE *arquivo);
int tamanhoTabelaFreq(unsigned int *tabela);
void imprimirLista(Lista *lista);
void imprimirDicionario(char **dicionario);
void imprimirArvore(No *raiz, int altura_no);

#endif