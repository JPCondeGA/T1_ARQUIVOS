#include "avl.h"
#define maior(a, b) ((a > b) ? a : b) // Macro que escolhe o maior de dois valores

typedef struct no_ NO;
typedef struct arvore_ ARVORE;
typedef struct par_ PAR;

// Estrutura que representa um par de números de forma ordenado
struct par_{
  int menor; // Menor código do par (código da estação, código da próxima estação)
  int maior; // Maior código do par (código da estação, código da próxima estação)
};

// Estrutura que representa um nó da árvore binário que pode ser ordenada por nome oou por pares de inteiros
struct no_{
  uint frequencia; // Quantidade de vezes que uma chave aparece no arquivo.
  int altura; // Altura do nó
  char* nome; // Chave (no caso da avl de nomes)
  PAR par; // Chave (no caso da avl de pares)
  NO* dir; // Ponteiro para o filho direito
  NO* esq; // Ponteiro para o filho esquerdo
};

// Estrutura que representa a árvore que pode ser de dois tipos 
struct arvore_{
    NO* raiz; // Nó raiz da árvore
    uint n; // Quantidade de nós do árvore
    bool tipo; // true -> árvore de pares; false -> árvore de nomes
};

/*=============FUNÇÕES OCULTAS============*/

/*=-=-ALOCAÇÃO E DESALOCAÇÃO DE NÓ-=-=-=*/

/* Dinamicamente aloca espaço para um nó e para uma chave.
  - Recebe uma string para o nome, uma estrutura de pares e booleano para indicar o tipo da ordenação da árvore.
  - Retorna um nó, com a string passada e o par de inteiros salvo dentro dele.

Usado nas funções de inserção. */
NO* avl_criar_no(char* nome, PAR par, bool tipo);

/* Desaloca os campos necessários do nó, então desaloca o nó em si.
  - Recebe um ponteiro de ponteiro do nó a ser apagado.
  - Retorna true, se o nó for apagado corretamente; false, caso contrário.

Usado nas funções de remoção. */
bool avl_apagar_no(NO** no);


/*=-=-BALANCEAMENTO=-=-=*/

/* Retorna o valor da altura do nó passado como entrada. 
  - Recebe um ponteiro para um nó.
  - Retorna a altura do nó correspondente; -1, no caso de ponteiro NULL

Usado nas funções de inserção e remoção para manter o equilíbrio da árvore. */
int avl_altura_no(NO* no);

/* Calcula o fator de balanceamento (FB) de um nó usando a altura de seus dois filhos.
  - Recebe o nó a ter seu FB calculado.
  - Retorna o FB correspondente

  Usado nas funções de inserção e remoção para manter o equilíbrio da árvore. */
int avl_fb(NO* no);

/* Realiza uma rotação à esquerda de acordo com o conceito de árvore AVL.
  - Recebe o ponteiro para o nó a ser analisado.
  - Retorna o nó que ficou na posição daquele passado por parâmetro.

Usado nas funções de inserção e remoção para manter o equilíbrio da árvore. */
NO* avl_rodar_e(NO* no);

/* Realiza uma rotação à direta de acordo com o conceito de árvore AVL.
  - Recebe o ponteiro para o nó a ser analisado.
  - Retorna o nó que ficou na posição daqueel passado por parâmetro.

Usado nas funções de inserção e remoção para manter o equilíbrio da árvore. */
NO* avl_rodar_d(NO* no);

/* Calcula se um dado nó está desequilibrado, e aplica a rotação necessária de acordo com o conceito de árvore AVL.
  - Usado nas funções de inserção e remoção.*/
NO *avl_balanceia(NO* no);

/*=-=-INSERÇÃO-=-=*/

/* Função recursiva que insere um novo nó na árvore. Caso já exista nó com a chave, o nó já existente tem sua frequência incrementada e a flag continua false para correções na função de inserir.
  - Recebe o nó atual que está sendo visitado na árvore (cima), o nó a ser inserido (baixo), o tipo de ordenação da árvore e um ponteiro para flag para indicar se foi de fato adicionado um novo nó.
  - Retorna a nova raiz da (sub)árvore na volta da recursão. */
NO* avl_inserir_no(NO* cima, NO* baixo, bool tipo, bool *novo);


/*=-=-REMOÇÃO-=-=*/

