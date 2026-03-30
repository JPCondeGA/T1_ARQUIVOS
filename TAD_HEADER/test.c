// CUIDADO: PARA O TESTE FUNCIONAR, DEVE EXISTIR UM ARQUIVO CHAMADO arq.bin NA MESMA PASTA DO EXECUTÁVEL (CRIE-O SE NECESSÁRIO)

#include "header.h"

int main(){
    int op;
    HEADER *h = header_create();
    FILE *f = fopen("arq.bin", "rb+");

    printf(" [1] LER\n [2] ESCREVER\nSelecione: ");
    scanf("%d", &op);

    if(f != NULL){
        if(op == 1){
            header_load_all(h, f);
            //header_load_field(h, STATUS, f);
            //header_load_field(h, TOP, f);
            //header_load_field(h, PROXRRN, f);
            //header_load_field(h, EST, f);
            //header_load_field(h, PARES, f);
            printf("status: %c\n", header_get_status(h));
            printf("topo: %d\n", header_get_topo(h));
            printf("proxRRN: %d\n", header_get_proxRRN(h));
            printf("nmr de estações: %d\n", header_get_nmr_estacoes(h));
            printf("nmr de pares: %d\n", header_get_nmr_pares(h));
        }
        else if(op == 2){
            int aux;
            printf("status: "); scanf(" %c", &aux); header_set_status(h, aux);
            printf("topo: "); scanf("%d", &aux); header_set_topo(h, aux);
            printf("prox RRN: "); scanf("%d", &aux); header_set_proxRRN(h, aux);
            printf("nmr de estações: "); scanf("%d", &aux); header_set_nmr_estacoes(h, aux);
            printf("nmr de pares: "); scanf("%d", &aux); header_set_nmr_pares(h, aux);
            header_save_all(h, f);
            //header_save_field(h, STATUS, f);
            //header_save_field(h, TOP, f);
            //header_save_field(h, PROXRRN, f);
            //header_save_field(h, EST, f);
            //header_save_field(h, PARES, f);
        }
    }

    printf("\n");
    header_print(h);

    header_delete(&h);
    if(h == NULL) printf("ok\n");
    
    return 0;
}