#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "externHash.h"

//#define TAM 100

//Estrutura de Dados
struct linhas{
  int Linha;
  struct linhas *Prox;
};
struct nodo{
  char Palavra[50];
  struct linhas *IndiceI;
  struct linhas *IndiceF;
  struct nodo *Prox;
};
struct tabela{
  struct nodo *Inicio;
  struct nodo *Fim;
  int Vazio;
  int Colisoes;
};
typedef struct nodo *Nodo;
typedef struct linhas *Linha;

//Cabeçalho
TabelaExtern ECriaTabela(void);
Nodo  EConsultaPalavra(TabelaExtern EHash, char *Palavra, int Op, int* vetor);
Nodo* ECriaVetorPalavras(TabelaExtern EHash);
void  EInserePalavra(TabelaExtern EHash, char *Palavra, int Op, int* vetor);
void  ECopiaPalavra(char *Palavra, char *Destino);
int   EParticao(Nodo *Vetor, int inicio, int fim);
void  EOrdenacao(Nodo *Vetor, int inicio, int fim);
// void  Insercao(Nodo *Vetor, int t);
// int   EPalavra1MaiorPalavra2(char *Palavra1, char *Palavra2);
int   EPalavra1MenorIgualPalavra2(char *Palavra1, char *Palavra2);
int   EComparaPalavra(char *Palavra1, char *Palavra2);
int   EVerificaColisao(TabelaExtern OHash, int posi);
int   EVerificaPalavraValida(char *Palavra);
int   ETamanhoChaves(TabelaExtern EHash);
// int   EConverteString(char *Palavra);
int   EFuncaoHash1(char *Palavra);
int   EFuncaoHash2(char *Palavra);
int   EFuncaoHash3(char *Palavra, int* vetor);

//Implementação
TabelaExtern ECriaTabela(void){
  int i;
  TabelaExtern aux = (TabelaExtern) malloc(TAM*sizeof(struct tabela));

  for(i=0; i<TAM ; i++){  //apos alocar a memoria para a hash predefino valores iniciais para a Estrutura
    aux[i].Inicio = NULL;
    aux[i].Fim = NULL;
    aux[i].Vazio = 0;
    aux[i].Colisoes = 0;
  }
  return(aux);
}

TabelaExtern ELeChaves(char Arq[], int Op, int* vetor){
  TabelaExtern EHash = ECriaTabela();                                 //função central de leitura de palavras chaves
  FILE *arquivo;
  char frase[100];
  int PInicio, i;

  arquivo = fopen(Arq, "r");
  if(arquivo){
    while(fgets(frase, sizeof(frase), arquivo)){                      //leio a frase do arquivo
      PInicio = 0;                                                    //variavel que controla o inicio de uma palavra
      for(i=0; i < strlen(frase); i++){                               //faço um for em todos os caracteres da frase
        if(!isalpha(frase[i])){                                       //enquanto não chegar a o fim de uma palavra ele continua lendo
          if(EVerificaPalavraValida(&frase[PInicio])){                //verifica se a palavra é valida
            if(!EConsultaPalavra(EHash, &frase[PInicio], Op, vetor)){ //consulta se a palavra encontrada ainda não foi inserida na hash para que não existam repetições
              EInserePalavra(EHash, &frase[PInicio], Op, vetor);      //insere a palavra
            }
          }
          PInicio = i + 1;                                            //apos o termino de uma palavra o proximo caracter podeser o inicio de outra
        }
      }
    }
    fclose(arquivo);
    return(EHash);
  }else{
    return(NULL);
  }
}

int ETamanhoChaves(TabelaExtern EHash){
  int i, cont = 0;

  for(i=0; i<TAM; i++){ //conta todas as palavras chaves da Hash
    if(EHash[i].Vazio){
      cont = cont + EHash[i].Colisoes + 1;
    }
  }
  return(cont);
}

int EVerificaPalavraValida(char *Palavra){
  int i,cont = 0;
  for(i=0; i<=strlen(Palavra); i++){    //faz um for contando a quantidade de caracteres de uma palavra
    if(isalpha(Palavra[i])){
      cont++;
    }else{
      if(cont>3){                       //se tiver tamanho > 3 retorna 1 caso contrario 0
        return(1);
      }else{
        return(0);
      }
    }
  }
  if(cont>3){
    return(1);
  }else{
    return(0);
  }
}