/* Encontra o maior filho à esquerda do nó passado como entrada, guarda seu ponteiro na variável auxiliar "*aux" e o remove da árvore, rebalanceando. 
  - Recebe um nó e um ponteiro de ponteiro auxiliar.
  - Retorna a nova raiz da subárvore à esquerda do nó de entrada.
  
Note que, apesar do nome da função, a troca ocorre após seu retorno (ela só encontra o maior filho à esquerda, pega-o e remove-o).
Usado na função de remoção. */
NO* avl_troca_com_maior_filho(NO* no, NO** aux);

/* Procura o nó a ser removido e escolhe uma estratégia de remoção dependendo de seu estado.
  * 1. Se o nó possui frequência maior que 1, ela é atualizada e a remoção é encerrada sem remover o nó.
  * 2. Se o nó possuir frequência igual a 1, então confere seu número de filhos.
  * 3. Se ele possuir 0 ou 1 filhos, remove o nó e transfere o filho (se existir) para seu lugar.
  * 4. Se ele possuir 2 filhos, chama "avl_troca_com_maior_filho()" e então troca seu lugar com o de seu maior filho à esquerda.
  Depois, apaga o nó e re-equilibra a árvore.
  - Recebe a raiz da (sub)árvore que está sendo buscado o nó a ser removido, a chave (nome ou par) que deve ser procurada para a remoção, o tipo de ordenação da árvore e um ponteiro para uma flag que guarda informação se o nó que deve ser removido foi encontrado ou não e de qual estratégia foi usada.
  - Retorna a nova raiz da (sub)árvore na volta da recursão.
  
resp terá o valor 1 para o caso 1; 2 para o casos 2, 3 e 4; 0 para o caso de não se achar o nó a ser removido*/
NO* avl_remover_no(NO* no, char* nome, PAR par, bool tipo, int8 *resp);

/*=-=-OUTRAS-=-=*/

/* Função recursiva que percorre a árvore em pós-ordem, apagando seus nós pelo caminho.
  - Recebe a raiz da (sub)árvore a ser percorrida.

Não se preocupa em mantê-la equilibrada, pois seu intuito é esvaziar a árvore completamente.*/
void avl_apagar_arvore_no(NO* no);

/* Compara a chave de um nó com uma das chaves passadas a depender do tipo de ordenação.
  - Recebe o nó a ser comparado, as duas chaves passadas (nome ou par) e o tipo da ordenação da árvore.
  - Retorna 1 se a chave correspondente ao tipo de ordenação for maior que a mesma chave do nó; retorna 0, se forem iguais; e -1, se for menor. */
int8 avl_compara_no(NO* no, char* nome, PAR par, bool tipo);

/* Cria um par, colocando a ou b como o valor maior do par, e o outro como menor.
  - Recebe os dois inteiros do par (pode ser em qualquer ordem).
  - Retorna uma estrutura que representa um par de inteiros ordenada.*/
PAR avl_criar_par(int a, int b);

/* Função auxiliar de "avl_print_em_ordem()". Percorre a árvore em em-ordem, imprimindo o nome salvo em cada nó pelo caminho.
  - Recebe a raiz da (sub)árvore a ser percorrida e o tipo de ordenação da árvore.*/
void avl_print_em_ordem_no(NO* no, bool tipo);


/*=============ALOCAÇÃO E DESALOCAÇÃO============*/

ARVORE* avl_criar_arvore(bool tipo){
  
  ARVORE* ar = (ARVORE*)malloc(sizeof(ARVORE)); // Alocando a árvore 
  if (ar == NULL) return NULL; // Verificando alocação
  
  // Inicializando valores
  ar->raiz = NULL;
  ar->tipo = tipo; // Tipo de ordenação da árvore (não será mudada após ser criada)
  ar->n = 0;
  
  return ar;
}


bool avl_apagar_arvore(ARVORE** ar) {
    if (ar == NULL || *ar == NULL) return false; // Verificando ponteiros
    
    avl_apagar_arvore_no((*ar)->raiz); // Percurso em-ordem recursivo
    
    free(*ar); // Desalocando a árvore em si
    *ar = NULL; // Setando NULL para evitar acessos indevido
    
    return true;
}

/*=============INSERÇÃO============*/

