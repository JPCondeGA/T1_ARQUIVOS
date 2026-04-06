#include "../CONTEXTO/contexto.h"

bool func_update(){
  
  // Consideramos que o nome do arquivo não será maior que 30 bytes
  char nome_arq[31];
  // Lendo o nome do arquivo de entrada
  scanf(" %30s", nome_arq);
  // Ponteiro para o arquivo de entrada
  FILE* f = fopen(nome_arq, "rb+"); //Abre o arquivo no modo de leitura e escritura binário.
  if(f == NULL){
    printf("Falha no processamento do arquivo.\n");
    return false; //Se houve falha na abertura do arquivo, avisa o usuário e retorna "false".
  }
  
  HEADER* h; //Registro de cabeçalho.
  DATA *d, *filtro, *carimbo; //Registro de dados, registro auxiliar para realizar a busca do usuário e registro auxiliar para guardar os valores que o usuário quer substitutuir.
  ARVORE *ar_nomes, *ar_pares; //Árvores que irão guardar quantas estações e pares de códigos de estação diferentes existem no registro de dados, respectivamente.

  filtro = data_create();
  carimbo = data_create();
  if (!func_inicializar(&h, &d) || filtro == NULL || carimbo == NULL){
    //Caso houver erro na inicialização de alguma estrutura, avisa o usuário e retorna false.
    data_delete(&filtro); //data_delete() confere se o ponteiro passado é NULL, então não há necessidade de realizar mais testes para evitar double free().
    data_delete(&carimbo);
    fclose(f); //func_inicializar também já libera quaisquer estruturas falhas.
    printf("Falha no processamento do arquivo.\n");
    return false;
  }
  
  header_load_all(h, f);
  if (!func_inicializar_arvores(&ar_nomes, &ar_pares, h, d, f)){
    //func_inicializar_arvores libera as árvores falhas, restando apenas liberar os registros.
    data_delete(&d);
    data_delete(&filtro);
    data_delete(&carimbo);
    header_delete(&h);
    fclose(f);
    printf("Falha no processamento do arquivo.\n");
    return false;
  }
  
  
  //Inicialização da funcionalidade finalizada!
  header_set_status(h, REMOVIDO); //Arquivo foi aberto para escrita e seu status deve ser atualizado.
  header_save_field(h, REMOVIDO, f); //Salva a mudança no registro de cabeçalho.
  
  FLAG_FIELD flag_filtro, flag_carimbo; //Marcam quais campos possuem filtros ativos para a busca e quais campos serão alterados.
  if (!func_init_flag_field(&flag_filtro) || !func_init_flag_field(&flag_carimbo)){
    data_delete(&d);
    data_delete(&filtro);
    data_delete(&carimbo);
    header_delete(&h);
    avl_apagar_arvore(&ar_nomes);
    avl_apagar_arvore(&ar_pares);
    fclose(f);
    printf("Falha no processamento do arquivo.\n");
    return false;
  }
  
  uint RRN = 0; //Demarca qual o próximo registro que deve ser lido.
  char* nomeest; //Variável auxiliar para receber data_get_nome_est().
  uint qtdeUpd; //Guarda a quantidade de alterações a serem feitas.
  uint qtdeCampos; //Guarda a quantidade de campos que serão usados para a busca e a quantidade de campos que serão alterados.
  char campo[16]; //Guarda nomes de campos do registro. Os maiores nomes de campo possuem 15 caracteres + '/0', portanto tamanho 16.
  char valor[TAM_BUFFER]; //Guarda valores que podem estar salvos em campos de registro.
  scanf("%d", &qtdeUpd);
  
  while (qtdeUpd > 0){
  
    scanf("%d", &qtdeCampos);
    
    while (qtdeCampos > 0){ //O seguinte laço recebe um dos pares que serão buscados, portanto deve repetir um número de vezes igual ao número de pares.
      
      scanf(" %s", campo);
      func_attribute_flag_field(&flag_filtro, campo); //Atualiza "flag_filtro" para marcar que o campo na string "campo" será usado como filtro.
      ScanQuoteString(valor);
      func_attribute_value(valor, campo, filtro); //Salva o valor inserido no filtro.
      qtdeCampos--;
    }
    
    scanf("%d", &qtdeCampos);
    
    while (qtdeCampos > 0){
      
      scanf(" %s", campo);
      func_attribute_flag_field(&flag_carimbo, campo); //Atualiza "flag_carimbo" para marcar que o campo na string "campo" será usado para alterar o registro original.
      ScanQuoteString(valor);
      func_attribute_value(valor, campo, carimbo); //Salva o valor inserido no carimbo.
      qtdeCampos--;
    }
    
    RRN = 0; //Vamos iniciar a busca pelo início do registro.
    while (1){
      data_load_all(d, RRN, f);
      if (feof(f)) break; //Checa por END OF FILE depois de ler do arquivo.
      if (data_get_removido(d) != '1'){ //Pula registros removidos
        if (func_where_compare(flag_filtro, d, filtro)){ //Se encontrar um registro que se encaixa no filtro, inicia a lógica de alteração.
          
          if (func_checa_nome_alterado(&flag_carimbo)){ //Caso o campo de nomes tenha sido marcado para alteração, é necessário remover seu nome antigo da árvore e inserir seu nome novo.
            //Remoção do nome antigo          
            //Se um nó foi removido da árvore de nomes, então sua frequência era 1 e não há mais estações com esse nome no registro. Portanto, o cabeçalho é atualizado.
            nomeest = data_get_nome_est(d); //"d" possui nome atual.
            if (avl_remover(ar_nomes, nomeest, 0, 0)) header_set_nmr_estacoes(h, header_get_nmr_estacoes(h) - 1);
            free(nomeest);
            
            //Inserção do nome novo
            //Se um nó for inserido na árvore (não apenas atualização de frequência), então seu nome era diferente de todos os outros.
            nomeest = data_get_nome_est(carimbo); //"carimbo" possui novo nome.
            if (avl_inserir(ar_nomes, nomeest, 0, 0)) header_set_nmr_estacoes(h, header_get_nmr_estacoes(h) + 1);
            free(nomeest);
            nomeest = NULL;
            //Note que se um nome foi alterado para ele mesmo, ele é removido e logo em seguida re-inserido.
            }
          
          if (func_checa_par_alterado(&flag_carimbo)){ //Mesma lógica porém para pares de codEstacao e codProxEstacao.
              
            if (avl_remover(ar_pares, NULL, data_get_cod_est(d), data_get_cod_prox(d))) header_set_nmr_pares(h, header_get_nmr_pares(h) - 1);
            if (avl_inserir(ar_pares, NULL, data_get_cod_est(carimbo), data_get_cod_prox(carimbo))) header_set_nmr_pares(h, header_get_nmr_pares(h) + 1);
          }
          
          func_copy_data(&flag_carimbo, d, carimbo); //Copia todos os campos em "carimbo" marcados na flag em "d" 
          data_save_all(d, RRN, f); //Salva as alterações do registro no arquivo.
          
        }
      }
      RRN++;  //Atualiza o contador de registros.
    }
    func_init_flag_field(&flag_filtro); //Apaga a seleção de filtros atual.
    func_init_flag_field(&flag_carimbo); //Apaga a seleção de campos atual.
    qtdeUpd--;
  }
  
  header_set_status(h, '1'); //Atualiza o status do arquivo, agora que foi regularizado novamente.
  header_save_field(h, STATUS, f); //Salva apenas os campos que haviam possibilidade de alteração.
  header_save_field(h, EST, f);
  header_save_field(h, PARES, f);
  
  fclose(f); //Fecha o arquivo e salva as alterações.
  
  //Desaloca todas as estruturas.
  data_delete(&d);
  data_delete(&filtro);
  data_delete(&carimbo);
  header_delete(&h);
  avl_apagar_arvore(&ar_nomes);
  avl_apagar_arvore(&ar_pares);
  //"flags_filtro" e "flags_carimbo" não precisam ser apagadas porque são structs estáticas, não dinâmica.
  
  BinarioNaTela(nome_arq); //Execução da funcionalidade foi terminada.
  
  return true;
}