void EInserePalavra(TabelaExtern EHash, char *Palavra, int Op, int* vetor){
  int posi;
  Nodo nodo;

  if(Op==1){                              //atravez da opção passada como parametro a função define qual função hash deve ser usada
    posi = EFuncaoHash1(Palavra);
  }else{
    if(Op==2){
      posi = EFuncaoHash2(Palavra);
    }else{
      if(Op==3){
        posi = EFuncaoHash3(Palavra, vetor);
      }
    }
  }
  nodo = (Nodo) malloc(sizeof(struct nodo));
  nodo->Prox    = NULL;
  nodo->IndiceI = NULL;
  nodo->IndiceF = NULL;
  if(!EVerificaColisao(EHash, posi)){     //se existir colisão o nodo vai ser inserido no fim da lista
    EHash[posi].Fim    = nodo;
    EHash[posi].Inicio = nodo;
    EHash[posi].Vazio  = 1;
  }else{
    EHash[posi].Fim->Prox = nodo;         //se não ouver colisão o nodo será inserido no começo da lista
    EHash[posi].Fim       = nodo;
    EHash[posi].Colisoes++;
  }
  ECopiaPalavra(Palavra, EHash[posi].Fim->Palavra);    //copia a palavra para o destino no nodo
}

int EFuncaoHash1(char *Palavra){
  int i, h = 0;                         //função hash 1

  for(i=0; isalpha(Palavra[i]); i++){
    h = h + Palavra[i];
  }
  return(h%TAM);
}

int EFuncaoHash2(char *Palavra){
  int i, h = 0;                         //função hash 2

  for(i=0; isalpha(Palavra[i]); i++){
    h = ((h*2 + Palavra[i])+1932)%TAM;
  }
  return(h%TAM);
}

int EFuncaoHash3(char *Palavra, int* vetor){
  int i, h = 0;                         //função hash 3 indicada na bibiliografia de ziviane

  for(i=0; isalpha(Palavra[i]); i++){
    h = h + Palavra[i] * vetor[i];
  }
  return(h%TAM);
}

int* EPreencheVetorPesos(void){
  int* vetor = (int*) malloc(50*sizeof(int));   //o tamanho do vetor de pesos será 50 pois nenhuma palavra irá ultrapassar este tamanho
	int i;

	for(i=0; i < 50; i++){
		vetor[i] = (rand()/(double)RAND_MAX) * 10000;   //o vetor é preenchido com numeros de 0 a 10000
	}
  return(vetor);
}

int EVerificaColisao(TabelaExtern EHash, int posi){
  if(EHash[posi].Vazio == 0){   //existe uma variavel de controle "Vazio" que controla se ja foi inserido uma palavra ou não na hash
    return(0);
  }else{
    return(1);
  }
}

/*int EConverteString(char *Palavra){
  int i, h = 0;

  for (i = 0; isalpha(Palavra[i]); i++){
    h = h*2 + tolower(Palavra[i]);//h = h * 10 + s[i];
  }
  return(h);
}*/

void ECopiaPalavra(char *Palavra, char *Destino){
  int i;

  for(i=0; i <= strlen(Palavra); i++){    //um for em todo o tamanho da frase passada
    if(isalpha(Palavra[i])!=0){           //quando encontrar um caracter que não é uma letra ele define o fim da frase e retorna a função
      Destino[i] = tolower(Palavra[i]);
    }else{
      Destino[i]='\0';
      return;
    }
  }
}

int ETamanhoHash(void){
  return(TAM);
}

int EColisoes(TabelaExtern EHash){
  int i, cont = 0;

  for(i=0; i<TAM; i++){             //conta todas as colisões que aconteceram na hash
    if(EHash[i].Colisoes>0){
      cont = cont + EHash[i].Colisoes;
    }
  }
  return(cont);
}

int EComparaPalavra(char *Palavra1, char *Palavra2){
  int i;

  for(i=0; i< strlen(Palavra2); i++){
    if(isalpha(Palavra2[i])){
      if(tolower(Palavra1[i])!=tolower(Palavra2[i])){   //para que possa comparar as palavras pego o caracter minusculo de ambas para casa alguma seja maiuscula
        return(0);                                      //se for diferente retorna 0
      }
    }else{                                              //se chegar ao fim da palavra ele retorna 1
      return(1);
    }
  }
  return(1);
}

