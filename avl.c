#include "avl.h"
#define maior(a, b) ((a > b) ? a : b)

typedef struct no_ NO;
typedef struct arvore_ ARVORE;

struct no_{
    uint frequencia; // Quantidade de vezes que uma chave aparece no arquivo.
    int altura; // Altura do nó
    char* nome; // Chave
    NO* dir; // Ponteiro para o filho direito
    NO* esq; // Ponteiro para o filho esquerdo
};

struct arvore_{
    NO* raiz;
    uint n; // Quantidade de nós do árvore
};

/*=============FUNÇÕES OCULTAS============*/

/* Dinamicamente aloca espaço para um nó e para uma chave.
  Retorna um nó, com o string passado em "nome" salvo dentro dele.
  - Usado nas funções de inserção.*/
NO* avl_criar_no(char* nome);

/* Desaloca o espaço da chave do nó, então desaloca o nó em si.
  - Recebe um ponteiro de ponteiro do nó a ser apagado.
  - Retorna "true" se o nó for apagado corretamente e "false" caso contrário.
  - Usado nas funções de remoção.*/
bool avl_apagar_no(NO** no);

/* Retorna o valor da altura do nó passado como entrada, retorna 0 caso o ponteiro para nó passado for NULL.
  - Usado nas funções de inserção e remoção para manter o equilíbrio da árvore.*/
int avl_altura_no(NO* no);

/* Realiza uma rotação à esquerda de acordo com o conceito de árvore AVL.
  - Usado nas funções de inserção e remoção para manter o equilíbrio da árvore.*/
NO* avl_rodar_e(NO* no);

/* Realiza uma rotação à direta de acordo com o conceito de árvore AVL.
  - Usado nas funções de inserção e remoção para manter o equilíbrio da árvore.*/
NO* avl_rodar_d(NO* no);

/* Calcula o fator de balanceamento de um nó usando a altura de seus dois filhos.
  - Usado nas funções de inserção e remoção para manter o equilíbrio da árvore.*/
int avl_fb(NO* no);

/* Função recursiva que insere um novo nó na árvore.
  "*flag" é sempre passada como "false".
  Caso um nó for inserido com sucesso, "*flag" será atualizada para "true". Isso não ocorre se um nó com o mesmo nome já se encontrava na árvore.
  - Usada na função de inserção.*/
NO* avl_inserir_no(NO* cima, NO* baixo, bool *flag);

/* Recebe um nó e um ponteiro de ponteiro auxiliar.
  Encontra o maior filho à esquerda do nó passado como entrada, guarda seu ponteiro na variável auxiliar "*aux" e o remove da árvore.
  Rebalanceia e retorna a nova subárvore à esquerda do nó de entrada.
  É necessário passar um ponteiro de ponteiro auxiliar, pois "*aux" apontará para o maior filho à esquerda no retorno.
  Se um ponteiro regular fosse passado, a informação seria perdida ao sair do escopo.
  Note que, apesar do nome da função, a troca ocorre após seu retorno.
  - Usado na função de remoção.*/
NO* avl_troca_com_maior_filho(NO* no, NO** aux);

/* Procura o nó a ser removido e escolhe uma estratégia de remoção dependendo de seu estado.
  Se o nó possui frequência maior que 1, ela é atualizada e a remoção é encerrada sem remover o nó.
  Se o nó possuir frequência igual a 1, então confere seu número de filhos.
  Se ele possuir 0 ou 1 filhos, remove o nó e transfere o filho (se existir) para seu lugar.
  Se ele possuir 2 filhos, chama "avl_troca_com_maior_filho()" e então troca seu lugar com o de seu maior filho à esquerda.
  Depois, apaga o nó e re-equilibra a árvore.
  "no" é a raiz da árvore, não o nó a ser removido.
  "nome" é a chave que deve ser procurada para a remoção.
  "*resp" guarda informação se o nó que deve ser removido foi encontrado ou não e de qual estratégia foi usada, para decidir se "avl_remover()" retornará "true" ou "false".
  - Usado na função de remoção.*/
