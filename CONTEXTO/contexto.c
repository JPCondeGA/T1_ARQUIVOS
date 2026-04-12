#include "contexto.h"

/*================WHERE e DELETE================*/

bool cntx_where_input(FLAG_FIELD *flags, DATA *filtro, int m){
    bool ok = true; // Verifica se a função foi executada com sucesso
    char campo[TAM_BUFFER]; // Guarda o campo a ser especificado
    char valor[TAM_BUFFER]; // Guarda o valor desejado para determinado campo
    ok = cntx_init_flag(flags); // Setando todos campos para false (as buscas anteriores não podem interferir nessa)
    
    // Lendo filtros
    for(int i = 0; i < m && ok; i++){
        scanf(" %30s", campo); // Lendo nome do campo que será especificado

        // Lendo o valor desse campo
        if(!strcmp(campo, "nomeEstacao") || !strcmp(campo, "nomeLinha")) ScanQuoteString(valor); // Se o valor será uma string entre aspas, usamos a função apropriada
        else scanf(" %30s", valor); // Lendo como string, pois não sabemos se será "NULO" ou um número

        ok = cntx_atribuir_valor_data(valor, campo, filtro); // Atribuindo valor ao campo correto da estrutra
        ok = ok && cntx_atribuir_valor_flag(flags, campo); // Guardando que esse campo deve ser analisado na comparação
    }

    return ok;
}

bool cntx_where_compare(FLAG_FIELD flags, DATA *d, DATA *filtro){
    if(d == NULL || filtro == NULL) return false;

    // Se o registro está logicamente marcado como removido, não precisamos fazer as comparações
    if(data_get_removido(d) == '1') return false;
    
    // Resultado da comparção
    bool resp = true; 

    // Conferindo campos que são comparados (se uma comparação não bater, não precisamos verificar as demais)
    if(flags.cod_estacao) resp = cntx_comparar_data(d, filtro, COD_ESTACAO);
    if(resp && flags.cod_linha) resp = cntx_comparar_data(d, filtro, COD_LINHA);
    if(resp && flags.cod_prox_estacao) resp = cntx_comparar_data(d, filtro, COD_PROX_ESTACAO);
    if(resp && flags.dist_prox_estacao) resp = cntx_comparar_data(d, filtro, DIST_PROX_ESTACAO);
    if(resp && flags.cod_linha_integra) resp = cntx_comparar_data(d, filtro, COD_LINHA_INTEGRA);
    if(resp && flags.cod_est_integra) resp = cntx_comparar_data(d, filtro, COD_EST_INTEGRA);
    if(resp && flags.nome_estacao) resp = cntx_comparar_data(d, filtro, NOME_ESTACAO);
    if(resp && flags.nome_linha) resp = cntx_comparar_data(d, filtro, NOME_LINHA);

    return resp;
}

/*================UPDATE================*/

bool cntx_copiar_data(FLAG_FIELD *flag_carimbo, DATA *destino, DATA *fonte){
  //Copia todos os valores de "fonte" para "destino", pulando os campos em que "flag_carimbo" é "false".
  bool ok = true; //Confere se a função falhar em algum momento.
  char* nome_estacao; //Variável auxiliar para receber data_get_nome_est() e data_get_nome_lin().
  
  if (flag_carimbo->cod_estacao) ok = ok && data_set_cod_estacao(destino, data_get_cod_estacao(fonte));
  
  if (flag_carimbo->cod_linha) ok = ok && data_set_cod_linha(destino, data_get_cod_linha(fonte));
  
  if (flag_carimbo->cod_prox_estacao) ok = ok && data_set_cod_prox_estacao(destino, data_get_cod_prox_estacao(fonte));
  
  if (flag_carimbo->dist_prox_estacao) ok = ok && data_set_dist_prox_estacao(destino, data_get_dist_prox_estacao(fonte));
  
  if (flag_carimbo->cod_linha_integra) ok = ok && data_set_cod_linha_integra(destino, data_get_cod_linha_integra(fonte));
  
  if (flag_carimbo->cod_est_integra) ok = ok && data_set_cod_est_integra(destino, data_get_cod_est_integra(fonte));
  
  if (flag_carimbo->nome_estacao){
    nome_estacao = data_get_nome_estacao(fonte);
    ok = ok && data_set_nome_estacao(destino, nome_estacao);
    free(nome_estacao);
    nome_estacao = NULL;
  }
    
  if (flag_carimbo->nome_linha){
    nome_estacao = data_get_nome_linha(fonte);
    ok = ok && data_set_nome_linha(destino, nome_estacao);
    free(nome_estacao);
    nome_estacao = NULL;
  }
    
  return ok;
}

