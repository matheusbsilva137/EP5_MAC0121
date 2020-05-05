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

  IMDB: main.c 


  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:
  - função mallocc retirada de: http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/


#include <stdio.h>  /* printf(), scanf() */ 
#include <time.h>   /* CLOCKS_PER_SECOND */
#include <string.h>   /* CLOCKS_PER_SECOND */

#include "main.h"

#include "filmes.h"   /* crieListaFilmes(), crieFilme(), 
                         inseraFilme(),  mergeSortFilmes() */
#include "iofilmes.h" /* carregueListaFilmes(), graveListaFilmes(), 
                         mostreFilme(), mostreListaFilmes() */
#include "util.h"     /* mallocSafe(), leiaString() */
#include "st.h"       /* initST(), putFilmeST(), getFilmeST(), showST(),
                         freeST() */

/*------------------------------------------------------------------- 
  P R O T O T I P O S 
*/

/* A funcao leiaOpcao so e usada neste arquivo. */
static char  leiaOpcao(); 


/*------------------------------------------------------------------- 
  M A I N 
*/
int 
main(int argc, char *argv[])
{
    ListaFilmes *lst = crieListaFilmes();
    ListaPtrFilmes *listaOcor;
    Filme *res, *aux;
    char         opcao;
    char* trecho = mallocSafe(sizeof(char));
    clock_t start, end; /* usadas para medir tempo de processamento */
    double elapsed;
    float maxNota;
    int ordenadoNota = 0, numFilmes, minVotos, tabelaCriada = 0, i = 0, j;
    /* declaracao das demais variaveis do main */

    /*------------------------------------------------------------*/
    /* imprima o cabecalho */
    printf("MAC0121 2019 - EP5\n");
    printf("The Internet Movie Database (%s, %s)\n", __DATE__, __TIME__);
    printf("[GCC %s] on %s\n", __VERSION__, SYSTEM);

    /*-----------------------------------------------------*/
    /* iterara ate o usuario digitar 'x' para sair         */
    do 
    { 

        opcao = leiaOpcao();

        /* comeca a cronometrar */
        start = clock(); 

        switch (opcao) 
        {
            /*---------------------------------------------*/
        case CARREGAR_SER:
        {
            carregueListaFilmes(lst,FALSE);
            break;
        }  

        /*---------------------------------------------*/
        case CARREGAR_ER:
        {
            carregueListaFilmes(lst,TRUE);
            break;
        }  

        /*---------------------------------------------*/
        case GRAVAR:
        {
            graveListaFilmes(lst);
            break;
        }

        /*---------------------------------------------*/
        case PROCURAR:
        {
            printf("Digite parte do nome do filme a ser procurado: ");
            leiaString(trecho, 256); /*Lê uma string com tamanho máximo de 256 caracteres*/

            res = procuraFilme(lst, trecho);
            if (res == NULL) printf("\nFilme nao encontrado\n");
            break;
        }

        /*---------------------------------------------*/
        case HASH:
        {
            if(tabelaCriada) freeST();
            initST();

            for(aux = lst->cab->prox; aux != lst->cab; aux = aux->prox){
                do{
                    trecho[0] = '\0';
                    for(j = 0; aux->nome[i] != '\0' && aux->nome[i] != ' '; j++){
                        if(aux->nome[i] >= 'A' && aux->nome[i] <= 'Z') trecho[j] = aux->nome[i++] -'A'+'a';
                        else trecho[j] = aux->nome[i++];
                    }
                    trecho[j] = '\0';
                    i++;

                    if( strCmp(" ", trecho) != 0 && strCmp("", trecho) != 0 ) putFilmeST(trecho, aux);
                }while (trecho[0] != '\0');

                i = 0;
            }
            tabelaCriada = 1;
            break;
        }

        /*---------------------------------------------*/
        case PROCURAR_HASH:
        {
            printf("Digite um parte do nome do filme a ser procurado: ");
            leiaString(trecho, 256);
            for(j = 0; trecho[j] != '\0'; j++){
                if(trecho[j] >= 'A' && trecho[j] <= 'Z') trecho[j] = trecho[j] - 'A' + 'a';
            }

            listaOcor = getFilmeST(trecho);
            if(listaOcor == NULL) AVISO(procureFilmesHash: nenhum filme encontrado);
            while(listaOcor != NULL){
                mostreFilme(listaOcor->ptrFlm);
                listaOcor = listaOcor->proxPtr;
            }
            break;
        }

        /*---------------------------------------------*/
        case MOSTRAR_HASH: /* opcional */
        {
            if(tabelaCriada) showST();
            break;
        }

        /*---------------------------------------------*/
        case LIMPAR_HASH: /* opcional */
        {
            freeST();
            tabelaCriada = 0;
            break;
        }

        /*---------------------------------------------*/
        case INSERIR:
        {
            Filme       *flm = NULL;      

            char  dist[TAM_DIST+1];  
            int   votos;             
            float nota;              
            char  nome[TAM_STR+1];   
            int   ano;               

            printf("Digite o nome do filme: ");
            leiaString(nome, TAM_STR);

            printf("Digite o ano: ");
            scanf("%d", &ano);

            printf("Digite a nota: ");
            scanf("%f", &nota);

            printf("Digite o numero de votos: "); 
            scanf("%d", &votos);

            printf("Digite a distribuicao: ");
            leiaString(dist, TAM_DIST+1);
   
            flm = crieFilme(dist, votos, nota, nome, ano);
            mostreFilme(flm);
            
            /* completar essa opcao */
            insiraFilme(lst, flm);
            ordenadoNota = 0;
            break;
        }

        /*---------------------------------------------*/
        case REMOVER:
        {
            printf("Digite parte do nome do filme a ser procurado: ");
            leiaString(trecho, 256); /*Lê uma string com tamanho máximo de 256 caracteres*/

            res = procuraFilme(lst, trecho);

            if (res == NULL) printf("\nFilme nao encontrado\n");
            else removaFilme(lst, res);
            break;
        }

        /*---------------------------------------------*/
        case ORDENAR_NOTA_M:
        {
            mergeSortFilmes(lst, NOTA);
            printf("Lista de filmes ordenada por nota\n");
            ordenadoNota = 1;
            break;
        }

        /*---------------------------------------------*/
        case ORDENAR_NOME_M:
        {
            mergeSortFilmes(lst, NOME);
            printf("Lista de filmes ordenada por nome\n");
            ordenadoNota = 0;
            break;
        }

        /*---------------------------------------------*/
        case ORDENAR_NOTA_Q: /* opcional */
        {
            quickSortFilmes(lst, NOTA);
            printf("Lista de filmes ordenada por nota\n");
            ordenadoNota = 1;
            break;
        }

        /*---------------------------------------------*/
        case ORDENAR_NOME_Q: /* opcional */
        {
            quickSortFilmes(lst, NOME);
            printf("Lista de filmes ordenada por nome\n");
            ordenadoNota = 0;
            break;
        }

        /*---------------------------------------------*/
        case MOSTRAR:
        {
            mostreListaFilmes(lst);
            break;
        }

        /*---------------------------------------------*/
        case MOSTRAR_MENOR:
        {
            if (!ordenadoNota) printf("Lista de filmes nao esta ordenada por notas\n");
            else{
                printf("Qual o numero de filmes a ser mostrado: ");
                scanf(" %d", &numFilmes);
                printf("Qual a nota maxima: ");
                scanf(" %f", &maxNota);
                printf("Qual o numero minimo de votos: ");
                scanf(" %d", &minVotos);
                mostreFilmesNota(numFilmes, maxNota, minVotos, lst, MOSTRAR_MENOR);
            }
            break;
        }

        /*---------------------------------------------*/
        case MOSTRAR_MAIOR:
        {
            if (!ordenadoNota) printf("Lista de filmes nao esta ordenada por notas\n");
            else{
                printf("Qual o numero de filmes a ser mostrado: ");
                scanf(" %d", &numFilmes);
                printf("Qual a nota minima: ");
                scanf(" %f", &maxNota);
                printf("Qual o numero minimo de votos: ");
                scanf(" %d", &minVotos);
                mostreFilmesNota(numFilmes, maxNota, minVotos, lst, MOSTRAR_MAIOR);
            }
            break;
        }
      
        /*---------------------------------------------*/
        case LIMPAR:
        {
            libereListaFilmes(lst);
            printf("Lista de filmes foi limpa\n");
            break;
        }

        /*---------------------------------------------*/
        case SAIR:
        {
            libereFilme(lst->cab);
            free(lst);
            libereListaFilmes(lst);
            free(trecho);
            break;
        }

        default :
        {
            printf("main: opcao '%c' nao reconhecida\n", opcao);
            break;
        }
        
        } /* switch */
        
        /* trava o cronometro */  
        end = clock();
        
        /* calcula o tempo */
        elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("\n(%g segundos)\n", elapsed);
    } 
    while (opcao != SAIR);

    freeST(); 

    return 0;
}


