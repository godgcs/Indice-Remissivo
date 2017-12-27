#ifndef _arvoreBinaria_XXX
#define _arvoreBinaria_XXX

typedef struct nodo *Arv;

//Declaracoes funções publicas
Arv  ABLeChaves(char Arq[]);
void ABResultadoBusca(Arv AArvore, FILE *arquivo);
void ABBuscaPalavras(Arv AArvore, char Arq[]);
void ABLiberaArvore(Arv AArvore);

#endif
