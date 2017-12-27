#ifndef _OpenHash_XXX
#define _OpenHash_XXX

typedef struct nodo *TabelaOpen;

//Cabeçalho de Funções
TabelaOpen OLeChaves(char Arq[], int Op, int Tam, int* vetor);
void OBuscaPalavras(TabelaOpen OHash, int Tam, char ArqFrases[], int Op, int* vetor);
void OResultadoBusca(TabelaOpen OHash, int Tam, FILE *arquivo);
void OLiberaHash(TabelaOpen OHash, int Tam);
int  OColisoes(TabelaOpen OHash, int Tam);
int  OTamanhoHash(char Arq[]);
int* OPreencheVetorPesos(void);

#endif
