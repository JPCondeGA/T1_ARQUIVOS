#ifndef HEADER_H
    #define HEADER_H

    #include <stdbool.h>
    #include <stdio.h> 
    #include <stdlib.h>

    typedef char int8; // Entende-se como um inteiro de 8 bits

    /* Para todas as funções consideramos que o registro de cabeçalho do arquivo binário siga exatamente as especifiações do trabalho, isto é, possua os mesmos campos na mesma ordem, e que seja o primeiro registro do arquivo (byte-offset = 0). */
    /* Para funções com parâmetro op, deve-se seguir as definições expressas abaixo*/
    /* Por fim, é importante ressaltar que estamos trabalhando somente com arquviso binários - sem padding entre os valores - e, nas funções de leitura e  gravação, supomos que o modo de abertura do arquivo é o correto. */


    /*=============DEFINIÇÕES============*/

    #define STATUS 0 
    #define TOPO 1
    #define PROXRRN 2
    #define NMR_ESTACOES 3
    #define NMR_PARES_ESTACAO 4    

    #define INCONSISTENTE '0'
    #define CONSISTENTE '1'

    typedef struct header_ HEADER;

    
    /*=============ALOCAÇÃO E DESALOCAÇÃO============*/

    /* Aloca memória para a estrutura que representa o registro de cabeçalho.
    - Retorna o ponteiro para o espaço alocado, ou NULL, no caso de falha.
    
    Os campos são inicializados com valores padrão:
        - status = '1'
        - topo = -1
        - proxRRN = 0
        - nmr_estacoes = 0
        - nmr_pares_estacoes = 0 */
    HEADER *header_criar(); 

    /* Desaloca o espaço da estrutura que representa o registro de cabeçalho.
    - Recebe um ponteiro de ponteiro para esse tipo de estrutura.
    - Retorna true, se a desalocação ocorreu corretamente; false, caso contrário. 
    
    Seta NULL para o ponteiro da estrutura. */
    bool header_apagar(HEADER **h); 

    /*================LEITURA===============*/

    /* Carrega todos os campos do registro de cabeçalho de um arquivo para a estrutura.
    - Recebe um ponteiro para a estrutura que representa o registro de cabeçalho e um ponteiro para o arquivo em que está o registro.
    - Retorna true, se a leitura ocorreu corretamente; false, caso contrário. 
    
    O arquivo deve estar aberto em modo que permite leitura. */
    bool header_carregar(HEADER *h, FILE *f); 
    
    /* Função que carrega um único campo do registro de cabeçalho de um aquivo para a estrutura.
    - Recebe um ponteiro para a estrutura que representa o arquivo de cabeçalho, um seletor do campo e um ponteiro para o arquivo em que está o registro.
    - Retorna true, se a leitura ocorreu corretamente; false, caso contrário.

    O arquivo deve estar aberto em modo que permite leitura. */
    bool header_carregar_campo(HEADER *h, int8 op, FILE *f);

    /* Salva todos os campos da estrutura no registro de cabeçalho de um arquivo.
    - Recebe um ponteiro para a estrutura que representa o registro de cabeçalho e um ponteiro para o arquivo em que está o registro.
    - Retorna true, se a escrita ocorreu corretamente; false, caso contrário.

    O arquivo deve estar aberto em modo que permite gravação. */
    bool header_salvar(HEADER *h, FILE *f);

    /* Salva um único campo da estrutura no registro de cabeçalho de um arquivo.
    - Recebe um ponteiro para a estrutura que representa o registro de cabeçalho, um seletor do campo e um ponteiro para o arquivo em que está o registro.    
    - Retorna true, se a escrita ocorreu corretamente; false, caso contrário.

    O arquivo deve estar aberto em modo que permite gravação. */
    bool header_salvar_campo(HEADER *h, int8 op, FILE *f);


    /*===============GETTERS===============*/

    /* Recupera o valor do campo status da estrutura.
    - Recebe um ponteiro para a a estrutura que representa o registro de cabeçalho.
    - Retorna o valor do campo; retorna-se por padrão '0', em caso de falha. */
    char header_get_status(HEADER *h);

    /* Recupera o valor do campo topo da estrutura.
    - Recebe um ponteiro para a a estrutura que representa o arquivo de cabeçalho.
    - Retorna o valor do campo; retorna-se por padrão -1, em caso de falha. */
    int header_get_topo(HEADER *h);

    /* Recupera o valor do campo proxRRN da estrutura.
    - Recebe um ponteiro para a a estrutura que representa o arquivo de cabeçalho.
    - Retorna o valor do campo; retorna-se por padrão -1, em caso de falha. */
    int header_get_proxRRN(HEADER *h);

    /* Recupera o valor do campo nmrEstacoes da estrutura.
    - Recebe um ponteiro para a a estrutura que representa o arquivo de cabeçalho.
    - Retorna o valor do campo; retorna-se por padrão -1, em caso de falha. */
    int header_get_nmr_estacoes(HEADER *h);

    /* Recupera o valor do campo número de pares da estrutura.
    - Recebe um ponteiro para a a estrutura que representa o arquivo de cabeçalho.
    - Retorna o valor do campo; retorna-se por padrão -1, em caso de falha. */
    int header_get_nmr_pares_estacao(HEADER *h);


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
    bool header_set_nmr_pares_estacao(HEADER *h, int nmr_pares);

#endif