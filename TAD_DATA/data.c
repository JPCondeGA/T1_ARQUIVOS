#include "data.h"

#define SEEK_INI 17 // Tamanho em bytes do registro de cabeçalho
#define TAM_FIXO 33 // Tamanho em bytes do registro de dados até primeiro campo de tamanho variável
#define LIXO '$' // Caractere utilizado para indicar bytes não utilizados

// Estrutura que representa um registro de dados
struct data_{
    char removido; // Indica se o registro está logicamente removido
    int proximo; // Indica o próximo na pilha de removidos, se esse estiver removido; caso contrário, -1
    int cod_estacao; // Código da estação
    int cod_linha; // Código da linha
    int cod_prox_estacao; // Código da próxima estação
    int dist_prox_estacao; // Distância para a próxima estação
    int cod_linha_integra; // Códiga da linha que faz a integração entre as linhas
    int cod_est_integra; // Código da estação que faz a integração entre as linhas
    uint tam_nome_estacao; // Inidicador de tamanho (quantidade de caracteres) do nome da estação
    char* nome_estacao; // Nome da estação
    uint tam_nome_linha; // Inidicador de tamanho (quantidade de caracteres) do nome da linha
    char* nome_linha; // Nome da linha
}; 

// 64 bytes de tamanho -> padding entre removido e proximo, tam_nome_estacao e nome_estacao, e tam_nome_linha e nome_linha


/*=============FUNÇÕES OCULTAS============*/

/* Preenche um registro de dados com lixo (caractere '$') de um byte do registro até seu fim.
- Recebe o RRN do registro que está sendo preenchido, o byte-offset do primeiro byte a ser preenchido preenchimento e o arquivo em que está o registro de dados. 

Passe corretamente o byte_off para evitar sobreescritas incorretas. Essa função não move o cursor do arquivo (é para ser usada com o cursor já na posição correta). */
void data_preencher_lixo(uint RRN, uint byte_off, FILE *f);

/* Calcula o byte-offset do primeiro byte do registro de dados com RRN esepecífico.
- Recebe o RRN do registro.
- Retorna o byte-offset. */
ull data_byte_offset(uint RRN);

/* Calcula quantos bytes o registro representado pela estrutura ocupa no arquivo. 
    - Recebe uma estrutura que representa um registro de dados.
    - Retorna a quantidade bytes que esse registro representado ocuparia no arquivo binário. */
ull data_tamanho_atual(DATA *d);

/*=============ALOCAÇÃO E DESALOCAÇÃO============*/

DATA *data_criar(){
    DATA *d = (DATA*)malloc(sizeof(DATA)); // Alocando memória

    // Se a alocação foi bem sucedida, inicializamos
    if(d != NULL){ 
        // Inicialmente, consideramos que não está removido, mas todos campos são nulos
        d->removido = '0'; 
        d->proximo = -1;
        d->cod_estacao = d->cod_linha = d->cod_prox_estacao = d->dist_prox_estacao = d->cod_linha_integra = d->cod_est_integra = -1;
        d->tam_nome_estacao = d->tam_nome_linha = 0;
        d->nome_estacao = NULL;
        d->nome_linha = NULL;
    }

    return d;
}


bool data_apagar(DATA **d){
    if(d == NULL || *d == NULL) return false; // Verificando se os ponteiros são válidos

    if((*d)->nome_estacao != NULL) free((*d)->nome_estacao), (*d)->nome_estacao = NULL; // Desalocando nome da estação, se houver
    if((*d)->nome_linha != NULL) free((*d)->nome_linha), (*d)->nome_linha = NULL; // Desalocando nome da linha , se houver

    free(*d); // Desalocando
    *d = NULL; // Setando o ponteiro para NULL

    return true;
}


/*===========CARREGAMENTO E SALVAMENTO==========*/

