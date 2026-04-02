#include "data.h"

#define SEEK_INI 17 // Tamanho em bytes do registro de cabeçalho
#define TAM_FIXO 33 // Tamanho em bytes do registro de dados até primeiro campo de tamanho variável
#define LIXO '$'

struct data_{
    char removido;
    // -> padding de 3 bytes
    int prox;
    int cod_estacao;
    int cod_linha;
    int cod_prox_estacao;
    int dist_prox_estacao;
    int cod_linha_integra;
    int cod_est_integra;
    uint tam_nome_estacao;
    // -> padding de 4 bytes
    char* nome_estacao;
    uint tam_nome_linha;
    // -> padding de 4 bytes
    char* nome_linha;
}; // Tamanho em bytes na memória primária é de 64 bytes

/*=============FUNÇÕES OCULTAS============*/

/* Preenche um registro de dados com lixo (caractere '$') de um byte do registro até seu fim.
- Recebe o RRN do registro que está sendo preenchido, o byte de início do preenchimento e o arquivo em que está o registro de dados.

O parâmetro byte_registro deve ser um valor entre 0 e 79 (byte offset em relação ao início do registro de dados com aquele RRN). */
void data_fill_trash(uint RRN, uint byte_registro, FILE *f);

/*=============ALOCAÇÃO E DESALOCAÇÃO============*/

DATA *data_create(){
    DATA *d = (DATA*)malloc(sizeof(DATA));

    if(d != NULL){ // Inicialmente, consideramos que não está removido, mas todos campos são nulos
        d->removido = '0'; 
        d->prox = -1;
        d->cod_estacao = d->cod_linha = d->cod_prox_estacao = d->dist_prox_estacao = d->cod_linha_integra = d->cod_est_integra = -1;
        d->tam_nome_estacao = d->tam_nome_linha = 0;
        d->nome_estacao = NULL;
        d->nome_linha = NULL;
    }

    return d;
}


bool data_delete(DATA **d){
    if(d == NULL || *d == NULL) return false;

    if((*d)->nome_estacao != NULL) free((*d)->nome_estacao), (*d)->nome_estacao = NULL; // Desalocando nome da estação
    if((*d)->nome_linha != NULL) free((*d)->nome_linha), (*d)->nome_linha = NULL; // Desalocando nome da linha 

    free(*d);
    *d = NULL;

    return true;
}


/*===========CARREGAMENTO E SALVAMENTO==========*/

bool data_load_all(DATA *d, uint RRN, FILE *f){
    if(d == NULL || f == NULL) return false;

    ull off = RRN * 80 + SEEK_INI; // Calculando o byte offset do registro de dado a ser carregado
    fseek(f, off, SEEK_SET);
    
    // Lendo somente o removido (1 byte)
    fread(&(d->removido), sizeof(d->removido), 1, f);
    // Lendo até o primeiro campo de tamnanho variável (ler separado faz pular o padding da estrutura)
    fread(&(d->prox), TAM_FIXO - sizeof(d->removido), 1, f); 
    if(d->tam_nome_estacao > 0){
        if(d->nome_estacao != NULL) free(d->nome_estacao); // Se necessário, o nome anterior que estava na struct será sobrescrito
        
        d->nome_estacao = (char*)malloc(sizeof(char)*d->tam_nome_estacao);
        fread(d->nome_estacao, sizeof(char), d->tam_nome_estacao, f); // Gravando nome da estação no espaço alocada
    }

    fread(&(d->tam_nome_linha), 4, 1, f); // Lendo o tamanho do nome da linha
    if(d->tam_nome_linha > 0){
        if(d->nome_linha != NULL) free(d->nome_linha); // O nome anterior pode ser sobrescrito

        d->nome_linha = (char*)malloc(sizeof(char)*d->tam_nome_linha);
        fread(d->nome_linha, sizeof(char), d->tam_nome_linha, f); // Gravando nome da linha no espaço alocada
    }

    return true;
}


