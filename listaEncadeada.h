#ifndef _listaEncadeada_XXX
#define _listaEncadeada_XXX

typedef struct lista *Lista;

//Declaracoes funções publicas
Lista LELeChaves(char Arq[]);
void LEResultadoBusca(Lista PLista, FILE *arquivo);
void LEBuscaPalavras(Lista PLista, char Arq[]);
void LELiberaLista(Lista PLista);

#endif