bool cntx_comparar_data(DATA *d1, DATA *d2, int8 op){
    // Para não entrar dentro de um possível if, em que use esses ponteiros 
    if(d1 == NULL || d2 == NULL) return false;

    bool resp; // Guarda o resultado da comparação das strings
    char *aux_str1; // Auxiliar para receber o retorno do get
    char *aux_str2; // Auxiliar para receber o retonro do get

    // Verificando o campo especificado
    switch(op){
        case COD_ESTACAO:
            return data_get_cod_estacao(d1) == data_get_cod_estacao(d2); // Comparando por get

        case COD_LINHA:
            return data_get_cod_linha(d1) == data_get_cod_linha(d2); // Comparando por get

        case COD_PROX_ESTACAO:
            return data_get_cod_prox_estacao(d1) == data_get_cod_prox_estacao(d2); // Comparando por get

        case DIST_PROX_ESTACAO:
            return data_get_dist_prox_estacao(d1) == data_get_dist_prox_estacao(d2); // Comparando por get

        case COD_LINHA_INTEGRA:
            return data_get_cod_linha_integra(d1) == data_get_cod_linha_integra(d2); // Comparando por get

        case COD_EST_INTEGRA:
            return data_get_cod_est_integra(d1) == data_get_cod_est_integra(d2); // Comparando por get

        case NOME_ESTACAO:
            aux_str1 = data_get_nome_estacao(d1); // Pegando o nome da primeira estrutura
            aux_str2 = data_get_nome_estacao(d2); // Pegando o nome da segunda estrutura
            
            if(aux_str1 != NULL && aux_str2 != NULL) // Verificando se a alocação ocorreu corretamente
                resp = (strcmp(aux_str1, aux_str2) == 0); // Guardando resultado da comparação

            // Desalocando
            if(aux_str1 != NULL) free(aux_str1), aux_str1 = NULL;
            if(aux_str2 != NULL) free(aux_str2), aux_str2 = NULL;

            return resp;

        case NOME_LINHA: 
            aux_str1 = data_get_nome_linha(d1); // Pegando o nome da primeira estrutura
            aux_str2 = data_get_nome_linha(d2); // Pegando o nome da segunda estrutura
            
            if(aux_str1 != NULL && aux_str2 != NULL) // Verificando se a alocação ocorreu corretamente
                resp = (strcmp(aux_str1, aux_str2) == 0); // Guardando resultado da comparação

            // Desalocando
            if(aux_str1 != NULL) free(aux_str1), aux_str1 = NULL;
            if(aux_str2 != NULL) free(aux_str2), aux_str2 = NULL;

            return resp;

        default:
            return false; // Em caso de campo inválido, retornamos false
    }
}

