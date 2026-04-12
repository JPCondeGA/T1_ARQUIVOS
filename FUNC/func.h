#ifndef FUNC
    #define FUNC
    
    #include "../CONTEXTO/contexto.h"
    #include "../TAD_DATA/data.h"
    #include "../TAD_HEADER/header.h"
    #include "../TAD_AVL/avl.h"
    

    /*=============FUNCIONALIDADES============*/

    /* Realiza a funcionalidade [1] (CREATE TABLE) por meio de funções auxiliares. Inicializa valores e executa operações de entrada e saída. Acessível do arquivo main.c do projeto. */
    void func_create_interface();

    /* Realiza a funcionalidade [2] (SELECT) por meio de funções auxiliares. Inicializa valores e executa operações de entrada e saída. Acessível do arquivo main.c do projeto. */
    void func_select_interface();

    /* Realiza a funcionalidade [3] (SELECT FROM WHERE) por meio de funções auxiliares. Inicializa valores e executa operações de entrada e saída. Acessível do arquivo main.c do projeto. */
    void func_select_where_interface();    
    
    /* Realiza a funcionalidade [4] (DELETE) por meio de funções auxiliares. Inicializa valores e executa operações de entrada e saída. Acessível do arquivo main.c do projeto. */
    void func_delete_interface();
    
    /* Realiza a funcionalidade [5] (INSERT INTO) por meio de funções auxiliares. Inicializa valores e executa operações de entrada e saída. Acessível do arquivo main.c do projeto. */
    void func_insert_interface();
    
    /* Realiza a funcionalidade [6] (UPDATE) por meio de funções auxiliares. Inicializa valores e executa operações de entrada e saída. Acessível do arquivo main.c do projeto. */
    void func_update_interface();

#endif
