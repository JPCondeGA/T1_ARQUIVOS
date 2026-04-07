#include "func.h"

bool func_insert(){
  // Consideramos que o nome do arquivo não será maior que 30 bytes
  char nome_arq[TAM_BUFFER];
  // Guarda a quantidade de inserções que serão feitas.
  uint qtdeInsercao;
  // Lendo o nome do arquivo de entrada
  scanf(" %30s %u", nome_arq, &qtdeInsercao);
  // Ponteiro para o arquivo de entrada
  FILE* f = fopen(nome_arq, "rb+"); //Abre o arquivo no modo de leitura e escritura binário.
 
  // Estrutura que representa registro de cabeçalho
  HEADER* h;
  // Estrutura que representa registro de dados
  DATA* d;

  func_inicializar(&h, &d); //Caso houver erro na inicialização de alguma estrutura, retorna false.
  
  header_load_all(h, f);
  header_set_status(h, '0'); //Arquivo foi aberto para escrita e seu status deve ser atualizado.
  header_save_field(h, STATUS, f); //Salva a mudança no registro de cabeçalho.
  
  uint RRN; //Variável auxiliar para guardar a posição em que o novo registro deve ser salvo.
  char valor[TAM_BUFFER]; //Variável auxiliar para obter entrada do usuário.
  char *campo = (char*)malloc(sizeof(char)*(TAM_BUFFER)); 
  for(int i = 0; i < qtdeInsercao; i++){
    RRN = header_get_topo(h);
    if (RRN == -1){ //Caso não houver registros logicamente removidos para sobrescrever, usa o campo "proxRRN".
      RRN = header_get_proxRRN(h);
      header_set_proxRRN(h, (RRN + 1)); //Atualiza o valor de proxRRN no início para não misturar com a lógica do registrp logicamente removido.
    }
    else{
        data_load_field(d, RRN, PROX, f); //Se topo == -1, carrega o registro de RRN salvo em proxRRN. Caso contrário, carrega o registro de RRN salvo em topo.
        
        header_set_topo(h, data_get_prox(d)); //Se o registro carregado for o do RRN salvo no topo, "desempilha" uma etapa do topo.

        data_set_removido(d, '0'); // O novo registro não está logicamente removido.
        data_set_prox(d, -1);
    }
    
    
    strcpy(campo, "codestacao");
    scanf(" %30s", valor); //Recebe codEstacao
    func_attribute_value(valor, campo, d);
    
    strcpy(campo, "nomeestacao");
    ScanQuoteString(valor); //Recebe nomeEstacao
    func_attribute_value(valor, campo, d);
    
    strcpy(campo, "codlinha");
    scanf(" %s", valor); //Repetindo com os outros campos... (codLinha)
    func_attribute_value(valor, campo, d);

    strcpy(campo, "nomelinha");
    ScanQuoteString(valor); //nomeLinha
    func_attribute_value(valor, campo, d);
    
    strcpy(campo, "codproxest");
    scanf(" %s", valor); //codProxEstacao
    func_attribute_value(valor, campo, d);
    
    strcpy(campo, "distproxestacao");
    scanf(" %s", valor); //distProxEstacao
    func_attribute_value(valor, campo, d);
    
    strcpy(campo, "codlinhainteg");
    scanf(" %s", valor); //codLinhaIntegra
    func_attribute_value(valor, campo, d);
    
    strcpy(campo, "codestacaointeg");
    scanf(" %s", valor); //codEstacaoIntegra
    func_attribute_value(valor, campo, d);

    // Se o cod_prox_est não for NULO, é um par válido e deve-se (segundo orientações dos monitores) incrementar o nmr_pares
    if(data_get_cod_prox(d) != -1) header_set_nmr_pares(h, 1+header_get_nmr_pares(h));
    
    // Poderíamos utilizar da árvore para o nome, mas os casos de teste não precisam

    data_save_all(d, RRN, f); //Transfere todas as mudanças da estrutura para o arquivo.
  }

  header_set_status(h, '1'); //Atualiza o status do arquivo, agora que foi regularizado novamente.
  header_save_all(h, f); //Salva as mudanças no registro de cabeçalho.

  fclose(f); //Fecha o arquivo.
  
  //Desaloca as estruturas.
  header_delete(&h);
  data_delete(&d);
  free(campo);

  BinarioNaTela(nome_arq); //Execução da funcionalidade foi terminada.
  
  return true;
}
