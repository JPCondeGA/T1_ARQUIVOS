#ifndef CONT
    #define CONT

    #include "../TAD_DATA/data.h"
    #include "../TAD_HEADER/header.h"
    #include "../TAD_AVL/avl.h"
    #include <ctype.h>

    #define NMR_CAMPOS 8
    #define TAM_BUFFER 31


    typedef struct flag_field_ FLAG_FIELD;

    struct flag_field_{
        bool cod_estacao;
        bool cod_linha;
        bool cod_prox_estacao;
        bool dist_prox_estacao;
        bool cod_linha_integra;
        bool cod_est_integra;
        bool nome_estacao;
        bool nome_linha;
    };

    /*================WHERE e DELETE================*/

    /* Lê os valores de um filtro conforme as especificações.
    - Recebe um ponteiro para uma estrutura de flags que indicará os campos que possuem valores correspondentes no filtro, um ponteiro para a estrutura que representa um registro de dados que será usada para comparar com os registros do arquivo e quantidade de pares CAMPO valor.
    - Retorna se executou corretamente (true, se sim; false, se não). */
    bool cntx_where_input(FLAG_FIELD *flags, DATA *filtro, int m);

    /* Compara dois registros em campos determinados.
    - Recebe uma estrutura de flags que indica quais campos a serem comparados, um ponteiro para uma estrutura que representa um registro de dados e um ponteiro para outra estrutura desse tipo que terá os valores desejados. 
    - Retorna se as estruturas são iguais nos campos determinados (true, se sim; false, se não). */
    bool cntx_where_compare(FLAG_FIELD flags, DATA *d, DATA *filtro);

    /*================DATA================*/
    
    /* Copia todos os valores de uma estrutura de registro de dados para a outra, conforme o filtro passado por "flag_carimbo".
    - Recebe um ponteiro para uma estrutura de flags que indicará os campos que devem ser alterados e dois ponteiros para estruturas que representam um registro de dados. "Destino" é a estrutura a ser alterada e "Fonte" a estrutura que contém os dados que serão copiados para "Destino".
    - Retorna se executou corretamente (true, se sim; false, se não). */
    bool cntx_copiar_data(FLAG_FIELD *flag_carimbo, DATA *destino, DATA *fonte);

    /* Compara se o campo especificado é igual nas duas estruturas.
    - Recebe as duas estruturas a serem comparadas que representam um registro de dados e um seletor do campo.
    - Retorna true, se são iguais no campo; false, caso contrário. */
    bool cntx_comparar_data(DATA *d1, DATA *d2, int8 op);

    /* Imprime os campos de um registro de dados em uma linha na ordem especificada.
    - Recebe um ponteiro para a estrutura que representa um registro de dados, onde está os valores a serem impressos. */
    void cntx_imprimir_data(DATA *d);

    /*================FLAGS================*/

    /* Atribui true para uma flag de um campo específico da estrutura.
    - Recebe um ponteiro para uma estrutura de flags e uma string com nome de algum campo.
    - Retorna se a execução ocorreu corretamente (true, se sim; false, se não). */
    bool cntx_atribuir_valor_flag(FLAG_FIELD *flags, char *campo);

    /* Inicializa todos campos da estrutura como false.
    - Recebe um ponteiro para uma estrutura de flags.
    - Retorna se a execução ocorreu corretamente (true, se sim; false, se não). */
    bool cntx_init_flag(FLAG_FIELD *flags);

    /*================AUXILIAR================*/


    /* Inicializa uma estrutura de registro de cabeçalho e uma estrutura de registro de dados.
    - Recebe ponteiro de ponteiro para os dois tipos
    - Retorna "true" se ambos inicializarem corretamente, "false" caso contrário.
    - (*h) se torna ponteiro para registro de cabeçalho e (*d) para registro de dados.*/
    bool cntx_inicializar(HEADER** h, DATA** d);

    /**/
    bool cntx_checa_consistencia(HEADER *h, bool carrega, bool move, FILE *f);

    /**/
    void cntx_altera_consistencia(HEADER *h, bool valor, bool move, FILE *f);

    /* Atribui um valor a um campo específico da estrutura.
    - Recebe um string com o valor desejado para o campo, outra string indicando o campo e um ponteiro para estrutura que representa um registro de dados. 
    - Retorna se a execução ocorreu corretamente (true, se sim; false, se não).*/
    bool cntx_atribuir_valor_data(char *valor, char *campo, DATA *d);

    /* Faz todos os letras de uma string se tornarem minúsculas.
    - Recebe a string. */
    void cntx_str_min(char *str);

    /*=============FORNECIDAS============*/

    void BinarioNaTela(char *arquivo);
    void ScanQuoteString(char *str);

#endif
