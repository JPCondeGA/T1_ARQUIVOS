#ifndef AVL_H
    #define AVL_H

    #include <stdbool.h>
    #include <stdio.h> 
    #include <stdlib.h>

    typedef unsigned int uint;
    typedef unsigned long long ull;
    typedef char int8;
    typedef struct arvore_ ARVORE;

    ARVORE* criar_arvore(); //Retorna ponteiro para a árvore criada
    bool apagar_arvore(ARVORE** ar);
    bool inserir(ARVORE* ar, int tamanho, char* nome);
    bool remover(ARVORE* ar, char* nome);
    
    void print_em_ordem(ARVORE* ar);
    
    
#endif
