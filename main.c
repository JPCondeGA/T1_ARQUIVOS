/*
NOME: JOÃO PEDRO CONDE GOMES ALVES; N° USP: 16816271
NOME: EDUARDO BENEDINI BUENO; N° USP: 16862551 
*/

#include "FUNC/func.h"

int main(){
    int op; scanf("%d", &op);

    switch(op){
        case 1:
            func_create_interface();
            break;
        case 2:
            func_select_where_interface();
            break;
        case 3:
            
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        default:
            printf("Opção inválida.\n");
    }
    
    return 0;
}