NO* avl_inserir_no(NO* cima, NO* baixo, bool tipo, bool *novo){
  
  // Se chegamos ao fim da árvore, o nó deve ser adicionado ali
  if(cima == NULL){
    cima = baixo;
    *novo = true; // O novo nó foi adicionado
    return cima;
  }
  
  //Caso contrário, devemos saber em qual direção devemos continuar procurando o local para inserí-lo, por isso comparando
  int8 comp = avl_compara_no(cima, baixo->nome, baixo->par, tipo); 

  // Se cima e baixo possuem a mesma chave, então são a mesma estação ou mesmo par. O valor da frequência do nó existente é atualizado e o nó novo não será inserido.
  if(comp == 0){
    cima->frequencia++;
    *novo = false; // O novo nó não foi adicionado 
    return cima; 
  }

  // Inserção regular de AVL
  else if (comp == -1){ 
    cima->esq = avl_inserir_no(cima->esq, baixo, tipo, novo); // Continuando pela esquerda
  }
  else{
    cima->dir = avl_inserir_no(cima->dir, baixo, tipo, novo); // Continuando pela direita
  }
  
  return avl_balanceia(cima); // Rebalaceando e retornando o tipo da (sub)árvore rebalanceada
}

bool avl_inserir(ARVORE* ar, char* nome, int cod_estacao, int cod_prox_estacao){
    if (ar == NULL) return false; // Verificnado ponteiro
    
    PAR par = avl_criar_par(cod_estacao, cod_prox_estacao); // Criando par

    // Alocando NO
    NO* no = avl_criar_no(nome, par, ar->tipo);
    if (no == NULL) return false; // Falha na alocação do nó

    bool novo = false; // Confere se um novo nó foi realmente inserido ou se só houve incremento de frequência de algum já existente

    ar->raiz = avl_inserir_no(ar->raiz, no, ar->tipo, &novo); // Iniciando recursão

    // Verificando se foi adicionado de fato um novo nó
    if(novo) ar->n++; // Se houve a inserção de fato de um novo nó, aumentamos a quantidade de nós da árvore
    else avl_apagar_no(&no); // Senão, devemos apagar o nó alocado
    
    return true;
}

/*=============REMOÇÃO============*/

NO* avl_troca_com_maior_filho(NO* no, NO** aux){
    // Esse caso não é possível na prática
    if (no == NULL)
        return NULL;
    
    // Quando o nó não possui mais filhos à direita, encontramos o maior filho.
    if (no->dir == NULL) {
        
        // Esta função é recursiva, então aux é um ponteiro de ponteiro para podermos acessar este ponteiro na volta das chamadas.
        *aux = no; 

        // Salva a subárvore esquerda, caso exista
        NO* aux2 = no->esq; 
        
        no->esq = NULL;
        no->dir = NULL;

        return aux2;
    }
    
    no->dir = avl_troca_com_maior_filho(no->dir, aux); // Continua a procura pelo maior filho
    
    // A subárvore direita do nó anterior ao maior filho agora é a subárvore esquerda do maior filho
    return avl_balanceia(no); // Retornando a raiz da árvore balanceada (o maior filho foi retirado)
}


NO* avl_remover_no(NO* no, char* nome, PAR par, bool tipo, int8 *resp){
  if (no == NULL){ // Chegou-se no fim da árvore e não encontrou o nó procurado
    *resp = 0;
    return NULL; 
  }
  
  int8 comp = avl_compara_no(no, nome, par, tipo); // Comprando o nó atual com as chaves passadas

  // Procurando nó
  if(comp < 0) no->esq = avl_remover_no(no->esq, nome, par, tipo, resp); // Continuando pela esquerda 
  else if(comp > 0) no->dir = avl_remover_no(no->dir, nome, par, tipo, resp); // Continuando pela direita
  // Nó encontrado
  else{
    
    //Primeiro caso: O nó a ser removido foi inserido mais de uma vez (frequência maior que 1)
    if (no->frequencia > 1){ 
        // Caso houver mais de uma estação com o mesmo nome ou mais de um par (codEstação, codProxEstação), o nó não é removido, apenas se decrementa o contador de quantas vezes ela foi inserida.
        no->frequencia--;
        *resp = 1;
        return no;
    }
    
    NO* aux; // Recebe o filho do nó a ser removido ou maior descendente à esquerda
    *resp = 2;
    // Segundo caso: O nó deve ser de fato removido e tem 0 ou 1 filho(s)
    if (no->esq == NULL || no->dir == NULL){
        // Para não perder os possíveis filhos do nó que será apagado
        if(no->esq == NULL){
            aux = no->dir;
            no->dir = NULL;
        }
        else{
            aux = no->esq;
            no->esq = NULL;
        }

        avl_apagar_no(&no);
      
        return aux;
    }
    // Terceiro caso: O nó deve ser removido e tem 2 filhos
    else{ 
    
        aux = NULL;      
        no->esq = avl_troca_com_maior_filho(no->esq, &aux); // O maior filho é removido da subárvore esquerda e salvo em aux        

        // Substitui no com aux
        aux->esq = no->esq;
        aux->dir = no->dir;
        aux->altura = no->altura;
                
        avl_apagar_no(&no); // Apaga o no substituido

        no = aux; //Atualiza o ponteiro no para o nó em aux.
      }
      
    }
    
    return avl_balanceia(no); // Balanceando e retornando a raiz da (sub)árvore balanceada
}

