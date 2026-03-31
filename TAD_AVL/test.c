#include "avl.h"

int main(void){
 
 ARVORE* ar = criar_arvore();
 inserir(ar, 9, "tucuruvi");
 inserir(ar, 15, "parada inglesa");
 inserir(ar, 17, "jardim sao paulo");
 inserir(ar, 8, "santana");
 inserir(ar, 10, "carandiru");
 inserir(ar, 4, "luz");
 
 print_em_ordem(ar);
 printf("\n");
 remover(ar, "jardim sao paulo");
 remover(ar, "tucuruvi");
 remover(ar, "santana");
 
 print_em_ordem(ar);
 apagar_arvore(&ar);
 
 return 0;
 
}
