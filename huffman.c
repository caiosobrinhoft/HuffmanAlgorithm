#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "huffman.h"

#define TAM 256

//Funcoes da tabela de frequencia

void iniciarTabelaFreq(unsigned int *tabela){
    int i = 0;
    //Cria tabela de frequencia nula
    for(i = 0; i < TAM; i++){
            tabela[i] = 0;
    }
}

int lerTabelaFreq(FILE *arquivo){
    //arquivo = fopen("compactado.huf", "rb");
    int tam;

    fscanf(arquivo, "%d\n", &tam);
    char ch[tam];
    int freq[tam];

    for (int i = 0; i < tam; i++)
    {
        fscanf(arquivo, "%c\t%d\n", &ch[i], &freq[i]);
    }
    int pos;
    pos = ftell(arquivo);

    return pos;
}
    
void preencherTabelaFreq(unsigned int *tabela, unsigned char *string){
    int i = 0;
    //Enquanto a string for diferente do final do vetor, faz a contagem dos caracteres
    iniciarTabelaFreq(tabela);
	while(string[i] != '\0'){
        tabela[string[i]]++; //repeticoes de cada caractere
		i++;
    }
}

//Funcoes para lista

void inicializaLista(Lista *lista){
    lista->inicio = NULL;
	lista->tam = 0;
}

void inserirNaLista(No *no, Lista *lista){
    No *aux;
    //Verifica se o inicio esta vazio, se sim, insere no inicio
    if(lista->inicio == NULL){   
		lista->inicio = no;   
		lista->tam++;
	}
    //se nao, verifica a frequencia dos dois proximos caracteres
    else if(no->frequencia < (lista)->inicio->frequencia){
        no->proximo = lista->inicio;
        lista->inicio = no;
        lista->tam++;
    }
    else{
        aux = lista->inicio;
        //verifica se o proximo no existe e 
        //se a frequencia do novo caracter for maior que a do proximo,anda na lista
        while (aux->proximo && no->frequencia >= aux->proximo->frequencia){
            aux = aux->proximo;
        }
        no->proximo = aux->proximo;
		aux->proximo = no;
		lista->tam++;
    }
}

void preencherLista(Lista *lista, unsigned int *tabela){
	No *novo;

	for(int i = 0; i < TAM; i++){
		if(tabela[i] > 0){  //verifica se a frequencia e maior que 0
			novo = calloc(1, sizeof(No));
			novo->caracter = i; 
			novo->frequencia = tabela[i];
			novo->esquerda = NULL;
			novo->direita = NULL;
			novo->proximo = NULL;
			inserirNaLista(novo, lista);
		}
	}
}

No* removerDaLista(Lista *lista){
    //Coloca as frequencias ja ordenadas na arvore e remove dalista
	No *remover = NULL;

	if(lista->inicio){ //Verifica se a lista eh nula
		remover = lista->inicio;
		lista->inicio = remover->proximo;
		lista->tam--;
	}
	return remover; //retorna o nó removido
}

void liberaLista(Lista* lista){
    //desaloca a memoria onde esta a lista encadeada
	No* aux = lista->inicio;
	No* aux2;
	while(aux!=NULL){
		aux2 = aux->proximo;
		free(aux);
		aux = aux2;
	}
}

//Funcoes para arvore
No* criaArvore(Lista *lista){
	No *novo, *aux, *primeiro, *segundo;

	while(lista->tam >= 2){
		primeiro = removerDaLista(lista); //remove o inicio da lista.
		segundo = removerDaLista(lista);  //remove o início da lista
		novo = calloc(1, sizeof(No));
		novo->caracter = 'P';
		novo->frequencia = primeiro->frequencia + segundo->frequencia; //somas as frequencias anteriores
		novo->esquerda = primeiro; 
		novo->direita = segundo; 
		novo->proximo = NULL;
		inserirNaLista(novo, lista);
	}
	return lista->inicio;
}

int alturaArvore(No *raiz){
    int esquerda, direita;

    if(raiz == NULL){
        return -1;
    }
    else{
        esquerda = alturaArvore(raiz->esquerda) + 1;
        direita = alturaArvore(raiz->direita) + 1;

        if(esquerda > direita){
            return esquerda;
        }
        else{
            return direita;
        }
    }
}

void liberaArvore(No* raiz){
	if(raiz->esquerda == NULL && raiz->direita == NULL){
		free(raiz);
	}
	else{
		liberaArvore(raiz->esquerda);
		liberaArvore(raiz->direita);
	}
}

//Dicionario

char** alocaDicionario(int colunas){
    //aloca o dicionario na memoria
	int i;
	char** dicionario;

    dicionario = calloc(TAM, sizeof(char*));
    
    for(i = 0; i < TAM; i++){
        dicionario[i] = calloc(colunas, sizeof(char));
    }
    return dicionario;
}


