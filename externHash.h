#ifndef _externHash_XXX
#define _externHash_XXX

typedef struct tabela *TabelaExtern;

//Declaracoes funções publicas
TabelaExtern ELeChaves(char Arq[], int Op, int* vetor);
int  ETamanhoHash(void);
void EBuscaPalavras(TabelaExtern EHash, char Arq[], int Op, int* vetor);
void EResultadoBusca(TabelaExtern EHash, FILE *arquivo);
void ELiberaHash(TabelaExtern EHash);
int  EColisoes(TabelaExtern EHash);
int* EPreencheVetorPesos(void);

#endif