bool data_carregar(DATA *d, uint RRN, bool move, FILE *f){
    if(d == NULL || f == NULL) return false; // Verificando se os ponteiros são válidos

    // Se for necessário mover o cursor
    ull off_ini = 0; // Byte_offset do registro
    if(move){ // Se necessário mover
        off_ini = data_byte_offset(RRN); // Calculando o byte-offset do registro de dado a ser carregado
        fseek(f, off_ini, SEEK_SET); // Movendo cursos para o primeiro byte do registro
    }
    
    fread(&(d->removido), sizeof(d->removido), 1, f); // Lendo somente o removido (1 byte) para evitar padding
    fread(&(d->proximo), TAM_FIXO - sizeof(d->removido), 1, f); // Lendo até o primeiro campo de tamenho variável
    
    // Alocando e lendo o nome da estação
    if(d->nome_estacao != NULL) free(d->nome_estacao); // Se necessário, o nome anterior que estava na struct será sobrescrito
    if(d->tam_nome_estacao > 0){ // Só alocamos se há algo para ler
        d->nome_estacao = (char*)malloc(sizeof(char)*(d->tam_nome_estacao+1)); // +1 para o terminador
        fread(d->nome_estacao, sizeof(char), d->tam_nome_estacao, f); // Gravando nome da estação no espaço alocada
        d->nome_estacao[d->tam_nome_estacao] = '\0'; // Concatenando com o terminador
    }

    else d->nome_estacao = NULL; // Para não ter acesso indevido

    fread(&(d->tam_nome_linha), sizeof(d->tam_nome_linha), 1, f); // Lendo o tamanho do nome da linha

    // Alocando e lendo o nome da linha
    if(d->nome_linha != NULL) free(d->nome_linha); // Se necessário, o nome anterior que estava na struct será sobrescrito
    if(d->tam_nome_linha > 0){ // Só alocamos se há algo para ler
        d->nome_linha = (char*)malloc(sizeof(char)*(d->tam_nome_linha+1)); // +1 para o terminador
        fread(d->nome_linha, sizeof(char), d->tam_nome_linha, f); // Gravando nome da linha no espaço alocada
        d->nome_linha[d->tam_nome_linha] = '\0'; // Concatenando com o terminador
    }

    else d->nome_linha = NULL; // Para não ter acesso indevido
    
    if(feof(f)) return false; // Alguma(s) das leituras falhou
        
    fseek(f, TAM_DATA - data_tamanho_atual(d), SEEK_CUR); // Indo para o primeiro byte do próximo registro

    return true;
}

