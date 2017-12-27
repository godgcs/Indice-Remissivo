/*Referencia http://www.geeksforgeeks.org/avl-tree-set-1-insertion/*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "arvoreAVL.h"

struct linhas{
  int Linha;
  struct linhas *Prox;
};
struct nodo{
  char Palavra[50];
  struct linhas *IndiceI;
  struct linhas *IndiceF;
  struct nodo *Esq;
  struct nodo *Dir;
  int Altura;
};

typedef struct nodo *Nodo;
typedef struct linhas *Linha;
/*Cabeçalho*/
int AAVLAltura(Nodo N);
int AAVLMax(int a, int b);
int AAVLGetBalance(ArvAVL N);
int AAVLVerificaPalavraValida(char *Palavra);
int AAVLPalavra1MenorIgualPalavra2(char *Palavra1, char *Palavra2);
int AAVLComparaPalavra(char *Palavra1, char *Palavra2);
void AAVLCopiaPalavra(char *Palavra, char *Destino);
void AAVLRetiraMaiusculas(char *Palavra, char *Destino);
ArvAVL AAVLConsultaPalavra(ArvAVL AArvore, char *Palavra);
ArvAVL AAVLCriaNoArvore(char *Palavra);
ArvAVL AAVLRightRotate(ArvAVL y);
ArvAVL AAVLLeftRotate(ArvAVL x);

/*Implementações*/
int AAVLAltura(Nodo N){
  if(!N){
    return(0);
  }
  return(N->Altura);
}

int AAVLMax(int a, int b){
  return (a > b)? a : b;
}

ArvAVL AAVLCriaNoArvore(char *Palavra){
  ArvAVL AArvore = (ArvAVL) malloc(sizeof(struct nodo));

  AArvore->Esq     = NULL;
  AArvore->Dir     = NULL;
  AArvore->IndiceI = NULL;
  AArvore->IndiceF = NULL;
  AArvore->Altura  = 1;
  AAVLCopiaPalavra(Palavra, AArvore->Palavra);

  return(AArvore);
}

ArvAVL AAVLRightRotate(ArvAVL y){
    ArvAVL x = y->Esq;
    ArvAVL T2 = x->Dir;

    // Perform rotation
    x->Dir = y;
    y->Esq = T2;

    // Update heights
    y->Altura = AAVLMax(AAVLAltura(y->Esq), AAVLAltura(y->Dir))+1;
    x->Altura = AAVLMax(AAVLAltura(x->Esq), AAVLAltura(x->Dir))+1;

    // Return new root
    return x;
}

ArvAVL AAVLLeftRotate(ArvAVL x){
    ArvAVL y = x->Dir;
    ArvAVL T2 = y->Esq;

    // Perform rotation
    y->Esq = x;
    x->Dir = T2;

    //  Update heights
    x->Altura = AAVLMax(AAVLAltura(x->Esq), AAVLAltura(x->Dir))+1;
    y->Altura = AAVLMax(AAVLAltura(y->Esq), AAVLAltura(y->Dir))+1;

    // Return new root
    return y;
}

int AAVLGetBalance(ArvAVL N){
  if(N == NULL){
    return(0);
  }
  return(AAVLAltura(N->Esq) - AAVLAltura(N->Dir));
}

ArvAVL AAVLInsert(ArvAVL node, char *Palavra){
  /* 1.  Perform the normal BST insertion */
  if(!node){
    return(AAVLCriaNoArvore(Palavra));
  }
  // printf("Comparando %s com %s\n", Palavra, node->Palavra);
  if(strcmp(Palavra, node->Palavra) < 0){
    node->Esq  = AAVLInsert(node->Esq, Palavra);
  }else{
    if(strcmp(Palavra, node->Palavra) > 0){
      node->Dir = AAVLInsert(node->Dir, Palavra);
    }else{ // Equal keys are not allowed in BST
      // printf("Palavra %s repetida\n",Palavra);
      return node;
    }
  }

  /* 2. Update height of this ancestor node */
  node->Altura = 1 + AAVLMax(AAVLAltura(node->Esq), AAVLAltura(node->Dir));

  /* 3. Get the balance factor of this ancestor
        node to check whether this node became
        unbalanced */
  int balance = AAVLGetBalance(node);

  // If this node becomes unbalanced, then
  // there are 4 cases

  // Left Left Case
  if ((balance > 1) && (strcmp(Palavra, node->Esq->Palavra) < 0))
    return AAVLRightRotate(node);

  // Right Right Case
  if ((balance < -1) && (strcmp(Palavra, node->Dir->Palavra) > 0))
    return AAVLLeftRotate(node);

  // Left Right Case
  if ((balance > 1) && (strcmp(Palavra, node->Esq->Palavra) > 0)){
    node->Esq =  AAVLLeftRotate(node->Esq);
    return AAVLRightRotate(node);
  }

  // Right Left Case
  if ((balance < -1) && (strcmp(Palavra, node->Dir->Palavra) < 0)){
    node->Dir = AAVLRightRotate(node->Dir);
    return AAVLLeftRotate(node);
  }

  /* return the (unchanged) node pointer */
  return(node);
}