bool data_load_field(DATA *d, uint RRN, int8 op, FILE *f){
    if(d == NULL || f == NULL) return false;

    // Byte offset do primeiro byte do arquivo
    ull off_ini = RRN * 80 + SEEK_INI;

    switch(op){
        case REMOVIDO:
            fseek(f, off_ini, SEEK_SET);
            fread(d, 1, 1, f);
            break;

        case PROX:
            fseek(f, off_ini + 1 + sizeof(int)*(op-1), SEEK_SET);
            fread(&(d->prox), sizeof(d->prox), 1, f);
            break;

        case COD_EST:
            fseek(f, off_ini + 1 + sizeof(int)*(op-1), SEEK_SET);
            fread(&(d->cod_estacao), sizeof(d->cod_estacao), 1, f);
            break;

        case COD_LIN:
            fseek(f, off_ini + 1 + sizeof(int)*(op-1), SEEK_SET);
            fread(&(d->cod_linha), sizeof(d->cod_linha), 1, f);
            break;

        case COD_PROX:
            fseek(f,  off_ini + 1 + sizeof(int)*(op-1), SEEK_SET);
            fread(&(d->cod_prox_estacao), sizeof(d->cod_prox_estacao), 1, f);
            break;

        case DIST:
            fseek(f, off_ini + 1 + sizeof(int)*(op-1), SEEK_SET);
            fread(&(d->dist_prox_estacao), sizeof(d->dist_prox_estacao), 1, f);
            break;

        case COD_LIN_INT:
            fseek(f, off_ini + 1 + sizeof(int)*(op-1), SEEK_SET);
            fread(&(d->cod_linha_integra), sizeof(d->cod_linha_integra), 1, f);
            break;
        
        case COD_EST_INT:
            fseek(f, off_ini + 1 + sizeof(int)*(op-1), SEEK_SET);
            fread(&(d->cod_est_integra), sizeof(d->cod_est_integra), 1, f);
            break;

        case NOME_EST:
            // Lendo tamanho string
            fseek(f, off_ini + TAM_FIXO - sizeof(d->tam_nome_estacao), SEEK_SET);
            fread(&(d->tam_nome_estacao), sizeof(d->tam_nome_estacao), 1, f);
            // Alocando memória
            if(d->tam_nome_estacao > 0){
                // Sobrescrevendo se necessário
                if(d->nome_estacao == NULL) free(d->nome_estacao);

                d->nome_estacao = (char*)malloc(sizeof(char)*d->tam_nome_estacao);
                fread(d->nome_estacao, sizeof(char), d->tam_nome_estacao, f); // Gravando nome da linha no espaço alocada
            }

            break;
            
        case NOME_LIN:
            uint aux_U; // Guardará o tamanho do nome da estação (se necessário) para saltá-lo na leitura
            // Lendo tamanho do nome da estação (precisamos dessa informação para ir para a byte correto e ler o nome da linha)
            fseek(f, off_ini + TAM_FIXO - sizeof(d->tam_nome_estacao), SEEK_SET);
            fread(&aux_U, sizeof(d->tam_nome_estacao), 1, f);
            // Lendo tamanho string
            fseek(f, aux_U, SEEK_CUR); // Movendo o ponteiro para o campo tam_nome_linha (pulando o nome da estação - aux_U bytes)
            fread(&(d->tam_nome_linha), sizeof(d->tam_nome_linha), 1, f);
            // Alocando memória
            if(d->tam_nome_linha > 0){
                // Sobrescrevendo se necessário
                if(d->nome_linha == NULL) free(d->nome_linha);

                d->nome_linha= (char*)malloc(sizeof(char)*d->tam_nome_linha);
                fread(d->nome_linha, sizeof(char), d->tam_nome_linha, f); // Gravando nome da linha no espaço alocada
            }

            break;

        default:
            return false;
    }
        
    return true;
}

