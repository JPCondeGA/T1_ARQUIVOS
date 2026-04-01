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

/**/
NO* criar_no(char* nome);

/**/
bool apagar_no(NO** no);

/**/
int altura_no(NO* no);

/**/
NO* rodar_e(NO* no);

/**/
NO* rodar_d(NO* no);

/**/
int fb(NO* no);

/**/
NO* troca_com_maior_filho(NO* no, NO** aux);

/**/
NO* inserir_no(NO* cima, NO* baixo, bool *flag);

/**/
NO* troca_com_maior_filho(NO* no, NO** aux);

/**/
NO* remover_no(NO* no, char* nome, int8 *resp);

/**/
void apagar_subarvore(NO* no);

/**/
NO *avl_balanceia(NO* no);

/*=============ALOCAÇÃO E DESALOCAÇÃO============*/

NO* criar_no(char* nome){ 
  
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


bool apagar_no(NO** no){

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

void apagar_arvore_no(NO* no){
    if (no == NULL) return;

    apagar_arvore_no(no->esq);
    apagar_arvore_no(no->dir);

    apagar_no(&no);
    return;
}

// Novamente, como o usuário não tem acesso a nós, uma função auxiliar é necessária para iniciar funções recursivas que requerem nós como entrada.
bool avl_apagar_arvore(ARVORE** ar) {
    if (ar == NULL || *ar == NULL) return false;
    
    apagar_arvore_no((*ar)->raiz);
    
    free(*ar);
    *ar = NULL;
    
    return true;
}

/*=============INSERÇÃO============*/

/* Função recursiva inicialmente chamada por "inserir()".
Baixo é o nó a ser inserido. Cima é o nó "atual" que está sendo comparado com baixo.
Não é necessário conferir se "baixo" é NULL pois "inserir()" faz a verificação antes de iniciar esta função. */
NO* inserir_no(NO* cima, NO* baixo, bool *flag){
  
  if(cima == NULL){
    cima = baixo;
    *flag = true; // Um novo nó foi adicionado
    return cima;
  }
  
  // Se cima e baixo possuem a mesma chave, então são a mesma estação. O valor da frequência do nó existente é atualizado e o nó a ser inserido não é mais necessário.
  if(strcmp(baixo->nome, cima->nome) == 0){
    cima->frequencia++;
    apagar_no(&baixo);
    return cima; 
  }
  // Inserção regular de AVL
  else if (strcmp(baixo->nome, cima->nome) < 0){ 
    cima->esq = inserir_no(cima->esq, baixo, flag);
  }
  else{
    cima->dir = inserir_no(cima->dir, baixo, flag);
  }
  
  return avl_balanceia(cima);
}

// O usuário não tem acesso a nós, então inserção e remoção precisam de uma função auxiliar para iniciar a recursão
bool avl_inserir(ARVORE* ar, char* nome){
    if (ar == NULL) return false;
    
    NO* no = criar_no(nome);
    // Falha na alocação do nó
    if (no == NULL) return false;

    bool flag = false;
  
    ar->raiz = inserir_no(ar->raiz, no, &flag);

    // Se houve a inserção de um novo nó
    if(flag) ar->n++; 

    return true;

}

/*=============REMOÇÃO============*/

NO* troca_com_maior_filho(NO* no, NO** aux){
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
    no->dir = troca_com_maior_filho(no->dir, aux); 
    
    // A subárvore direita do nó anterior ao maior filho agora é a subárvore esquerda do maior filho
    return avl_balanceia(no);
}


NO* remover_no(NO* no, char* nome, int8 *resp){
  // Chegou-se no fim da árvore e não encontrou o nó procurado
  if (no == NULL) return NULL;
  
  // Procurando nó
  if(strcmp(nome, no->nome) < 0) no->esq = remover_no(no->esq, nome, resp);
  else if(strcmp(nome, no->nome) > 0) no->dir = remover_no(no->dir, nome, resp);
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

        apagar_no(&no);
      
        return aux;
    }
    // Terceiro caso: O nó deve ser removido e tem 2 filhos
    else{ 
    
        aux = NULL;      
        // O maior filho é removido da subárvore esquerda e salvo em aux
        no->esq = troca_com_maior_filho(no->esq, &aux); 
                
        // Substitui no com aux
        aux->esq = no->esq;
        aux->dir = no->dir;
        aux->altura = no->altura;
                
        //Apaga o no substituído.
        apagar_no(&no);
                
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
    - 2 -> se nó a ser removidoo foi de fato a removido (e não só decrementou a frequência)*/
    int8 resp = 0; 
    
    ar->raiz = remover_no(ar->raiz, nome, &resp);

    // O nó a ser removido foi encontrado
    if(resp != 0){
        // Um nó foi de fato removido removido
        if(resp == 2) ar->n--;
        
        return true;
    }

    return false;
}

/*=============ROTAÇÕES============*/

NO* rodar_e(NO* no){
    NO* aux = no->dir;
    no->dir = aux->esq;
    aux->esq = no;

    // Atualizando alturas.
    no->altura = maior(altura_no(no->esq), altura_no(no->dir)) + 1; 
    aux->altura = maior(altura_no(aux->esq), altura_no(aux->dir)) + 1;
  
    return aux;
}

NO* rodar_d(NO* no){
  NO* aux = no->esq;
  no->esq = aux->dir;
  aux->dir = no;
  
  no->altura = maior(altura_no(no->esq), altura_no(no->dir)) + 1; //Atualizando alturas
  aux->altura = maior(altura_no(aux->esq), altura_no(aux->dir)) + 1;
  
  return aux;
}

/*=============GETTER============*/

uint avl_get_n(ARVORE *ar){
    if(ar == NULL) return 0;

    return ar->n;
}


/*=============AUXILIAR============*/

int altura_no(NO* no){
  if (no == NULL) return -1; 
  return no->altura;
}

int fb(NO* no){ 
  return (altura_no(no->esq) - altura_no(no->dir));
}

NO *avl_balanceia(NO* no){
    // Essa condição será verdadeira no caso da árvore ter apenas 1 nó.
    if (no == NULL) return NULL;
    
    // Atualizando altura
    no->altura = maior(altura_no(no->esq), altura_no(no->dir)) + 1;
    
    if (fb(no) == -2){
      if (fb(no->dir) <= 0) no = rodar_e(no); //Rotação esquerda simples
      else{
        no->dir = rodar_d(no->dir); //Rotação direita esquerda.
        no = rodar_e(no);
      }
    }
  
    if (fb(no) == 2){
      if (fb(no->esq) >= 0) no = rodar_d(no); //Rotação direita simples
      else{
        no->esq = rodar_e(no->esq); //Rotação esquerda direita.
        no = rodar_d(no);
      }
    }
    return no;
}

/*=============TESTE============*/

void print_em_ord(NO* no){
  
  if (no == NULL) return;
  
  if (no->esq != NULL) print_em_ord(no->esq);
  printf(" %s - %d\n", no->nome, no->frequencia);
  if (no->dir != NULL) print_em_ord(no->dir);
  return;

}

void avl_print_em_ordem(ARVORE* ar){
  
  print_em_ord(ar->raiz);
  return;
  
}