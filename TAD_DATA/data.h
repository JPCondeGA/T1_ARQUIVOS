#ifndef DATA_H
    #define DATA_H

    #include <stdbool.h>
    #include <stdio.h> 
    #include <stdlib.h>
    #include <string.h>

    typedef unsigned int uint;
    typedef unsigned long long ull;
    typedef char int8;

    /* Para todas as funções consideramos que o registro de cabeçalho do arquivo binário siga exatamente as especifiações do Trabalho,
    isto é, possua os mesmos campos na mesma ordem, e que seja o primeiro registro do arquivo. */
    /* Além disso, todas as funções de leitura ou escrita em um arquivo conservará o valor do cursor do arquivo no fim de sua execução. */
    /* Por fim, é importante ressaltar que estamos trabalhando somente com arquviso binários - sem padding entre os valores. */

    /*=============DEFINIÇÕES============*/

    #define REMOVIDO 0 
    #define PROXIMO 1 
    #define COD_ESTACAO 2
    #define COD_LINHA 3
    #define COD_PROX_ESTACAO 4
    #define DIST_PROX_ESTACAO 5
    #define COD_LINHA_INTEGRA 6
    #define COD_EST_INTEGRA 7
    #define NOME_ESTACAO 8
    #define NOME_LINHA 9

    typedef struct data_ DATA;

    /*=============ALOCAÇÃO E DESALOCAÇÃO============*/

    /* Aloca memória para a estrutura que representa um registro de dados.
    - Retorna o ponteiro para o espaço alocado, ou NULL, no caso de falha.
    
    Os campos são inicializados como se fossem nulos e o registro é considerado não removido, inicialmente. */
    DATA *data_criar();

    /* Desaloca o espaço da estrutura que representa um registro de dados.
    - Recebe um ponteiro de ponteiro para esse tipo de estrutura.
    - Retorna true, se a desalocações ocorreu corretamente; false, caso contrário.

    Seta NULL pata o ponteiro da estrutura. */
    bool data_apagar(DATA **d);

    /*================LEITURA===============*/

    /* Carrega todos os campos de um registro de dados de um arquivo para a estrutura.
    - Recebe um ponteiro para a estrutura que representa um registro de dados, o RRN do registro de origem e um ponteiro para o arquivo em que está o registro.
    - Retorna true, se a leitura ocorreu corretamente; false, caso contrário.

    O arquivo deve estar aberto em modo que permite leitura. */
    bool data_carregar(DATA *d, uint RRN, FILE *f);

    /* Carrega um único campo de um registro de dados de um arquivo para a estrutura.
    - Recebe um ponteiro para a estrutura que representa um registro de dados, o RRN do registro de origem, um seletor do campo e um ponteiro para o arquivo em que está o registro.
    - Retorna true, se a leitura ocorreu corretamente; false, caso contrário.

    O arquivo deve estar aberto em modo que permite leitura. */
    bool data_carregar_campo(DATA *d, uint RRN, int8 op, FILE *f);

    /*================GRAVAÇÃO===============*/

    /* Salva todos os campos da estrutura em um registro de dados de um arquivo.
    - Recebe um ponteiro para a estrutura que representa um registro de dados, o RRN do registro de destino e um ponteiro para o arquivo. 
    - Retorna true, se a leitura ocorreu corretamente; false, caso contrário.

    O arquivo deve estar aberto em modo que permite gravação. */
    bool data_salvar(DATA *d, uint RRN, FILE *f);

    /* Salva um único campo da estrutura em um registro de dados de um arquivo.
    - Recebe um ponteiro para a estrutura que representa um registro de dados, o RRN do registro de destino, um seletor do campo e um ponteiro para o arquivo em que está o registro. 
    - Retorna true, se a escrita ocorreu corretamente; false, caso contrário.
    
    O arquivo deve estar aberto em modo que permite gravação. */
    bool data_salvar_campo(DATA *d, uint RRN, int8 op, FILE *f);


    /*===============GETTERS===============*/

    /* Recupera o valor do campo removido da estrutura.
    - Recebe um ponteiro para a estrutura que representa um registro de dados.
    - Retorna o valor do campo; retorna-se por padrão'1', em caso de falha. */
    char data_get_removido(DATA *d);
    
    /* Recupera o valor do campo próximo da estrutura.
    - Recebe um ponteiro para a estrutura que representa um registro de dados.
    - Retorna o valor do campo; retorna-se por padrão -1, em caso de falha. */
    int data_get_proximo(DATA *d);
    
    /* Recupera o valor do campo código da estação da estrutura.
    - Recebe um ponteiro para a estrutura que representa um registro de dados.
    - Retorna o valor do campo; retorna-se por padrão -1, em caso de falha. */
    int data_get_cod_estacao(DATA *d);
    
    /* Recupera o valor do campo código da linha da estrutura.
    - Recebe um ponteiro para a estrutura que representa um registro de dados.
    - Retorna o valor do campo; retorna-se por padrão -1, em caso de falha. */
    int data_get_cod_linha(DATA *d);
    
    /* Recupera o valor do campo código da próxima estação da estrutura.
    - Recebe um ponteiro para a estrutura que representa um registro de dados.
    - Retorna o valor do campo; retorna-se por padrão -1, em caso de falha. */
    int data_get_cod_prox_estacao(DATA *d);
    
    /* Recupera o valor do campo distância da próxima estação da estrutura.
    - Recebe um ponteiro para a estrutura que representa um registro de dados.
    - Retorna o valor do campo; retorna-se por padrão -1, em caso de falha. */
    int data_get_dist_prox_estacao(DATA *d);

    /* Recupera o valor do campo código da linha de integração da estrutura.
    - Recebe um ponteiro para a estrutura que representa um registro de dados.
    - Retorna o valor do campo; retorna-se por padrão -1, em caso de falha.*/
    int data_get_cod_linha_integra(DATA *d);

    /* Recupera o valor do campo código da estação de integração da estrutura.
    - Recebe um ponteiro para a estrutura que representa um registro de dados.
    - Retorna o valor do campo; retorna-se por padrão -1, em caso de falha. */
    int data_get_cod_est_integra(DATA *d);
    
    /* Recupera o valor do campo tamanho do nome da estação da estrutura.
    - Recebe um ponteiro para a estrutura que representa um registro de dados.
    - Retorna o valor do campo; retorna-se por padrão 0, em caso de falha. */
    uint data_get_tam_nome_estacao(DATA *d);
    
    /* Recupera o valor do campo nome da estação da estrutura.
    - Recebe um ponteiro para a estrutura que representa um registro de dados.
    - Retorna o valor do campo; retorna-se por padrão NULL, em caso de falha. 

    A string retornada possui o terminador '\0' e é uma string copiada da que a estrutura armazena. */
    char* data_get_nome_estacao(DATA *d);
    
    /* Recupera o valor do campo tamanho do nome da linha da estrutura.
    - Recebe um ponteiro para a estrutura que representa um registro de dados.
    - Retorna o valor do campo; retorna-se por padrão 0, em caso de falha. */
    uint data_get_tam_nome_linha(DATA *d);
    
    /* Recupera o valor do campo nome da linha da estrutura.
    - Recebe um ponteiro para a estrutura que representa um registro de dados.
    - Retorna o valor do campo; retorna-se por padrão NULL, em caso de falha. 

    A string retornada possui o terminador '\0' e é uma string copiada da que a estrutura armazena. */
    char* data_get_nome_linha(DATA *d);

    /*===============SETTERS===============*/
    
    /* Atribui para o campo removido da estrutura um valor especificado.
    - Recebe um ponteiro para a estrutura que representa um registro de dados e o valor que será atribuído ao campo.
    - Retorna true, se a atribuição ocorreu corretamente; false, em caso de falha (valor ou ponteiro inválido). */
    bool data_set_removido(DATA *d, char removido);

    /* Atribui para o campo próximo da estrutura um valor especificado.
    - Recebe um ponteiro para a estrutura que representa um registro de dados e o valor que será atribuído ao campo.
    - Retorna true, se a atribuição ocorreu corretamente; false, em caso de falha (valor ou ponteiro inválido). */
    bool data_set_proximo(DATA *d, int prox);
    
    /* Atribui para o campo código da estação da estrutura um valor especificado.
    - Recebe um ponteiro para a estrutura que representa um registro de dados e o valor que será atribuído ao campo.
    - Retorna true, se a atribuição ocorreu corretamente; false, em caso de falha (valor ou ponteiro inválido). */
    bool data_set_cod_estacao(DATA *d, int cod_estacao);
    
    /* Atribui para o campo código da linha da estrutura um valor especificado.
    - Recebe um ponteiro para a estrutura que representa um registro de dados e o valor que será atribuído ao campo.
    - Retorna true, se a atribuição ocorreu corretamente; false, em caso de falha (valor ou ponteiro inválido). */
    bool data_set_cod_linha(DATA *d, int cod_linha);

    /* Atribui para o campo código da próxima estação da estrutura um valor especificado.
    - Recebe um ponteiro para a estrutura que representa um registro de dados e o valor que será atribuído ao campo.
    - Retorna true, se a atribuição ocorreu corretamente; false, em caso de falha (valor ou ponteiro inválido). */    
    bool data_set_cod_prox_estacao(DATA *d, int cod_prox_estacao);

    /* Atribui para o campo distância da próxima estação da estrutura um valor especificado.
    - Recebe um ponteiro para a estrutura que representa um registro de dados e o valor que será atribuído ao campo.
    - Retorna true, se a atribuição ocorreu corretamente; false, em caso de falha (valor ou ponteiro inválido). */
    bool data_set_dist_prox_estacao(DATA *d, int dist_prox_estacao);
    
    /* Atribui para o campo código da linha de integração da estrutura um valor especificado.
    - Recebe um ponteiro para a estrutura que representa um registro de dados e o valor que será atribuído ao campo.
    - Retorna true, se a atribuição ocorreu corretamente; false, em caso de falha (valor ou ponteiro inválido). */
    bool data_set_cod_linha_integra(DATA *d, int cod_linha_integra);

    /* Atribui para o campo código da estação de integração da estrutura um valor especificado.
    - Recebe um ponteiro para a estrutura que representa um registro de dados e o valor que será atribuído ao campo.
    - Retorna true, se a atribuição ocorreu corretamente; false, em caso de falha (valor ou ponteiro inválido). */
    bool data_set_cod_est_integra(DATA *d, int cod_est_integra);

    /* Atribui para o campo nome da estação da estrutura um valor especificado.
    - Recebe um ponteiro para a estrutura que representa um registro de dados e o valor que será atribuído ao campo.
    - Retorna true, se a atribuição ocorreu corretamente; false, em caso de falha (valor ou ponteiro inválido). 

    A string passada de deve ter o terminador '\0'. Em caso de string NULL ou "", será entendido que esse campo tem valor nulo. */
    bool data_set_nome_estacao(DATA *d, char *nome_estacao);
    
    /* Atribui para o campo nome da linha da estrutura um valor especificado.
    - Recebe um ponteiro para a estrutura que representa um registro de dados e o valor que será atribuído ao campo.
    - Retorna true, se a atribuição ocorreu corretamente; false, em caso de falha (valor ou ponteiro inválido). 

    A string passada de deve ter o terminador '\0'. Em caso de string NULL ou "", será entendido que esse campo terá valor nulo. */
    bool data_set_nome_linha(DATA *d, char *nome_linha);
    
#endif