#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "externHash.h"
#include "arvoreBinaria.h"
#include "arvoreAVL.h"
#include "listaEncadeada.h"

int main(int argc, char** argv){
  if(argc == 4){
    FILE *arquivo;
    TabelaExtern EHash;
    Arv AArvore;
    ArvAVL AVLArvore;
    Lista PLista;
    double start, finish, elapsed;
  	int i=3, *vetor;

    srand(time(NULL));

    // arquivo = fopen(argv[3], "wt");
    // arquivo = fopen("saidaHash.txt", "wt");
    /*Hash de endereçamento externo --------------------------------------------*/
  	start = (double)clock()/CLOCKS_PER_SEC;
      printf("**** Hash Externa ****\n");
      printf("Lendo Chaves ...\n");
      if(i==3){vetor = EPreencheVetorPesos();}
      EHash = ELeChaves(argv[1], i, vetor);
      printf("Chaves Lidas com Sucesso.\n");
      printf("Buscando Palavras...\n");
      EBuscaPalavras(EHash, argv[2], i, vetor);
      printf("Palavras Buscadas com Sucesso.\n");
      // EResultadoBusca(EHash, arquivo);
    finish = (double)clock()/CLOCKS_PER_SEC;
    elapsed = finish - start;
    printf("A função Hash %d gastou %f segundos\n", i, elapsed);
    printf("*******************\n\n");
    ELiberaHash(EHash);

    // fclose(arquivo);
    free(vetor);

    // arquivo = fopen(argv[3], "wt");
    // arquivo = fopen("saidaArvBinaria.txt", "wt");
    /*Arvore Binaria de Busca não balanceada------------------------------------*/
    start = (double)clock()/CLOCKS_PER_SEC;
      printf("**** Arvore Binaria não balanceada ****\n");
      printf("Lendo Chaves ...\n");
      AArvore = ABLeChaves(argv[1]);
      printf("Chaves Lidas com Sucesso.\n");
      printf("Buscando Palavras...\n");
      ABBuscaPalavras(AArvore, argv[2]);
      printf("Palavras Buscadas com Sucesso.\n");
      // ABResultadoBusca(AArvore, arquivo);
    finish = (double)clock()/CLOCKS_PER_SEC;
    elapsed = finish - start;
    printf("A Arvore Binaria gastou %f segundos\n", elapsed);
    printf("*******************\n\n");
    ABLiberaArvore(AArvore);
    // fclose(arquivo);

    // arquivo = fopen(argv[3], "wt");
    // arquivo = fopen("saidaLista.txt", "wt");
    /*Lista Encadeada-----------------------------------------------------------*/
    start = (double)clock()/CLOCKS_PER_SEC;
      printf("**** Lista Encadeada ****\n");
      printf("Lendo Chaves ...\n");
      PLista = LELeChaves(argv[1]);
      printf("Chaves Lidas com Sucesso.\n");
      printf("Buscando Palavras...\n");
      LEBuscaPalavras(PLista, argv[2]);
      printf("Palavras Buscadas com Sucesso.\n");
      // LEResultadoBusca(PLista, arquivo);
    finish = (double)clock()/CLOCKS_PER_SEC;
    elapsed = finish - start;
    printf("A Lista Encadeada gastou %f segundos\n", elapsed);
    printf("*******************\n\n");
    LELiberaLista(PLista);
    // fclose(arquivo);

    arquivo = fopen(argv[3], "wt");
    // arquivo = fopen("saidaAVL.txt", "wt");
    /*Arvore AVL----------------------------------------------------------------*/
    start = (double)clock()/CLOCKS_PER_SEC;
      printf("**** Arvore AVL ****\n");
      printf("Lendo Chaves ...\n");
      AVLArvore = AAVLLeChaves(argv[1]);
      printf("Chaves Lidas com Sucesso.\n");
      printf("Buscando Palavras...\n");
      AAVLBuscaPalavras(AVLArvore, argv[2]);
      printf("Palavras Buscadas com Sucesso.\n");
      AAVLResultadoBusca(AVLArvore, arquivo);
    finish = (double)clock()/CLOCKS_PER_SEC;
    elapsed = finish - start;
    printf("A Arvore AVL gastou %f segundos\n", elapsed);
    printf("*******************\n\n");
    AAVLLiberaArvore(AVLArvore);
    fclose(arquivo);

  }else{
    printf("Parametros Invalidos\n");
    printf("Excute a aplicação com: './<executavel> <arquivoChaves> <arquivoTexto> <arquivoSaida>'\n");
    return(-1);
  }
}
