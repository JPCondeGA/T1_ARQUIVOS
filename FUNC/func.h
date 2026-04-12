#ifndef FUNC
    #define FUNC
    
    #include "../CONTEXTO/contexto.h"
    #include "../TAD_DATA/data.h"
    #include "../TAD_HEADER/header.h"
    #include "../TAD_AVL/avl.h"
    

    /*=============FUNCIONALIDADES============*/

    /* Realiza a funcionalidade [1] (CREATE TABLE) por meio de funções auxiliares. Inicializa valores e executa operações de entrada e saída. Acessível do arquivo main.c do projeto. */
    void func_create_interface();

    /**/
    void func_select_interface();

    /* Realiza a funcionalidade [3] (SELECT FROM WHERE) por meio de funções auxiliares. Inicializa valores e executa operações de entrada e saída. Acessível do arquivo main.c do projeto. */
    void func_select_where_interface();    
    
    /**/
    void func_delete_interface();
    
    /**/
    void func_insert_interface();
    
    /**/
    void func_update_interface();

#endif
