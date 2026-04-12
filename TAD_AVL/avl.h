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

    /* Para evitar a criação de outro TAD AVL, utilizamos um nó com dupla chave. As funções irão acessar uma ou outra chave, a depender do parâmetro tipo cujo valor true significa ordenação pelo campo par (e false, pelo o nome). */

    /* O par de inteiros não depende da ordem. */

    /* Para ordenação de uma AVL de pares foi usado o seguinte método: consideramos o campo maior como mais relevante; desse modo se n1.maior > n2.maior (n1 e n2 são nós), temos certeza que n1 está a direita de n2. Em caso de n1.maior == n2.maior, olhamos o campo menor. */

    /*=============ALOCAÇÃO E DESALOCAÇÃO============*/

    /* Realiza alocação dinâmica para a estrutura que servirá de árvore AVL
    - Recebe um booleando tipo que indica o tipo da árvore: ordenado pelos pares ou ordenada pelos nomes. No primeiro caso, tipo deve ser igual a true; no segundo, false.
    - Retorna o ponteiro para o espaço alocado, ou NULL no caso de falha.
    
    O tipo da árvore determinará como serão feitas as buscas também. Portanto, não é possível remover nó pelo nome em uma árvore ordenada por pares. */
    ARVORE* avl_criar_arvore(bool tipo); 

    /* Desaloca toda a informação salva na árvore em pós-ordem, depois desaloca a árvore em si.
    - Recebe um ponteiro de ponteiro para a árvore a ser apagada como entrada.
    - Retorna "true" se a desalocação ocorreu corretamente, ou "false" caso houve alguma falha ou o ponteiro de ponteiro passado não é válido. */
    bool avl_apagar_arvore(ARVORE** ar);

    /*=============INSERÇÃO============*/

    /* Insere um nome e um par de inteiros na árvore. Caso a mesma chave já tenha sido inserida previamente, sua frequência é incrementada ao invés de armazenar a mesma informação várias vezes.
    - Recebe um ponteiro para a árvore em que a chave será salva, o nome que será salvo e os inteiros que formarão o par.
    - Retorna "true" se a inserção ocorreu corretamente ou "false" caso contrário.
    
    
    Note que a função retornará "true" caso um nó com o mesmo nome foi encontrado na árvore. Não houve inserção, apenas atualização de frequência, mas sem falhas
    Importante ressaltar que a função copia o nome para outro espaço de memória, mantendo o conceito de TAD. */
    bool avl_inserir(ARVORE* ar, char* nome, int cod_estacao, int cod_prox_estacao);

    /*=============REMOÇÃO============*/

    /* Remove um nó da árvore. Caso sua frequência seja maior que 1, o nó não é removido de fato, mas sua frequência é decrementada. Caso contrário, ele é apagado.
    - Recebe um ponteiro para a árvore em que o nó será removidp, o nome e o par que deve ser procurado a depender do tipo da árvore.
    - Retorna "true" se um nó foi removido, e "false" caso contrário.
    
    Note que a função retornará "true" se um nó foi encontrado, mas sua frequência era maior que 1. Não houve remoção, apenas atualização de frequência. */
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
