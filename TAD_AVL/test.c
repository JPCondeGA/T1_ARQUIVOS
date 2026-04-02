#include "avl.h"

int main(){
    int op1, op2, cod_estacao, cod_prox_estacao;
    char resp; char nome[31];
    ARVORE *ar;

    printf("TIPO DA ARVORE\n [1] PARES\n [2] NOMES\nSELECIONE: "); scanf("%d", &op1);
    ar = avl_criar_arvore(op1 == 1);
   
    do{
        printf("Opções:\n [1] INSERIR\n [2] REMOVER\nSelecione: "); scanf("%d", &op2);
        
        if(!avl_get_op(ar)){
            printf("NOME: "); 
            scanf(" %30[^\n]", nome);    
        }
        else{
            printf("COD ESTACAO: "); scanf("%d", &cod_estacao);
            printf("COD PROX ESTACAO: "); scanf("%d", &cod_prox_estacao);
        }    
        if(op2 == 1){
            avl_inserir(ar, nome, cod_estacao, cod_prox_estacao);
        }
        else if(op2 == 2){
            avl_remover(ar, nome, cod_estacao, cod_prox_estacao);
        }

        printf("\nÁRVORE ATUAL (QTD: %d)\n", avl_get_n(ar));
        avl_print_em_ordem(ar);

        printf("CONTINUAR [S/N]? "); scanf(" %c", &resp);
        printf("\n");
    }while(resp != 'N');

    avl_apagar_arvore(&ar);
    return 0;
}