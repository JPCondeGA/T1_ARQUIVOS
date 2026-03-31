#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "avl.h"
#define maior(a, b) ((a > b) ? a : b)

typedef struct no_ NO;
typedef struct arvore_ ARVORE;


struct no_{

  uint frequencia; //Campo usado para guardar quantas vezes a chave aparece no arquivo.
  int altura;
  NO* dir;
  NO* esq;
  int8* nome;
  
};

struct arvore_{
  NO* raiz;
};

NO* criar_no(int tamanho, char* nome){ //A variável "tamanho" é o valor do tamanho do campo de tamanho variável.
  
  NO* no = (NO*)malloc(sizeof(NO));
  if (no == NULL) return NULL;
  
  no->nome = (int8*)malloc(sizeof(int8) * tamanho); //Não é necessário alocar um bit a mais para o caractere "\0" pois o arquivo já o considera ao marcar o tamanho do campo.
  if (no->nome == NULL){
    free(no);
    return NULL;
  }
  strcpy(no->nome, nome);
  
  no->dir = NULL;
  no->esq = NULL;
  no->frequencia = 1; //Inicia em 1, pois um nó com frequência 0 é deletado.
  no->altura = 0;
  
  return no;
}

bool apagar_no(NO** no){

  if (no == NULL || *no == NULL) return false;
  
  free((*no)->nome);
  (*no)->nome = NULL;
  
  free(*no);
  *no = NULL;
  
  return true;
  
}

ARVORE* criar_arvore(){
  
  ARVORE* ar = (ARVORE*)malloc(sizeof(ARVORE));
  if (ar == NULL) return NULL;
  
  ar->raiz = NULL;
  
  return ar;
}

int altura_no(NO* no){
  if (no == NULL) return 0; //Função auxiliar para que, quando conferir a altura de um dos filhos de um nó, não precisar conferir constantemente se aquele filho existe.
  return no->altura;
}