void criarDicionario(char **dicionario, No *raiz, char *caminho, int coluna){
    //Percorre a arvore, assim, criando o dicionario
    char esquerda[coluna], direita[coluna];
    if(raiz->esquerda == NULL && raiz->direita == NULL){ //ao cair na folha da arvore, passa o caminho para o dicionario
        strcpy(dicionario[raiz->caracter], caminho);     
    }

    else{
        strcpy(esquerda, caminho); //acrescentado um "0" ao código ao percorrer o lado esquerdo da arvore
        strcpy(direita, caminho);  //acrescentando um "1" ao código ao percorrer o lado direito da arvore

        strcat(esquerda, "0");
        strcat(direita, "1");

        criarDicionario(dicionario, raiz->esquerda, esquerda, coluna);
        criarDicionario(dicionario, raiz->direita, direita, coluna);
    }
}

void liberaDicionario(char** dicionario){
	int i;

	for(i = 0; i < TAM; i++)
		free(dicionario[i]);
	free(dicionario);
}

int tamanhoString(char **dicionario, unsigned char *string){
    int i = 0, tam = 0;

    while(string[i] != '\0'){ // Enquanto não for o fim da string
        tam += strlen(dicionario[string[i]]); // tam recebe a quantidade de caracteres na string
        i++; 
    }
    return tam + 1;
}

// Função para codificar o texto.
char *codificacao(char **dicionario, unsigned char *string){
    char *texto_codificado;

	texto_codificado = calloc(tamanhoString(dicionario, string)+2, sizeof(char));

    int i = 0; 

    while(string[i] != '\0'){ // Enquanto não for o fim de string.
        strcat(texto_codificado, dicionario[string[i]]);
        i++;
    }
    return texto_codificado; 
}

char *decodificacao(No *raiz, unsigned char *str){
    No *aux = NULL;
    aux = raiz;
    char aux2[2], *texto_decodificado;
    int i = 0;

	texto_decodificado = calloc(strlen(str), sizeof(char));

    while(str[i] != '\0'){ // Enquanto não for o fim de "str".
        if(str[i] == '1'){ // Se na minha codificação for "1", ando a direita da árvore, senão vou para a esquerda.
            aux = aux->direita;
        }

        else{
            aux = aux->esquerda; // Se na minha codificação for "0", ando a esquerda da árvore.
        }

        if(aux->esquerda == NULL && aux->direita == NULL){ // Se entrar aqui, então eu cheguei em um nó folha.
            aux2[0] = aux->caracter; // Recebo o caracter que está no nó folha.
            aux2[1] = '\0'; // Recebo o fim de string.
            strcat(texto_decodificado, aux2); // Vou concatenando os caracteres que vão sendo decodificados.
            aux = raiz; // Auxiliar volta novamente ao topo da árvore.
        }
        i++; 
    }
    return texto_decodificado;
}

//Compactacao e descompactacao

//compactar arquivo
void compactacao(unsigned char *string, int tamanho, unsigned int *tabela){
	FILE *arquivo = fopen("compactado.huf", "wb");
	unsigned char byte = 0, deslocamento;
	int i = 0, j = 7;

	if(!arquivo){
		printf("\nNao foi possivel criar o arquivo!\n");
	}
	else{
        fprintf(arquivo, "%d\n", tamanho);
        headerTabelaFreq(tabela, arquivo);
		while(string[i] != '\0'){ //Pega APENAS os caracteres do arquivo
			deslocamento = 1;
			if(string[i] == '1'){
				deslocamento = deslocamento << j; // Mascara será deslocado 7 vezes(10000000).
				byte = byte | deslocamento; // Operação entre máscara e byte, se mascara é 1, então o byte é 10000000.  
			}
			j--;

			if(j < 0){ 
				fwrite(&byte, sizeof(unsigned char), 1, arquivo); //arquivo << compactado
				byte = 0; //seta o byte como inicial(0)
				j = 7; 
			}
			i++;
		}
		if(j != 7){ // Se caso não for formado 1 byte inteiro.
			fwrite(&byte, sizeof(unsigned char), 1, arquivo);
		}
		fclose(arquivo);
	}
}

//Descompactacao

unsigned int verificaBits(unsigned char byte, int i){
    unsigned char deslocamento = (1 << i);
    return byte & deslocamento; //operacao and entre byte e a mascara
}

int tamanhoCompactado(No* raiz){
    FILE *arquivo = fopen("compactado.huf", "rb");
    No *aux = raiz;
    unsigned char byte;
    int i;
    int tamanho = 0;

    if(arquivo){
        while(fread(&byte, sizeof(unsigned char), 1, arquivo)){//enquanto nao for o fim do arquivo
            for(i = 7; i >= 0; i--){
                if(verificaBits(byte, i))//bit 1
                    aux = aux->direita;
                else//bit 0
                    aux = aux->esquerda;

                if(aux->esquerda == NULL && aux->direita == NULL){//no folha
                    tamanho++;
                    aux = raiz;
                }
            }
        }
        fclose(arquivo);
    }
    else
        printf("\nErro ao abrir arquivo em tamanhoCompactado\n");

    return tamanho;
}