bool data_carregar_campo(DATA *d, uint RRN, int8 op, bool move, FILE *f){
    if(d == NULL || f == NULL) return false; // Verificando se os ponteiros são válidos

    ull off_ini = 0; // Byte-offset do registro
    uint bytes = 0; // Quantos bytes precisa para chegar no próximo registro
    if(move) off_ini = data_byte_offset(RRN); // Calculando o byte-offset do registro

    // Carregando o campo selecionado
    switch(op){
        case REMOVIDO:
            if(move) fseek(f, off_ini, SEEK_SET); // Movendo para o primeiro campo, se necessário

            fread(d, sizeof(d->removido), 1, f); // Lendo removido
            bytes = TAM_DATA - sizeof(d->removido);
            break;

        case PROXIMO:
            // Movendo cursor para o campo próximo (a fórmula pode ser aplicada nos seguintes) 
            if(move) fseek(f, off_ini + sizeof(d->removido) + sizeof(int)*(op-1), SEEK_SET);
            else fseek(f, sizeof(d->removido) + sizeof(int)*(op-1), SEEK_CUR); 
            
            fread(&(d->proximo), sizeof(d->proximo), 1, f); 
            bytes = TAM_DATA - (sizeof(d->removido) + sizeof(int)*(op));
            break;

        case COD_ESTACAO:
            // Movendo cursor para o campo código da estação
            if(move) fseek(f, off_ini + sizeof(d->removido) + sizeof(int)*(op-1), SEEK_SET);
            else fseek(f, sizeof(d->removido) + sizeof(int)*(op-1), SEEK_CUR); 
 
            fread(&(d->cod_estacao), sizeof(d->cod_estacao), 1, f);
            bytes = TAM_DATA - (sizeof(d->removido) + sizeof(int)*(op));
            break;

        case COD_LINHA:
            // Movendo cursor para o campo código da linha
            if(move) fseek(f, off_ini + sizeof(d->removido) + sizeof(int)*(op-1), SEEK_SET);
            else fseek(f, sizeof(d->removido) + sizeof(int)*(op-1), SEEK_CUR); 
            
            fread(&(d->cod_linha), sizeof(d->cod_linha), 1, f);
            bytes = TAM_DATA - (sizeof(d->removido) + sizeof(int)*(op));
            break;

        case COD_PROX_ESTACAO:
            // Movendo cursor para o campo código da próxima estação
            if(move) fseek(f, off_ini + sizeof(d->removido) + sizeof(int)*(op-1), SEEK_SET);
            else fseek(f, sizeof(d->removido) + sizeof(int)*(op-1), SEEK_CUR); 
            
            fread(&(d->cod_prox_estacao), sizeof(d->cod_prox_estacao), 1, f);
            bytes = TAM_DATA - (sizeof(d->removido) + sizeof(int)*(op));
            break;

        case DIST_PROX_ESTACAO:
            // Movendo cursor para o campo distância para próxima estação
            if(move) fseek(f, off_ini + sizeof(d->removido) + sizeof(int)*(op-1), SEEK_SET);
            else fseek(f, sizeof(d->removido) + sizeof(int)*(op-1), SEEK_CUR); 
            
            fread(&(d->dist_prox_estacao), sizeof(d->dist_prox_estacao), 1, f);
            bytes = TAM_DATA - (sizeof(d->removido) + sizeof(int)*(op));
            break;

        case COD_LINHA_INTEGRA:
            // Movendo cursor para o campo código da linha de integração
            if(move) fseek(f, off_ini + sizeof(d->removido) + sizeof(int)*(op-1), SEEK_SET);
            else fseek(f, sizeof(d->removido) + sizeof(int)*(op-1), SEEK_CUR); 
            
            fread(&(d->cod_linha_integra), sizeof(d->cod_linha_integra), 1, f);
            bytes = TAM_DATA - (sizeof(d->removido) + sizeof(int)*(op));
            break;
        
        case COD_EST_INTEGRA:
            // Movendo cursor para o campo código da estação de integração
            if(move) fseek(f, off_ini + sizeof(d->removido) + sizeof(int)*(op-1), SEEK_SET);
            else fseek(f, sizeof(d->removido) + sizeof(int)*(op-1), SEEK_CUR); 
            
            fread(&(d->cod_est_integra), sizeof(d->cod_est_integra), 1, f);
            bytes = TAM_DATA - (sizeof(d->removido) + sizeof(int)*(op));
            break;

        case NOME_ESTACAO:
            // Lendo tamanho string
            
            // Movendo cursor para o tamanho do nome da estação
            if(move) fseek(f, off_ini + TAM_FIXO - sizeof(d->tam_nome_estacao), SEEK_SET);
            else fseek(f, TAM_FIXO - sizeof(d->tam_nome_estacao), SEEK_CUR); 
            fread(&(d->tam_nome_estacao), sizeof(d->tam_nome_estacao), 1, f);

            if(d->nome_estacao != NULL) free(d->nome_estacao); // Sobrescrevendo, se necessário

            // Alocando e lendo string
            if(d->tam_nome_estacao > 0 ){ // Não faz sentido alocarmos memória se não formos escrever uma string nela
                d->nome_estacao = (char*)malloc(sizeof(char)*(d->tam_nome_estacao+1)); // +1 para o terminador 
                fread(d->nome_estacao, sizeof(char), d->tam_nome_estacao, f); // Gravando nome da linha no espaço alocada
                d->nome_estacao[d->tam_nome_estacao] = '\0';  // Concatenando com o terminador
            }

            else d->nome_estacao = NULL; // Para não ter acesso indevido

            bytes = TAM_DATA - (TAM_FIXO + d->tam_nome_estacao);
            
            break;
            
        case NOME_LINHA:
            uint tam_nome_estacao; // Tamanho do nome da estação (se necessário) para saltá-lo na leitura
            
            // Lendo tamanho do nome da estação (precisamos dessa informação para ir para a byte correto e ler o nome da linha)
            // Movendo cursor para o campo
            if(move) fseek(f, off_ini + TAM_FIXO - sizeof(d->tam_nome_estacao), SEEK_SET);
            else fseek(f, TAM_FIXO - sizeof(d->tam_nome_estacao), SEEK_CUR);

            fread(&tam_nome_estacao, sizeof(d->tam_nome_estacao), 1, f); 

            // Lendo tamanho do nome da linha
            fseek(f, tam_nome_estacao, SEEK_CUR); // Pulando o nome da estação
            fread(&(d->tam_nome_linha), sizeof(d->tam_nome_linha), 1, f);

            if(d->nome_linha != NULL) free(d->nome_linha); // Sobrescrevendo se necessário

            // Alocando e lendo nome da linha
            if(d->tam_nome_linha > 0){ // Só alocamos se há algo para ler
                d->nome_linha= (char*)malloc(sizeof(char)*(d->tam_nome_linha+1));
                fread(d->nome_linha, sizeof(char), d->tam_nome_linha, f); // Gravando nome da linha no espaço alocada
                d->nome_linha[d->tam_nome_linha] = '\0'; // Concatenando com o terminador
            }

            else d->nome_linha = NULL; // Para evitar acesso indevido

            bytes = TAM_DATA - data_tamanho_atual(d); // Esse é o último campo, então é o tamanho que reservado a ele menos o tamanho que de fato possui

            break;

        default:
            return false; // Em caso se campo passado inválido
    }

    if(feof(f)) return false; // Alguma(s) leitura(s) falhou

    fseek(f, bytes, SEEK_CUR); // Indo para o primeiro byte do próximo registro
        
    return true;
}