bool data_save_all(DATA *d, uint RRN, FILE *f){
    if(d == NULL || f == NULL) return false;

    ull off = RRN * 80 + SEEK_INI;
    fseek(f, off, SEEK_SET);

    // Escrevendo o campo removido separado (evitar padding)
    fwrite(&(d->removido), sizeof(d->removido), 1, f);
   
    // Escrevendo até antes do primeiro campo de tamanho variável
    fwrite(&(d->prox), sizeof(d->prox), 1, f);
    fwrite(&(d->cod_estacao), sizeof(d->cod_estacao), 1, f);
    fwrite(&(d->cod_linha), sizeof(d->cod_linha), 1, f);
    fwrite(&(d->cod_prox_estacao), sizeof(d->cod_prox_estacao), 1, f);
    fwrite(&(d->dist_prox_estacao), sizeof(d->dist_prox_estacao), 1, f);
    fwrite(&(d->cod_linha_integra), sizeof(d->cod_linha_integra), 1, f);
    fwrite(&(d->cod_est_integra), sizeof(d->cod_est_integra), 1, f);
    fwrite(&(d->tam_nome_estacao), sizeof(d->tam_nome_estacao), 1, f);
   
    // Escrevendo nome da estação, se houver
    if(d->nome_estacao != NULL) fwrite(d->nome_estacao, sizeof(char), d->tam_nome_estacao, f);
    // Escrevendo tamanho do nome da linha, se houver
    fwrite(&(d->tam_nome_linha), sizeof(d->tam_nome_linha), 1, f);
    // Salvando nome da linha
    if(d->nome_linha != NULL) fwrite(d->nome_linha, sizeof(char), d->tam_nome_linha, f);

    // Tamanho até o primeiro campo de tamanho variável + tamanho dos dois campos variáveis + 4 bytes do tam_nome_linha
    uint byte_registro = TAM_FIXO + sizeof(d->tam_nome_linha) + d->tam_nome_estacao + d->tam_nome_linha;
    data_fill_trash(RRN, byte_registro, f); // Preenchendo o restante do registro com lixo

    return true;
}


bool data_save_field(DATA *d, uint RRN, int8 op, FILE *f){
    if(d == NULL || f == NULL) return false;
    
    // Byte offset do primeiro byte do registro
    ull off_ini = RRN * 80 + SEEK_INI;

    switch(op){
        case REMOVIDO:
            fseek(f, off_ini, SEEK_SET);
            fwrite(&(d->removido), sizeof(d->removido), 1, f);
            break;

        case PROX:
            fseek(f, off_ini + 1 + sizeof(int)*(op-1), SEEK_SET);
            fwrite(&(d->prox), sizeof(d->prox), 1, f);
            break;

        case COD_EST:
            fseek(f, off_ini + 1 + sizeof(int)*(op-1), SEEK_SET);
            fwrite(&(d->cod_estacao), sizeof(d->cod_estacao), 1, f);
            break;

        case COD_LIN:
            fseek(f, off_ini + 1 + sizeof(int)*(op-1), SEEK_SET);
            fwrite(&(d->cod_linha), sizeof(d->cod_linha), 1, f);
            break;

        case COD_PROX:
            fseek(f, off_ini + 1 + sizeof(int)*(op-1), SEEK_SET);
            fwrite(&(d->cod_prox_estacao), sizeof(d->cod_prox_estacao), 1, f);
            break;

        case DIST:
            fseek(f, off_ini + 1 + sizeof(int)*(op-1), SEEK_SET);
            fwrite(&(d->dist_prox_estacao), sizeof(d->dist_prox_estacao), 1, f);
            break;

        case COD_LIN_INT:
            fseek(f, off_ini + 1 + sizeof(int)*(op-1), SEEK_SET);
            fwrite(&(d->cod_linha_integra), sizeof(d->cod_linha_integra), 1, f);
            break;

        case COD_EST_INT:
            fseek(f, off_ini + 1 + sizeof(int)*(op-1), SEEK_SET);
            fwrite(&(d->cod_est_integra), sizeof(d->cod_est_integra), 1, f);
            break;

        case NOME_EST:
            // Se o nome da estação a ser gravado for maior ou menor do que o que está armazenado, precisamos arrumar a posição dos campos posteriores
            // Para não perder nenhum dado armazenado na struct d, criamos uma auxiliar
            DATA *aux = data_create(); 
            // Lendo o campo posterior do mesmo registro (nome da linha e tamanho do nome da linha)
            data_load_field(aux, RRN, NOME_LIN, f); 
            
            // Movendo cursor para tamanho do nome da estação 
            fseek(f, off_ini + TAM_FIXO - sizeof(d->tam_nome_estacao), SEEK_SET);
            // Escrevendo novo tamanho do nome da estação (essa modificação tem que ser feita depois do data_load_field)
            fwrite(&(d->tam_nome_estacao), sizeof(d->tam_nome_estacao), 1, f);
            // Escrevendo o novo nome da estação
            if(d->nome_estacao != NULL) fwrite(d->nome_estacao, sizeof(char), d->tam_nome_estacao, f);

            // Escrevendo o tamanho do nome da linha (armazenada em aux), logo após o nome da estação
            fwrite(&(aux->tam_nome_linha), sizeof(aux->tam_nome_linha), 1, f);
            // Escrevendo o nome da linha, se houver nome
            if(aux->nome_linha != NULL) fwrite(aux->nome_linha, sizeof(char), aux->tam_nome_linha, f);

            data_delete(&aux);
            // Preenchendo o resto do registro com lixo
            uint byte_registro_1 = TAM_FIXO + sizeof(d->tam_nome_linha) + d->tam_nome_estacao + d->tam_nome_linha;

            data_fill_trash(RRN, byte_registro_1, f);

            break;

        case NOME_LIN:
            uint aux_U;
            // Lendo o tamanho do nome da estação
            fseek(f, off_ini + TAM_FIXO - sizeof(d->tam_nome_estacao), SEEK_SET);
            fread(&aux_U, sizeof(d->tam_nome_estacao), 1, f);
            // Movendo cursor para escrever tamanho do nome da linha (pulando nome da estação)
            fseek(f, aux_U, SEEK_CUR);
            // Salvando tamanho do nome da linha
            fwrite(&(d->tam_nome_linha), sizeof(d->tam_nome_linha), 1, f);
            // Salvando nome da linha
            fwrite(d->nome_linha, sizeof(char), d->tam_nome_linha, f);
            // Calculando até qual byte foi escrito no registro
            uint byte_registro_2 = TAM_FIXO + sizeof(aux->tam_nome_linha) + d->tam_nome_estacao + aux->tam_nome_linha;
            // Preenchendo o resto com lixo
            data_fill_trash(RRN, byte_registro_2, f);
            // Não é necessário "consertar" os campos posteriores, pois não há campos posteriores

            break;

        default:
            return false;
    }

    return true;    
}