NO* rodar_e(NO* no){
  NO* aux = no->dir;
  no->dir = aux->esq;
  aux->esq = no;
  
  no->altura = maior(altura_no(no->esq), altura_no(no->dir)) + 1; //Atualizando alturas.
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

int fb(NO* no){ //Função auxiliar para calcular o fator de balanceamento de um dado nó.
  return (altura_no(no->esq) - altura_no(no->dir));
}

NO* inserir_no(NO* cima, NO* baixo){

/*Função recursiva inicialmente chamada por "inserir()".
Baixo é o nó a ser inserido. Cima é o nó "atual" que está sendo comparado com baixo.
Não é necessário conferir se "baixo" é NULL pois "inserir()" faz a verificação antes de iniciar esta função.*/
  
  if (cima == NULL){
    cima = baixo;
    baixo->altura++;
    return cima;
  }
  
  if (strcmp(baixo->nome, cima->nome) == 0){
    //Se cima e baixo possuem a mesma chave, então são a mesma estação. O valor da frequência do nó existente é atualizado e o nó a ser inserido não é mais necessário.
    cima->frequencia++;
    apagar_no(&baixo);
    return cima; //Não é necessário fazer rotações neste caso, já que o número de nós na árvore se manteve o mesmo.
  }
  else if (strcmp(baixo->nome, cima->nome) < 0){ //Inserção regular de AVL.
    cima->esq = inserir_no(cima->esq, baixo);
  }
  else{
    cima->dir = inserir_no(cima->dir, baixo);
  }
  
  cima->altura = maior(altura_no(cima->esq), altura_no(cima->dir)) + 1;

  if (fb(cima) == -2){
    if (fb(cima->dir) <= 0) cima = rodar_e(cima); //Rotação esquerda simples
    else{
      cima->dir = rodar_d(cima->dir); //Rotação direita esquerda.
      cima = rodar_e(cima);
    }
  }
  
  if (fb(cima) == 2){
    if (fb(cima->esq) >= 0) cima = rodar_d(cima); //Rotação direita simples
    else{
      cima->esq = rodar_e(cima->esq); //Rotação esquerda direita.
      cima = rodar_d(cima);
    }
  }
  
  return cima;
}

bool inserir(ARVORE* ar, int tamanho, char* nome){
  //O usuário não tem acesso a nós, então inserção e remoção precisam de uma função auxiliar para iniciar a recursão.
  if (ar == NULL) return false;
  NO* no = criar_no(tamanho, nome);
  if (no == NULL) return false;
  
  ar->raiz = inserir_no(ar->raiz, no);
  return true;

}

NO* troca_com_maior_filho(NO* no, NO** aux){
    if (no == NULL)
        return NULL;
    
    if (no->dir == NULL) {
        //Quando o nó não possui mais filhos à direita, encontramos o maior filho.
        *aux = no; //Esta função é recursiva, então aux é um ponteiro de ponteiro para podermos acessar este ponteiro na volta das chamadas.
        NO* aux2 = no->esq; //Salva a subárvore esquerda, caso exista
        
        no->esq = NULL;
        no->dir = NULL;
        return aux2;
    }
    
    no->dir = troca_com_maior_filho(no->dir, aux); //Continua a procura pelo maior filho
    //A subárvore direita do nó anterior ao maior filho agora é a subárvore esquerda do maior filho
    
    
    //Ao retornar, atualiza alturas e realiza rotações necessárias.
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


NO* remover_no(NO* no, char* nome){
  
  if (no == NULL) return NULL;
  
  //Procurando nó
  if (strcmp(nome, no->nome) < 0) no->esq = remover_no(no->esq, nome);
  else if (strcmp(nome, no->nome) > 0) no->dir = remover_no(no->dir, nome);
  else{
    
    //Primeiro caso: O nó a ser removido foi inserido mais de uma vez (frequência maior que 1)
    if (no->frequencia > 1){ //Caso houver mais de uma estação com o mesmo nome, o nó não é removido, apenas se decrementa o contador de quantas vezes ela foi inserida.
      no->frequencia--;
      return no;
    }
    
    NO* aux = no; //Ponteiro auxiliar.
    
    //Segundo caso: O nó a ser removido tem 0 ou 1 filho(s)
    if (aux->esq == NULL || aux->dir == NULL){
      if (aux->esq == NULL) no = no->dir;
      else no = no->esq;
      
      apagar_no(&aux);
      return no;
    }
    else{ //Terceiro caso: O nó a ser removido tem 2 filhos
    
      aux = NULL;      
      no->esq = troca_com_maior_filho(no->esq, &aux); //O maior filho é removido da subárvore esquerda e salvo em aux.
            
      //Substitui no com aux
      aux->esq = no->esq;
      aux->dir = no->dir;
      aux->altura = no->altura;
            
      //Apaga o no substituído.
      apagar_no(&no);
            
      //Atualiza o ponteiro no para o nó em aux.
      no = aux;
      }
      
    }
    
    //Essa condição será verdadeira no caso da árvore ter apenas 1 nó.
    if (no == NULL) return NULL;
    
    //Atualiza altura
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

bool remover(ARVORE* ar, char* nome){
  //O usuário não tem acesso a nós, então inserção e remoção precisam de uma função auxiliar para iniciar a recursão.
  if (ar == NULL || ar->raiz == NULL) return false;
  
  if (remover_no(ar->raiz, nome) != NULL) return true;

  return false;
}

void apagar_subarvore(NO* no) {
    //Novamente, como o usuário não tem acesso a nós, uma função auxiliar é necessária para iniciar funções recursivas que requerem nós como entrada.
    if (no == NULL) return;
    apagar_subarvore(no->esq);
    apagar_subarvore(no->dir);
    apagar_no(&no);
    return;
}

bool apagar_arvore(ARVORE** ar) {
    if (ar == NULL || *ar == NULL) return false;
    
    apagar_subarvore((*ar)->raiz);
    
    free(*ar);
    *ar = NULL;
    
    return true;
}


//Funções usadas para testar se a árvore funciona corretamente
void print_em_ord(NO* no){
  
  if (no == NULL) return;
  
  if (no->esq != NULL) print_em_ord(no->esq);
  printf(" %s\n", no->nome);
  if (no->dir != NULL) print_em_ord(no->dir);
  return;
  
  
}

void print_em_ordem(ARVORE* ar){
  
  print_em_ord(ar->raiz);
  return;
  
  
}