bool data_salvar(DATA *d, uint RRN, bool move, FILE *f){
    if(d == NULL || f == NULL) return false; // Verificando se os ponteiros são válidos

    ull off_ini = 0; // Byte-offset do registro
    if(move){ // Se necessário mover
        off_ini = data_byte_offset(RRN); // Calculando byte-offset do registro
        fseek(f, off_ini, SEEK_SET); // Movendo cursor para o primeiro byte do registro
    }

    fwrite(&(d->removido), sizeof(d->removido), 1, f); // Escrevendo o campo removido separado (evitar padding)
   
    // Escrevendo até antes do primeiro campo de tamanho variável
    fwrite(&(d->proximo), sizeof(d->proximo), 1, f);
    fwrite(&(d->cod_estacao), sizeof(d->cod_estacao), 1, f);
    fwrite(&(d->cod_linha), sizeof(d->cod_linha), 1, f);
    fwrite(&(d->cod_prox_estacao), sizeof(d->cod_prox_estacao), 1, f);
    fwrite(&(d->dist_prox_estacao), sizeof(d->dist_prox_estacao), 1, f);
    fwrite(&(d->cod_linha_integra), sizeof(d->cod_linha_integra), 1, f);
    fwrite(&(d->cod_est_integra), sizeof(d->cod_est_integra), 1, f);
    fwrite(&(d->tam_nome_estacao), sizeof(d->tam_nome_estacao), 1, f);
    
    if(d->nome_estacao != NULL) fwrite(d->nome_estacao, sizeof(char), d->tam_nome_estacao, f); // Escrevendo nome da estação, se houver

    fwrite(&(d->tam_nome_linha), sizeof(d->tam_nome_linha), 1, f); // Escrevendo tamanho do nome da linha

    if(d->nome_linha != NULL) fwrite(d->nome_linha, sizeof(char), d->tam_nome_linha, f); // Escrevendo nome da linha, se houver

    // Tamanho até o primeiro campo de tamanho variável + tamanho dos dois campos variáveis + 4 bytes do tam_nome_linha + 1
    data_preencher_lixo(RRN, data_tamanho_atual(d), f); // Preenchendo o restante do registro com lixo
    
    // O cursor já estará no primeiro byte do próximo registro

    return true;
}


