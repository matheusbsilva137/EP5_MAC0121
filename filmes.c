/*
  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__

  AO PREENCHER ESSE CABEÇALHO COM O MEU NOME E O MEU NÚMERO USP, 
  DECLARO QUE SOU O ÚNICO AUTOR E RESPONSÁVEL POR ESSE PROGRAMA. 
  TODAS AS PARTES ORIGINAIS DESSE EXERCÍCIO PROGRAMA (EP) FORAM 
  DESENVOLVIDAS E IMPLEMENTADAS POR MIM SEGUINDO AS INSTRUÇÕES DESSE EP
  E QUE PORTANTO NÃO CONSTITUEM PLÁGIO. DECLARO TAMBÉM QUE SOU RESPONSÁVEL
  POR TODAS AS CÓPIAS DESSE PROGRAMA E QUE EU NÃO DISTRIBUI OU FACILITEI A
  SUA DISTRIBUIÇÃO. ESTOU CIENTE QUE OS CASOS DE PLÁGIO SÃO PUNIDOS COM 
  REPROVAÇÃO DIRETA NA DISCIPLINA.

  Nome: Matheus Barbosa Silva
  NUSP: 11221741

  IMDB: filmes.c


  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:
  - função mallocc retirada de: http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/


/*----------------------------------------------------------*/
/* filmes.h e a interface para as funcoes neste modulo      */
#include "filmes.h" 

/*----------------------------------------------------------*/
#include <stdlib.h>  /* NULL, free() */
#include <stdio.h>   /* printf(), scanf() */ 
#include <string.h>  /* strlen(), strncpy(), strcmp(), strtok() */

#include "util.h"    /* Bool, mallocSafe() */
#include "iofilmes.h"
#include "st.h"      /* freeST(), initST(), putFilmeST(), getFilmeST(),
                        showST(), freeST() */
#define eps 0.0001
#define LINHA  printf("------------------------------------------"      \
                      "--------------------------------------\n")

/*----------------------------------------------------------------------
 *  crieFilme
 *
 *  Recebe informacoes dobre um filme 
 *
 *      - DIST  : distribuicao de notas
 *      - VOTOS : numero de votos
 *      - NOTA  : nota do filme 
 *      - ANO   : ano de producao do filme
 *
 *  e cria uma celula do tipo Filme para armazenar essa informacao. 
 *  A funcao retorna o endereco da celula criada.
 */
Filme *
crieFilme (char dist[], int votos, float nota, char *nome, int ano)
{
    Filme *flm;
    int    len = strlen(nome) + 1; /* +1 para o '\0' */
    
    flm = mallocSafe(sizeof *flm);
    
    strncpy(flm->dist, dist, TAM_DIST+1); /* +1 para o '\0' */
    
    flm->votos = votos;
    flm->nota  = nota;
    
    flm->nome = mallocSafe(len*sizeof(char));
    strncpy(flm->nome, nome, len);
    
    flm->ano  = ano;

    flm->prox = flm->ant = NULL; /* paranoia */
    
    return flm;
}

/*----------------------------------------------------------------------
 *  crieListaFilmes
 * 
 *  Cria uma estrutura que representa lista de filmes vazia.
 *  A funcao retorna o endereco dessa estrutura.
 *
 *  Um lista de filmes e representada por uma lista encadeada 
 *  duplamente ligada com cabeca. 
 */
ListaFilmes *
crieListaFilmes()
{
    ListaFilmes* lista;
    Filme *cabeca;

    cabeca = mallocSafe(sizeof(Filme));
    cabeca->prox = cabeca;
    cabeca->ant = cabeca;

    lista = mallocSafe(sizeof(ListaFilmes));
    lista->cab = cabeca;
    lista->nFilmes = 0;

    return lista;
}

