#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "openHash.h"
#include "externHash.h"

int main(int argc, char** argv){
  if(argc == 4){
    FILE *arquivo;
    arquivo = fopen(argv[3], "wt");
    TabelaExtern EHash;
    TabelaOpen OHash;
    double start, finish, elapsed;
  	int i, colisoes, Tam;
    int *vetor;

    srand(time(NULL));
    for(i=1; i<=3; i++){
  	  start = (double)clock()/CLOCKS_PER_SEC;
        printf("**** Hash Externa - %d Execução ****\n",i);
        // fprintf(arquivo, "**** Hash Externa - %d Execução ****\n",i);
        printf("Lendo Chaves ...\n");
        if(i==3){vetor = EPreencheVetorPesos();}
        EHash = ELeChaves(argv[1], i, vetor);
        printf("Chaves Lidas com Sucesso.\n");
        printf("Buscando Palavras...\n");
        EBuscaPalavras(EHash, argv[2], i, arquivo, vetor);
        printf("Palavras Buscadas com Sucesso.\n");
        // EResultadoBusca(EHash, arquivo);
        // fprintf(arquivo, "*******************\n\n");
      finish = (double)clock()/CLOCKS_PER_SEC;
    	elapsed = finish - start;
      colisoes = EColisoes(EHash);
      printf("A função Hash %d teve %d Colisões e gastou %f segundos\n", i, colisoes, elapsed);
      printf("*******************\n\n");
      ELiberaHash(EHash);
    }
    free(vetor);vetor = NULL;

    for(i=1; i<=3; i++){
      start = (double)clock()/CLOCKS_PER_SEC;
        printf("**** Open Hash - %d Execução ****\n",i);
        // fprintf(arquivo, "**** Open Hash - %d Execução ****\n",i);
        printf("Lendo Chaves ...\n");
        if(i==3){vetor = OPreencheVetorPesos();}
        Tam = OTamanhoHash(argv[1]);
        OHash = OLeChaves(argv[1], i, Tam, vetor);
        printf("Chaves Lidas com Sucesso.\n");
        printf("Buscando Palavras...\n");
        OBuscaPalavras(OHash, Tam, argv[2], i, vetor);
        printf("Palavras Buscadas com Sucesso.\n");
        if(i==3){OResultadoBusca(OHash, Tam, arquivo);}
        // fprintf(arquivo, "*******************\n\n");
      finish = (double)clock()/CLOCKS_PER_SEC;
    	elapsed = finish - start;
      colisoes = OColisoes(OHash, Tam);
      printf("A função Hash %d teve %d Colisões e gastou %f segundos\n", i, colisoes, elapsed);
      printf("*******************\n\n");
      OLiberaHash(OHash, Tam);
    }
    fclose(arquivo);
    free(vetor);vetor = NULL;
    return(0);
  }else{
    printf("Parametros Invalidos\n");
    printf("Excute a aplicação com: './<executavel> <arquivoChaves> <arquivoTexto> <arquivoSaida>'\n");
    return(-1);
  }
}