bool data_salvar_campo(DATA *d, uint RRN, int8 op, bool move, FILE *f){
    if(d == NULL || f == NULL) return false; // Verificando se os ponteiros são válidos

    ull off_ini = 0; // Byte-offset do registro
    ull bytes = 0; // Quantidade de bytes a ser pulado para se chegar no próximo registro

    if(move) off_ini = data_byte_offset(RRN); // Calculando byte-offset do registro

    // Escrevendo o campo selecionado
    switch(op){
        case REMOVIDO:
            if(move) fseek(f, off_ini, SEEK_SET); // Movendo para o primeiro byte se necessário

            fwrite(&(d->removido), sizeof(d->removido), 1, f);
            bytes = TAM_DATA - sizeof(d->removido);
            break;

        case PROXIMO:
            // Movendo cursor para o campo próximo
            if(move) fseek(f, off_ini + sizeof(d->removido) + sizeof(int)*(op-1), SEEK_SET);
            else fseek(f, sizeof(d->removido) + sizeof(int)*(op-1), SEEK_CUR); 

            fwrite(&(d->proximo), sizeof(d->proximo), 1, f);
            bytes = TAM_DATA - (sizeof(d->removido) + sizeof(int)*(op));
            break;

        case COD_ESTACAO:
            // Movendo cursor para o campo código estação
            if(move) fseek(f, off_ini + sizeof(d->removido) + sizeof(int)*(op-1), SEEK_SET);
            else fseek(f, sizeof(d->removido) + sizeof(int)*(op-1), SEEK_CUR); 

            fwrite(&(d->cod_estacao), sizeof(d->cod_estacao), 1, f);
            bytes = TAM_DATA - (sizeof(d->removido) + sizeof(int)*(op));
            break;

        case COD_LINHA:
            // Movendo cursor para o campo código linha
            if(move) fseek(f, off_ini + sizeof(d->removido) + sizeof(int)*(op-1), SEEK_SET);
            else fseek(f, sizeof(d->removido) + sizeof(int)*(op-1), SEEK_CUR); 

            fwrite(&(d->cod_linha), sizeof(d->cod_linha), 1, f);
            bytes = TAM_DATA - (sizeof(d->removido) + sizeof(int)*(op));
            break;

        case COD_PROX_ESTACAO:
            // Movendo cursor para o campo código próximo estação
            if(move) fseek(f, off_ini + sizeof(d->removido) + sizeof(int)*(op-1), SEEK_SET);
            else fseek(f, sizeof(d->removido) + sizeof(int)*(op-1), SEEK_CUR); 
            
            fwrite(&(d->cod_prox_estacao), sizeof(d->cod_prox_estacao), 1, f);
            bytes = TAM_DATA - (sizeof(d->removido) + sizeof(int)*(op));
            break;

        case DIST_PROX_ESTACAO:
            // Movendo cursor para o campo distância próximo estação
            if(move) fseek(f, off_ini + sizeof(d->removido) + sizeof(int)*(op-1), SEEK_SET);
            else fseek(f, sizeof(d->removido) + sizeof(int)*(op-1), SEEK_CUR); 
            
            fwrite(&(d->dist_prox_estacao), sizeof(d->dist_prox_estacao), 1, f);
            bytes = TAM_DATA - (sizeof(d->removido) + sizeof(int)*(op));
            break;

        case COD_LINHA_INTEGRA:
            // Movendo cursor para o campo código linha integra
            if(move) fseek(f, off_ini + sizeof(d->removido) + sizeof(int)*(op-1), SEEK_SET);
            else fseek(f, sizeof(d->removido) + sizeof(int)*(op-1), SEEK_CUR); 

            fwrite(&(d->cod_linha_integra), sizeof(d->cod_linha_integra), 1, f);
            bytes = TAM_DATA - (sizeof(d->removido) + sizeof(int)*(op));
            break;

        case COD_EST_INTEGRA:
            // Movendo cursor para o campo código estação integra
            if(move) fseek(f, off_ini + sizeof(d->removido) + sizeof(int)*(op-1), SEEK_SET);
            else fseek(f, sizeof(d->removido) + sizeof(int)*(op-1), SEEK_CUR); 

            fwrite(&(d->cod_est_integra), sizeof(d->cod_est_integra), 1, f);
            bytes = TAM_DATA - (sizeof(d->removido) + sizeof(int)*(op));
            break;

        case NOME_ESTACAO:
            // Se o nome da estação a ser gravado for maior ou menor do que o que está armazenado, precisamos arrumar a posição dos campos posteriores
            
            // Inicializnado auxiliar
            DATA *aux = data_criar(); // Para não perder nenhum dado armazenado na struct d, criamos uma auxiliar que carregará os campos posteriores
            if(aux == NULL) return false; // Se a alocação falhou
            data_carregar_campo(aux, RRN, NOME_LINHA, move, f); // Carregando o nome da linha e o tamanho do nome da linha (cursor pode não estar no primeiro byte do registro, assim passa o próprio move como parâmetro)

            // Escrvendo o tamanho do nome da estação e o nome da estação
            fseek(f, (- TAM_DATA) + TAM_FIXO - sizeof(d->tam_nome_estacao), SEEK_CUR); // Movendo cursor para tamanho do nome da estação (estamos voltando o cursor um registro pois ele foi movido no carregar)
            fwrite(&(d->tam_nome_estacao), sizeof(d->tam_nome_estacao), 1, f); // Escrevendo novo tamanho do nome da estação (essa modificação tem que ser feita depois do data_load_field)
            if(d->nome_estacao != NULL) fwrite(d->nome_estacao, sizeof(char), d->tam_nome_estacao, f); // Escrevendo o novo nome da estação, se não for nulo

            // Reescrevendo campos posteriores nos bytes corretos  
            fwrite(&(aux->tam_nome_linha), sizeof(aux->tam_nome_linha), 1, f); // Escrevendo o tamanho do nome da linha (armazenada em aux), logo após o nome da estação
            if(aux->nome_linha != NULL) fwrite(aux->nome_linha, sizeof(char), aux->tam_nome_linha, f); // Escrevendo o nome da linha, se houver nome
            data_apagar(&aux); // Desalocando a estrutura auxiliar
            
            // Preechendo o restante com lixo
            uint byte_registro_1 = TAM_FIXO + sizeof(d->tam_nome_linha) + d->tam_nome_estacao + d->tam_nome_linha+1; // Calculando o byte-offset (em relação ao primeiro byte-offset) em que estamos
            data_preencher_lixo(RRN, byte_registro_1, f);

            // Não irá precisar mover o cursor, então não precisamos calcular o bytes

            break;

        case NOME_LINHA:
            uint tam_nome_estacao; // Tamanho do nome da estação (se necessário) para saltá-lo na leitura
            
            // Lendo o tamanho do nome da estação
            if(move) fseek(f, off_ini + TAM_FIXO - sizeof(d->tam_nome_estacao), SEEK_SET); // Movendo cursor para o campo tamanho do nome da estação
            else fseek(f, TAM_FIXO - sizeof(d->tam_nome_estacao), SEEK_CUR);
            
            fread(&tam_nome_estacao, sizeof(d->tam_nome_estacao), 1, f);

            fseek(f, tam_nome_estacao, SEEK_CUR); // Pulando o nome da estação

            fwrite(&(d->tam_nome_linha), sizeof(d->tam_nome_linha), 1, f); // Salvando tamanho do nome da linha
            if(d->nome_estacao != NULL) fwrite(d->nome_linha, sizeof(char), d->tam_nome_linha, f); // Salvando nome da linha
            
            // Preechendo o restante com lixo
            uint byte_registro_2 = TAM_FIXO + sizeof(aux->tam_nome_linha) + d->tam_nome_estacao + aux->tam_nome_linha+1; // Calculando até qual byte foi escrito no registro
            data_preencher_lixo(RRN, byte_registro_2, f); 

            // Não é necessário "consertar" os campos posteriores, pois não há campos posteriores

            // Não irá precisar mover o cursor, então não precisamos calcular o bytes

            break;

        default:
            return false; // Em caso se campo passado inválido
    }

    if(bytes > 0) fseek(f, bytes, SEEK_CUR); // Movendo para o primeiro byte do próximo registro

    return true;    
}