/*----------------------------------------------------------------------
 *  libereListaFilmes(lst)
 *
 *  Recebe um ponteiro lst para uma estrutura que representa uma lista 
 *  de filmes e libera toda a memoria alocada para a lista.
 *
 *  Esta funcao utiliza a funcao libereFilme().
 */

void
libereListaFilmes(ListaFilmes *lst)
{
    Filme *cabeca, *aux, *proximo;
    for(cabeca = lst->cab, aux = lst->cab->prox; aux != cabeca; aux = proximo){
        proximo = aux->prox;
        libereFilme(aux);
    }
    lst->cab->prox = cabeca;
    lst->cab->ant = cabeca;
}

/*----------------------------------------------------------------------
 *  libereFilme
 *
 *  Recebe um ponteiro FLM para uma estrutura que representa um 
 *  filme e libera a area alocada.
 *
 */
void 
libereFilme(Filme *flm)
{
    if(flm->nome != NULL) free(flm->nome);
    if (flm != NULL) free(flm);
}

/*----------------------------------------------------------------------
 *  insiraFilme
 *
 *  Recebe um ponteito LST para uma estrutura que representa
 *  uma lista de filmes e um ponteiro FLM para uma estrutura
 *  que representa uma filme.
 *
 *  A funcao insere o filme na lista.
 *  
 */
void 
insiraFilme(ListaFilmes *lst, Filme *flm)
{
    flm->prox = lst->cab->prox;
    lst->cab->prox->ant = flm;
    flm->ant = lst->cab;
    lst->cab->prox = flm;
    lst->nFilmes++;
}


/*---------------------------------------------------------------------
 *  contemFilme
 *
 *  Recebe um ponteito LST para uma estrutura que representa
 *  uma lista de filmes e um ponteiro FLM para uma estrutura
 *  que representa uma filme.
 *
 *  A funcao retorna  TRUE se o filme esta na lista e 
 *  FALSE em caso contrario. 
 *
 *  Consideramos que dois filmes f e g sao iguais se
 *
 *       - f->nome e' igual a g->nome 
 *       - f->nota e' igual a g->nota
 *       - f->ano  e' igual a g->ano 
 *
 *  Para comparar dois nomes voce pode usar alguma funcao da 
 *  bibliteca do c  como strcmp, strncmp (string,h) 
 *  ou a funcao strCmp (util.h).
 *
 */
Bool 
contemFilme(ListaFilmes *lst, Filme *flm)
{
    Filme *cabeca, *aux, *proximo;
    for(cabeca = lst->cab, aux = lst->cab->prox; aux != cabeca; aux = proximo){
        proximo = aux->prox;
        if(flm->ano == aux->ano && flm->nota < aux->nota + eps && flm->nota > aux->nota - eps && strCmp(flm->nome, aux->nome) == 0 )
            return TRUE;
    }
    return FALSE;
}

/*----------------------------------------------------------------------
 *  removaFilme
 *
 *  Remove da lista de filmes LST o filme apontado por FLM.
 *    
 *  Pre-condicao: a funcao supoe que o filme FLM esta 
 *                na lista LST.
 */
void 
removaFilme(ListaFilmes *lst, Filme *flm)
{
    flm->ant->prox = flm->prox;
    flm->prox->ant = flm->ant;
    free(flm);
    lst->nFilmes--;
}

Filme* 
procuraFilme(ListaFilmes *lst, char* trechoNome){
    Filme *cabeca, *aux;
    char res = 'n';

    for(cabeca = lst->cab, aux = lst->cab->prox; aux != cabeca && res == 'n'; aux = aux->prox){
        if(achePalavra((uchar *)trechoNome, strlen(trechoNome), (uchar *) aux->nome, strlen(aux->nome)) == TRUE ){
            mostreFilme(aux);
            printf("Esse e' o filme procurado? [s/n/x] (x para sair): ");
            scanf(" %c", &res);
            if(res == 's') return aux;
        }
    }
    
    return NULL;
}

