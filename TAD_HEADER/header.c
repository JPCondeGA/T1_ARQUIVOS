#include "header.h"

// Estrutura que representa o registro de cabeçalho
struct header_{
    char status; // Consistência do arquivo ('0' -> inconsistente; '1' -> consistente) 
    int topo;  // Byte offset do topo da pilha dos registros logicamente removidos (-1 no caso de pilha vazia)
    int proxRRN; // Próximo RRN disponível para inserção de algum registro
    int nmr_estacoes; // Número de estações diferentes no arquivo (diferencia-se pelo nome)
    int nmr_pares_estacao; // Número de pares (cod_estação, cod_prox_estação) diferentes no arquivo 
}; 

// 20 bytes de tamanho -> padding entre o campo status e topo

/*=============ALOCAÇÃO E DESALOCAÇÃO============*/

HEADER *header_criar(){
    HEADER *h = (HEADER*)malloc(sizeof(HEADER)); // Alocando memória
    
    // Inicializando valores, se a alocação ocorreu corretamente
    if(h != NULL){
        h->status = '1'; // O arquivo incialmente está consistente
        h->topo = -1; // A pilha está vazia
        h->proxRRN = h->nmr_estacoes = h->nmr_pares_estacao = 0; 
    }

    return h;
}

bool header_apagar(HEADER **h){
    if(h == NULL || *h == NULL) return false; // Verificando se o ponteiro é válido

    // Desalocando e setando NULL
    free(*h);
    *h = NULL;
    
    return true;
}

/*================LEITURA===============*/

bool header_carregar(HEADER *h, bool move, FILE *f){
    if(h == NULL || f == NULL) return false; // Verificando se os ponteiros são válidos

    if(move) fseek(f, 0, SEEK_SET); // Movendo para o arquivo de cabeçalho se já não estivermos nele

    // Carregando os campos separadamente para evitar problemas com o padding da estrutura 
    fread(&(h->status), sizeof(h->status), 1, f); // Somente o status
    fread(&(h->topo), sizeof(int)*4, 1, f); // O restante

    if(feof(f)) return false;

    // O cursor está no primeiro byte do primeiro registro de dados

    return true;
}

bool header_carregar_campo(HEADER *h, int8 op, bool move, FILE *f){
    if(h == NULL || f == NULL) return false; // Verificando se os ponteiros são válidos
    
    uint bytes; // Quantos bytes devem ser pulados para chegar no primeiro byte do próximo registro de dados

    // Verificando se o campo é o primeiro
    if(op == STATUS){ 
        if(move) fseek(f, 0, SEEK_SET); // Movendo ponteiro para o registro de cabeçalho se já não estiver lá
        fread(&(h->status), sizeof(h->status), 1, f); // Carregando um byte (status)
        bytes = TAM_HEADER - sizeof(h->status); 
    }
    // Se não for o primeiro campo, podemos usar uma fórmula para chegar no campo
    else{ 
        fseek(f, sizeof(h->status) + sizeof(int)*(op-1), SEEK_SET); // Movendo cursor para o campo correto (nesse caso, usar SEEK_SET e SEEK_CUR dá na mesma)
        bytes = TAM_HEADER - (sizeof(h->status) + sizeof(int)*(op-1));
        // Carregando campo correto
        if(op == TOPO){
            fread(&(h->topo), sizeof(h->topo), 1, f);
        }
        else if(op == PROXRRN){
            fread(&(h->proxRRN), sizeof(h->proxRRN), 1, f);
        }
        else if(op == NMR_ESTACOES){
            fread(&(h->nmr_estacoes), sizeof(h->nmr_estacoes), 1, f);
        }
        else if(op == NMR_PARES_ESTACAO){
            fread(&(h->nmr_pares_estacao), sizeof(h->nmr_pares_estacao), 1, f);
        } 
        else return false; // Se não for um campo válido, retornamos false
    }

    if(feof(f)) return false; // Se alguma leitura deu errado

    if(bytes > 0) fseek(f, bytes, SEEK_CUR); // Movendo cursor para o primeiro registro de dados 

    return true;
}


/*================GRAVAÇÃO===============*/

bool header_salvar(HEADER *h, bool move, FILE *f){
    if(h == NULL || f == NULL) return false; // Verificando se os ponteiro são inválidos

    if(move) fseek(f, 0, SEEK_SET); // Indo para início do arquivo, se necessário

    // Escrevendo campos (campo a campo como especificado)
    fwrite(&(h->status), sizeof(h->status), 1, f); 
    fwrite(&(h->topo), sizeof(h->topo), 1, f); 
    fwrite(&(h->proxRRN), sizeof(h->proxRRN), 1, f); 
    fwrite(&(h->nmr_estacoes), sizeof(h->nmr_estacoes), 1, f); 
    fwrite(&(h->nmr_pares_estacao), sizeof(h->nmr_pares_estacao), 1, f); 

    // Não é necessário mover o cursor

    return true;
}