bool data_salvar_removido_proximo(DATA *d, uint RRN, bool move, FILE *f){
    if(d == NULL || f == NULL) return false; // Verifcando ponteiros

    if(move) fseek(f, data_byte_offset(RRN), SEEK_SET); // Se necessário movemos o cursor para o começo do registro

    // Escrevendo campos (separadamente para evitar padding)
    fwrite(&(d->removido), sizeof(d->removido), 1, f);
    fwrite(&(d->proximo), sizeof(d->proximo), 1, f);

    fseek(f, TAM_DATA - sizeof(d->removido) - sizeof(d->proximo), SEEK_CUR); // Indo para o próximon registro

    return true;
}

/*==============GETTERS=============*/

char data_get_removido(DATA *d){
    if(d != NULL) return d->removido; // Se o ponteiro não for inválido, retornamos o campo

    return '1'; // Se o ponteiro é inválido, é melhor dizer que está removido, para que não seja acessado outros campos
}

int data_get_proximo(DATA *d){
    if(d != NULL) return d->proximo; // Se o ponteiro não for inválido, retornamos o campo

    return -1; // Se o ponteiro é inválido, retornando que não há próximo na pilha para evitar acessos indevidos
}

int data_get_cod_estacao(DATA *d){
    if(d != NULL) return d->cod_estacao; // Se o ponteiro não for inválido, retornamos o campo

    return -1; // Se o ponteiro é inválido, tratamos o campo como nulo
}

int data_get_cod_linha(DATA *d){
    if(d != NULL) return d->cod_linha; // Se o ponteiro não for inválido, retornamos o campo

    return -1; // Se o ponteiro é inválido, tratamos o campo como nulo
}

int data_get_cod_prox_estacao(DATA *d){ 
    if(d != NULL) return d->cod_prox_estacao; // Se o ponteiro não for inválido, retornamos o campo

    return -1; // Se o ponteiro é inválido, tratamos o campo como nulo
}

int data_get_dist_prox_estacao(DATA *d){
    if(d != NULL) return d->dist_prox_estacao; // Se o ponteiro não for inválido, retornamos o campo

    return -1; // Se o ponteiro é inválido, tratamos o campo como nulo
}

