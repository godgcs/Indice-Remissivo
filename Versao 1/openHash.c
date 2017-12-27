#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "openHash.h"

//Estrutura de Dados
struct linhas{
  int Linha;
  struct linhas *Prox;
};
struct nodo{
  char Palavra[50];
  struct linhas *IndiceI;
  struct linhas *IndiceF;
  int Vazio;
  int Colisoes;
};
typedef struct nodo *Nodo;
typedef struct linhas *Linha;

//Cabeçalho
TabelaOpen OCriaTabela(int Tam);
Nodo  OConsultaPalavra(TabelaOpen OHash, char *Palavra, int Tam, int Op, int Peso, int* vetor);
Nodo* OCriaVetorPalavras(TabelaOpen OHash, int Tam);
void  OInserePalavra(TabelaOpen OHash, char *Palavra, int Tam, int Op, int Peso, int* vetor);
void  OTestaPalavrasChaves(TabelaOpen OHash, int tam);
void  OCopiaPalavra(char *Palavra, char *Destino);
int   OParticao(Nodo *Vetor, int inicio, int fim);
void  OOrdenacao(Nodo *Vetor, int inicio, int fim);
int   OPalavra1MaiorPalavra2(char *Palavra1, char *Palavra2);
int   OPalavra1MenorIgualPalavra2(char *Palavra1, char *Palavra2);
int   OComparaPalavra(char *Palavra1, char *Palavra2);
int   OVerificaPalavraValida(char *Palavra);
int   OConverteString(char *Palavra);
int   OFuncaoHash1(char *Palavra, int Tam, int Peso);
int   OFuncaoHash2(char *Palavra, int Tam, int Peso);
int   OFuncaoHash3(char *Palavra, int Tam, int Peso, int* vetor);

//Implementação
int OTamanhoHash(char Arq[]){
  FILE *arquivo;
  char frase[100000];
  int cont = 0;

  arquivo = fopen(Arq,"r");
  if(arquivo){                                  //leio todo o arquivo contando quantas linhas existem nele
    while(fgets(frase,sizeof(frase),arquivo)){
      if(OVerificaPalavraValida(frase)){
        cont++;
      }
    }
    fclose(arquivo);
  }
  return(cont);
}

TabelaOpen OCriaTabela(int Tam){
  TabelaOpen aux = (TabelaOpen) malloc(Tam*sizeof(struct nodo));
  int i;

  for(i=0; i<Tam ; i++){          //apos alocar a memoria para a hash predefino valores iniciais para a Estrutura
    aux[i].Vazio      = 0;
    aux[i].Colisoes   = 0;
    aux[i].IndiceI    = NULL;
    aux[i].IndiceF    = NULL;
    aux[i].Palavra[0] = 'a';aux[i].Palavra[1] = '\0';
  }
  return(aux);
}

