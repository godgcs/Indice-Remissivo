#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "listaEncadeada.h"

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
struct lista{
  int Tam;
  struct nodo *ListaI;
};

typedef struct nodo *Nodo;
typedef struct linhas *Linha;

Lista LEcriaLista();
Nodo  LECriaNoLista(char *Palavra);
Nodo  LEConsultaPalavra(Lista PLista, char *Palavra);
int   LEVerificaPalavraValida(char *Palavra);
int   LEComparaPalavra(char *Palavra1, char *Palavra2);
int   LEPalavra1MenorIgualPalavra2(char *Palavra1, char *Palavra2);
void  LERetiraMaiusculas(char *Palavra, char *Destino);
void  LECopiaPalavra(char *Palavra, char *Destino);
void  LEInserePalavra(Lista PLista, char *Palavra);
void  LEInsereNoLista(Lista PLista, Nodo No);

Lista LEcriaLista(){
  Lista PLista = (Lista) malloc(sizeof(struct lista));
  PLista->ListaI = NULL;
  PLista->Tam = 0;

  return(PLista);
}

int LEComparaPalavra(char *Palavra1Buscada, char *Palavra2){
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

Nodo LEConsultaPalavra(Lista PLista, char *Palavra){
  Nodo NodoLista = PLista->ListaI;

  while(NodoLista){
    if(LEComparaPalavra(Palavra, NodoLista->Palavra)){
      return(NodoLista);
    }else{
      NodoLista = NodoLista->Prox;
    }
  }
  return(NodoLista);
}

int LEVerificaPalavraValida(char *Palavra){
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

int LEPalavra1MenorIgualPalavra2(char *Palavra1, char *Palavra2){
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
          if(LEPalavra1MenorIgualPalavra2(&Palavra1[1], &Palavra2[1])){    //assim é feita uma pilha que no final retorna o valor desejado
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

void LEInsereNoLista(Lista PLista, Nodo No){
  Nodo NodoLista1, NodoLista2;

  if(!PLista->ListaI){
    PLista->ListaI = No;
    PLista->Tam++;
  }else{
    NodoLista1 = PLista->ListaI;
    if(LEPalavra1MenorIgualPalavra2(No->Palavra, NodoLista1->Palavra)){
      No->Prox = NodoLista1;
      PLista->ListaI = No;
      PLista->Tam++;
      return;
    }else{
      while(NodoLista1 && LEPalavra1MenorIgualPalavra2(NodoLista1->Palavra, No->Palavra)){
        NodoLista2 = NodoLista1;
        NodoLista1 = NodoLista1->Prox;
      }
      NodoLista2->Prox = No;
      No->Prox = NodoLista1;
      PLista->Tam++;
      return;
    }
  }
}

void LECopiaPalavra(char *Palavra, char *Destino){
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

Nodo LECriaNoLista(char *Palavra){
  Nodo No = (Nodo) malloc(sizeof(struct nodo));

  No->IndiceI = NULL;
  No->IndiceF = NULL;
  No->Prox = NULL;
  LECopiaPalavra(Palavra, No->Palavra);

  return(No);
}

void LEInserePalavra(Lista PLista, char *Palavra){
  Nodo nodo = LECriaNoLista(Palavra);

  LEInsereNoLista(PLista, nodo);
}

Lista LELeChaves(char Arq[]){
  Lista PLista = LEcriaLista();   //função central de leitura de palavras chaves
  FILE *arquivo;
  char frase[100];
  int PInicio, i;
  Nodo NodoLista;

  arquivo = fopen(Arq, "r");
  if(arquivo){
    while(fgets(frase, sizeof(frase), arquivo)){                      //leio a frase do arquivo
      LERetiraMaiusculas(frase,frase);
      PInicio = 0;                                                    //variavel que controla o inicio de uma palavra
      for(i=0; i < strlen(frase); i++){                               //faço um for em todos os caracteres da frase
        if(!isalpha(frase[i])){                                       //enquanto não chegar a o fim de uma palavra ele continua lendo
          if(LEVerificaPalavraValida(&frase[PInicio])){               //verifica se a palavra é valida
            NodoLista = LEConsultaPalavra(PLista, &frase[PInicio]);
            if(!NodoLista){                                          //consulta se a palavra encontrada ainda não foi inserida na hash para que não existam repetições
              // printf("Inserindo : %s",&frase[PInicio]);
              LEInserePalavra(PLista, &frase[PInicio]);               //insere a palavra
            }else{
              // printf("Não Inserindo : %s",&frase[PInicio]);
            }
          }
          PInicio = i + 1;                                            //apos o termino de uma palavra o proximo caracter podeser o inicio de outra
        }
      }
    }
    fclose(arquivo);

    return(PLista);
  }
  return(NULL);
}

void LEResultadoBusca(Lista PLista, FILE *arquivo){
  Nodo No = PLista->ListaI;
  Linha NodoLinha;

  while(No){
    NodoLinha = No->IndiceI;
    if(NodoLinha){
      fprintf(arquivo,"%s ",No->Palavra);
      while(NodoLinha){
        fprintf(arquivo, "%d ", NodoLinha->Linha);
        NodoLinha = NodoLinha->Prox;
      }
      fprintf(arquivo, "\n");
    }
    No = No->Prox;
  }
}

void LERetiraMaiusculas(char *Palavra, char *Destino){
  int i;

  for(i=0; i <= strlen(Palavra); i++){    //um for em todo o tamanho da frase passada
    if(isalpha(Palavra[i])!=0){           //quando encontrar um caracter que não é uma letra ele define o fim da frase e retorna a função
      Destino[i] = tolower(Palavra[i]);
    }else{
      Destino[i] = Palavra[i];
    }
  }
}

void LEBuscaPalavras(Lista PLista, char Arq[]){
  FILE *arquivotexto;                                                           //procedimento central de busca de palavras
  char frase[100000];
  int PInicio, i, linha = 0;
  Nodo NodoPalavra;
  Linha NodoLinha;

  arquivotexto = fopen(Arq, "r");
  if(arquivotexto){
    while(fgets(frase, sizeof(frase), arquivotexto)){                           //leio cada frase no arquivo
      LERetiraMaiusculas(frase, frase);
      PInicio = 0;                                                              //variavel de controle de inicio de uma palavra
      linha++;                                                                  //variavel de controle de qual linha está sendo lida no arquivo para armazenar o indice
      for(i=0; i < strlen(frase); i++){                                         //loop em toda a frase lida
        if(!isalpha(frase[i])){                                                 //enquanto o caracter lido for considerado uma letra ele continua lendo, quando encontrar algo diferente ele considera como fim de uma palavra
          if(LEVerificaPalavraValida(&frase[PInicio])){                         //verifica se o tamanho da palavra é valido
            NodoPalavra = LEConsultaPalavra(PLista, &frase[PInicio]);           //consulta a palavra na hash
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

void LELiberaLista(Lista PLista){
  Linha aux2;
  Nodo auxNodo;

  auxNodo = PLista->ListaI;
  while(PLista->ListaI){
    PLista->ListaI = auxNodo->Prox;
    aux2 = auxNodo->IndiceI;
    while(auxNodo->IndiceI){
      auxNodo->IndiceI = aux2->Prox;
      free(aux2);
      aux2 = auxNodo->IndiceI;
    }
    free(auxNodo);
    auxNodo = PLista->ListaI;
  }
  free(PLista);
}
