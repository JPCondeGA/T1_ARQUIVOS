#ifndef FUNC
    #define FUNC
    
    #include "../CONTEXTO/contexto.h"
    #include "../TAD_DATA/data.h"
    #include "../TAD_HEADER/header.h"
    #include "../TAD_AVL/avl.h"
    

    /*=============FUNCIONALIDADES============*/

    /* Realiza a funcionalidade [1] (CREATE TABLE) por meio de funções auxiliares. Inicializa valores e executa operações de entrada e saída. Acessível do arquivo main.c do projeto. */
    void func_create_interface();

    bool func_select();

    /* Realiza a funcionalidade [3] (SELECT FROM WHERE) por meio de funções auxiliares. Inicializa valores e executa operações de entrada e saída. Acessível do arquivo main.c do projeto. */
    void func_select_where_interface();    
    
    bool func_delete();
    
    bool func_insert();
    
    bool func_update();

#endif
