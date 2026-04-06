#include "contexto.h"

/*================WHERE================*/

bool func_where_input(FLAG_FIELD *flags, DATA *filtro, int m){
    // Verifica se a função foi executada com sucesso
    bool ok = true;
    // Guarda o campo a ser especificado
    char campo[TAM_BUFFER];
    // Guarda o valor desejado para determinado campo
    char valor[TAM_BUFFER];
    // Setando todos campos para false (as buscas anteriores não podem interferir nessa)
    ok = func_init_flag_field(flags);
    // Lendo filtros
    for(int i = 0; i < m && ok; i++){
        scanf(" %s", campo);
        // Se o valor será uma string entre aspas, usamos a função apropriada
        if(!strcmp(campo, "nomeEstacao") || !strcmp(campo, "nomeLinha")){
            ScanQuoteString(valor);
        }
        else{
            // Lendo como string, pois não sabemos se será "NULO" ou um número
            scanf(" %s", valor);
        }

        // Atribuindo valor ao campo correto da estrutra
        ok = func_attribute_value(valor, campo, filtro);
        ok = ok && func_atribbute_flag_field(flags, campo);
    }

    return ok;
}

bool func_where_compare(FLAG_FIELD flags, DATA *d, DATA *filtro){
    if(d == NULL || filtro == NULL) return false;

    // Se o registro está logicamente marcado como removido, não precisamos fazer as comparações
    if(data_get_removido(d) == '1') return false;
    
    // Resultado da comparção
    bool resp = true; 

    // Conferindo campos que são comparados (se uma comparação não bater, não precisamos verificar as demais)
    if(flags.cod_estacao) resp = data_compare(d, filtro, COD_EST);
    if(resp && flags.cod_linha) resp = data_compare(d, filtro, COD_LIN);
    if(resp && flags.cod_prox_estacao) resp = data_compare(d, filtro, COD_PROX);
    if(resp && flags.dist_prox_estacao) resp = data_compare(d, filtro, DIST);
    if(resp && flags.cod_linha_integra) resp = data_compare(d, filtro, COD_LIN_INT);
    if(resp && flags.cod_est_integra) resp = data_compare(d, filtro, COD_EST_INT);
    if(resp && flags.nome_estacao) resp = data_compare(d, filtro, NOME_EST);
    if(resp && flags.nome_linha) resp = data_compare(d, filtro, NOME_LIN);

    return resp;
}


/*================FLAGS================*/

bool func_atribbute_flag_field(FLAG_FIELD *flags, char *campo){
    if(flags == NULL || campo == NULL) return false;
    
    // Colocando o nome do campo em minusculo -> Permite que a mesma função seja usado no Create e nas outras funcionalidades
    func_str_to_lower(campo);

    // Verificando qual campo é e atribuindo true para o campo especificado
    if(strcmp(campo, "codestacao") == 0){
        flags->cod_estacao = true;
    }

    else if(strcmp(campo, "nomeestacao") == 0){
        flags->nome_estacao = true;
    }
        
    else if(strcmp(campo, "codlinha") == 0){
        flags->cod_linha = true;
    }

    else if(strcmp(campo, "nomelinha") == 0){
        flags->nome_linha = true;
    }

    else if(strcmp(campo, "codproxestacao") == 0){
        flags->cod_prox_estacao = true;
    }

    else if(strcmp(campo, "distproxestacao") == 0){
        flags->dist_prox_estacao = true;
    }

    else if(strcmp(campo, "codlinhaintegra") == 0){
        flags->cod_linha_integra = true;
    }

    else if(strcmp(campo, "codestintegra") == 0){
        flags->cod_est_integra = true;
    }

    else return false;

    return true;
}

bool func_init_flag_field(FLAG_FIELD *flags){
    if(flags == NULL) return false;

    flags->cod_estacao = flags->cod_linha = flags->cod_prox_estacao = flags->dist_prox_estacao = flags->cod_est_integra = flags->cod_linha_integra = flags->nome_estacao = flags->nome_linha = false;

    return true;
}

/*================AUXILIAR================*/

bool func_attribute_value(char *valor, char *campo, DATA *d){
    if(valor == NULL || campo == NULL || d == NULL) return false;

    // Colocando o nome do campo em minusculo -> Permite que a mesma função seja usado no Create e nas outras funcionalidades
    func_str_to_lower(campo);

    // Verificando qual campo é e atribuindo valor no campo devido (temos duas opções para funcionar tanto para o CREATE tanto para WHERE)
    // Atribuindo código da estação
    if(strcmp(campo, "codestacao") == 0){
        if(valor[0] != '\0' && strcmp(valor, "NULO") != 0) data_set_cod_est(d, atoi(valor));
        else data_set_cod_est(d, -1);
    }

    // Atribuindo nome da estação
    else if(strcmp(campo, "nomeestacao") == 0){
        if(strcmp(valor, "NULO") != 0) data_set_nome_est(d, valor);
        else data_set_nome_est(d, NULL);
    }
        
    // Atribuindo código da linha
    else if(strcmp(campo, "codlinha") == 0){
        if(valor[0] != '\0' && strcmp(valor, "NULO") != 0) data_set_cod_lin(d, atoi(valor));
        else data_set_cod_lin(d, -1);
    }

    // Atribuindo nome da linha
    else if(strcmp(campo, "nomelinha") == 0){
        if(strcmp(valor, "NULO") != 0) data_set_nome_lin(d, valor);
        else data_set_nome_lin(d, NULL);
    }

    // Atribuindo código próxima estação
    else if(strcmp(campo, "codproxest") == 0 || strcmp(campo, "codproxestacao") == 0){
        if(valor[0] != '\0' && strcmp(valor, "NULO") != 0) data_set_cod_prox(d, atoi(valor));
        else data_set_cod_prox(d, -1);
    }

    // Atribuindo distância da próxima estação
    else if(strcmp(campo, "distproxestacao") == 0 || strcmp(campo, "distanciaproxest") == 0){
        if(valor[0] != '\0' && strcmp(valor, "NULO") != 0) data_set_dist(d, atoi(valor));
        else data_set_dist(d, -1);
    }

    // Atribuindo código da linha de integração
    else if(strcmp(campo, "codlinhainteg") == 0 || strcmp(campo, "codlinhaintegra") == 0){
        if(valor[0] != '\0' && strcmp(valor, "NULO") != 0) data_set_cod_lin_int(d, atoi(valor));
        else data_set_cod_lin_int(d, -1);
    }

    // Atribuindo código da estação de integração
    else if(strcmp(campo, "codestacaointeg") == 0 || strcmp(campo, "codestintegra") == 0){
        if(valor[0] != '\0' && strcmp(valor, "NULO") != 0) data_set_cod_est_int(d, atoi(valor));
        else data_set_cod_est_int(d, -1);
    }

    // O campo lido não corresponde a nenhum dos campos esperados
    else return false;

    return true;
}

bool func_insert_end(DATA *d, HEADER *h, FILE *bin){
    if(d == NULL || h == NULL || bin == NULL) return false;

    data_save_all(d, header_get_proxRRN(h), bin);
    header_set_proxRRN(h, header_get_proxRRN(h) + 1);

    return true;
}

void func_str_to_lower(char *str){
    if(str == NULL) return;

    // Tamanho da string a ser transformada
    int tam = strlen(str); 
    // Colocando letras em minúsculo
    for(int i = 0; i < tam; i++) str[i] = tolower(str[i]);
}