/*==============GETTERS=============*/

char data_get_removido(DATA *d){
    if(d != NULL) return d->removido;

    return '1'; // Se o ponteiro é inválido, é melhor dizer que está removido, para que não seja acessado outros campos
}

int data_get_prox(DATA *d){
    if(d != NULL) return d->prox;

    return -1; 
}

int data_get_cod_est(DATA *d){
    if(d != NULL) return d->cod_estacao;

    return -1;
}

int data_get_cod_lin(DATA *d){
    if(d != NULL) return d->cod_linha;

    return -1;
}

int data_get_cod_prox(DATA *d){
    if(d != NULL) return d->cod_prox_estacao;

    return -1;
}

int data_get_dist(DATA *d){
    if(d != NULL) return d->dist_prox_estacao;

    return -1;
}

int data_get_cod_lin_int(DATA *d){
    if(d != NULL) return d->cod_linha_integra;

    return -1;
}

int data_get_cod_est_int(DATA *d){
    if(d != NULL) return d->cod_est_integra;

    return -1;
}

uint data_get_tam_nome_est(DATA *d){
    if(d != NULL) return d->tam_nome_estacao;

    return 0;
}

char* data_get_nome_est(DATA *d){
    // É impossível o usuário ter a string do nome, mas não ter o tamanho correto da string na struct
    if(d != NULL){ 
        char *aux = (char*)malloc(sizeof(char)*(d->tam_nome_estacao+1));
        
        for(int i = 0; i < d->tam_nome_estacao; i++){
           aux[i] = d->nome_estacao[i];
        }

        aux[d->tam_nome_estacao] = '\0'; // Como não necessariamente o usuário vai ter o tamanho junto da string, é melhor colocarmos o terminador \0 nela

        return aux; // Não podemos retornar diretamente o nome da estação que está na struct, ou quebramos a ideia do TAD
    }

    return NULL;
}

uint data_get_tam_nome_lin(DATA *d){
    if(d != NULL) return d->tam_nome_linha;
    
    return 0;
}

char* data_get_nome_lin(DATA *d){
    // É impossível o usuário ter a string do nome, mas não ter o tamanho correto da string na struct
    if(d != NULL){ 
        char *aux = (char*)malloc(sizeof(char)*(d->tam_nome_linha+1));
        
        for(int i = 0; i < d->tam_nome_linha; i++){
           aux[i] = d->nome_linha[i];
        }

        aux[d->tam_nome_linha] = '\0'; // Como não necessariamente o usuário vai ter o tamanho junto da string, é melhor colocarmos o terminador \0 nela

        return aux; // Não podemos retornar diretamente o nome da estação que está na struct, ou quebramos a ideia do TAD
    }

    return NULL;
}

