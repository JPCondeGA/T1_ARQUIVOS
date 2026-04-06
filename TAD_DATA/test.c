// CUIDADO: PARA O TESTE FUNCIONAR, DEVE EXISTIR UM ARQUIVO CHAMADO arq.bin NA MESMA PASTA DO EXECUTÁVEL (CRIE-O SE NECESSÁRIO)

#include "data.h"


int main(){
    int op, RRN;
    DATA *d = data_create();
    FILE *f = fopen("arq.bin", "rb+");

    printf(" [1] LER\n [2] ESCREVER\nSelecione: ");
    scanf("%d", &op);
    printf("RRN: ");
    scanf("%d", &RRN);

    if(op == 1){
        char *nome;
        data_load_all(d, RRN, f);
        //data_load_field(d, RRN, REMOVIDO, f);
        //data_load_field(d, RRN, PROX, f);
        //data_load_field(d, RRN, COD_EST_INT, f);
        //data_load_field(d, RRN, NOME_EST, f);
        //data_load_field(d, RRN, NOME_LIN, f);
        printf("REMOVIDO: %c\n", data_get_removido(d));
        printf("PROX: %d\n", data_get_prox(d));
        printf("COD EST: %d\n", data_get_cod_est(d));
        printf("COD LIN: %d\n", data_get_cod_lin(d));
        printf("COD PROX: %d\n", data_get_cod_prox(d));
        printf("DIST: %d\n", data_get_dist(d));
        printf("COD LIN INT: %d\n", data_get_cod_lin_int(d));
        printf("COD EST INT: %d\n", data_get_cod_est_int(d));
        printf("TAM NOME EST: %u\n", data_get_tam_nome_est(d));
        printf("NOME EST: %s\n", nome = data_get_nome_est(d));
        free(nome); nome = NULL;
        printf("TAM NOME LIN: %u\n", data_get_tam_nome_lin(d));
        printf("NOME LIN: %s\n", nome = data_get_nome_lin(d));
        free(nome); nome = NULL;

    }
    else if(op == 2){
        int aux;
        char aux_C;
        char nome[31];
        printf("REMOVIDO: "); scanf(" %c", &aux_C); data_set_removido(d, aux_C);
        printf("PROX: "); scanf("%d", &aux); data_set_prox(d, aux);
        printf("COD EST: "); scanf("%d", &aux); data_set_cod_est(d, aux);
        printf("COD LIN: "); scanf("%d", &aux); data_set_cod_lin(d, aux);
        printf("COD PROX: "); scanf("%d", &aux); data_set_cod_prox(d, aux);
        printf("DIST: "); scanf("%d", &aux); data_set_dist(d, aux);
        printf("COD LIN INT: "); scanf("%d", &aux); data_set_cod_lin_int(d, aux);
        printf("COD EST INT: "); scanf("%d", &aux); data_set_cod_est_int(d, aux);
        printf("NOME EST: "); scanf(" %30[^\n]", nome); data_set_nome_est(d, nome);
        printf("NOME LIN: "); scanf(" %30[^\n]", nome); data_set_nome_lin(d, nome);
        //data_save_all(d, RRN, f);
        //data_save_field(d, RRN, REMOVIDO, f);
        //data_save_field(d, RRN, PROX, f);
        //data_save_field(d, RRN, DIST, f);
        //data_save_field(d, RRN, NOME_EST, f);
        data_save_field(d, RRN, NOME_LIN, f);
    }

    data_delete(&d);
    return 0;
}
