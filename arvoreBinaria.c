#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "arvoreBinaria.h"

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
};

typedef struct nodo *Nodo;
typedef struct linhas *Linha;
/*Cabeçalho*/
Arv  ABCriaNoArvore(char *Palavra);
Arv  ABConsultaPalavra(Arv AArvore, char *Palavra);
void ABCopiaPalavra(char *Palavra, char *Destino);
void ABInserePalavra(Arv AArvore, char *Palavra);
void ABRetiraMaiusculas(char *Palavra, char *Destino);
int  ABVerificaPalavraValida(char *Palavra);
int  ABComparaPalavra(char *Palavra1, char *Palavra2);
int  ABPalavra1MenorIgualPalavra2(char *Palavra1, char *Palavra2);

/*Implementações*/
void ABCopiaPalavra(char *Palavra, char *Destino){
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

Arv ABCriaNoArvore(char *Palavra){
  Arv AArvore = (Arv) malloc(sizeof(struct nodo));

  AArvore->Esq     = NULL;
  AArvore->Dir     = NULL;
  AArvore->IndiceI = NULL;
  AArvore->IndiceF = NULL;
  ABCopiaPalavra(Palavra, AArvore->Palavra);

  return(AArvore);
}

int ABPalavra1MenorIgualPalavra2(char *Palavra1, char *Palavra2){
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
          if(ABPalavra1MenorIgualPalavra2(&Palavra1[1], &Palavra2[1])){    //assim é feita uma pilha que no final retorna o valor desejado
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

void ABInsereNoArvore(Arv AArvore, Arv No){
  if(AArvore){
    if(ABPalavra1MenorIgualPalavra2(No->Palavra, AArvore->Palavra)){
      if(!AArvore->Esq){
        AArvore->Esq = No;
      }else{
        ABInsereNoArvore(AArvore->Esq, No);
      }
    }else{
      if(!AArvore->Dir){
        AArvore->Dir = No;
      }else{
        ABInsereNoArvore(AArvore->Dir, No);
      }
    }
  }else{
    AArvore = No;
  }
}

int ABVerificaPalavraValida(char *Palavra){
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

int ABComparaPalavra(char *Palavra1Buscada, char *Palavra2){
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

Arv ABConsultaPalavra(Arv AArvore, char *Palavra){
  if(AArvore){
    if(ABComparaPalavra(Palavra, AArvore->Palavra)){
      return(AArvore);
    }else{
      if(ABPalavra1MenorIgualPalavra2(Palavra, AArvore->Palavra)){
        if(AArvore->Esq){
          return(ABConsultaPalavra(AArvore->Esq, Palavra));
        }else{
          return(NULL);
        }
      }else{
        if(AArvore->Dir){
          return(ABConsultaPalavra(AArvore->Dir, Palavra));
        }else{
          return(NULL);
        }
      }
    }
  }else{
    return(NULL);
  }
}

void ABInserePalavra(Arv AArvore, char *Palavra){
  Nodo nodo = ABCriaNoArvore(Palavra);

  ABInsereNoArvore(AArvore, nodo);
}

void ABResultadoBusca(Arv AArvore, FILE *arquivo){
  if(AArvore){
    Linha NodoLinha;
    ABResultadoBusca(AArvore->Esq, arquivo);
    NodoLinha = AArvore->IndiceI;
    if(NodoLinha){
      fprintf(arquivo,"%s ",AArvore->Palavra);
      while(NodoLinha){
        fprintf(arquivo, "%d ", NodoLinha->Linha);
        NodoLinha = NodoLinha->Prox;
      }
      fprintf(arquivo, "\n");
    }
    ABResultadoBusca(AArvore->Dir, arquivo);
  }
}

Arv ABLeChaves(char Arq[]){
  Arv AArvore = NULL;   //função central de leitura de palavras chaves
  FILE *arquivo;
  char frase[100];
  int PInicio, i;
  Nodo NodoArvore = NULL;

  arquivo = fopen(Arq, "r");
  if(arquivo){
    while(fgets(frase, sizeof(frase), arquivo)){                      //leio a frase do arquivo
      PInicio = 0;                                                    //variavel que controla o inicio de uma palavra
      for(i=0; i < strlen(frase); i++){                               //faço um for em todos os caracteres da frase
        ABRetiraMaiusculas(frase,frase);
        if(!isalpha(frase[i])){                                       //enquanto não chegar a o fim de uma palavra ele continua lendo
          if(ABVerificaPalavraValida(&frase[PInicio])){               //verifica se a palavra é valida
            ABCopiaPalavra(&frase[PInicio],&frase[PInicio]);
            if(AArvore){
              NodoArvore = ABConsultaPalavra(AArvore, &frase[PInicio]);
              if(!NodoArvore){                                        //consulta se a palavra encontrada ainda não foi inserida na hash para que não existam repetições
                ABInserePalavra(AArvore, &frase[PInicio]);            //insere a palavra
              }
            }else{
              AArvore = ABCriaNoArvore(&frase[PInicio]);
            }
          }
          PInicio = i + 1;                                            //apos o termino de uma palavra o proximo caracter podeser o inicio de outra
        }
      }
    }
    fclose(arquivo);

    return(AArvore);
  }else{
    return(NULL);
  }
}

void ABRetiraMaiusculas(char *Palavra, char *Destino){
  int i;

  for(i=0; i <= strlen(Palavra); i++){    //um for em todo o tamanho da frase passada
    if(isalpha(Palavra[i])!=0){           //quando encontrar um caracter que não é uma letra ele define o fim da frase e retorna a função
      Destino[i] = tolower(Palavra[i]);
    }else{
      Destino[i] = Palavra[i];
    }
  }
}

void ABBuscaPalavras(Arv AArvore, char Arq[]){
  FILE *arquivotexto;                                                           //procedimento central de busca de palavras
  char frase[100000];
  int PInicio, i, linha = 0;
  Nodo NodoPalavra;
  Linha NodoLinha;

  arquivotexto = fopen(Arq, "r");
  if(arquivotexto){
    while(fgets(frase, sizeof(frase), arquivotexto)){                           //leio cada frase no arquivo
      ABRetiraMaiusculas(frase, frase);
      PInicio = 0;                                                              //variavel de controle de inicio de uma palavra
      linha++;                                                                  //variavel de controle de qual linha está sendo lida no arquivo para armazenar o indice
      for(i=0; i < strlen(frase); i++){                                         //loop em toda a frase lida
        if(!isalpha(frase[i])){                                                 //enquanto o caracter lido for considerado uma letra ele continua lendo, quando encontrar algo diferente ele considera como fim de uma palavra
          if(ABVerificaPalavraValida(&frase[PInicio])){                         //verifica se o tamanho da palavra é valido
            NodoPalavra = ABConsultaPalavra(AArvore, &frase[PInicio]);            //consulta a palavra na hash
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

void ABLiberaArvore(Arv AArvore){
  Linha aux2;

  if(AArvore){
    ABLiberaArvore(AArvore->Esq);
    ABLiberaArvore(AArvore->Dir);
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