/*==============SETTERS============*/

bool data_set_removido(DATA *d, char removido){
    if(d != NULL && (removido == '0' || removido == '1')){
        d->removido = removido;
        return true;
    }

    return false;
}

bool data_set_prox(DATA *d, int prox){
    if(d != NULL && prox >= -1){
        d->prox = prox;
        return true;
    }

    return false; 
}

bool data_set_cod_est(DATA *d, int cod_estacao){
    if(d != NULL && cod_estacao >= -1){
        d->cod_estacao = cod_estacao;
        return true;
    }

    return false;
}

bool data_set_cod_lin(DATA *d, int cod_linha){
    if(d != NULL && cod_linha >= -1){
        d->cod_linha = cod_linha;
        return true;
    }

    return false;
}

bool data_set_cod_prox(DATA *d, int cod_prox_estacao){
    if(d != NULL && cod_prox_estacao >= -1){
        d->cod_prox_estacao = cod_prox_estacao;
        return true;
    }

    return false;
}

bool data_set_dist(DATA *d, int dist_prox_estacao){
    if(d != NULL && dist_prox_estacao >= -1){
        d->dist_prox_estacao = dist_prox_estacao;
        return true;
    }

    return false;
}

bool data_set_cod_lin_int(DATA *d, int cod_linha_integra){
    if(d != NULL && cod_linha_integra >= -1){
        d->cod_linha_integra = cod_linha_integra;
        return true;
    }

    return false;
}

bool data_set_cod_est_int(DATA *d, int cod_est_integra){
    if(d != NULL && cod_est_integra >= -1){
        d->cod_est_integra = cod_est_integra;
        return true;
    }

    return false;
}


// Não faz sentido haver um set separado para tamanho dos nomes, pois isso daria liberdade para o usuário modificar o tamanho para um valor errado, o que prejudicaria todo sistema

bool data_set_nome_est(DATA *d, char *nome_estacao){
    if(d != NULL){
        if(d->nome_estacao != NULL){
            free(d->nome_estacao);
            d->nome_estacao = NULL;
        }
        
        // Consideramos que a string passada possui o terminador \0
        if(nome_estacao != NULL ) d->tam_nome_estacao = strlen(nome_estacao);
        else d->tam_nome_estacao = 0;

        if(d->tam_nome_estacao > 0) d->nome_estacao = (char*)malloc(sizeof(char)*d->tam_nome_estacao); 

        //A string na struct não terá o terminador
        for(int i = 0; i < d->tam_nome_estacao; i++){
            d->nome_estacao[i] = nome_estacao[i];
        }
        
        return true;
    }

    return false;
}

bool data_set_nome_lin(DATA *d, char *nome_linha){
    if(d != NULL && nome_linha != NULL){
        if(d->nome_linha != NULL){
            free(d->nome_linha);
            d->nome_linha = NULL;
        }
        // Consideramos que a string passada possui o terminador \0
        d->tam_nome_linha = strlen(nome_linha);
        if(d->tam_nome_linha > 0) d->nome_linha = (char*)malloc(sizeof(char)*d->tam_nome_linha); 

        //A string na struct não terá o terminador
        for(int i = 0; i < d->tam_nome_linha; i++){
            d->nome_linha[i] = nome_linha[i];
        }

        return true;
    }

    return false;
}

/*==============AUXILIAR============*/

void data_fill_trash(uint RRN, uint byte_registro, FILE *f){
    if(f == NULL) return;

    // Calculando quantos bytes devem ser preenchidos
    ull resto = 80-byte_registro; 

    char *preenche = NULL;
    if(resto > 0 && resto <= 80) preenche = (char*)malloc(sizeof(char)*resto);
    if(preenche != NULL){
        // Criando a string formada por '$'
        for(int i = 0; i < resto; i++){
            preenche[i] = LIXO;
        }
        // Escrevendo no registro
        fwrite(preenche, sizeof(char), resto, f);
        free(preenche);
        preenche = NULL;
    }
}