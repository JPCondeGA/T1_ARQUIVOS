#include "header.h"

struct header_h{
    char status;
    uint topo;
    uint proxRRN;
    uint nmr_estacoes;
    uint nmr_pares;
};

/*=============ALOCAÇÃO E DESALOCAÇÃO============*/
HEADER *header_create(){
    HEADER *h = (HEADER*)malloc(sizeof(HEADER));
    
    if(h != NULL){
        h->status = '1';
        h->topo = -1;
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

    ull origem = ftell(f);
    fseek(f, 0, SEEK_SET);
    fread(h, sizeof(HEADER), 1, f);

    fseek(f, origem, SEEK_SET);

    return true;
}

bool header_load_field(HEADER *h, int8 op, FILE *f){
    if(h == NULL || f == NULL) return false;
    
    ull origem = ftell(f);

    if(op == STATUS){
        fseek(f, 0, SEEK_SET);
        fread(&(h->status), sizeof(char), 1, f);
    }
    else{
        fseek(f, 1 + sizeof(uint)*(op-1), SEEK_SET);
        if(op == TOP){
            fread(&(h->topo), sizeof(uint), 1, f);
        }
        else if(op == TOP){
            fread(&(h->topo), sizeof(uint), 1, f);
        }
        else if(op == PROX){
            fread(&(h->proxRRN), sizeof(uint), 1, f);
        }
        else if(op == EST){
            fread(&(h->nmr_estacoes), sizeof(uint), 1, f);
        }
        else if(op == PARES){
            fread(&(h->nmr_pares), sizeof(uint), 1, f);
        } 
    }

    fseek(f, origem, SEEK_SET);

    return true;
}

bool header_save_all(HEADER *h, FILE *f){
    if(h == NULL || f == NULL) return false;

    ull origem = ftell(f); // Guardando o ponteiro de onde estamos
    fseek(f, 0, SEEK_SET); // Indo para início do arquivo
    fwrite(h, sizeof(HEADER), 1, f); // Escrevendo

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
        fseek(f, 1 + sizeof(uint)*(op-1), SEEK_SET);
        if(op == TOP){
            fwrite(&(h->topo), sizeof(uint), 1, f);
        }
        else if(op == TOP){
            fwrite(&(h->topo), sizeof(uint), 1, f);
        }
        else if(op == PROX){
            fwrite(&(h->proxRRN), sizeof(uint), 1, f);
        }
        else if(op == EST){
            fwrite(&(h->nmr_estacoes), sizeof(uint), 1, f);
        }
        else if(op == PARES){
            fwrite(&(h->nmr_pares), sizeof(uint), 1, f);
        } 
    }

    fseek(f, origem, SEEK_SET);

    return true;
}

/*===========GETTERS E SETTERS==========*/

char get_status(HEADER *h){
    if(h != NULL) return h->status;

    return '0';
}

uint get_topo(HEADER *h){
    if(h != NULL) return h->topo;

    return -1;
}

uint get_proxRNN(HEADER *h){
    if(h != NULL) return h->proxRRN;

    return -1;
}

uint get_nmr_estacoes(HEADER *h){
    if(h != NULL) return h->nmr_estacoes;

    return -1;
}

uint get_nmr_pares(HEADER *h){
    if(h != NULL) return h->nmr_pares;

    return -1;
}

bool set_status(HEADER *h, char status){
    if(h != NULL){
        h->status = status;
        return true;
    }

    return false;
}

bool set_topo(HEADER *h, uint topo){
    if(h != NULL){
        h->topo = topo;
        return true;
    }

    return false;
}

bool set_proxRNN(HEADER *h, uint proxRNN){
    if(h != NULL){
        h->proxRRN = proxRNN;
        return true;
    }

    return false;
}

bool set_nmr_estacoes(HEADER *h, uint nmr_estacoes){
    if(h != NULL){
        h->nmr_estacoes = nmr_estacoes;
        return true;
    }

    return false;
}

bool set_nmr_pares(HEADER *h, uint nmr_pares){
    if(h != NULL){
        h->nmr_pares = nmr_pares;
        return true;
    }

    return false;
}


/*===========PRINT==========*/

void header_print(HEADER *h){
    if(h == NULL) return;

    printf("STATUS: %c\n", h->status);
    printf("TOPO: %d\n", h->topo);
    printf("PROX RRN: %d\n", h->proxRRN);
    printf("NÚMERO DE ESTAÇÕS: %d\n", h->nmr_estacoes);
    printf("NÚMERO DE PARES: %d\n\n", h->nmr_pares);
}

void header_set_all(HEADER *h, char status, uint topo, uint proxRRN, uint nmr_estacoes, uint nmr_pares){
    if(h == NULL) return;

    h->status = status;
    h->topo = topo;
    h->proxRRN = proxRRN;
    h->nmr_estacoes = nmr_estacoes;
    h->nmr_pares = nmr_pares;
}