int data_get_cod_linha_integra(DATA *d){
    if(d != NULL) return d->cod_linha_integra; // Se o ponteiro não for inválido, retornamos o campo

    return -1; // Se o ponteiro é inválido, tratamos o campo como nulo
}

int data_get_cod_est_integra(DATA *d){
    if(d != NULL) return d->cod_est_integra; // Se o ponteiro não for inválido, retornamos o campo

    return -1; // Se o ponteiro é inválido, tratamos o campo como nulo
}

uint data_get_tam_nome_estacao(DATA *d){
    if(d != NULL) return d->tam_nome_estacao; // Se o ponteiro não for inválido, retornamos o campo

    return 0; // Se o ponteiro é inválido, tratamos o campo como nulo
}

char* data_get_nome_estacao(DATA *d){
    if(d != NULL){
        // É basicamente impossível o usuário ter a string do nome, mas não ter o tamanho correto da string na struct
        char *aux = (char*)malloc(sizeof(char)*(d->tam_nome_estacao+1)); // Alocando memória para uma segunda string
        if(aux == NULL) return NULL;

        //Se o campo for nulo, retorna uma string apenas com o terminador
        if (d->tam_nome_estacao == 0){
            aux[0] = '\0';
        }
                
        // Copiando a string da estrutura para a segunda
        for(int i = 0; i <= d->tam_nome_estacao; i++){
           aux[i] = d->nome_estacao[i];
        }

        return aux; // Não podemos retornar diretamente o nome da estação que está na struct, ou quebramos a ideia do TAD
    }

    return NULL;
}

uint data_get_tam_nome_linha(DATA *d){
    if(d != NULL) return d->tam_nome_linha; // Se o ponteiro é inválido, tratamos o campo como nulo
    
    return 0;
}

char* data_get_nome_linha(DATA *d){
    if(d != NULL){
        // É basicamente impossível o usuário ter a string do nome, mas não ter o tamanho correto da string na struct
        char *aux = (char*)malloc(sizeof(char)*(d->tam_nome_linha+1)); // Alocando memória para uma segunda string
        if(aux == NULL) return NULL;

        //Se o campo for nulo, retorna uma string apenas com o terminador
        if (d->tam_nome_linha == 0){ 
            aux[0] = '\0';
            return aux;
        } 

        // Copiando a string da estrutura para a segunda
        for(int i = 0; i <= d->tam_nome_linha; i++){
           aux[i] = d->nome_linha[i];
        }

        return aux; // Não podemos retornar diretamente o nome da estação que está na struct, ou quebramos a ideia do TAD
    }

    return NULL;
}

/*==============SETTERS============*/

bool data_set_removido(DATA *d, char removido){
    // Se o ponteiro é válido e o valor do campo possível (topo pode receber -1 em caso de pilha vazia), atribuimos
    if(d != NULL && (removido == '0' || removido == '1')){
        d->removido = removido;
        return true;
    }

    return false;
}

bool data_set_proximo(DATA *d, int proximo){
    // Se o ponteiro é válido e o valor do campo possível (topo pode receber -1 em caso de pilha vazia), atribuimos
    if(d != NULL && proximo >= -1){
        d->proximo = proximo;
        return true;
    }

    return false; 
}

bool data_set_cod_estacao(DATA *d, int cod_estacao){
    // Se o ponteiro é válido e o valor do campo possível (topo pode receber -1 em caso de pilha vazia), atribuimos
    if(d != NULL && cod_estacao >= -1){
        d->cod_estacao = cod_estacao;
        return true;
    }

    return false;
}

bool data_set_cod_linha(DATA *d, int cod_linha){
    // Se o ponteiro é válido e o valor do campo possível (topo pode receber -1 em caso de pilha vazia), atribuimos
    if(d != NULL && cod_linha >= -1){
        d->cod_linha = cod_linha;
        return true;
    }

    return false;
}

bool data_set_cod_prox_estacao(DATA *d, int cod_prox_estacao){
    // Se o ponteiro é válido e o valor do campo possível (topo pode receber -1 em caso de pilha vazia), atribuimos
    if(d != NULL && cod_prox_estacao >= -1){
        d->cod_prox_estacao = cod_prox_estacao;
        return true;
    }

    return false;
}

