#ifndef HEADER_H
    #define HEADER_H

    #include "../TAD_AVL/avl.h"

    #define STATUS 0 
    #define TOP 1
    #define PROX 2
    #define EST 3
    #define PARES 4    

    typedef struct header_h HEADER;


    HEADER *header_create();
    bool header_delete(HEADER **h);


    bool header_load_all(HEADER *h, FILE *f);
    bool header_load_field(HEADER *h, int8 op, FILE *f);

    bool header_save_all(HEADER *h, FILE *f);
    bool header_save_field(HEADER *h, int8 op, FILE *f);


    char header_get_status(HEADER *h);
    uint header_get_topo(HEADER *h);
    uint header_get_proxRNN(HEADER *h);
    uint header_get_nmr_estacoes(HEADER *h);
    uint header_get_nmr_pares(HEADER *h);

    bool heade_set_status(HEADER *h, char status);
    bool header_set_topo(HEADER *h, uint topo);
    bool header_set_proxRNN(HEADER *h, uint proxRNN);
    bool header_set_nmr_estacoes(HEADER *h, uint nmr_estacoes);
    bool header_set_nmr_pares(HEADER *h, uint nmr_pares);


    void header_print(HEADER *h);
    void header_set_all(HEADER *h, char status, uint topo, uint proxRRN, uint nmr_estacoes, uint nmr_pares);

#endif