void AAVLCopiaPalavra(char *Palavra, char *Destino){
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

void AAVLRetiraMaiusculas(char *Palavra, char *Destino){
  int i;

  for(i=0; i <= strlen(Palavra); i++){    //um for em todo o tamanho da frase passada
    if(isalpha(Palavra[i])!=0){           //quando encontrar um caracter que não é uma letra ele define o fim da frase e retorna a função
      Destino[i] = tolower(Palavra[i]);
    }else{
      Destino[i] = Palavra[i];
    }
  }
}

int AAVLVerificaPalavraValida(char *Palavra){
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

ArvAVL AAVLLeChaves(char Arq[]){
  ArvAVL AArvore = NULL;   //função central de leitura de palavras chaves
  FILE *arquivo;
  char frase[100];

  arquivo = fopen(Arq, "r");
  if(arquivo){
    while(fgets(frase, sizeof(frase), arquivo)){                      //leio a frase do arquivo
      AAVLRetiraMaiusculas(frase,frase);
      if(!isalpha(frase[strlen(frase)-1])){
        frase[strlen(frase)-1] = '\0';
      }
      AAVLCopiaPalavra(frase,frase);
      if(AAVLVerificaPalavraValida(frase)){
        AArvore = AAVLInsert(AArvore, frase);
      }
    }
    fclose(arquivo);

    return(AArvore);
  }else{
    return(NULL);
  }
}

void AAVLResultadoBusca(ArvAVL AArvore, FILE *arquivo){
  if(AArvore){
    Linha NodoLinha;
    AAVLResultadoBusca(AArvore->Esq, arquivo);
    NodoLinha = AArvore->IndiceI;
    if(NodoLinha){
      fprintf(arquivo,"%s ",AArvore->Palavra);
      while(NodoLinha){
        fprintf(arquivo, "%d ", NodoLinha->Linha);
        NodoLinha = NodoLinha->Prox;
      }
      fprintf(arquivo, "\n");
    }
    AAVLResultadoBusca(AArvore->Dir, arquivo);
  }
}

int AAVLPalavra1MenorIgualPalavra2(char *Palavra1, char *Palavra2){
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
          if(AAVLPalavra1MenorIgualPalavra2(&Palavra1[1], &Palavra2[1])){    //assim é feita uma pilha que no final retorna o valor desejado
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

int AAVLComparaPalavra(char *Palavra1Buscada, char *Palavra2){
  int i;

  for(i=0; i<= strlen(Palavra2); i++){
    if(isalpha(Palavra2[i])){
      if(tolower(Palavra1Buscada[i])!=tolower(Palavra2[i])){   //para que possa comparar as palavras pego o caracter minusculo de ambas para casa alguma seja maiuscula
        return(0);                                      //se for diferente retorna 0
      }
    }else{                                              //se chegar ao fim da palavra ele retorna 1
      if(!isalpha(Palavra1Buscada[i])){
        return(1);
      }else{
        return(0);
      }
    }
  }
  return(1);
}

ArvAVL AAVLConsultaPalavra(ArvAVL AArvore, char *Palavra){
  if(AArvore){
    if(AAVLComparaPalavra(Palavra, AArvore->Palavra)){
      return(AArvore);
    }else{
      if(AAVLPalavra1MenorIgualPalavra2(Palavra, AArvore->Palavra)){
        if(AArvore->Esq){
          return(AAVLConsultaPalavra(AArvore->Esq, Palavra));
        }else{
          return(NULL);
        }
      }else{
        if(AArvore->Dir){
          return(AAVLConsultaPalavra(AArvore->Dir, Palavra));
        }else{
          return(NULL);
        }
      }
    }
  }else{
    return(NULL);
  }
}

void AAVLBuscaPalavras(ArvAVL AArvore, char Arq[]){
  FILE *arquivotexto;                                                           //procedimento central de busca de palavras
  char frase[100000];
  int PInicio, i, linha = 0;
  Nodo NodoPalavra;
  Linha NodoLinha;

  arquivotexto = fopen(Arq, "r");
  if(arquivotexto){
    while(fgets(frase, sizeof(frase), arquivotexto)){                           //leio cada frase no arquivo
      AAVLRetiraMaiusculas(frase, frase);
      PInicio = 0;                                                              //variavel de controle de inicio de uma palavra
      linha++;                                                                  //variavel de controle de qual linha está sendo lida no arquivo para armazenar o indice
      for(i=0; i < strlen(frase); i++){                                         //loop em toda a frase lida
        if(!isalpha(frase[i])){                                                 //enquanto o caracter lido for considerado uma letra ele continua lendo, quando encontrar algo diferente ele considera como fim de uma palavra
          if(AAVLVerificaPalavraValida(&frase[PInicio])){                       //verifica se o tamanho da palavra é valido
            NodoPalavra = AAVLConsultaPalavra(AArvore, &frase[PInicio]);        //consulta a palavra na hash
            // printf("Buscando Palavra %s\n",NodoPalavra->Palavra);

            if(NodoPalavra){                                                    //se encontrar contabiliza linhas, caso contrario nao faz nada
              // printf("Palavra %s encontrada\n",NodoPalavra->Palavra);
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

void AAVLLiberaArvore(ArvAVL AArvore){
  Linha aux2;

  if(AArvore){
    AAVLLiberaArvore(AArvore->Esq);
    AAVLLiberaArvore(AArvore->Dir);
    aux2 = AArvore->IndiceI;
    while(AArvore->IndiceI){
      AArvore->IndiceI = aux2->Prox;
      free(aux2);
      aux2 = AArvore->IndiceI;
    }
    free(AArvore);
    AArvore=NULL;
  }
}
