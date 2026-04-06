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

    /*================WHERE e DELETE================*/

    /* Lê os valores de um filtro conforme as especificações.
    - Recebe um ponteiro para uma estrutura de flags que indicará os campos que possuem valores correspondentes no filtro, um ponteiro para a estrutura que representa um registro de dados que será usada para comparar com os registros do arquivo e quantidade de pares CAMPO valor.
    - Retorna se executou corretamente (true, se sim; false, se não). */
    bool func_where_input(FLAG_FIELD *flags, DATA *filtro, int m);

    /* Compara dois registros em campos determinados.
    - Recebe uma estrutura de flags que indica quais campos a serem comparados, um ponteiro para uma estrutura que representa um registro de dados e um ponteiro para outra estrutura desse tipo que terá os valores desejados. 
    - Retorna se as estruturas são iguais nos campos determinados (true, se sim; false, se não). */
    bool func_where_compare(FLAG_FIELD flags, DATA *d, DATA *filtro);

    /*================UPDATE================*/
    
    /* Copia todos os valores de uma estrutura de registro de dados para a outra, conforme o filtro passado por "flag_carimbo".
    - Recebe um ponteiro para uma estrutura de flags que indicará os campos que devem ser alterados e dois ponteiros para estruturas que representam um registro de dados. "Destino" é a estrutura a ser alterada e "Fonte" a estrutura que contém os dados que serão copiados para "Destino".
    - Retorna se executou corretamente (true, se sim; false, se não). */
    bool func_copy_data(FLAG_FIELD *flag_carimbo, DATA *destino, DATA *fonte);
    
    /* Confere se o campo que representa "nomeEstacao" foi marcado para alteração.
    - Recebe um ponteiro para uma estrutura de flags que indica os campos marcados para alteração.
    - Retorna "true" se "nomeEstacao" está marcado para alteração, "false" se não.*/
    bool func_checa_nome_alterado(FLAG_FIELD *flag_carimbo);
    
    /* Confere se o campo que representa "codEstacao" e/ou o campo que representa "codProxEstacao" foi/foram marcado(s) para alteração.
    - Recebe um ponteiro para uma estrutura de flags que indica os campos marcados para alteração.
    - Retorna "true" se "codEstacao" e/ou "codProxEstacao" estão marcados para alteração, "false" se não.*/
    bool func_checa_par_alterado(FLAG_FIELD *flag_carimbo);


    /*================FLAGS================*/

    /* Atribui true para uma flag de um campo específico da estrutura.
    - Recebe um ponteiro para uma estrutura de flags e uma string com nome de algum campo.
    - Retorna se a execução ocorreu corretamente (true, se sim; false, se não). */
    bool func_attribute_flag_field(FLAG_FIELD *flags, char *campo);

    /* Inicializa todos campos da estrutura como false.
    - Recebe um ponteiro para uma estrutura de flags.
    - Retorna se a execução ocorreu corretamente (true, se sim; false, se não). */
    bool func_init_flag_field(FLAG_FIELD *flags);

    /*================AUXILIAR================*/


    /* Inicializa uma estrutura de registro de cabeçalho e uma estrutura de registro de dados.
    - Recebe ponteiro de ponteiro para os dois tipos
    - Retorna "true" se ambos inicializarem corretamente, "false" caso contrário.
    - (*h) se torna ponteiro para registro de cabeçalho e (*d) para registro de dados.*/
    bool func_inicializar(HEADER** h, DATA** d);
    
    /* Inicializa duas estruturas de árvore, uma para nomes de estações e uma para pares de códigos. Depois, preenche as duas com os dados do registro.
    - Recebe ponteiro de ponteiro para os dois tipos e um ponteiro para registro de cabeçalho, dados e arquivo.
    - Retorna "true" se ambas inicializarem corretamente, "false" caso contrário.
    - (*nomes) se torna ponteiro para árvore de nomes e (*pares) para árvore de pares.
    - Ambas as árvores estarão preenchidas no final da função.
      ATENÇÃO: Essa função chama header_load_all() para evitar erros caso o cabeçalho passado não tenha sido inicializado.
      Portanto, chame-a apenas antes de realizar alterações na estrutura do cabeçalho, ou após salvar essas alterações no arquivo. Caso contrário, divergências podem ocorrer.*/
    bool func_inicializar_arvores(ARVORE** nomes, ARVORE** pares, HEADER* h, DATA* d, FILE* f);

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