int OVerificaPalavraValida(char *Palavra){
  int i,cont = 0;

  for(i=0; i<=strlen(Palavra); i++){        //faz um for contando a quantidade de caracteres de uma palavra
    if(isalpha(Palavra[i])){
      cont++;
    }else{
      if(cont>3){                           //se tiver tamanho > 3 retorna 1 caso contrario 0
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

TabelaOpen OLeChaves(char Arq[], int Op, int Tam, int* vetor){
  TabelaOpen OHash = OCriaTabela(Tam);                                    //função central de leitura de palavras chaves
  FILE *arquivo;
  char frase[100000];
  int PInicio, i;

  arquivo = fopen(Arq, "r");
  if(arquivo){
    while(fgets(frase, sizeof(frase), arquivo)){                          //leio a frase do arquivo
      PInicio = 0;                                                        //variavel que controla o inicio de uma palavra
      for(i=0; i < strlen(frase); i++){                                   //faço um for em todos os caracteres da frase
        if(!isalpha(frase[i])){                                           //enquanto não chegar a o fim de uma palavra ele continua lendo
          if(OVerificaPalavraValida(&frase[PInicio])){                    //verifica se a palavra é valida
            OInserePalavra(OHash, &frase[PInicio], Tam, Op, 0, vetor);    //insere a palavra
          }
          PInicio = i + 1;                                                //apos o termino de uma palavra o proximo caracter podeser o inicio de outra
        }
      }
    }
    fclose(arquivo);
    return(OHash);
  }else{
    return(NULL);
  }
}

/*int OConverteString(char *Palavra){
  int i, h = 0;
  for (i = 0; isalpha(Palavra[i]); i++){
    h = h + Palavra[i];
  }
  return(h);
}*/

int OFuncaoHash1(char *Palavra, int Tam, int Peso){
  int i, h = 0;                           //função hash 1

  for(i=0; isalpha(Palavra[i]); i++){
    h = h + Palavra[i];
  }
  h = h + Peso;                           //tratamento de colisão
  return(h%Tam);
}

int OFuncaoHash2(char *Palavra, int Tam, int Peso){
  int i, h = 0;                           //função hash 2

  for(i=0; isalpha(Palavra[i]); i++){
    h = ((h*2 + Palavra[i])+1932)%Tam;
  }
  h = h + Peso;                           //tratamento de colisão
  return(h%Tam);
}

int OFuncaoHash3(char *Palavra, int Tam, int Peso, int* vetor){ //Ziviane
  int i, h = 0;                           //função hash 3 indicada na bibiliografia de ziviane

  for(i=0; isalpha(Palavra[i]); i++){
    h = h + Palavra[i] * vetor[i];
  }
  h = h + Peso;                           //tratamento de colisão
  return(h%Tam);
}

int OVerificaColisao(TabelaOpen OHash, int posi){
  if(!OHash[posi].Vazio){   //existe uma variavel de controle "Vazio" que controla se ja foi inserido uma palavra ou não na hash
    return(0);
  }else{
    return(1);
  }
}

void OInserePalavra(TabelaOpen OHash, char *Palavra, int Tam, int Op, int Peso, int* vetor){
  int posi;

  if(Op==1){                                                  //atravez da opção passada como parametro a função define qual função hash deve ser usada
    posi = OFuncaoHash1(Palavra, Tam, Peso);
  }else{
    if(Op==2){
      posi = OFuncaoHash2(Palavra, Tam, Peso);
    }else{
      if(Op==3){
        posi = OFuncaoHash3(Palavra, Tam, Peso, vetor);
      }
    }
  }
  // printf("Posição %d ",posi);
  if(!OVerificaColisao(OHash, posi)){                         //se não existir colisão o nodo vai ser inserido na posição retornada pela hash
    OCopiaPalavra(Palavra, OHash[posi].Palavra);
    OHash[posi].Vazio = 1;
  }else{
    OHash[posi].Colisoes++;                                   //se existir colisão ele chama novamente a função de inserção porem com um +1 no peso da palavra
    OInserePalavra(OHash, Palavra, Tam, Op, Peso+1, vetor);
  }
}

int OColisoes(TabelaOpen OHash, int Tam){
  int i, cont = 0;
  for(i=0; i<Tam; i++){
    if(OHash[i].Colisoes>0){
      cont = cont + OHash[i].Colisoes;   //conta todas as colisões que aconteceram na hash
    }
  }
  return(cont);
}

void OCopiaPalavra(char *Palavra, char *Destino){
  int i;

  for(i=0; i <= strlen(Palavra); i++){          //um for em todo o tamanho da frase passada
    if(isalpha(Palavra[i])!=0){                 //quando encontrar um caracter que não é uma letra ele define o fim da frase e retorna a função
      Destino[i] = tolower(Palavra[i]);
    }else{
      Destino[i]='\0';
    }
  }
}

/*void OTestaPalavrasChaves(TabelaOpen OHash, int tam){
  int i;
  for(i=0; i<tam; i++){
    printf("%s\n",OHash[i].Palavra);
  }
}*/

int OComparaPalavra(char *Palavra1, char *Palavra2){
  int i;

  for(i=0; i< strlen(Palavra2); i++){
    if(isalpha(Palavra2[i])){
      if(tolower(Palavra1[i])!=tolower(Palavra2[i])){ //para que possa comparar as palavras pego o caracter minusculo de ambas para casa alguma seja maiuscula
        return(0);
      }
    }else{
      return(1);
    }
  }
  return(1);
}

Nodo* OCriaVetorPalavras(TabelaOpen OHash, int Tam){
  int i;
  Nodo* VetPalavras = (Nodo*) malloc(Tam * sizeof(Nodo)); //aloco um vetor de ponteiros de nodo palavras do tamanho da quantidade de palavras na hash
  //a ideia de usar ponteiros para a estutura é de reaproveitar que os dados que necessito já estão na memoria
  //e então apenas vou controlar um ponteiro para tais dados, e economizar memoria

  for(i=0; i<Tam; i++){     //faço um loop em toda a estrutura copiando o ponteiro de cada nodo palavra para o vetor
    VetPalavras[i] = &OHash[i];
  }
  return(VetPalavras);
}

int OParticao(Nodo *Vetor, int inicio, int fim){
  Nodo aux, x;
	int i, j;

	x = Vetor[fim]; //pivo
	j = inicio-1;
	for(i=inicio; i < fim; i++){
		if(OPalavra1MenorIgualPalavra2(Vetor[i]->Palavra, x->Palavra)){
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

void OOrdenacao(Nodo *Vetor, int inicio, int fim){
  if(inicio < fim){
    int q = OParticao(Vetor, inicio, fim);
    OOrdenacao(Vetor, inicio, q - 1);
    OOrdenacao(Vetor, q + 1, fim);
  }
}

void OResultadoBusca(TabelaOpen OHash, int Tam, FILE *arquivo){
  Nodo* VetPalavras = OCriaVetorPalavras(OHash, Tam);
  Linha NodoLinha;
  int i;

  OOrdenacao(VetPalavras, 0, Tam-1);//ordeno o vetor de ponteiros por ordem alfabetica pelas palavras que estão contidos nelas
  for(i=0; i<Tam; i++){             //faço um loop no vetor inserindo cada palavra no arquivo texto, apos cada palavra insiro os indices das frases em que foram encontradas
    if(VetPalavras[i]->Vazio){
      fprintf(arquivo, "%s ",VetPalavras[i]->Palavra);
      NodoLinha = VetPalavras[i]->IndiceI;
      while(NodoLinha){
        fprintf(arquivo, "%d ", NodoLinha->Linha);
        NodoLinha = NodoLinha->Prox;
      }
      fprintf(arquivo, "\n");
    }
  }
}

void OBuscaPalavras(TabelaOpen OHash, int Tam, char ArqFrases[], int Op, int* vetor){
  FILE *arquivotexto;                                                           //procedimento central de busca de palavras
  char frase[100000];
  int PInicio, i, linha = 0;
  Nodo NodoPalavra;
  Linha NodoLinha;

  arquivotexto = fopen(ArqFrases, "r");
  if(arquivotexto){
    while(fgets(frase, sizeof(frase), arquivotexto)){                           //leio cada frase no arquivo
      PInicio = 0;                                                              //variavel de controle de inicio de uma palavra
      linha++;                                                                  //variavel de controle de qual linha está sendo lida no arquivo para armazenar o indice
      for(i=0; i < strlen(frase); i++){                                         //loop em toda a frase lida
        if(!isalpha(frase[i])){                                                 //enquanto o caracter lido for considerado uma letra ele continua lendo, quando encontrar algo diferente ele considera como fim de uma palavra
          if(OVerificaPalavraValida(&frase[PInicio])){                          //verifica se o tamanho da palavra é valido
            NodoPalavra = OConsultaPalavra(OHash, &frase[PInicio], Tam, Op, 0, vetor);//consulta a palavra na hash
            if(NodoPalavra){                                                    //se encontrar contabiliza linhas, caso contrario nao faz nada
              NodoLinha = (Linha) malloc(sizeof(struct linhas));                //cria uma estrutura para armazenar a nova linha onde a palavra foi encontrada
              NodoLinha->Linha = linha;
              NodoLinha->Prox = NULL;                                           //se esta palavra já tiver sido encontrada insire no final da lista
              if(NodoPalavra->IndiceI){                                         //se esta palavra ja foi encontrada nesta mesma linha ele não armazena o numero repetido
                if(NodoPalavra->IndiceF->Linha != linha){
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

Nodo OConsultaPalavra(TabelaOpen OHash, char *Palavra, int Tam, int Op, int Peso, int* vetor){
  int orig, posi;
  Nodo NodoPalavra;

  if(Op==1){                                              //atravez da opção passada como parametro a função define qual função hash deve ser usada
    posi = OFuncaoHash1(Palavra, Tam, Peso);              //para verificar se eu ja consultei todas as palavras na hash e não entrar em um loop infinito
    orig = OFuncaoHash1(Palavra, Tam, 0);                 //armazeno o valor da palavra na hash em uma variavel orig
  }else{
    if(Op==2){
      posi = OFuncaoHash2(Palavra, Tam, Peso);
      orig = OFuncaoHash2(Palavra, Tam, 0);
    }else{
      if(Op==3){
        posi = OFuncaoHash3(Palavra, Tam, Peso, vetor);
        orig = OFuncaoHash3(Palavra, Tam, 0, vetor);
      }
    }
  }
  if(orig){
    orig--;
  }else{
    orig=Tam-1;
  }
  if(OComparaPalavra(OHash[posi].Palavra, Palavra)){    //se encontrar ambas as palavras forem iguais ele retorna o endereço do nodo
    return(&OHash[posi]);
  }else{                                                //se ainda não encontrou ele chama a função recursivamente com peso +1
    if(posi != (orig)){     //quando posi for igual a orig-1 siginifica que ja consultou todas as palavras, e então deve retornar que a palavra nao foi encontrada
      NodoPalavra = OConsultaPalavra(OHash, Palavra, Tam, Op, Peso +1, vetor);
      return(NodoPalavra);
    }else{
      return(NULL);
    }
  }
}

int OPalavra1MenorIgualPalavra2(char *Palavra1, char *Palavra2){
  int tam1 = strlen(Palavra1);
  int tam2 = strlen(Palavra2);
  int i;

  if(tam1>tam2){
      i = tam1;
      tam1 = tam2;
      tam2 = i;
  }
  if(isalpha(Palavra1[0]) && isalpha(Palavra2[0])){     //verifica se nao esta acessando posições invalidas da palava, pode acontecer se ambas as palavras forem iguais
    if(Palavra1[0] < Palavra2[0]){                      //se a primeira posição da 1 palavra for menor que a primeira da 2 palavra retorna 1
      return(1);
    }else{
      if(Palavra1[0] > Palavra2[0]){                    //se a primeira posição da 1 palavra for maior que a primeira da 2 palavra retorna 0
        return(0);
      }else{
        if(Palavra1[0] == Palavra2[0]){                 //caso ambos sejam iguais, eu chamo a função recursivamente como parametros a string começando pela proxima letra
          if(OPalavra1MenorIgualPalavra2(&Palavra1[1], &Palavra2[1])){    //assim é feita uma pilha que no final retorna o valor desejado
            return(1);
          }else{
            return(0);
          }
        }
      }
    }
  }return(1);
}

void OLiberaHash(TabelaOpen OHash, int Tam){
  int i;
  Linha aux;                          //libera toda a estrutura

  for(i=0; i<Tam; i++){
    aux = OHash[i].IndiceI;
    while(OHash[i].IndiceI){
      OHash[i].IndiceI = aux->Prox;
      free(aux);
      aux = OHash[i].IndiceI;
    }
  }
  free(OHash);
  OHash = NULL;
}

int* OPreencheVetorPesos(void){
  int* vetor = (int*) malloc(50*sizeof(int));           //o tamanho do vetor de pesos será 50 pois nenhuma palavra irá ultrapassar este tamanho
	int i;

	for(i=0; i < 50; i++){
		vetor[i] = (rand()/(double)RAND_MAX) * 10000;       //o vetor é preenchido com numeros de 0 a 10000
	}
  return(vetor);
}