void
mostreFilmesNota(int numFilmes, float nota, int minVotos, ListaFilmes* lst, int opcao){
    Filme *cabeca, *aux;
    int contFilmes = 0;

    cabeca = lst->cab;
    if(opcao == MOSTRAR_MAIOR) aux = lst->cab->ant;
    else if (opcao == MOSTRAR_MENOR) aux = lst->cab->prox;

    while( aux != cabeca && contFilmes < numFilmes){
        if( (opcao == MOSTRAR_MENOR && aux->nota < nota && aux->votos >= minVotos ) || 
            (opcao == MOSTRAR_MAIOR && aux->nota > nota && aux->votos >= minVotos) ){
            mostreFilme(aux);
            contFilmes++;
        }
        if(opcao == MOSTRAR_MAIOR) aux = aux->ant;
        else if (opcao == MOSTRAR_MENOR) aux = aux->prox;
    }

    LINHA;
    if(opcao == MOSTRAR_MENOR) printf("Esses sao os %d melhores filmes com nota menor que %.2f e pelo menos %d votos.\n", contFilmes, nota, minVotos);
    else if(opcao == MOSTRAR_MAIOR) printf("Esses sao os %d piores filmes com nota maior que %.2f e pelo menos %d votos.\n", contFilmes, nota, minVotos);
}

/*----------------------------------------------------------------------
 *  mergeSortFilmes
 *
 *  Recebe uma lista de filmes LST e ordena a lista utilizando o
 *  algoritmo mergeSort recursivo adaptado para listas encadeadas
 *  duplamente ligadas com cabeca.
 *
 *  A funcao recebe ainda um parametro CRITERIO tal que:
 *
 *  Se CRITERIO == NOTA, entao a lista deve ser ordenada
 *      em ordem decrescente de nota.
 *
 *  Se CRITERIO == NOME, entao a lista deve ser ordenada
 *      em ordem crescente de nome (ordem alfabetica).
 *
 *  ------------------------------------------------------------------
 *  OBSERVACAO IMPORTANTE:
 *
 *  A ordenacao deve ser feita 'in-place', ou seja o conteudo das
 *  celulas _nao deve_ ser copiado, apenas os ponteiros devem ser
 *  alterados.
 *
 *  A funcao so deve utilizar espaco extra O(1).  
 *
 *  Hmmm, ok, sem levar em consideracao o espaco O(lg n) utilizado
 *  pela pilha da recursao.  Em outras palavras, a funcao pode conter
 *  apenas declaracoes de umas poucas variaveis (um vetor v[0..n]
 *  conta como n variaveis e nao vale).
 *
 *  ------------------------------------------------------------------
 *  Para ordenar por nome, veja a funcao strCmp em util.[h|c].
 */
void 
mergeSortFilmes(ListaFilmes *lst, Criterio criterio)
{
    ListaFilmes l1, l2;
    Filme *fimL1, *fimL2;
    Filme cab1, cab2;
    int q;

    if(lst->nFilmes >= 2){
        q = (lst->nFilmes)/2;
        l1.nFilmes = q;
        l2.nFilmes = lst->nFilmes - q;
        l1.cab = &cab1;
        l2.cab = &cab2;
        for(fimL1 = lst->cab->prox; q > 1; q--, fimL1 = fimL1->prox);
        for(fimL2 = fimL1->prox; fimL2->prox != lst->cab; fimL2 = fimL2->prox);

        l2.cab->prox = fimL1->prox;
        fimL2->prox = l2.cab;
        l2.cab->ant = fimL2;

        l1.cab->prox = lst->cab->prox;
        fimL1->prox = l1.cab;
        l1.cab->ant = fimL1;

        mergeSortFilmes(&l1, criterio);
        mergeSortFilmes(&l2, criterio);
        intercale(l1, l2, lst, criterio);
    }
}

