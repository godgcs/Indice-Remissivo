#ifndef _arvoreAVL_XXX
#define _arvoreAVL_XXX

typedef struct nodo *ArvAVL;

//Declaracoes funções publicas
ArvAVL AAVLLeChaves(char Arq[]);
void   AAVLResultadoBusca(ArvAVL AArvore, FILE *arquivo);
void   AAVLBuscaPalavras(ArvAVL AArvore, char Arq[]);
void   AAVLLiberaArvore(ArvAVL AArvore);

#endif
