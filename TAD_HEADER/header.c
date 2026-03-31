#include "header.h"

struct header_{
    char status; // Consistência do arquivo
    // -> padding de 3 bytes
    int topo; // Byte offset do topo da pilha dos registros logicamente removidos (-1 no caso de pilha vazia)
    int proxRRN; // Próximo RRN disponível para inserção de algum registro
    int nmr_estacoes; // Número de estações diferentes no arquivo (diferencia-se pelo nome)
    int nmr_pares; // Número de pares (cod_estação, cod_prox_estação) diferentes no arquivo -- Não direcionado?????
}; //Tamanho na memória primária é de 20 bytes

/*=============ALOCAÇÃO E DESALOCAÇÃO============*/

HEADER *header_create(){
    HEADER *h = (HEADER*)malloc(sizeof(HEADER));
    
    if(h != NULL){
        h->status = '1'; // O arquivo incialmente está consistente
        h->topo = -1; // A pilha está vazia
        h->proxRRN = h->nmr_estacoes = h->nmr_pares = 0; 
    }

    return h;
}

bool header_delete(HEADER **h){
    if(h == NULL || *h == NULL) return false; 

    free(*h);
    *h = NULL;
    
    return true;
}

/*===========CARREGAMENTO E SALVAMENTO==========*/

bool header_load_all(HEADER *h, FILE *f){
    if(h == NULL || f == NULL) return false;

    ull origem = ftell(f); // Guardando posição original do cursor
    fseek(f, 0, SEEK_SET); // Movendo para o arquivo de cabeçalho
    
    fread(&(h->status), sizeof(h->status), 1, f);
    fread(&(h->topo), sizeof(HEADER) - 4, 1, f);

    fseek(f, origem, SEEK_SET); // Voltando para posição original

    return true;
}

bool header_load_field(HEADER *h, int8 op, FILE *f){
    if(h == NULL || f == NULL) return false;
    
    ull origem = ftell(f);

    if(op == STATUS){ // Status é o primeiro campo do arquivo
        fseek(f, 0, SEEK_SET);
        fread(&(h->status), sizeof(h->status), 1, f);
    }
    else{ // Status ocupa 1 byte e cada um dos restantes 4 byter
        fseek(f, sizeof(h->status) + sizeof(int)*(op-1), SEEK_SET); // Movendo cursor para o campo correto
        if(op == TOP){
            fread(&(h->topo), sizeof(h->topo), 1, f);
        }
        else if(op == PROXRRN){
            fread(&(h->proxRRN), sizeof(h->proxRRN), 1, f);
        }
        else if(op == EST){
            fread(&(h->nmr_estacoes), sizeof(h->nmr_estacoes), 1, f);
        }
        else if(op == PARES){
            fread(&(h->nmr_pares), sizeof(h->nmr_pares), 1, f);
        } 
        else return false;
    }

    fseek(f, origem, SEEK_SET);

    return true;
}

bool header_save_all(HEADER *h, FILE *f){
    if(h == NULL || f == NULL) return false;

    ull origem = ftell(f); // Guardando o ponteiro de onde estamos
    fseek(f, 0, SEEK_SET); // Indo para início do arquivo
    
    fwrite(h, sizeof(h->status), 1, f); // Escrevendo o status
    fwrite(&(h->topo), sizeof(HEADER) - 4, 1, f); // Excrevendo o resto (para evitar o padding)

    fseek(f, origem, SEEK_SET); // Voltando o ponteiro para onde estavamos

    return true;
}

bool header_save_field(HEADER *h, int8 op, FILE *f){
    if(h == NULL || f == NULL) return false;

    ull origem = ftell(f);

    if(op == STATUS){
        fseek(f, 0, SEEK_SET);
        fwrite(&(h->status), sizeof(char), 1, f);
    }
    else{
        fseek(f, sizeof(h->status) + sizeof(int)*(op-1), SEEK_SET);
        if(op == TOP){
            fwrite(&(h->topo), sizeof(h->topo), 1, f);
        }
        else if(op == PROXRRN){
            fwrite(&(h->proxRRN), sizeof(h->proxRRN), 1, f);
        }
        else if(op == EST){
            fwrite(&(h->nmr_estacoes), sizeof(h->nmr_estacoes), 1, f);
        }
        else if(op == PARES){
            fwrite(&(h->nmr_pares), sizeof(h->nmr_pares), 1, f);
        } 
        else return false;
    }

    fseek(f, origem, SEEK_SET);

    return true;
}

/*===========GETTERS E SETTERS==========*/

char header_get_status(HEADER *h){
    if(h != NULL) return h->status;

    return '0';
}

int header_get_topo(HEADER *h){
    if(h != NULL) return h->topo;

    return -1;
}

int header_get_proxRRN(HEADER *h){
    if(h != NULL) return h->proxRRN;

    return -1;
}

int header_get_nmr_estacoes(HEADER *h){
    if(h != NULL) return h->nmr_estacoes;

    return -1;
}

int header_get_nmr_pares(HEADER *h){
    if(h != NULL) return h->nmr_pares;

    return -1;
}

bool header_set_status(HEADER *h, char status){
    if(h != NULL && (status == '0' || status == '1')){
        h->status = status;
        return true;
    }

    return false;
}

bool header_set_topo(HEADER *h, int topo){
    if(h != NULL && topo >= -1){
        h->topo = topo;
        return true;
    }

    return false;
}

bool header_set_proxRRN(HEADER *h, int proxRRN){
    if(h != NULL && proxRRN >= 0){
        h->proxRRN = proxRRN;
        return true;
    }

    return false;
}

bool header_set_nmr_estacoes(HEADER *h, int nmr_estacoes){
    if(h != NULL && nmr_estacoes >= 0){
        h->nmr_estacoes = nmr_estacoes;
        return true;
    }

    return false;
}

bool header_set_nmr_pares(HEADER *h, int nmr_pares){
    if(h != NULL && nmr_pares >= 0){
        h->nmr_pares = nmr_pares;
        return true;
    }

    return false;
}


/*===========TESTE==========*/

void header_print(HEADER *h){
    if(h == NULL) return;

    printf("STATUS: %c\n", h->status);
    printf("TOPO: %d\n", h->topo);
    printf("PROX RRN: %d\n", h->proxRRN);
    printf("NÚMERO DE ESTAÇÕS: %d\n", h->nmr_estacoes);
    printf("NÚMERO DE PARES: %d\n\n", h->nmr_pares);
}

void header_set_all(HEADER *h, char status, int topo, int proxRRN, int nmr_estacoes, int nmr_pares){
    if(h == NULL) return;

    h->status = status;
    h->topo = topo;
    h->proxRRN = proxRRN;
    h->nmr_estacoes = nmr_estacoes;
    h->nmr_pares = nmr_pares;
}

