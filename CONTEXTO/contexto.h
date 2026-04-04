#ifndef CONT
    #define CONT

    #include "../TAD_DATA/data.h"
    #include "../TAD_HEADER/header.h"
    #include "../TAD_AVL/avl.h"
    #include <ctype.h>

    #define NMR_CAMPOS 8
    #define TAM_BUFFER 30

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

    /*================WHERE================*/

    /* Lê os valores de um filtro conforme as especificações.
    - Recebe um ponteiro para uma estrutura de flags que indicará os campos que possuem valores correspondentes no filtro, um ponteiro para a estrutura que representa um registro de dados que será usada para comparar com os registros do arquivo e quantidade de pares CAMPO valor.
    - Retorna se executou corretamente (true, se sim; false, se não). */
    bool func_where_input(FLAG_FIELD *flags, DATA *filtro, int m);

    /* Compara dois registros em campos determinados.
    - Recebe uma estrutura de flags que indica quais campos a serem comparados, um ponteiro para uma estrutura que representa um registro de dados e um ponteiro para outra estrutura desse tipo que terá os valores desejados. 
    - Retorna se as estruturas são iguais nos campos determinados (true, se sim; false, se não). */
    bool func_where_compare(FLAG_FIELD flags, DATA *d, DATA *filtro);


    /*================FLAGS================*/

    /* Atribui true para uma flag de um campo específico da estrutura.
    - Recebe um ponteiro para uma estrutura de flags e uma string com nome de algum campo.
    - Retorna se a execução ocorreu corretamente (true, se sim; false, se não). */
    bool func_atribbute_flag_field(FLAG_FIELD *flags, char *campo);

    /* Inicializa todos campos da estrutura como false.
    - Recebe um ponteiro para uma estrutura de flags.
    - Retorna se a execução ocorreu corretamente (true, se sim; false, se não). */
    bool func_init_flag_field(FLAG_FIELD *flags);

    /*================AUXILIAR================*/

    /* Atribui um valor a um campo específico da estrutura.
    - Recebe um string com o valor desejado para o campo, outra string indicando o campo e um ponteiro para estrutura que representa um registro de dados. 
    - Retorna se a execução ocorreu corretamente (true, se sim; false, se não).*/
    bool func_attribute_value(char *valor, char *campo, DATA *d);

    /* Grava um registro de dados no próximo RRN disponível.
    - Recebe ponteiro para estrutura que representa um registro de dados, um ponteiro para estrturua que representa uo registro de cabeçalho e ponteiro para o arquivo em que está ambos registros. 
    - Retorna se a execução ocorreu corretamente (true, se sim; false, se não). */
    bool func_insert_end(DATA *d, HEADER *h, FILE *bin);

    /* Faz todos os letras de uma string se tornarem minúsculas.
    - Recebe a string. */
    void func_str_to_lower(char *str);

    /*=============FORNECIDAS============*/

    void BinarioNaTela(char *arquivo);
    void ScanQuoteString(char *str);

#endif