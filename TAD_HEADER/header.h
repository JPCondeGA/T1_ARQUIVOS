#ifndef HEADER_H
    #define HEADER_H

    #include <stdbool.h>
    #include <stdio.h> 
    #include <stdlib.h>

    typedef unsigned int uint;
    typedef unsigned long long ull;
    typedef char int8;

    /* Para todas as funções consideramos que o registro de cabeçalho do arquivo binário siga exatamente as especifiações do Trabalho,
    isto é, possua os mesmos campos na mesma ordem, e que seja o primeiro registro do arquivo. */
    /* Além disso, todas as funções de leitura ou escrita em um arquivo conservará o valor do cursor do arquivo no fim de sua execução. */
    /* Por fim, é importante ressaltar que estamos trabalhando somente com arquviso binários - sem padding entre os valores. */


    /*=============DEFINIÇÕES============*/

    #define STATUS 0 
    #define TOP 1
    #define PROXRRN 2
    #define EST 3
    #define PARES 4    

    typedef struct header_ HEADER;

    
    /*=============ALOCAÇÃO E DESALOCAÇÃO============*/

    /* Aloca memória para a estrutura que representa o registro de cabeçalho.
    - Retorna o ponteiro para o espaço alocado, ou NULL, no caso de falha.
    
    Os campos são inicializados com valores padrão. */
    HEADER *header_create(); 

    /* Desaloca o espaço da estrutura que representa o registro de cabeçalho.
    - Recebe um ponteiro de ponteiro para esse tipo de estrutura.
    - Retorna true, se a desalocação ocorreu corretamente; false, caso contrário. 
    
    Seta NULL para o ponteiro da estrutura. */
    bool header_delete(HEADER **h); 

    /*================LEITURA===============*/

    /* Carrega todos os campos do registro de cabeçalho de um arquivo para a estrutura.
    - Recebe um ponteiro para a estrutura que representa o registro de cabeçalho e um ponteiro para o arquivo em que está o registro.
    - Retorna true, se a leitura ocorreu corretamente; false, caso contrário. 
    
    O arquivo deve estar aberto em modo que permite leitura. */
    bool header_load_all(HEADER *h, FILE *f); 
    
    /* Função que carrega um único campo do registro de cabeçalho de um aquivo para a estrutura.
    - Recebe um ponteiro para a estrutura que representa o arquivo de cabeçalho, um seletor do campo e um ponteiro para o arquivo em que está o registro.
    - Retorna true, se a leitura ocorreu corretamente; false, caso contrário.

    O arquivo deve estar aberto em modo que permite leitura. */
    bool header_load_field(HEADER *h, int8 op, FILE *f);

    /* Salva todos os campos da estrutura no registro de cabeçalho de um arquivo.
    - Recebe um ponteiro para a estrutura que representa o registro de cabeçalho e um ponteiro para o arquivo em que está o registro.
    - Retorna true, se a escrita ocorreu corretamente; false, caso contrário.

    O arquivo deve estar aberto em modo que permite gravação */
    bool header_save_all(HEADER *h, FILE *f);

    /* Salva um único campo da estrutura no registro de cabeçalho de um arquivo.
    - Recebe um ponteiro para a estrutura que representa o registro de cabeçalho, um seletor do campo e um ponteiro para o arquivo em que está o registro.    
    - Retorna true, se a escrita ocorreu corretamente; false, caso contrário.

    O arquivo deve estar aberto em modo que permite gravação. */
    bool header_save_field(HEADER *h, int8 op, FILE *f);


    /*===============GETTERS===============*/

    /* Recupera o valor do campo status da estrutura.
    - Recebe um ponteiro para a a estrutura que representa o registro de cabeçalho.
    - Retorna o valor do campo; retorna-se '0', em caso de falha. */
    char header_get_status(HEADER *h);

    /* Recupera o valor do campo topo da estrutura.
    - Recebe um ponteiro para a a estrutura que representa o arquivo de cabeçalho.
    - Retorna o valor do campo; retorna-se por padrão -1, em caso de falha. */
    int header_get_topo(HEADER *h);

    /* Recupera o valor do campo próximo RRN da estrutura.
    - Recebe um ponteiro para a a estrutura que representa o arquivo de cabeçalho.
    - Retorna o valor do campo; retorna-se por padrão -1, em caso de falha. */
    int header_get_proxRRN(HEADER *h);

    /* Recupera o valor do campo número de estações da estrutura.
    - Recebe um ponteiro para a a estrutura que representa o arquivo de cabeçalho.
    - Retorna o valor do campo; retorna-se por padrão -1, em caso de falha. */
    int header_get_nmr_estacoes(HEADER *h);

    /* Recupera o valor do campo número de pares da estrutura.
    - Recebe um ponteiro para a a estrutura que representa o arquivo de cabeçalho.
    - Retorna o valor do campo; retorna-se por padrão -1, em caso de falha. */
    int header_get_nmr_pares(HEADER *h);


    /*===============SETTERS===============*/

    /* Atribui para o campo status da estrutura um valor especificado.
    - Recebe um ponteiro para a estrutura que representa o registro de cabeçalho e o valor que será atribuído ao campo.
    - Retorna true, se a atribuição ocorreu corretamente; false, em caso de falha (valor ou ponteiro inválido). */
    bool header_set_status(HEADER *h, char status);
    
    /* Atribui para o campo topo da estrutura um valor especificado.
    - Recebe um ponteiro para a estrutura que representa o registro de cabeçalho e o valor que será atribuído ao campo.
    - Retorna true, se a atribuição ocorreu corretamente; false, em caso de falha (valor ou ponteiro inválido). */
    bool header_set_topo(HEADER *h, int topo);

    /* Atribui para o campo próximo RRN da estrutura um valor especificado.
    - Recebe um ponteiro para a estrutura que representa o registro de cabeçalho e o valor que será atribuído ao campo.
    - Retorna true, se a atribuição ocorreu corretamente; false, em caso de falha (valor ou ponteiro inválido). */
    bool header_set_proxRRN(HEADER *h, int proxRNN);

    /* Atribui para o campo número de estações da estrutura um valor especificado.
    - Recebe um ponteiro para a estrutura que representa o registro de cabeçalho e o valor que será atribuído ao campo.
    - Retorna true, se a atribuição ocorreu corretamente; false, em caso de falha (valor ou ponteiro inválido). */
    bool header_set_nmr_estacoes(HEADER *h, int nmr_estacoes);

    /* Atribui para o campo número de pares da estrutura um valor especificado.
    - Recebe um ponteiro para a estrutura que representa o registro de cabeçalho e o valor que será atribuído ao campo.
    - Retorna true, se a atribuição ocorreu corretamente; false, em caso de falha (valor ou ponteiro inválido). */
    bool header_set_nmr_pares(HEADER *h, int nmr_pares);

    /*===========TESTE==========*/
    
    /* Imprime o valor atual de cada um dos campos da estrutura.
    - Recebe um ponteiro para a estrutura que representa o arquivo de cabeçalho. */
    void header_print(HEADER *h);
    /* Atribui para todos os campos da estrutura um valor novo especificado.
    - Recebe o novo valor de cada campo e um ponteiro para a estrutura que representa o arquivo de cabeçalho. */
    void header_set_all(HEADER *h, char status, int topo, int proxRRN, int nmr_estacoes, int nmr_pares);

#endif