NO* avl_remover_no(NO* no, char* nome, int8 *resp);

/* Função auxiliar de "avl_apagar_arvore()".
  Recursão que percorre a árvore em pós-ordem, apagando seus nós pelo caminho.
  Não se preocupa em mantê-la equilibrada, pois seu intuito é esvaziar a árvore completamente.*/
void avl_apagar_arvore_no(NO* no);

/* Calcula se um dado nó está desequilibrado, e aplica a rotação necessária de acordo com o conceito de árvore AVL.
  - Usado nas funções de inserção e remoção.*/
NO *avl_balanceia(NO* no);

/* Função auxiliar de "avl_print_em_ordem()".
  Percorre a árvore em em-ordem, imprimindo o nome salvo em cada nó pelo caminho.*/
void avl_print_em_ord(NO* no);

/*=============ALOCAÇÃO E DESALOCAÇÃO============*/

NO* avl_criar_no(char* nome){ 
  
    NO* no = (NO*)malloc(sizeof(NO));
    if (no == NULL) return NULL;
  
    int tamanho = strlen(nome);
    // Alocando um byte a mais para o terminador
    no->nome = (char*)malloc(sizeof(char) * (tamanho+1));
    // Em caso de falha de alocação de espaço para o nome
    if (no->nome == NULL){
        free(no);
        return NULL;
    }
    strcpy(no->nome, nome);
  
    no->dir = NULL;
    no->esq = NULL;
    no->frequencia = 1; //Inicia em 1, pois um nó com frequência 0 é deletado.
    no->altura = 0; // O nó é uma folha
  
    return no;
}


bool avl_apagar_no(NO** no){

  if (no == NULL || *no == NULL) return false;
  
  if((*no)->nome != NULL){
    free((*no)->nome);
    (*no)->nome = NULL;
  }
  
  free(*no);
  *no = NULL;
  
  return true;
}

ARVORE* avl_criar_arvore(){
  
  ARVORE* ar = (ARVORE*)malloc(sizeof(ARVORE));
  if (ar == NULL) return NULL;
  
  ar->raiz = NULL;
  ar->n = 0;
  
  return ar;
}

void avl_apagar_arvore_no(NO* no){
    if (no == NULL) return;

    avl_apagar_arvore_no(no->esq);
    avl_apagar_arvore_no(no->dir);

    avl_apagar_no(&no);
    return;
}

// Novamente, como o usuário não tem acesso a nós, uma função auxiliar é necessária para iniciar funções recursivas que requerem nós como entrada.
bool avl_apagar_arvore(ARVORE** ar) {
    if (ar == NULL || *ar == NULL) return false;
    
    avl_apagar_arvore_no((*ar)->raiz);
    
    free(*ar);
    *ar = NULL;
    
    return true;
}

/*=============INSERÇÃO============*/

/* Função recursiva inicialmente chamada por "avl_inserir()".
Baixo é o nó a ser inserido. Cima é o nó "atual" que está sendo comparado com baixo.
Não é necessário conferir se "baixo" é NULL pois "avl_inserir()" faz a verificação antes de iniciar esta função. */
NO* avl_inserir_no(NO* cima, NO* baixo, bool *flag){
  
  if(cima == NULL){
    cima = baixo;
    *flag = true; // Um novo nó foi adicionado
    return cima;
  }
  
  // Se cima e baixo possuem a mesma chave, então são a mesma estação. O valor da frequência do nó existente é atualizado e o nó a ser inserido não é mais necessário.
  if(strcmp(baixo->nome, cima->nome) == 0){
    cima->frequencia++;
    avl_apagar_no(&baixo);
    return cima; 
  }
  // Inserção regular de AVL
  else if (strcmp(baixo->nome, cima->nome) < 0){ 
    cima->esq = avl_inserir_no(cima->esq, baixo, flag);
  }
  else{
    cima->dir = avl_inserir_no(cima->dir, baixo, flag);
  }
  
  return avl_balanceia(cima);
}

