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

  IMDB: util.c


  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:
  - função mallocc retirada de: http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/* 
 * IMPLEMENTACAO de funcoes de uso geral 
 *
 */

/* interface para este modulo */
#include "util.h" 

#include <stdio.h>     /* fgets(), printf() */
#include <stdlib.h>    /* malloc(), NULL, EXIT_FAILURE */
#include <string.h>    /* strlen() */
#include <ctype.h>     /* tolower() */

/* tamanho maximo de um string neste programa como nome de arquivo 
   ou nome um filme */
#define TAM_STR 256

#define DEBUG


/*----------------------------------------------------------------------
 * achePalavra
 *
 * Recebe o string PAL com tamanho TPAL, e outro string TEXTO com
 * tamanho TTEX e retornar TRUE caso a PAL ocorra em TEXTO, e
 * FALSE em caso contrário. 
 * 
 * A busca nao deve levar em consideracao se as letras em PAL ou
 * TEXTO sao em caixa alta (maiusculas) ou caixa normal
 * (minusculas). Assim, a busca deve ser 'case insensitive'.
 *
 */
Bool
achePalavra(unsigned char *pal, int tPal, unsigned char *texto, int tTex)
{
    /*Implementação do algoritmo de Boyer-Moore*/
    int *alcance = preProcessamento(pal, tPal);
    int i, r, k = tPal, ult[256];

    /*Criação de vetor de deslocamento do primeiro algoritmo de Boyer-Moore*/
    for(i = 0; i < 256; i++) ult[i] = tPal + 1;
    for(i = 1; i <= tPal; i++){
        if (pal[i-1] >= 'A' && pal[i-1] <= 'Z') ult[pal[i-1]-'A'+'a'] = tPal - i;
        else ult[pal[i-1]] = tPal - i;
    }

    while(k <= tTex){
        r = 0;
        while(tPal - r - 1 >= 0 &&
              ( (pal[tPal - r - 1] >= 'a' && pal[tPal - r - 1] <= 'z' && pal[tPal - r - 1]-'a'+'A' == texto[k-r-1]) ||
                (pal[tPal - r - 1] >= 'A' && pal[tPal - r - 1] <= 'Z' && pal[tPal - r - 1]-'A'+'a' == texto[k-r-1]) ||
                 pal[tPal - r - 1] == texto[k - r - 1]) ){
            r++;
        }
        if(r == tPal ){
            free(alcance);
            return TRUE;
        }
        if (r == 0) k++;
        else if ( ult[texto[k+1]] + 1 >= alcance[tPal-r] - 1){
            if(texto[k+1] >= 'A' && texto[k+1] <= 'Z') k += ult[texto[k+1]-'A'+'a'] + 1;
            else k += ult[texto[k+1]] + 1;
        }else{
            k += alcance[tPal-r] - 1;
        }
    }
    free(alcance);
    return FALSE;
}

int* preProcessamento(unsigned char* pal, int tPal){
    int i, r, j, *alcance;
    alcance = mallocSafe((tPal)*sizeof(int));
    for(i = tPal - 1; i >= 0; i--){
        j = tPal - 2;
        r = 0;
        while(tPal - r - 1 >= i && j - r >= 0){
            if(pal[j-r] == pal[tPal - 1 - r]) r++;
            else{
                j--;
                r = 0;
            }
        }
        alcance[i] = tPal-j;
    }
    return alcance;

}

/*----------------------------------------------------------------------
 *   strCmp(const char *s1, const char *s2)
 *
 *   Comportamento identico ao da funcao strcmp da interface string.h
 *   da libC. A unica diferenca e que esta funcao desconsidera se as
 *   letras sao maiusculas ou mininusculas.
 *
 *   copiado da  gLibc
 *
 *   Ver http://fossies.org/dox/glibc-2.20/string_2strcmp_8c_source.html
 *   ou a copia nas notas de aula de MAC0121.
 *     
 *   ------------------------------------------------------------------
 *   NAME
 *      strcmp - compare two strings
 *
 *   SYNOPSIS
 *      #include <string.h>
 *
 *      int strcmp(const char *s1, const char *s2);
 *
 *   DESCRIPTION
 *      The  strcmp() function compares the two strings s1 and s2.  
 *      It returns an integer less than, equal to, or greater than
 *      zero if s1 is found, respectively, to be less than, to match, 
 *      or be greater than s2.
 *
 */

int  
strCmp(const char *s1, const char *s2)
{
    const unsigned char *p1 = (const unsigned char *) s1;
    const unsigned char *p2 = (const unsigned char *) s2;
    unsigned char c1, c2;
 
    do
    {
        c1 = tolower((unsigned char) *p1++); /* tolower foi acrescentado */ 
        c2 = tolower((unsigned char) *p2++); /* tolower foi acrescentado */
        if (c1 == '\0')
            return c1 - c2;
    }
    while (c1 == c2);
 
    return c1 - c2;
}

/* 
 * Sobre a GLIBC:
 *
 * The GNU C Library, commonly known as glibc, is the GNU Project's 
 * implementation of the C standard library. Despite its name, it now 
 * also directly supports C++ (and, indirectly, other programming languages). 
 * It was started in the early 1990s by the Free Software Foundation (FSF) 
 * for their GNU operating system.
 * 
 * Released under the GNU Lesser General Public License,[3] glibc is free software. 
 * The GNU C Library project provides the core libraries for the GNU system and 
 * GNU/Linux systems, as well as many other systems that use Linux as the kernel. 
 * These libraries provide critical APIs including ISO C11, POSIX.1-2008, BSD, 
 * OS-specific APIs and more. These APIs include such foundational facilities 
 * as open, read, write, malloc, printf, getaddrinfo, dlopen, pthread_create, 
 * crypt, login, exit and more.
 */


/*--------------------------------------------------------------
 *   leiaString
 *
 *   RECEBE como parametros um string STR e um inteiro SIZE e LE
 *   da entrada padrao no maximo SIZE-1 caracteres e ARMAZENA em
 *   STR. A leitura para depois de um ENTER e o ENTER _nao_ e
 *   incluido no string STR.  A funcao retorna o numero de
 *   caracteres no string.
 *
 *   Pre-condicao: SIZE <= TAM_STR
 *
 *   . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
 *   > man fgets
 *   char *fgets(char *s, int size, FILE *stream);
 *
 *   fgets() reads in at most one less than size characters from
 *   stream and stores them into the buffer pointed to by s.
 *   Reading stops after an EOF or a newline.  If a newline is
 *   read, it is stored into the buffer.  A terminating null byte
 *   ('\0') is stored after the last character in the buffer.
 */

int 
leiaString(char str[], int size)
{
    int len;
    char s[TAM_STR];

    /* remove qualquer newline que estava no buffer */
    scanf(" ");

    /* leitura do string: ler info sobre fgets() */
    fgets(s, TAM_STR, stdin);

    /* sobreescreve  um possivel newline do final com '\0' */
    len = strlen(s); 
    if (s[len-1] == ENTER) 
    {
        len--;
        s[len] = '\0';
    }

    if (len > size)
    {
        s[size-1] = '\0';
        len = size-1;
    }

    strncpy(str, s, size);

#ifdef DEBUG
    printf("AVISO: leiaString: string lido '%s' tem %d caracteres\n", s, len);
#endif
    return len;
}

/*---------------------------------------------------
 *   mallocSafe() 
 *
 *   http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html
 */
void * 
mallocSafe(size_t n)
{
    void * p;

    p = malloc(n);
    if (p == NULL) 
    {
        printf("ERRO na alocacao de memoria.\n\n");
        exit(EXIT_FAILURE);
    }

    return p;
}


