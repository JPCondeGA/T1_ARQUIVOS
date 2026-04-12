// CUIDADO: PARA O TESTE FUNCIONAR, DEVE EXISTIR UM ARQUIVO CHAMADO arq.bin NA MESMA PASTA DO EXECUTÁVEL (CRIE-O SE NECESSÁRIO)

#include "header.h"

int main(){
    int op;
    HEADER *h = header_criar();
    FILE *f = fopen("arq.bin", "rb+");

    printf(" [1] LER\n [2] ESCREVER\nSelecione: ");
    scanf("%d", &op);

    if(f != NULL){
        if(op == 1){
            header_carregar(h, f);
            //header_carregar_campo(h, STATUS, f);
            //header_carregar_campo(h, TOPO, f);
            //header_carregar_campo(h, PROXRRN, f);
            //header_carregar_campo(h, NMR_ESTACOES, f);
            //header_carregar_campo(h, NMR_PARES_ESTACAO, f);
            printf("status: %c\n", header_get_status(h));
            printf("topo: %d\n", header_get_topo(h));
            printf("proxRRN: %d\n", header_get_proxRRN(h));
            printf("nmr de estações: %d\n", header_get_nmr_estacoes(h));
            printf("nmr de pares: %d\n", header_get_nmr_pares_estacao(h));
        }
        else if(op == 2){
            int aux;
            printf("status: "); scanf(" %c", &aux); header_set_status(h, aux);
            printf("topo: "); scanf("%d", &aux); header_set_topo(h, aux);
            printf("prox RRN: "); scanf("%d", &aux); header_set_proxRRN(h, aux);
            printf("nmr de estações: "); scanf("%d", &aux); header_set_nmr_estacoes(h, aux);
            printf("nmr de pares: "); scanf("%d", &aux); header_set_nmr_pares_estacao(h, aux);
            header_salvar(h, f);
            //header_salvar_campo(h, STATUS, f);
            //header_salvar_campo(h, TOPO, f);
            //header_salvar_campo(h, PROXRRN, f);
            //header_salvar_campo(h, NMR_ESTACOES, f);
            //header_salvar_campo(h, NMR_PARES_ESTACAO, f);
        }
    }

    header_apagar(&h);
    if(h == NULL) printf("ok\n");
    
    return 0;
}