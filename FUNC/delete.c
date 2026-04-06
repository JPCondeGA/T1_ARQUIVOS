#include "../CONTEXTO/contexto.h"

bool func_delete(){
  
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
  DATA *d, *filtro; //Registro de dados e registro auxiliar para realizar a busca do usuário.
  ARVORE *ar_nomes, *ar_pares; //Árvores que irão guardar quantas estações e pares de códigos de estação diferentes existem no registro de dados, respectivamente.

  filtro = data_create();
  if (!func_inicializar(&h, &d) || filtro == NULL){
    //Caso houver erro na inicialização de alguma estrutura, avisa o usuário e retorna false.
    data_delete(&filtro); //data_delete() confere se o ponteiro passado é NULL, então não há necessidade de realizar mais testes para evitar double free().
    fclose(f); //func_inicializar também já libera quaisquer estruturas falhas.
    printf("Falha no processamento do arquivo.\n");
    return false;
  }
  
  header_load_all(h, f);
  if (!func_inicializar_arvores(&ar_nomes, &ar_pares, h, d, f)){
    //func_inicializar_arvores libera as árvores falhas, restando apenas liberar os registros.
    data_delete(&d);
    data_delete(&filtro);
    header_delete(&h);
    fclose(f);
    printf("Falha no processamento do arquivo.\n");
    return false;
  }
  
  
  //Inicialização da funcionalidade finalizada!
  header_set_status(h, REMOVIDO); //Arquivo foi aberto para escrita e seu status deve ser atualizado.
  header_save_field(h, REMOVIDO, f); //Salva a mudança no registro de cabeçalho.
  
  
  FLAG_FIELD flags; //Marca quais campos possuem filtros ativos.
  if (!func_init_flag_field(&flags)){
    data_delete(&d);
    data_delete(&filtro);
    header_delete(&h);
    avl_apagar_arvore(&ar_nomes);
    avl_apagar_arvore(&ar_pares);
    fclose(f);
    printf("Falha no processamento do arquivo.\n");
    return false;
  }
  
  uint RRN = 0; //Demarca qual o próximo registro que deve ser lido.
  char* nomeest; //Variável auxiliar para receber data_get_nome_est().
  uint qtdeRem; //Guarda a quantidade de remoções a serem feitas.
  uint qtdeFil; //Guarda a quantidade de filtros que serão adicionados.
  char campo[16]; //Guarda o campo a ser procurado para remoção. Os maiores nomes de campo possuem 15 caracteres + '/0', portanto tamanho 16.
  char valor[TAM_BUFFER]; //Guarda o valor que será procurado no registro de dados.
  scanf("%d", &qtdeRem);
  
  while (qtdeRem > 0){
  
    scanf("%d", &qtdeFil);
    
    while (qtdeFil > 0){ //O seguinte laço recebe um dos pares que serão buscados, portanto deve repetir um número de vezes igual ao número de pares.
      
      scanf(" %s", campo);
      func_attribute_flag_field(&flags, campo); //Atualiza a estrutura "flags" para marcar que o campo na string "campo" será usado como filtro.
      ScanQuoteString(valor);
      func_attribute_value(valor, campo, filtro); //Salva o valor inserido no filtro.
      qtdeFil--;
    }
    
    RRN = 0; //Vamos iniciar a busca pelo início do registro.
    while (1){
      data_load_all(d, RRN, f);
      if (feof(f)) break; //Checa por END OF FILE depois de ler do arquivo.
      if (data_get_removido(d) != '1'){ //Pula registros removidos
        if (func_where_compare(flags, d, filtro)){ //Se encontrar um registro que se encaixa no filtro, o remove logicamente
          data_set_removido(d, '1'); //Marca seu registro como logicamente removido.
          data_set_prox(d, header_get_topo(h)); //Salva o último valor do topo em "próximo".
          header_set_topo(h, RRN); //Empilha o registro no cabeçalho.
          
          //Se um nó foi removido da árvore de nomes, então sua frequência era 1 e não há mais estações com esse nome no registro. Portanto, o cabeçalho é atualizado.
          nomeest = data_get_nome_est(d);
          if (avl_remover(ar_nomes, nomeest, 0, 0)) header_set_nmr_estacoes(h, header_get_nmr_estacoes(h) - 1);
          free(nomeest);
          nomeest = NULL;
          
          //Mesma lógica porém para pares de codEstacao e codProxEstacao.  
          if (avl_remover(ar_pares, NULL, data_get_cod_est(d), data_get_cod_prox(d))) header_set_nmr_pares(h, header_get_nmr_pares(h) - 1);
          
          //Salva apenas os campos que foram alterados.
          data_save_field(d, RRN, REMOVIDO, f);
          data_save_field(d, RRN, PROX, f);
          //O header é capaz de mudar mais vezes, portanto é salvo fora do laço.
        }
      }
      RRN++;  //Atualiza o contador de registros.
    }
    func_init_flag_field(&flags); //Apaga a seleção de filtros atual.
    qtdeRem--;
  }
  nomeest = NULL;
  
  
  //Salva o header após não ocorrer mais mudanças nele.
  header_save_field(h, TOP, f); //topo
  header_save_field(h, EST, f); //nroEstacoes
  header_save_field(h, PARES, f); //nroParesEstacao
  
  
  //Remoções feitas! Encerrando a execução do programa.
  header_set_status(h, '1'); //Atualiza o status do arquivo, agora que foi regularizado novamente.
  header_save_field(h, STATUS, f); //Salva a mudança no registro de cabeçalho.
  fclose(f); //Salva o arquivo
  
  //Apaga todas as estruturas de dados utilizadas.
  avl_apagar_arvore(&ar_nomes);
  avl_apagar_arvore(&ar_pares);
  data_delete(&d);
  data_delete(&filtro);
  header_delete(&h);
  //"flags" não precisa ser apagada porque é uma struct estática, não dinâmica.
  
  
  BinarioNaTela(nome_arq);
  
  return true;
}