void cntx_imprimir_data(DATA *d){
    if(d == NULL) return; 

    char *aux_str; // Auxiliar que recebe os campos de tamanho variável

    // Imprimindo cada campo -> ordem especificada na Funcionalidade [2]

    if(data_get_cod_estacao(d) != -1) printf("%d ", data_get_cod_estacao(d)); // Campo de tamanho fixo não -> valor != -1
    else printf("NULO "); // Se nulo, imprimir NULO

    if(data_get_tam_nome_estacao(d) > 0){ // Campo de tamanho variável não nulo -> tamanho do campo > 0 
        aux_str = data_get_nome_estacao(d); 
        if(aux_str != NULL){ // Verificando se alocação do get ocorreu corretamente
            printf("%s ", aux_str); 
            free(aux_str); aux_str =NULL;
        }

    }
    else printf("NULO "); // Se nulo, imprimir NULO
    
    if(data_get_cod_linha(d) != -1) printf("%d ", data_get_cod_linha(d)); // Campo de tamanho fixo não -> valor != -1
    else printf("NULO ");

    if(data_get_tam_nome_linha(d) > 0){ // Campo de tamanho variável não nulo -> tamanho do campo > 0 
        aux_str = data_get_nome_linha(d); 
        if(aux_str != NULL){ // Verificando se alocação do get ocorreu corretamente
            printf("%s ", aux_str); 
            free(aux_str); aux_str =NULL;
        }

    }
    else printf("NULO "); // Se nulo, imprimir NULO

    if(data_get_cod_prox_estacao(d) != -1) printf("%d ", data_get_cod_prox_estacao(d)); // Campo de tamanho fixo não -> valor != -1
    else printf("NULO "); // Se nulo, imprimir NULO

    if(data_get_dist_prox_estacao(d) != -1) printf("%d ", data_get_dist_prox_estacao(d)); // Campo de tamanho fixo não -> valor != -1
    else printf("NULO "); // Se nulo, imprimir NULO

    if(data_get_cod_linha_integra(d) != -1) printf("%d ", data_get_cod_linha_integra(d)); // Campo de tamanho fixo não -> valor != -1
    else printf("NULO "); // Se nulo, imprimir NULO

    if(data_get_cod_est_integra(d) != -1) printf("%d\n", data_get_cod_est_integra(d)); // Campo de tamanho fixo não -> valor != -1
    else printf("NULO\n"); // Se nulo, imprimir NULO
}


/*================FLAGS================*/

bool cntx_atribuir_valor_flag(FLAG_FIELD *flags, char *campo){
    if(flags == NULL || campo == NULL) return false; // Verificando ponteiros
    
    cntx_str_min(campo); // Colocando nome do campo em minúsculo para maior flexibilidade (não case sensitive)

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

    else return false; // Campo passado é inválido

    return true;
}

bool cntx_init_flag(FLAG_FIELD *flags){
    if(flags == NULL) return false;

    flags->cod_estacao = flags->cod_linha = flags->cod_prox_estacao = flags->dist_prox_estacao = flags->cod_est_integra = flags->cod_linha_integra = flags->nome_estacao = flags->nome_linha = false;

    return true;
}

/*================AUXILIAR================*/

bool cntx_inicializar(HEADER** h, DATA** d){
    if(h == NULL || d == NULL) return false;

    //Inicializa um registro de dados.
    *d = data_criar();  
    if(*d == NULL) return false;
    
    //Inicializa um registro de cabeçalho.
    *h = header_criar();
    if(*h == NULL){
      data_apagar(d); //Não é necessário passar o endereço como entrada, pois "d" já é ponteiro de ponteiro. 
      return false;
    }
    
    return true;
}


bool cntx_checa_consistencia(HEADER *h, bool carrega, bool move, FILE *f){
    if(h == NULL) return false; // Retorna-se false para não entrar em nenhum possível if que acesse esses ponteiro
    if(carrega && f == NULL) return false;  // Se a opção de carregar estiver ativa, é necessário o arquivo estar aberto e correto

    if(carrega) header_carregar_campo(h, STATUS, move, f); 

    return header_get_status(h) == CONSISTENTE; // '1' = consistente; '0' = inconsistente
}

void cntx_altera_consistencia(HEADER *h, bool valor, bool move, FILE *f){
    if(h == NULL || f == NULL) return; // Verificando ponteiros

    header_set_status(h, valor); // Arquivo foi aberto para escrita e seu status deve ser atualizado.
    header_salvar_campo(h, STATUS,  move, f); // Salva a mudança no registro de cabeçalho.
}