bool avl_remover(ARVORE* ar, char* nome, int cod_estacao, int cod_prox_estacao){
    if (ar == NULL || ar->raiz == NULL) return false; // Verificando ponteiros

    /* - 0 -> se nó a ser removido não foi encontrado;
    - 1 -> se nó a ser removido foi encontrado
    - 2 -> se nó a ser removido foi de fato removido (e não só decrementou a frequência)*/
    int8 resp = 0;

    PAR par = avl_criar_par(cod_estacao, cod_prox_estacao); // Criando par para busca, se necessário
    
    ar->raiz = avl_remover_no(ar->raiz, nome, par, ar->tipo, &resp);

    // O nó a ser removido foi encontrado
    if(resp != 0){
        if(resp == 2) ar->n--; // Um nó foi de fato removido, e não apenas atualizado.
        return true;
    }

    return false;
}

/*=============GETTER============*/

uint avl_get_n(ARVORE *ar){
    if(ar == NULL) return 0;

    return ar->n;
}

bool avl_get_tipo(ARVORE *ar){
    if(ar == NULL) return true; // Essa saída não influencia muito, por isso setamos que se o ponteiro é inválido, retorna-se que a árvore é de pares
    return ar->tipo;
}

NO *avl_balanceia(NO* no){
    // Essa condição será verdadeira no caso da árvore ter apenas 1 nó.
    if (no == NULL) return NULL;
    
    // Atualizando altura
    no->altura = maior(avl_altura_no(no->esq), avl_altura_no(no->dir)) + 1;
    
    if (avl_fb(no) == -2){
      if (avl_fb(no->dir) <= 0) no = avl_rodar_e(no); //Rotação esquerda simples
      else{
        no->dir = avl_rodar_d(no->dir); //Rotação direita esquerda.
        no = avl_rodar_e(no);
      }
    }
  
    if (avl_fb(no) == 2){
      if (avl_fb(no->esq) >= 0) no = avl_rodar_d(no); //Rotação direita simples
      else{
        no->esq = avl_rodar_e(no->esq); //Rotação esquerda direita.
        no = avl_rodar_d(no);
      }
    }
    return no;
}


/*=============ALOCAÇÃO E DESALOCAÇÃO DE NÓ============*/

NO* avl_criar_no(char* nome, PAR par, bool tipo){ 
  // Alocando NO
  NO* no = (NO*)malloc(sizeof(NO));
  if (no == NULL) return NULL; // Verificando se a alocação ocorreu corretamente

  if (nome == NULL && !tipo){ // Se a árvore é ordenada por nome, não pode receber um nome nulo
    free(no); // Evitando leak (esse nó não será adicionado na árvore já que não cumpre os critérios)
    return NULL;
  }
  else if(nome != NULL){ // Tanto para tipo = false ou tipo = true, iremos copiar o nome para o nó se esse não for nulo
    int tamanho = strlen(nome); // Consideramos que a string tem \0
    no->nome = (char*)malloc(sizeof(char) * (tamanho+1)); // +1 para colocar o terminador
    // Em caso de falha de alocação de espaço para o nome
    if (no->nome == NULL){
        free(no); // Evitando Leak
        return NULL;
    }
    strcpy(no->nome, nome); // Copiando o nome para o campo alocado do nó
  }
  else{ // Esse else corresponde a: nome == NULL && tipo == true, nesse caso é possível colocar o campo como nulo (já que não será usado como chave)
    no->nome = NULL;
  }
  

  no->par = par;

  // Inicialmente não tem filhos
  no->dir = NULL;
  no->esq = NULL;

  no->frequencia = 1; //Inicia em 1, pois um nó com frequência 0 é deletado.
  no->altura = 0; // O nó é uma folha

  return no;
}


