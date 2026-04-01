#ifndef AVL_H
    #define AVL_H

    #include <stdbool.h>
    #include <stdio.h> 
    #include <stdlib.h>
    #include <string.h>

    typedef unsigned int uint;
    typedef unsigned long long ull;
    typedef char int8;
    typedef struct arvore_ ARVORE;

    /*=============ALOCAÇÃO E DESALOCAÇÃO============*/

    /**/
    ARVORE* avl_criar_arvore(); 

    /**/
    bool avl_apagar_arvore(ARVORE** ar);

    /*=============INSERÇÃO============*/

    /**/
    bool avl_inserir(ARVORE* ar, char* nome);

    /*=============REMOÇÃO============*/

    /**/
    bool avl_remover(ARVORE* ar, char* nome);

    /*=============GETTER============*/

    /**/
    uint avl_get_n(ARVORE *ar);
    
    /*=============TESTE============*/

    /**/
    void avl_print_em_ordem(ARVORE* ar);
    
#endif