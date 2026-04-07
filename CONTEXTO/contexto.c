#include "contexto.h"

/*================WHERE e DELETE================*/

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
        ok = ok && func_attribute_flag_field(flags, campo);
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

/*================UPDATE================*/

bool func_copy_data(FLAG_FIELD *flag_carimbo, DATA *destino, DATA *fonte){
  //Copia todos os valores de "fonte" para "destino", pulando os campos em que "flag_carimbo" é "false".
  bool semErros = true; //Confere se a função falhar em algum momento.
  char* nomeest; //Variável auxiliar para receber data_get_nome_est() e data_get_nome_lin().
  
  if (flag_carimbo->cod_estacao) semErros = semErros && data_set_cod_est(destino, data_get_cod_est(fonte));
  
  if (flag_carimbo->cod_linha) semErros = semErros && data_set_cod_lin(destino, data_get_cod_lin(fonte));
  
  if (flag_carimbo->cod_prox_estacao) semErros = semErros && data_set_cod_prox(destino, data_get_cod_prox(fonte));
  
  if (flag_carimbo->dist_prox_estacao) semErros = semErros && data_set_dist(destino, data_get_dist(fonte));
  
  if (flag_carimbo->cod_linha_integra) semErros = semErros && data_set_cod_lin_int(destino, data_get_cod_lin_int(fonte));
  
  if (flag_carimbo->cod_est_integra) semErros = semErros && data_set_cod_est_int(destino, data_get_cod_est_int(fonte));
  
  if (flag_carimbo->nome_estacao){
    nomeest = data_get_nome_est(fonte);
    semErros = semErros && data_set_nome_est(destino, nomeest);
    free(nomeest);
    nomeest = NULL;
    }
    
  if (flag_carimbo->nome_linha){
    nomeest = data_get_nome_lin(fonte);
    semErros = semErros && data_set_nome_lin(destino, nomeest);
    free(nomeest);
    nomeest = NULL;
    }
    
  return semErros;
}

bool func_checa_nome_alterado(FLAG_FIELD *flag_carimbo){
  return flag_carimbo->nome_estacao; //Confere se o nome da estrutura foi marcado para ser alterado.
}

bool func_checa_par_alterado(FLAG_FIELD *flag_carimbo){
  if (flag_carimbo->cod_estacao || flag_carimbo->cod_prox_estacao) return true; //Confere se o código da estação ou o código da próxima estação foi marcado para ser alterado.
  return false;
}

/*================FLAGS================*/

bool func_attribute_flag_field(FLAG_FIELD *flags, char *campo){
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

bool func_inicializar(HEADER** h, DATA** d){
    
    //Inicializa um registro de dados.
    *d = data_create();  
    if(*d == NULL) return false;
    
    //Inicializa um registro de cabeçalho.
    *h = header_create();
    if(*h == NULL){
      data_delete(d); //Não é necessário passar o endereço como entrada, pois "d" já é ponteiro de ponteiro. 
      return false;
    }
    
    return true;
}

bool func_inicializar_arvores(ARVORE** nomes, ARVORE** pares, HEADER* h, DATA* d, FILE* f){
  
  //Todas as funções que chamam esta função conferem se "h == NULL || d == NULL" antes de chamar esta função, portanto não realizamos o teste novamente.
  
  *nomes = avl_criar_arvore(false); //Árvore que irá guardar quantas estações diferentes existem no registro de dados.
  *pares = avl_criar_arvore(true); //Árvore que irá guardar quantos pares de estação e código existem no registro de dados.
  if (*nomes == NULL || *pares == NULL){
  
    avl_apagar_arvore(nomes); //Não é necessário passar o endereço como entrada, pois "nomes" e "pares" já são ponteiro de ponteiro. 
    avl_apagar_arvore(pares); //avl_apagar_arvore() confere se os ponteiros passados são nulos, então não é necessário se preocupar com double free().
    return false;
  }
  
  int RRN = 0; //Marca qual o próximo registro a ser lido.
  char* nomeest; //Variável auxiliar para receber valores do registro.
  header_load_all(h, f); //Caso o cabeçalho não tenha sido inicializado, o carrega.
  
  while(1){
      data_load_all(d, RRN, f);
      if (feof(f)) break; //Checa por END OF FILE depois de ler do arquivo.
      if (data_get_removido(d) != '1'){ //Pula registros removidos
      
        //Preenche ambas as árvores com as informações do registro
        nomeest = data_get_nome_est(d); //data_get_nome_est() retorna um array dinâmico que precisa ser desalocado após ser usado.
        avl_inserir(*nomes, nomeest, 0, 0);
        free(nomeest);
        nomeest = NULL;
        avl_inserir(*pares, NULL, data_get_cod_est(d), data_get_cod_prox(d));
      }
      RRN++;  //Atualiza o contador de registros.
  }
  
  
  /*
  if(avl_get_n(ar_nomes) != header_get_nmr_estacoes(h) || avl_get_n(ar_pares) != header_get_nmr_pares(h)){
    //Se houver divergência entre o cabeçalho e as árvores, provavelmente algum nó não foi inserido corretamente.
    
    avl_apagar_arvore(&ar_nomes); //avl_apagar_arvore() confere se o ponteiro passado é NULL, então não há necessidade de realizar mais testes para evitar double free().
    avl_apagar_arvore(&ar_pares);
    return false;
  }
  */
  
  return true;
  
}

bool func_consistence(HEADER *h){
    // Retorna-se false para não entrar em nenhum possível if que acesse esses ponteiros
    if(h == NULL) return false;

    return header_get_status(h) == '1';
}

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

    int tam = strlen(str); 
    for(int i = 0; i < tam; i++) str[i] = tolower(str[i]);
    return;
}