bool cntx_atribuir_valor_data(char *valor, char *campo, DATA *d){
    if(valor == NULL || campo == NULL || d == NULL) return false;

    cntx_str_min(campo); // Colocando o nome do campo em minusculo -> Permite que a mesma função seja usado na funcionalidade [1] e em outras

    // Verificando qual campo é e atribuindo valor no campo devido 
    
        // Para verificar o campo, temos duas opções para funcionar tanto para a funcionalidade [1] tanto para as outras
        // Para verificar a nulidade do valor, temos duas opções para funcionar tanto para a funcionalidade [1] tanto para as outras
        // Nos campos que correspondem a inteiros, consideramos que se o valor não é nulo, é possível convertê-lo para inteiro

    // Atribuindo código da estação
    if(strcmp(campo, "codestacao") == 0){
        if(valor[0] != '\0' && strcmp(valor, "NULO") != 0) data_set_cod_estacao(d, atoi(valor)); // Se o campo não for nulo, atribuimos o valor correto
        else data_set_cod_estacao(d, -1); // Senão, -1 (campo de tamanho fixo)
    }

    // Atribuindo nome da estação
    else if(strcmp(campo, "nomeestacao") == 0){
        if(strcmp(valor, "NULO") != 0) data_set_nome_estacao(d, valor); // Se o campo não for nulo, atribuimos o valor correto
        else data_set_nome_estacao(d, NULL); // Senão, NULL (campo de tamanho fixo)
    }
        
    // Atribuindo código da linha
    else if(strcmp(campo, "codlinha") == 0){
        if(valor[0] != '\0' && strcmp(valor, "NULO") != 0) data_set_cod_linha(d, atoi(valor)); // Se o campo não for nulo, atribuimos o valor correto
        else data_set_cod_linha(d, -1); // Senão, -1 (campo de tamanho fixo)
    } 

    // Atribuindo nome da linha
    else if(strcmp(campo, "nomelinha") == 0){
        if(strcmp(valor, "NULO") != 0) data_set_nome_linha(d, valor); // Se o campo não for nulo, atribuimos o valor correto
        else data_set_nome_linha(d, NULL); // Senão, NULL (campo de tamanho fixo)
    }

    // Atribuindo código próxima estação
    else if(strcmp(campo, "codproxest") == 0 || strcmp(campo, "codproxestacao") == 0){
        if(valor[0] != '\0' && strcmp(valor, "NULO") != 0) data_set_cod_prox_estacao(d, atoi(valor)); // Se o campo não for nulo, atribuimos o valor correto
        else data_set_cod_prox_estacao(d, -1); // Senão, -1 (campo de tamanho fixo)
    }

    // Atribuindo distância da próxima estação
    else if(strcmp(campo, "distproxestacao") == 0 || strcmp(campo, "distanciaproxest") == 0){
        if(valor[0] != '\0' && strcmp(valor, "NULO") != 0) data_set_dist_prox_estacao(d, atoi(valor)); // Se o campo não for nulo, atribuimos o valor correto
        else data_set_dist_prox_estacao(d, -1); // Senão, -1 (campo de tamanho fixo)
    }

    // Atribuindo código da linha de integração
    else if(strcmp(campo, "codlinhainteg") == 0 || strcmp(campo, "codlinhaintegra") == 0){
        if(valor[0] != '\0' && strcmp(valor, "NULO") != 0) data_set_cod_linha_integra(d, atoi(valor)); // Se o campo não for nulo, atribuimos o valor correto
        else data_set_cod_linha_integra(d, -1); // Senão, -1 (campo de tamanho fixo)
    }

    // Atribuindo código da estação de integração
    else if(strcmp(campo, "codestacaointeg") == 0 || strcmp(campo, "codestintegra") == 0){
        if(valor[0] != '\0' && strcmp(valor, "NULO") != 0) data_set_cod_est_integra(d, atoi(valor)); // Se o campo não for nulo, atribuimos o valor correto
        else data_set_cod_est_integra(d, -1); // Senão, -1 (campo de tamanho fixo)
    }

    else return false; // O campo lido não corresponde a nenhum dos campos esperados

    return true;
}

void cntx_str_min(char *str){
    if(str == NULL) return;

    // Tamanho da string a ser transformada
    int tam = strlen(str); 
    // Colocando letras em minúsculo
    for(int i = 0; i < tam; i++) str[i] = tolower(str[i]);
    return;
}