/*---------------------------------------------------------------------
  leiaOpcao()
   
  Le e retorna o caractere correspondente a opcao do usuario.
*/

char 
leiaOpcao()
{
    char opcao;
    char listaOpcoes1[] = 
        "\n================================================"
        "======================\n"
        "  (c) carregar um arquivo de dados sem eliminar repeticoes\n"
        "  (C) carregar um arquivo de dados eliminando repeticoes\n"
        "  (g) gravar   a lista atual em um arquivo\n"
        "  (p) procurar a nota de um filme\n"
        "  (h) criar    a  TS com as palavras em nomes de filmes\n"
        "  (P) procurar na TS a nota de um filme\n"
        "  (M) mostrar  a  TS\n"
        "  (L) limpar   a  TS\n"
        "  (i) inserir  um filme\n"
        "  (r) remover  um filme\n";
    char listaOpcoes2[] = 
        "  (o) ordenar  a lista de filmes por nota (mergeSort)\n"
        "  (O) ordenar  a lista de filmes por nome (mergeSort)\n"
        "  (q) ordenar  a lista de filmes por nota (quickSort)\n"
        "  (Q) ordenar  a lista de filmes por nome (quickSort)\n"
        "  (m) mostrar  todos os filmes\n"
        "  (<) mostrar  N filmes com nota _menor_ que X e pelo menos V votos\n"
        "  (>) mostrar  N filmes com nota _maior_ que X e pelo menos V votos\n"
        "  (l) limpar   a lista de filmes\n"
        "  (x) sair     do programa\n\n";

    printf("%s",listaOpcoes1);
    printf("%s",listaOpcoes2);
    printf("Digite uma opcao: ");
    scanf(" %c", &opcao);

    /* main.c: warning: string length ‘679’ is greater than the length 
       ‘509’ ISO C90 compilers are required to support 
       [-Woverlength-strings] */

    return opcao;
}