// O usuário não tem acesso a nós, então inserção e remoção precisam de uma função auxiliar para iniciar a recursão
bool avl_inserir(ARVORE* ar, char* nome){
    if (ar == NULL) return false;
    
    NO* no = avl_criar_no(nome);
    // Falha na alocação do nó
    if (no == NULL) return false;

    bool flag = false;
  
    ar->raiz = avl_inserir_no(ar->raiz, no, &flag);

    // Se houve a inserção de um novo nó
    if(flag) ar->n++; 

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
    
    // Continua a procura pelo maior filho
    no->dir = avl_troca_com_maior_filho(no->dir, aux); 
    
    // A subárvore direita do nó anterior ao maior filho agora é a subárvore esquerda do maior filho
    return avl_balanceia(no);
}


NO* avl_remover_no(NO* no, char* nome, int8 *resp){
  // Chegou-se no fim da árvore e não encontrou o nó procurado
  if (no == NULL) return NULL;
  
  // Procurando nó
  if(strcmp(nome, no->nome) < 0) no->esq = avl_remover_no(no->esq, nome, resp);
  else if(strcmp(nome, no->nome) > 0) no->dir = avl_remover_no(no->dir, nome, resp);
  // Nó encontrado
  else{
    
    //Primeiro caso: O nó a ser removido foi inserido mais de uma vez (frequência maior que 1)
    if (no->frequencia > 1){ 
        // Caso houver mais de uma estação com o mesmo nome, o nó não é removido, apenas se decrementa o contador de quantas vezes ela foi inserida.
        no->frequencia--;
        *resp = 1;
        return no;
    }
    
    NO* aux; 
    *resp = 2;
    // Segundo caso: O nó deve ser de fato removido e tem 0 ou 1 filho(s)
    if (no->esq == NULL || no->dir == NULL){
        // Para não perder o nó que será apagado
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
        // O maior filho é removido da subárvore esquerda e salvo em aux
        no->esq = avl_troca_com_maior_filho(no->esq, &aux); 
                
        // Substitui no com aux
        aux->esq = no->esq;
        aux->dir = no->dir;
        aux->altura = no->altura;
                
        //Apaga o no substituído.
        avl_apagar_no(&no);
                
        //Atualiza o ponteiro no para o nó em aux.
        no = aux;
      }
      
    }
    
    return avl_balanceia(no);
}

// O usuário não tem acesso a nós, então inserção e remoção precisam de uma função auxiliar para iniciar a recursão.
bool avl_remover(ARVORE* ar, char* nome){
    if (ar == NULL || ar->raiz == NULL) return false;

    /* - 0 -> se nó a ser removido não foi encontrado;
    - 1 -> se nó a ser removido foi encontrado
    - 2 -> se nó a ser removido foi de fato removido (e não só decrementou a frequência)*/
    int8 resp = 0; 
    
    ar->raiz = avl_remover_no(ar->raiz, nome, &resp);

    // O nó a ser removido foi encontrado
    if(resp != 0){
        // Um nó foi de fato removido, e não apenas atualizado.
        if(resp == 2) ar->n--;
        
        return true;
    }

    return false;
}

/*=============ROTAÇÕES============*/

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

/*=============GETTER============*/

uint avl_get_n(ARVORE *ar){
    if(ar == NULL) return 0;

    return ar->n;
}


/*=============AUXILIAR============*/

int avl_altura_no(NO* no){
  if (no == NULL) return -1; 
  return no->altura;
}

int avl_fb(NO* no){ 
  return (avl_altura_no(no->esq) - avl_altura_no(no->dir));
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

/*=============TESTE============*/

void avl_print_em_ord(NO* no){
  
  if (no == NULL) return;
  
  if (no->esq != NULL) avl_print_em_ord(no->esq);
  printf(" %s - %d\n", no->nome, no->frequencia);
  if (no->dir != NULL) avl_print_em_ord(no->dir);
  return;

}

void avl_print_em_ordem(ARVORE* ar){
  
  avl_print_em_ord(ar->raiz);
  return;
  
}
