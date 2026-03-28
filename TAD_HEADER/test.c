#include "header.h"

int main(){
    FILE *f;

    HEADER *h = header_create();

    int op; scanf("%d", &op);

    if(op == 1){
        f = fopen("arq.bin", "wb");
        header_set_all(h, '0', 2, 3, 10, 25);
        header_save_all(h, f);
    }
    else{
        f = fopen("arq.bin", "rb");
        header_load_all(h, f);
        header_print(h);
    }

    fclose(f);

    header_delete(&h);

    return 0;
}