void escreverNoArq(char *string){
    FILE* arquivo = fopen("saida.txt", "w");
    fprintf(arquivo, "%s", string);
    fclose(arquivo);
}

void descompactar(No *raiz) {
    FILE *arquivo = fopen("compactado.huf", "rb");
    unsigned char byte;
    int i;
    char *string;
    int j = 0;
    int pos;
    No *aux = raiz;

    fseek(arquivo, lerTabelaFreq(arquivo), SEEK_SET);
    int tamanho = tamanhoCompactado(raiz);
    string = calloc(tamanho + 2,sizeof(unsigned char));


    if(arquivo){
        while(fread(&byte, sizeof(unsigned char), 1, arquivo)){
            for(i = 7; i >= 0; i--){
                if(verificaBits(byte, i))
                    aux = aux->direita;
                else
                    aux = aux->esquerda;

                if(aux->esquerda == NULL && aux->direita == NULL){
                    string[j] = aux->caracter; //guarda o no folha na string que sera escrita novamente
                    aux = raiz; //retorna ao inicio da arvore
                    j++;
                }
            }
        }
        fclose(arquivo);
        escreverNoArq(string);
    }
    else
        printf("\nErro ao abrir arquivo em descompactar\n");
}

//conta o numero de caracteres em um arquivo
int tamanhoTxt(char nome_arquivo[]){
    FILE *arquivo = fopen(nome_arquivo, "r");
    int tamanho = 0;

    if(arquivo){
        while(fgetc(arquivo) != -1)
            tamanho++;
        fclose(arquivo);
    }
    else
        printf("\nErro ao abrir arquivo em tamanhoTxt\n");
    return tamanho;
}

// função para ler um texto de um arquivo texto
void lerTxt(unsigned char *texto, char nome_arquivo[]){
    FILE *arquivo = fopen(nome_arquivo, "r");
    unsigned char letra;
    int i = 0;

    if(arquivo){
        while(!feof(arquivo)){
            letra = fgetc(arquivo);
            if(letra != -1){
                texto[i] = letra;
                i++;
            }
        }
        fclose(arquivo);
    }
    else
        printf("\nErro ao abri arquivo em ler_texto\n");
}

//aloca memoria para a string q recebera o conteudo do arquivo txt
unsigned char* alocaMemoriaTexto(char nome_arquivo[]){
	unsigned char* texto;
	int tamanho = tamanhoTxt(nome_arquivo);
	texto = calloc(tamanho + 2, sizeof(unsigned char));//quantidade alocacada de acordo com o tamanho do arquivo
	return texto;
}

void imprimirTabelaAscii(unsigned int *tabela){
	int i = 0;
	
	printf("\nTabela ASCII:\n Ind:  Carac:\n");
	for(i = 0; i < TAM; i++){
		printf("%3d\t%c\n", i, i);
	}
}

void imprimirTabelaFreq(unsigned int *tabela){
	int i = 0;
	
	printf("\nTabela de frequencia:\nRepeticao   Caracter\n");
	for(i = 0; i < TAM; i++){
		if(tabela[i] > 0){
			printf("%5d\t%8c\n", tabela[i], i);
		}
	}
}

void headerTabelaFreq(unsigned int *tabela, FILE *arquivo){
    //Imprime a tabela de frequencia como header no arquivo compactado
	int i = 0;
	
	for(i = 0; i < TAM; i++){
		if(tabela[i] > 0){
			fprintf(arquivo, "%c\t%d\n", i, tabela[i]);
		}
	}
}

int tamanhoTabelaFreq(unsigned int *tabela){
    //Calcula o numero de caracteres da tabela de frequencia, inclusive os espacos
	int i = 0;
	int count = 0;
	for(i = 0; i < TAM; i++){
		if(tabela[i] > 0){
			count++;
		}
	}
	return count;
}

void imprimirLista(Lista *lista){ //Imprime a lista ordenada
	No *aux;
	
	aux = lista->inicio;
	
	printf("\nLista Encadeada Ordenada:\n");
	printf("Frequencia  Caracter\n");
	while(aux){
		printf("%5d%11c\n", aux->frequencia, aux->caracter);
		aux = aux->proximo;
	}
}

void imprimirDicionario(char **dicionario){ //imprime o dicionario
    int i = 0;

	printf("\nDicionario\n");
	printf("Char     Binario\n");
    for(i = 0; i < TAM; i++){
        if(strlen(dicionario[i]) > 0){ //imprime os codigos do chars
            printf("%4c\t\t%s\n", i, dicionario[i]);
        }
    }
}

void 
imprimirArvore(No *raiz, int altura_no){
	if(raiz->esquerda == NULL && raiz->direita == NULL){ //caso seja nó folha
		printf("%4d%10c\n", altura_no, raiz->caracter);
	}

	else{ // Senão, vou percorrendo a árvore.
		imprimirArvore(raiz->esquerda, altura_no + 1);
		imprimirArvore(raiz->direita, altura_no + 1);
	}
}