bool avl_apagar_no(NO** no){
  if (no == NULL || *no == NULL) return false; // Verificando se os ponteiros são válidos

  // Se o nome não for nulo, deve ser desalcado
  if((*no)->nome != NULL){ 
    free((*no)->nome);
    (*no)->nome = NULL;
  }
  
  // Desalocando nó e setando NULL para evitar acessos indevidos
  free(*no); 
  *no = NULL;
  
  return true;
}


/*=============BALANCEAMENTO============*/

int avl_altura_no(NO* no){
  if (no == NULL) return -1; 
  return no->altura;
}

int avl_fb(NO* no){ 
  return (avl_altura_no(no->esq) - avl_altura_no(no->dir));
}

NO* avl_rodar_e(NO* no){
    NO* aux = no->dir;
    no->dir = aux->esq;
    aux->esq = no;

    // Atualizando alturas.
    no->altura = maior(avl_altura_no(no->esq), avl_altura_no(no->dir)) + 1; 
    aux->altura = maior(avl_altura_no(aux->esq), avl_altura_no(aux->dir)) + 1;
  
    return aux;
}

NO* avl_rodar_d(NO* no){
  NO* aux = no->esq;
  no->esq = aux->dir;
  aux->dir = no;
  
  no->altura = maior(avl_altura_no(no->esq), avl_altura_no(no->dir)) + 1; //Atualizando alturas
  aux->altura = maior(avl_altura_no(aux->esq), avl_altura_no(aux->dir)) + 1;
  
  return aux;
}


/*=============OUTRAS============*/

void avl_apagar_arvore_no(NO* no){
    if (no == NULL) return; // Verificando ponteiro (se acabou a árvore -> caso base)

    // Percurso em-ordem
    avl_apagar_arvore_no(no->esq); // Esquerda
    avl_apagar_arvore_no(no->dir); // Direita

    avl_apagar_no(&no); // Apagando o atual

    return;
}

int8 avl_compara_no(NO* no, char* nome, PAR par, bool tipo){
  if(no == NULL) return 0; // Para não tentar acessar o filho direito ou esquerdo
  
  // Retorn do strcmp
  int resp;
  
  // tipo = false -> AVL de nomes
  if(!tipo && nome != NULL && no->nome != NULL){
    
    resp = strcmp(no->nome, nome);
    
    // Conferindo retorno do strcmp 
    // no->nome é menor do que nome lexicograficamente
    if(resp < 0) return 1;
    // no->nome é maior do que nome lexicograficamente
    if(resp > 0) return -1;
    // Ambos são iguais
    else return 0;
  }
  else if(tipo){
    // Maior é mais relevante
    if(no->par.maior > par.maior) return -1;
    if(no->par.maior < par.maior) return 1;
    
    // Caso o maior seja igual, olhamos para o menor
    if(no->par.menor > par.menor) return -1;
    if(no->par.menor < par.menor) return 1;

    // Caso os dois forem iguais
    else return 0;
  }

  return 0;
}

PAR avl_criar_par(int a, int b){
  PAR par;
  // Os valores dentro do par seguem uma ordem para não se diferenciar o par (a,b) do (b,c) e para a busca na árvore
  if(a < b) par.menor = a, par.maior = b;
  else par.maior = a, par.menor = b;

  return par;
}

/*=============TESTE============*/

void avl_print_em_ordem_no(NO* no, bool tipo){
  
  if (no == NULL) return;
  
  if (no->esq != NULL) avl_print_em_ordem_no(no->esq, tipo);
  if(!tipo) printf(" %s", no->nome);
  else printf(" (%d, %d)", no->par.maior, no->par.menor);
  printf(" - %d\n", no->frequencia);
  if (no->dir != NULL) avl_print_em_ordem_no(no->dir, tipo);
  return;

}

void avl_print_em_ordem(ARVORE* ar){
  
  avl_print_em_ordem_no(ar->raiz, ar->tipo);
  return;
  
}