void
intercale(ListaFilmes listParc1, ListaFilmes listParc2, ListaFilmes* lst, Criterio criterio){
    Filme *proxIns = lst->cab, *proxL1 = listParc1.cab->prox, *proxL2 = listParc2.cab->prox;
    Filme *tmpPrL1, *tmpPrL2;
    
    lst->cab->prox = lst->cab->ant = lst->cab;

    while(listParc1.nFilmes > 0 && listParc2.nFilmes > 0){
        if( ( criterio == NOTA && proxL1->nota > proxL2->nota ) || 
              (criterio == NOME && strCmp(proxL1->nome, proxL2->nome) <= 0 ) ){
            tmpPrL1 = proxL1->prox;
            proxL1->ant = proxIns;
            proxL1->prox = proxIns->prox;
            proxIns->prox = proxL1;
            proxIns = proxL1;
            proxL1 = tmpPrL1;
            listParc1.nFilmes--;
        }else{
            tmpPrL2 = proxL2->prox;
            proxL2->ant = proxIns;
            proxL2->prox = proxIns->prox;
            proxIns->prox = proxL2;
            proxIns = proxL2;
            proxL2 = tmpPrL2;
            listParc2.nFilmes--;
        }

    }

    while(listParc1.nFilmes > 0){
        tmpPrL1 = proxL1->prox;
        proxL1->ant = proxIns;
        proxL1->prox = proxIns->prox;
        proxIns->prox = proxL1;
        proxIns = proxL1;
        proxL1 = tmpPrL1;
        listParc1.nFilmes--;
    }

    while(listParc2.nFilmes > 0){
        tmpPrL2 = proxL2->prox;
        proxL2->ant = proxIns;
        proxL2->prox = proxIns->prox;
        proxIns->prox = proxL2;
        proxIns = proxL2;
        proxL2 = tmpPrL2;
        listParc2.nFilmes--;
    }

    proxIns->prox = lst->cab;
    lst->cab->ant = proxIns;
}


/*----------------------------------------------------------------------
 *  quickSortFilmes [opcional]
 *
 *  Recebe uma lista de filmes LST e ordena a lista utilizando o
 *  algoritmo quickSort adaptado para listas encadeadas duplamente
 *  ligadas com cabeca.
 *
 *  A funcao recebe ainda um parametro CRITERIO tal que:
 * 
 *  Se CRITERIO == NOTA, entao a lista deve ser ordenada
 *      em ordem decrescente de nota.
 *
 *  Se CRITERIO == NOME, entao a lista deve ser ordenada
 *      em ordem crescente de nome (ordem alfabetica).
 *
 *  ------------------------------------------------------------------
 *  OBSERVACAO IMPORTANTE:
 *
 *  A ordenacao deve ser feita 'in-place', ou seja o conteudo das
 *  celulas _nao deve_ ser copiado, apenas os ponteiros devem ser
 *  alterados.
 *
 *  A funcao so deve utilizar espaco extra O(1).  
 *
 *  Hmmm, ok, sem levar em consideracao o espaco O(lg n) utilizado
 *  pela pilha da recursao.  Em outras palavras, a funcao pode conter
 *  apenas declaracoes de umas poucas variaveis (um vetor v[0..n]
 *  conta como n variaveis e nao vale).
 *
 *  ------------------------------------------------------------------
 *  Para ordenar por nome, veja a funcao strCmp em util.[h|c].
 */
