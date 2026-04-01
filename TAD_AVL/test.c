#include "avl.h"

int main(){
    ARVORE *ar = avl_criar_arvore();
    char resp; int op; char nome[31];
   
    do{
        printf("Opções:\n [1] INSERIR\n [2] REMOVER\nSelecione: "); scanf("%d", &op);
        printf("NOME: "); scanf(" %30[^\n]", nome);        
        if(op == 1){
            avl_inserir(ar, nome);
        }
        else if(op == 2){
            avl_remover(ar, nome);
        }

        printf("\nÁRVORE ATUAL (QTD: %d)\n", avl_get_n(ar));
        avl_print_em_ordem(ar);

        printf("CONTINUAR [S/N]? "); scanf(" %c", &resp);
        printf("\n");
    }while(resp != 'N');

    avl_apagar_arvore(&ar);
    return 0;
}