#include "func.h"

bool func_insert(){
  
  // Consideramos que o nome do arquivo não será maior que 30 bytes
  char nome_arq[31];
  // Guarda a quantidade de inserções que serão feitas.
  uint qtdeInsercao;
  // Lendo o nome do arquivo de entrada
  scanf(" %30s %u", nome_arq, &qtdeInsercao);
  // Ponteiro para o arquivo de entrada
  FILE* f = fopen(nome_arq, "rb+"); //Abre o arquivo no modo de leitura e escritura binário.
  if(f == NULL) return false; //Se houve falha na abertura do arquivo, retorna "false".
  
  HEADER* h;
  DATA* d;
  ARVORE *ar_nomes, *ar_pares;
  if (!func_inicializar(&h, &d)){ //Caso houver erro na inicialização de alguma estrutura, retorna false.
    fclose(f);
    printf("Falha no processamento do arquivo.\n");
    return false;
  }
  if (!func_inicializar_arvores(&ar_nomes, &ar_pares, h, d, f)){
    data_delete(&d); //header_delete e data_delete() conferem se o ponteiro passado é NULL, então não há necessidade de realizar mais testes para evitar double free().
    header_delete(&h);
    fclose(f);
    printf("Falha no processamento do arquivo.\n");
    return false;
  }
  
  header_load_all(h, f);
  header_set_status(h, '0'); //Arquivo foi aberto para escrita e seu status deve ser atualizado.
  header_save_field(h, REMOVIDO, f); //Salva a mudança no registro de cabeçalho.
  
  uint RRN; //Variável auxiliar para guardar a posição em que o novo registro deve ser salvo.
  char campo[31]; //Variável auxiliar para obter entrada do usuário.
  char *nomeest; //Variável auxiliar para tratar a saída de data_get_nome_est(), que é um array dinâmico que deve ser desalocado antes do término da execução.
  
  while (qtdeInsercao > 0){
    RRN = header_get_topo(h);
    if (RRN == -1){ //Caso não houver registros logicamente removidos para sobrescrever, usa o campo "proxRRN".
      RRN = header_get_proxRRN(h);
      header_set_proxRRN(h, (RRN + 1)); //Atualiza o campo de proxRRN no início para não misturar com a lógica do campo logicamente removido.
      }
    
    data_load_all(d, RRN, f); //Se topo == -1, carrega o registro de RRN salvo em proxRRN. Caso contrário, carrega o registro de RRN salvo em topo.
    if (header_get_topo(h) != -1){
      header_set_topo(h, data_get_prox(d)); //Se o registro carregado for o do RRN salvo no topo, "desempilha" uma etapa do topo.
    }
    
    data_set_removido(d, '0'); //O novo registro não está logicamente removido.
    
    scanf(" %s", campo); //Recebe codEstacao
    if (strcmp(campo, "NULO") == 0) data_set_cod_est(d, -1); //Confere se o usuário inseriu valor nulo.
    else data_set_cod_est(d, atoi(campo)); //Salva na estrutura
    
    ScanQuoteString(campo); //Recebe nomeEstacao
    if (strcmp(campo, "") == 0) data_set_nome_est(d, ""); //Confere se o usuário inseriu valor nulo.
    else data_set_nome_est(d, campo); //Salva na estrutura
    
    scanf(" %s", campo); //Repetindo com os outros campos... (codLinha)
    if (strcmp(campo, "NULO") == 0) data_set_cod_lin(d, -1);
    else data_set_cod_lin(d, atoi(campo));
    
    ScanQuoteString(campo); //nomeLinha
    if (strcmp(campo, "") == 0) data_set_nome_lin(d, "");
    else data_set_nome_lin(d, campo);
    
    scanf(" %s", campo); //codProxEstacao
    if (strcmp(campo, "NULO") == 0) data_set_cod_prox(d, -1);
    else data_set_cod_prox(d, atoi(campo));
    
    scanf(" %s", campo); //distProxEstacao
    if (strcmp(campo, "NULO") == 0) data_set_dist(d, -1);
    else data_set_dist(d, atoi(campo));
    
    scanf(" %s", campo); //codLinhaIntegra
    if (strcmp(campo, "NULO") == 0) data_set_cod_lin_int(d, -1);
    else data_set_cod_lin_int(d, atoi(campo));
    
    scanf(" %s", campo); //codEstacaoIntegra
    if (strcmp(campo, "NULO") == 0) data_set_cod_est_int(d, -1);
    else data_set_cod_est_int(d, atoi(campo));
    
    data_save_all(d, RRN, f); //Transfere todas as mudanças da estrutura para o arquivo.
    
    nomeest = data_get_nome_est(d); //Recebe o nome da estação que acabou de ser adicionada ao arquivo.
    //Se um nó for inserido na árvore (não apenas atualização de frequência), então seu nome era diferente de todos os outros.
    if (avl_inserir(ar_nomes, nomeest, 0, 0)) header_set_nmr_estacoes(h, header_get_nmr_estacoes(h) + 1);
    free(nomeest); //"nomeest" será usado novamente em breve, mas é necessário mudar seu valor para NULL imediatamente para evitar riscos de double free().
    nomeest = NULL;
    
    //Se um nó for inserido na árvore (não apenas atualização de frequência), então seu par era diferente de todos os outros.
    if (avl_inserir(ar_pares, NULL, data_get_cod_est(d), data_get_cod_prox(d))) header_set_nmr_pares(h, header_get_nmr_pares(h) + 1);
    
    qtdeInsercao--; //Atualiza o número de inserções a serem feitas.
  }
  
  header_set_status(h, '1'); //Atualiza o status do arquivo, agora que foi regularizado novamente.
  header_save_all(h, f); //Salva as mudanças no registro de cabeçalho.
  
  fclose(f); //Fecha o arquivo.
  
  //Desaloca as estruturas.
  header_delete(&h);
  data_delete(&d);
  avl_apagar_arvore(&ar_nomes);
  avl_apagar_arvore(&ar_pares);
  
  BinarioNaTela(nome_arq); //Execução da funcionalidade foi terminada.
  
  return true;
}