void 
quickSortFilmes(ListaFilmes *lst, Criterio criterio)
{
    Filme *separadorListas, *fimL1, *fimL2;
    ListaFilmes l1, l2;
    Filme cab1, cab2;
    int q;

    if(lst->nFilmes > 1){
        separadorListas = separa(lst, criterio);

        for(fimL1 = lst->cab, q = 0; fimL1->prox != separadorListas; q++, fimL1 = fimL1->prox);
        fimL2 = lst->cab->ant;

        l1.nFilmes = q;
        l2.nFilmes = lst->nFilmes - q - 1;
        l1.cab = &cab1;
        l2.cab = &cab2;

        fimL1->prox = l1.cab;
        l1.cab->ant = fimL1;

        l1.cab->prox = lst->cab->prox;
        lst->cab->prox->ant = l1.cab;

        l2.cab->prox = separadorListas->prox;
        separadorListas->prox->ant = l2.cab;

        fimL2->prox = l2.cab;
        l2.cab->ant = fimL2;

        quickSortFilmes(&l1, criterio);
        quickSortFilmes(&l2, criterio);

        if(l1.nFilmes > 0 && l2.nFilmes > 0){
            l1.cab->ant->prox = separadorListas;
            separadorListas->ant = l1.cab->ant;

            separadorListas->prox = l2.cab->prox;
            l2.cab->prox->ant = separadorListas;

            l2.cab->ant->prox = lst->cab;
            lst->cab->ant = l2.cab->ant;

            lst->cab->prox = l1.cab->prox;
            l1.cab->prox->ant = lst->cab;
        }else if(l1.nFilmes <= 0 && l2.nFilmes > 0){
            separadorListas->prox = l2.cab->prox;
            l2.cab->prox->ant = separadorListas;

            lst->cab->prox = separadorListas;

            l2.cab->ant->prox = lst->cab;
            lst->cab->ant = l2.cab->ant;
        }else if(l1.nFilmes > 0 && l2.nFilmes <= 0){
            lst->cab->prox = l1.cab->prox;
            l1.cab->prox->ant = lst->cab;
            
            l1.cab->ant->prox = separadorListas;
            separadorListas->ant = l1.cab->ant;
            separadorListas->prox = lst->cab;

            lst->cab->ant = separadorListas;
        }
    }
}

Filme*
separa(ListaFilmes* lst, Criterio criterio){
    Filme *x = lst->cab->ant, *proxMaiIg = lst->cab, *aux = lst->cab->prox, *tmp;
    while(aux != lst->cab){
        if(( criterio == NOTA && aux->nota >= x->nota ) || 
           ( criterio == NOME && strCmp(aux->nome, x->nome) <= 0 ) ) {
                aux->ant->prox = aux->prox;
                aux->prox->ant = aux->ant;
                tmp = aux->prox;
                aux->prox = proxMaiIg->prox;
                aux->ant = proxMaiIg;
                proxMaiIg->prox->ant = aux;
                proxMaiIg->prox = aux;
                proxMaiIg = aux;
                aux = tmp;
        }else{
            aux = aux->prox;
        }
    }
    return proxMaiIg;
}
/*----------------------------------------------------------------------
 *  hashFilmes [opcional]
 *
 *  Recebe uma lista de filmes LST e distribui as palavras que
 *  ocorrem nos nomes do filmes em uma tabela de dispersao 
 *  (hash table):
 *
 *     http://www.ime.usp.br/~pf/mac0122-2002/aulas/hashing.html
 *     http://www.ime.usp.br/~pf/mac0122-2003/aulas/symb-table.html
 *
 *  Antes de inserir uma palavra na tabela de dispersao todas a
 *  letras da palavra devem ser convertidas para minusculo. Com
 *  isto faremos que a busca de filmes que possuam uma dada
 *  palavra em seu nome nao seja 'case insensitive'. Para essa
 *  tarefa a funcao tolower() pode ser utilizada.
 *
 *  Esta funcao utiliza as funcoes freeST(), initST() e putFilmeST()
 *
 *  Para obter as palavras podemos escrever uma funcao peguePalavra()
 *  inspirada na funcao pegueNome do modulo lexer.c do EP3/EP4 ou
 *  ainda utilizar a funcao strtok() da biblioteca string.h:
 *  
 *      http://linux.die.net/man/3/strtok    (man page)
 *
 */
void
hashFilmes(ListaFilmes *lst)
{
    AVISO(hashFilmes em filmes.c: Vixe ainda nao fiz essa funcao...);
}