Nodo EConsultaPalavra(TabelaExtern EHash, char *Palavra, int Op, int* vetor){
  Nodo NodoPalavra;
  int posi;

  if(Op==1){                                              //atravez da opção passada como parametro a função define qual função hash deve ser usada
    posi = EFuncaoHash1(Palavra);
  }else{
    if(Op==2){
      posi = EFuncaoHash2(Palavra);
    }else{
      if(Op==3){
        posi = EFuncaoHash3(Palavra, vetor);
      }
    }
  }
  NodoPalavra = EHash[posi].Inicio;
  while(NodoPalavra){                                     //faz um loop até o dim da lista na posição de hash encontrada
    if(EComparaPalavra(NodoPalavra->Palavra, Palavra)){   //se encontrar ambas as palavras forem iguais ele retorna o endereço do nodo
      return(NodoPalavra);
    }else{
      NodoPalavra = NodoPalavra->Prox;
    }
  }
  return(NULL);
}

Nodo* ECriaVetorPalavras(TabelaExtern EHash){
  int i, cont=0, tam = ETamanhoChaves(EHash);
  Nodo* VetPalavras = (Nodo*) malloc(tam * sizeof(Nodo)); //aloco um vetor de ponteiros de nodo palavras do tamanho da quantidade de palavras na hash
  Nodo NodoPalavra;                                       //a ideia de usar ponteiros para a estutura é de reaproveitar que os dados que necessito já estão na memoria
  //e então apenas vou controlar um ponteiro para tais dados, e economizar memoria

  for(i=0; i<TAM; i++){                 //faço um loop em toda a estrutura copiando o ponteiro de cada nodo palavra para o vetor
    NodoPalavra = EHash[i].Inicio;
    while(NodoPalavra){
      VetPalavras[cont] = NodoPalavra;
      cont++;
      NodoPalavra = NodoPalavra->Prox;
    }
  }
  return(VetPalavras);
}

void EResultadoBusca(TabelaExtern EHash, FILE *arquivo){
  Nodo* VetPalavras = ECriaVetorPalavras(EHash);
  Linha NodoLinha;
  int tam, i;

  tam = ETamanhoChaves(EHash);
  EOrdenacao(VetPalavras, 0, TAM-1);//ordeno o vetor de ponteiros por ordem alfabetica pelas palavras que estão contidos nelas
  for(i=0; i<tam; i++){             //faço um loop no vetor inserindo cada palavra no arquivo texto, apos cada palavra insiro os indices das frases em que foram encontradas
    fprintf(arquivo, "%s ",VetPalavras[i]->Palavra);
    NodoLinha = VetPalavras[i]->IndiceI;
    while(NodoLinha){
      fprintf(arquivo, "%d ", NodoLinha->Linha);
      NodoLinha = NodoLinha->Prox;
    }
    fprintf(arquivo, "\n");
  }
}

void EBuscaPalavras(TabelaExtern EHash, char Arq[], int Op, FILE *arquivo, int* vetor){
  FILE *arquivotexto;                                                           //procedimento central de busca de palavras
  char frase[100000];
  int PInicio, i, linha = 0;
  Nodo NodoPalavra;
  Linha NodoLinha;

  arquivotexto = fopen(Arq, "r");
  if(arquivotexto){
    while(fgets(frase, sizeof(frase), arquivotexto)){                           //leio cada frase no arquivo
      PInicio = 0;                                                              //variavel de controle de inicio de uma palavra
      linha++;                                                                  //variavel de controle de qual linha está sendo lida no arquivo para armazenar o indice
      for(i=0; i < strlen(frase); i++){                                         //loop em toda a frase lida
        if(!isalpha(frase[i])){                                                 //enquanto o caracter lido for considerado uma letra ele continua lendo, quando encontrar algo diferente ele considera como fim de uma palavra
          if(EVerificaPalavraValida(&frase[PInicio])){                          //verifica se o tamanho da palavra é valido
            NodoPalavra = EConsultaPalavra(EHash, &frase[PInicio], Op, vetor);  //consulta a palavra na hash
            if(NodoPalavra){                                                    //se encontrar contabiliza linhas, caso contrario nao faz nada
              NodoLinha = (Linha) malloc(sizeof(struct linhas));                //cria uma estrutura para armazenar a nova linha onde a palavra foi encontrada
              NodoLinha->Linha = linha;
              NodoLinha->Prox = NULL;
              if(NodoPalavra->IndiceI){                                         //se esta palavra já tiver sido encontrada insire no final da lista
                if(NodoPalavra->IndiceF->Linha != linha){                       //se esta palavra ja foi encontrada nesta mesma linha ele não armazena o numero repetido
                  NodoPalavra->IndiceF->Prox = NodoLinha;
                  NodoPalavra->IndiceF       = NodoLinha;
                }
              }else{                                                            //se esta palavra ainda nao tiver sido encontrada no texto ele insere no inicio da lista
                NodoPalavra->IndiceI = NodoLinha;
                NodoPalavra->IndiceF = NodoLinha;
              }
            }
          }
          PInicio = i + 1;                                                      //inicio da proxima palavra é o final desta +1
        }
      }
    }
    fclose(arquivotexto);
  }else{
    return;
  }
}