bool header_salvar_campo(HEADER *h, int8 op, bool move, FILE *f){
    if(h == NULL || f == NULL) return false; // Verificando se os ponteiros são inválidos

    uint bytes; // Quantos bytes devem ser pulados para chegar no primeiro byte do próximo registro de dados

    // Verificando se é o primeiro campo
    if(op == STATUS){
        if(move) fseek(f, 0, SEEK_SET);
        fwrite(&(h->status), sizeof(h->status), 1, f); // Escrevendo status na memória
        
        bytes = TAM_HEADER - sizeof(h->status);
    }
    // Se não for o primeiro campo, podemos usar uma fórmula para chegar no campo
    else{
        fseek(f, sizeof(h->status) + sizeof(int)*(op-1), SEEK_SET); // Movendo cursor para o campo correto (nesse caso, usar SEEK_SET e SEEK_CUR dá na mesma)

        bytes = TAM_HEADER - (sizeof(h->status) + sizeof(int)*(op-1));
        // Escrevendo campo desejado
        if(op == TOPO){
            fwrite(&(h->topo), sizeof(h->topo), 1, f);
        }
        else if(op == PROXRRN){
            fwrite(&(h->proxRRN), sizeof(h->proxRRN), 1, f);
        }
        else if(op == NMR_ESTACOES){
            fwrite(&(h->nmr_estacoes), sizeof(h->nmr_estacoes), 1, f);
        }
        else if(op == NMR_PARES_ESTACAO){
            fwrite(&(h->nmr_pares_estacao), sizeof(h->nmr_pares_estacao), 1, f);
        } 
        else return false; // Se o campo for inválido
    }

    if(bytes > 0) fseek(f, bytes, SEEK_CUR);

    return true;
}

/*===============GETTERS===============*/

char header_get_status(HEADER *h){
    if(h != NULL) return h->status; // Se o ponteiro não for inválido, retornarmos o campo

    return '0'; // Retornando inconsistente, já que o ponteiro está falho
}

int header_get_topo(HEADER *h){
    if(h != NULL) return h->topo; // Se o ponteiro não for inválido, retornarmos o campo

    return -1; // Retornando pilha vazia
}

int header_get_proxRRN(HEADER *h){
    if(h != NULL) return h->proxRRN; // Se o ponteiro não for inválido, retornarmos o campo

    return -1; // Retornando valor impossível para sinalizar que o ponteiro está falho 
}

int header_get_nmr_estacoes(HEADER *h){
    if(h != NULL) return h->nmr_estacoes; // Se o ponteiro não for inválido, retornarmos o campo

    return -1; // Retornando valor impossível para sinalizar que o ponteiro está falho 
}

int header_get_nmr_pares_estacao(HEADER *h){
    if(h != NULL) return h->nmr_pares_estacao; // Se o ponteiro não for inválido, retornarmos o campo

    return -1; // Retornando valor impossível para sinalizar que o ponteiro está falho 
}

/*===============SETTERS===============*/

bool header_set_status(HEADER *h, char status){
    // Se o ponteiro é válido e o valor do campo possível, atribuimos
    if(h != NULL && (status == '0' || status == '1')){ 
        h->status = status;
        return true;
    }

    return false;
}

bool header_set_topo(HEADER *h, int topo){
    // Se o ponteiro é válido e o valor do campo possível (topo pode receber -1 em caso de pilha vazia), atribuimos
    if(h != NULL && topo >= -1){
        h->topo = topo;
        return true;
    }

    return false;
}

bool header_set_proxRRN(HEADER *h, int proxRRN){
    // Se o ponteiro é válido e o valor do campo possível, atribuimos
    if(h != NULL && proxRRN >= 0){
        h->proxRRN = proxRRN;
        return true;
    }

    return false;
}

bool header_set_nmr_estacoes(HEADER *h, int nmr_estacoes){
    // Se o ponteiro é válido e o valor do campo possível, atribuimos
    if(h != NULL && nmr_estacoes >= 0){
        h->nmr_estacoes = nmr_estacoes;
        return true;
    }

    return false;
}

bool header_set_nmr_pares_estacao(HEADER *h, int nmr_pares_estacao){
    // Se o ponteiro é válido e o valor do campo possível, atribuimos
    if(h != NULL && nmr_pares_estacao >= 0){
        h->nmr_pares_estacao = nmr_pares_estacao;
        return true;
    }

    return false;
}