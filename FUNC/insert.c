#include "func.h"

/*=============FUNÇÕES OCULTAS============*/

/* Checa a pilha de registros logicamente removidos e insere no próximo RRN disponível ou no topo da pilha.
  - Recebe um ponteiro para a estrutura que representa um registro de dados na qual será colocado os valores a serem inseridos, um ponteiro para estrutura que representa o registro de cabeçalho e um ponteiro para arquivo. */
void func_insert(DATA *d, HEADER *h, FILE *f);

/*=============INTERFACE============*/

void func_insert_interface(){
  bool ok = true; // Verifica se a execução ocorreu corretamente
  char nome_arq[TAM_BUFFER]; // Consideramos que o nome do arquivo não será maior que TAM_BUFFER
  uint n; // Guarda a quantidade de inserções que serão feitas.
  HEADER* h; // Estrutura que representa registro de cabeçalho
  DATA* d; // Estrutura que representa registro de dados

  scanf(" %30s %u", nome_arq, &n); // Lendo o nome do arquivo de entrada
  
  // Ponteiro para o arquivo de entrada
  FILE* f = fopen(nome_arq, "rb+"); // Abre o arquivo no modo de leitura e escritura binário.
  if(f == NULL) ok = false;

  ok = ok && cntx_inicializar(&h, &d); // Caso houver erro na inicialização de alguma estrutura, retorna false.
  
  // Verificando se é possível checar a consistência
  if(h != NULL && f != NULL){
    header_carregar(h, f); // Carregando cabeçalho atual do arquivo
    ok = ok && cntx_checa_consistencia(h, false, NULL); // Checando a consistência
  }

  // Se não houve erros até então 
  if(ok){
      cntx_altera_consistencia(h, INCONSISTENTE, f); // Mudando estado atual do arquivo

      // Realizando as inserções
      for(int i = 0; i < n; i++){
         func_insert(d, h, f); // Realizando a leitura dos valores dos campos e gravamento na posição correta
      }
      
      // Atualizando cabeçalho
      header_set_status(h, CONSISTENTE); //Atualiza o status do arquivo, agora que foi regularizado novamente.
      header_salvar(h, f); //Salva as mudanças no registro de cabeçalho.
  }  

  if(f != NULL) fclose(f), f = NULL; //Fecha o arquivo.
  
  //Desaloca as estruturas (verificações internas)
  header_apagar(&h);
  data_apagar(&d);

  if(ok) BinarioNaTela(nome_arq); //Execução da funcionalidade foi terminada.
  else printf("Falha no processamento do arquivo.\n");
}

/*=============PRINCIPAL============*/

void func_insert(DATA *d, HEADER *h, FILE *f){
    char valor[TAM_BUFFER]; // Variável auxiliar para obter entrada do usuário.
    uint RRN = header_get_topo(h); // Armazena o RRN em que será inserido o novo registro
    
    // Adicionado no proxRRN
    if (RRN == -1){ // Caso não houver registros logicamente removidos para sobrescrever, usa o campo "proxRRN".
      RRN = header_get_proxRRN(h);
      header_set_proxRRN(h, (RRN + 1)); // Atualiza o valor de proxRRN no início para não misturar com a lógica do registrp logicamente removido.
    }
    // Adicionando no lugar de um logicamente removido
    else{
        data_carregar_campo(d, RRN, PROXIMO, f); //Se topo == -1, carrega o registro de RRN salvo em proxRRN. Caso contrário, carrega o registro de RRN salvo em topo.
        
        header_set_topo(h, data_get_proximo(d)); //Se o registro carregado for o do RRN salvo no topo, "desempilha" uma etapa do topo.

        data_set_removido(d, '0'); // O novo registro não está logicamente removido.
        data_set_proximo(d, -1);
    }
    
    // Lendo e atribuindo valores na estrutura

    scanf(" %30s", valor); //Recebe codEstacao
    if(strcmp(valor, "NULO") != 0) data_set_cod_estacao(d, atoi(valor));
    else data_set_cod_estacao(d, -1);
    
    ScanQuoteString(valor); //Recebe nomeEstacao
    if(strcmp(valor, "NULO") != 0) data_set_nome_estacao(d, valor);
    else data_set_nome_estacao(d, NULL);
    
    scanf(" %30s", valor); //Repetindo com os outros campos... (codLinha)
    if(strcmp(valor, "NULO") != 0) data_set_cod_linha(d, atoi(valor));
    else data_set_cod_linha(d, -1);

    ScanQuoteString(valor); //nomeLinha
    if(strcmp(valor, "NULO") != 0) data_set_nome_linha(d, valor);
    else data_set_nome_linha(d, NULL);
    
    scanf(" %30s", valor); //codProxEstacao
    if(strcmp(valor, "NULO") != 0) data_set_cod_prox_estacao(d, atoi(valor));
    else data_set_cod_prox_estacao(d, -1);
    
    scanf(" %30s", valor); //distProxEstacao
    if(strcmp(valor, "NULO") != 0) data_set_dist_prox_estacao(d, atoi(valor));
    else data_set_dist_prox_estacao(d, -1);

    scanf(" %30s", valor); //codLinhaIntegra
    if(strcmp(valor, "NULO") != 0) data_set_cod_linha_integra(d, atoi(valor));
    else data_set_cod_linha_integra(d, -1);
    
    scanf(" %30s", valor); //codEstacaoIntegra
    if(strcmp(valor, "NULO") != 0) data_set_cod_est_integra(d, atoi(valor));
    else data_set_cod_est_integra(d, -1);

    // Se o cod_prox_est não for NULO, é um par válido e deve-se (segundo orientações dos monitores) incrementar o nmr_pares
    if(data_get_cod_prox_estacao(d) != -1) header_set_nmr_pares_estacao(h, 1+header_get_nmr_pares_estacao(h));
    
    // Poderíamos utilizar da árvore para o nome, mas os casos de teste não precisam

    data_salvar(d, RRN, f); //Transfere todas as mudanças da estrutura para o arquivo.
}