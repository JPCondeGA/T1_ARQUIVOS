#include "func.h"

bool func_select(){
    
    // Consideramos que o nome do arquivo não será maior que 30 bytes
    char nome_arq[TAM_BUFFER];
    // Lendo o nome do arquivo de entrada
    scanf(" %30s", nome_arq);
    // Ponteiro para o arquivo de entrada
    FILE* f = fopen(nome_arq, "rb"); //Abre o arquivo no modo de leitura binário.
    if(f == NULL) return false; //Se houve falha na abertura do arquivo, retorna "false".
    
    //Inicializa um registro de dados
    DATA* d = data_create();  
    if(d == NULL){
      fclose(f);
      return (false); //Se houve falha na inicialização do registro, fecha o arquivo e retorna "false".
    }
    
    uint RRN = 0; //Demarca qual o próximo registro que deve ser lido.
    int aux = 0; //Ajuda a detectar campos nulos.
    char* nomes; //data_get_nome_est() e data_get_nome_lin() retornam arrays dinâmicos que precisam ser desalocados após uso.
    bool existeregistro = false; //Detecta caso não há registros para imprimir.
    while (1){
      
      data_load_all(d, RRN, f);
      if (feof(f)) break; //Checa por END OF FILE depois de ler do arquivo.
      if (data_get_removido(d) != '1'){ //Pula registros removidos
      
        //Para todos os campos: Verifica se está marcado como -1 (para tamanho fixo) ou 0 (para tamanho variável) e imprime "NULO" se sim, ou o valor do campo se não.
        aux = data_get_cod_est(d); 
        if (aux == -1) printf("NULO ");
        else printf("%d ", aux);
        
        aux = data_get_tam_nome_est(d);
        if (aux == 0) printf("NULO ");
        else{
          nomes = data_get_nome_est(d); 
          printf("%s ", nomes);
          free(nomes); //Desaloca o nome recebido. Como "nomes" será usado novamente em breve, não o apontamos para NULL imediatamente.
        }
        
        aux = data_get_cod_lin(d);
        if (aux == -1) printf("NULO ");
        else printf("%d ", aux);
        
        aux = data_get_tam_nome_lin(d);
        if (aux == 0) printf("NULO ");
        else{
          nomes = data_get_nome_lin(d);
          printf("%s ", nomes);
          free(nomes);
        }
        
        aux = data_get_cod_prox(d);
        if (aux == -1) printf("NULO ");
        else printf("%d ", aux);
        
        aux = data_get_dist(d);
        if (aux == -1) printf("NULO ");
        else printf("%d ", aux);
        
        aux = data_get_cod_lin_int(d);
        if (aux == -1) printf("NULO ");
        else printf("%d ", aux);
        
        aux = data_get_cod_est_int(d);
        if (aux == -1) printf("NULO\n");
        else printf("%d\n", aux);
        existeregistro = true;
      }
      RRN++;  //Atualiza o contador de registros.
    }
  if (!existeregistro) printf("Registro inexistente.\n");
  nomes = NULL; //"nomes" recebe NULL para evitar o acesso indevido a um local da memória.
  fclose(f);
  data_delete(&d);
  return true;
}
