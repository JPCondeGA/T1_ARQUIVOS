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

    /* Realiza alocação dinâmica para a estrutura que servirá de árvore AVL
    - Recebe um booleando op que indica o tipo da árvore: ordenado pelos pares ou ordenada pelos nomes. No primeiro caso, op deve ser igual a true; no segundo, false.
    - Retorna o ponteiro para o espaço alocado, ou NULL no caso de falha.
    
    O tipo da árvore determinará como serão feitas as buscas também. Portanto, não é possível remover nó pelo nome em uma árvore ordenada por pares. */
    ARVORE* avl_criar_arvore(bool op); 

    /* Desaloca toda a informação salva na árvore em pós-ordem, depois desaloca a árvore em si.
    - Recebe um ponteiro de ponteiro para a árvore a ser apagada como entrada.
    - Retorna "true" se a desalocação ocorreu corretamente, ou "false" caso houve alguma falha ou o ponteiro de ponteiro passado não é válido.*/
    bool avl_apagar_arvore(ARVORE** ar);

    /*=============INSERÇÃO============*/

    /* Insere um nome na árvore. Caso a mesma chave já tenha sido inserida previamente, sua frequência é incrementada ao invés de armazenar a mesma informação várias vezes.
    - Recebe um ponteiro para a árvore em que a chave será salva e o nome que será salvo.
    - Retorna "true" se a inserção ocorreu corretamente ou "false" caso contrário.
    (Note que a função retornará "false" caso um nó com o mesmo nome foi encontrado na árvore. Não houve inserção, apenas atualização de frequência.)*/
    bool avl_inserir(ARVORE* ar, char* nome, int cod_estacao, int cod_prox_estacao);

    /*=============REMOÇÃO============*/

    /* Remove uma chave da árvore. Caso sua frequência seja maior que 1, a chave não é removida, e sua frequência é decrementada. Caso contrário, ele é apagado.
    - Recebe um ponteiro para a árvore em que a chave será removida e o nome que deve ser procurado.
    - Retorna "true" se um nó foi removido, e "false" caso contrário.
    (Note que a função retornará "false" se um nó foi encontrado, mas sua frequência era maior que 1. Não houve remoção, apenas atualização de frequência.)*/
    bool avl_remover(ARVORE* ar, char* nome, int cod_estacao, int cod_prox_estacao);

    /*=============GETTER============*/

    /* Recupera informação da estrutura da árvore.
    - Recebe um ponteiro de árvore como entrada
    - Retorna o número de chaves diferentes armazenadas nela. */
    uint avl_get_n(ARVORE *ar);

    /*Recupera informação da estrutura da árvore.
    - Recebe um ponteiro de árvore como entrada
    - Retorna o tipo da árvore: true, se for uma árvore de pares de inteiro; false, caso contrário. */
    bool avl_get_op(ARVORE *ar);
    
    /*=============TESTE============*/

    /* Imprime todas as chaves da árvore em-ordem e sua frequência. Função usada para testes.*/
    void avl_print_em_ordem(ARVORE* ar);
    
#endif