bool data_set_dist_prox_estacao(DATA *d, int dist_prox_estacao){
    // Se o ponteiro é válido e o valor do campo possível (topo pode receber -1 em caso de pilha vazia), atribuimos
    if(d != NULL && dist_prox_estacao >= -1){
        d->dist_prox_estacao = dist_prox_estacao;
        return true;
    }

    return false;
}

bool data_set_cod_linha_integra(DATA *d, int cod_linha_integra){
    // Se o ponteiro é válido e o valor do campo possível (topo pode receber -1 em caso de pilha vazia), atribuimos
    if(d != NULL && cod_linha_integra >= -1){
        d->cod_linha_integra = cod_linha_integra;
        return true;
    }

    return false;
}

bool data_set_cod_est_integra(DATA *d, int cod_est_integra){
    // Se o ponteiro é válido e o valor do campo possível (topo pode receber -1 em caso de pilha vazia), atribuimos
    if(d != NULL && cod_est_integra >= -1){
        d->cod_est_integra = cod_est_integra;
        return true;
    }

    return false;
}


// Não faz sentido haver um set separado para tamanho dos nomes, pois isso daria liberdade para o usuário modificar o tamanho para um valor errado, o que prejudicaria todo sistema.

bool data_set_nome_estacao(DATA *d, char *nome_estacao){
    if(d != NULL){ // Verificando se o ponteiro não é inválidp
        
        // Sobrescrevendo se necessário
        if(d->nome_estacao != NULL){
            free(d->nome_estacao);
        }
        
        // Setando o tamanho do nome da estação
        if(nome_estacao != NULL) d->tam_nome_estacao = strlen(nome_estacao); // Consideramos que a string passada possui o terminador \0
        else d->tam_nome_estacao = 0; // Em caso de campo nulo

        // Alocando e lendo nome da estação
        if(d->tam_nome_estacao > 0){ // Não faz sentido alocarmos memória se não formos escrever uma string nela
            d->nome_estacao = (char*)malloc(sizeof(char)*(d->tam_nome_estacao+1)); // +1 para o terminador
            // Copiando string
            for(int i = 0; i <= d->tam_nome_estacao; i++){
                d->nome_estacao[i] = nome_estacao[i];
            }
        }
        
        else d->nome_estacao = NULL; // Setando o campo para nulo
        
        return true;
    }
    return false;
}


bool data_set_nome_linha(DATA *d, char *nome_linha){
    if(d != NULL){ // Verificando se o ponteiro é válido

        // Sobrescrevendo se necessário
        if(d->nome_linha != NULL){
            free(d->nome_linha);
            d->nome_linha = NULL;
        }
        
        // Setando o tamanho do nome da linha
        if(nome_linha != NULL) d->tam_nome_linha = strlen(nome_linha); // Consideramos que a string passada possui o terminador \0
        else d->tam_nome_linha = 0; // Em caso de campo nulo

        // Alocando e lendo nome da linha  
        if(d->tam_nome_linha > 0){ // Não faz sentido alocarmos memória se não formos escrever uma string nela
            d->nome_linha = (char*)malloc(sizeof(char)*(d->tam_nome_linha+1)); // +1 para o terminador
            // Copiando string
            for(int i = 0; i <= d->tam_nome_linha; i++){
                d->nome_linha[i] = nome_linha[i];
            }
        }

        return true;
    }

    return false;
}

/*==============AUXILIAR============*/

void data_preencher_lixo(uint RRN, uint byte_off, FILE *f){
    if(f == NULL) return; // Verificando se o ponteiro é inválido

    ull resto = TAM_DATA - byte_off; // Calculando quantos bytes devem ser preenchidos (um registro tem 80 bytes)

    char preenche[TAM_DATA]; // buffer que receberá uma sequência de caractere LIXO para depois ser escrito no arquivo (tamanho máximo)
    if(resto <= 80){ // Verificando se a alocação ocorreu corretamente
        for(int i = 0; i < resto; i++) preenche[i] = LIXO; // Preenchendo string que será escrita

        fwrite(preenche, sizeof(char), resto, f); // Escrevendo no registro
    }
}

ull data_byte_offset(uint RRN){
    return SEEK_INI + RRN * TAM_DATA; // Tamanho do registro de cabeçalho + Soma dos tamanho dos registros de dados
}

ull data_tamanho_atual(DATA *d){
    if(d == NULL) return 0;
    return TAM_FIXO + sizeof(d->tam_nome_linha) + d->tam_nome_estacao + d->tam_nome_linha;
}