int EPalavra1MenorIgualPalavra2(char *Palavra1, char *Palavra2){
  int tam1 = strlen(Palavra1);
  int tam2 = strlen(Palavra2);
  int i;

  if(tam1>tam2){
      i = tam1;
      tam1 = tam2;
      tam2 = i;
  }
  if(isalpha(Palavra1[0]) && isalpha(Palavra2[0])){   //verifica se nao esta acessando posições invalidas da palava, pode acontecer se ambas as palavras forem iguais
    if(Palavra1[0] < Palavra2[0]){                    //se a primeira posição da 1 palavra for menor que a primeira da 2 palavra retorna 1
      return(1);
    }else{
      if(Palavra1[0] > Palavra2[0]){                  //se a primeira posição da 1 palavra for maior que a primeira da 2 palavra retorna 0
        return(0);
      }else{
        if(Palavra1[0] == Palavra2[0]){               //caso ambos sejam iguais, eu chamo a função recursivamente como parametros a string começando pela proxima letra
          if(EPalavra1MenorIgualPalavra2(&Palavra1[1], &Palavra2[1])){    //assim é feita uma pilha que no final retorna o valor desejado
            return(1);
          }else{
            return(0);
          }
        }
      }
    }
  }
  return(1);                                        
}

/*int EPalavra1MaiorPalavra2(char *Palavra1, char *Palavra2){
  int tam1 = strlen(Palavra1);
  int tam2 = strlen(Palavra2);
  int i;

  if(tam1>tam2){
      i = tam1;
      tam1 = tam2;
      tam2 = i;
  }
  for(i=0; i<tam1; i++){
    if(Palavra1[i] > Palavra2[i]){
      return(1);
    }else{
      if(Palavra1[i] < Palavra2[i]){
        return(0);
      }else{
        if(EPalavra1MaiorPalavra2(&Palavra1[1], &Palavra2[1])){
          return(1);
        }else{
          return(0);
        }
      }
    }
  }
  return(0);
}*/

int EParticao(Nodo *Vetor, int inicio, int fim){
  Nodo aux, x;
	int i, j;

	x = Vetor[fim]; //pivo
	j = inicio-1;
	for(i=inicio; i < fim; i++){
		if(EPalavra1MenorIgualPalavra2(Vetor[i]->Palavra, x->Palavra)){
      j++;
      aux = Vetor[j];
      Vetor[j] = Vetor[i];
      Vetor[i] = aux;
		}
	}
  aux = Vetor[j+1];
  Vetor[j+1] = x;
  Vetor[fim] = aux;
	return(j+1);
}

void EOrdenacao(Nodo *Vetor, int inicio, int fim){
  if(inicio < fim){
    int q = EParticao(Vetor, inicio, fim);
    EOrdenacao(Vetor, inicio, q - 1);
    EOrdenacao(Vetor, q + 1, fim);
  }
}

/*void Insercao(Nodo *Vetor, int t){
	int i, j;
	Nodo aux;

  j = 0;
  while (j < t) {
		aux = Vetor[j];
		i = j-1;
		while (i >= 0 && EPalavra1MaiorPalavra2(Vetor[i]->Palavra, aux->Palavra)){
			Vetor[i+1] = Vetor[i];
			i--;
		}
		Vetor[i+1] = aux;
		j++;
	}
}*/

void ELiberaHash(TabelaExtern EHash){
  int i;
  Nodo aux;
  Linha aux2;                 //libera toda a estrutura

  for(i=0; i<TAM; i++){
    aux = EHash[i].Inicio;
    while(EHash[i].Inicio){
      aux2 = aux->IndiceI;
      while(aux->IndiceI){
        aux->IndiceI = aux2->Prox;
        free(aux2);
        aux2 = aux->IndiceI;
      }
      EHash[i].Inicio = aux->Prox;
      free(aux);
      aux = EHash[i].Inicio;
    }
  }
  free(EHash);
  EHash = NULL;
}
