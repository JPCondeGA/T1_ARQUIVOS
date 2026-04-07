#include "../CONTEXTO/contexto.h"

bool func_delete(){
  // Verifica se a funcionalidade foi executada com sucesso
  bool ok = true;
  // Consideramos que o nome do arquivo não será maior que 30 bytes
  char nome_arq[TAM_BUFFER];

  HEADER* h; //Registro de cabeçalho.
  DATA *d, *filtro; //Registro de dados e registro auxiliar para realizar a busca do usuário.
  ARVORE *ar_nomes, *ar_pares; //Árvores que irão guardar quantas estações e pares de códigos de estação diferentes existem no registro de dados, respectivamente.

  // Lendo o nome do arquivo de entrada
  scanf(" %30s", nome_arq);
  // Ponteiro para o arquivo de entrada
  FILE* f = fopen(nome_arq, "rb+"); //Abre o arquivo no modo de leitura e escritura binário.
  if(f == NULL) ok = false;

  filtro = data_create();
  ok = ok && func_inicializar(&h, &d);
  
  //func_inicializar_arvores(&ar_nomes, &ar_pares, h, d, f);
  ar_nomes = avl_criar_arvore(false);
  if(ar_nomes == NULL) ok = false;

  ar_pares = avl_criar_arvore(true);
  if(ar_pares == NULL) ok = false;
  
  //Inicialização da funcionalidade finalizada!
  if(ok){
    header_load_all(h, f);

    header_set_status(h, '0'); //Arquivo foi aberto para escrita e seu status deve ser atualizado.
    header_save_field(h, STATUS, f); //Salva a mudança no registro de cabeçalho.
    
    
    FLAG_FIELD flags; //Marca quais campos possuem filtros ativos.
    func_init_flag_field(&flags);
    
    uint proxRRN = header_get_proxRRN(h); // Demarca até qual registro existe.
    char* nomeest; //Variável auxiliar para receber data_get_nome_est().
    uint n; //Guarda a quantidade de remoções a serem feitas.
    uint m; //Guarda a quantidade de filtros que serão adicionados.
    char campo[TAM_BUFFER]; //Guarda o campo a ser procurado para remoção. Os maiores nomes de campo possuem 15 caracteres + '/0', portanto tamanho 16.
    char valor[TAM_BUFFER]; //Guarda o valor que será procurado no registro de dados.
    bool primeira = true;
    scanf("%d", &n);
    
    for(int i = 0; i < n && ok; i++){
    
      scanf("%d", &m);

      func_init_flag_field(&flags);

      ok = func_where_input(&flags, filtro, m);

      for(uint j = 0; j < proxRRN && ok; j++){
        data_load_all(d, j, f);

        if (feof(f)) break; //Checa por END OF FILE depois de ler do arquivo.

        if (data_get_removido(d) != '1'){ //Pula registros removidos
          if(primeira){
            nomeest = data_get_nome_est(d);
            avl_inserir(ar_nomes, nomeest, 0, 0);
            free(nomeest); nomeest = NULL;
            if(data_get_cod_prox(d) != -1) 
              avl_inserir(ar_pares, NULL, data_get_cod_est(d), data_get_cod_prox(d));
          }


          if (func_where_compare(flags, d, filtro)){ //Se encontrar um registro que se encaixa no filtro, o remove logicamente
            data_set_removido(d, '1'); //Marca seu registro como logicamente removido.
            data_set_prox(d, header_get_topo(h)); //Salva o último valor do topo em "próximo".
            header_set_topo(h, j); //Empilha o registro no cabeçalho.
            
            //Salva apenas os campos que foram alterados.
            data_save_field(d, j, REMOVIDO, f);
            data_save_field(d, j, PROX, f);
            //O header é capaz de mudar mais vezes, portanto é salvo fora do laço.

            // Removendo da árvore
            nomeest = data_get_nome_est(d);
            avl_remover(ar_nomes, nomeest, 0, 0);
            free(nomeest); nomeest = NULL;
            avl_remover(ar_pares, NULL, data_get_cod_est(d), data_get_cod_prox(d));
          }
        }
      }
      primeira = false;
    }
  
    // Setando os números de estações e pares a partir da AVL
    header_set_nmr_estacoes(h, avl_get_n(ar_nomes));
    header_set_nmr_pares(h, avl_get_n(ar_pares)); 
    header_set_status(h, '1'); //Atualiza o status do arquivo, agora que foi regularizado novamente

    //Salva o header
    header_save_all(h, f);
  }
  if(f != NULL) fclose(f), f = NULL; //Salva o arquivo
  
  // Desalocando estruturas
  avl_apagar_arvore(&ar_nomes);
  avl_apagar_arvore(&ar_pares);
  data_delete(&d);
  data_delete(&filtro);
  header_delete(&h);
  //"flags" não precisa ser apagada porque é uma struct estática, não dinâmica.
  
  if(ok) BinarioNaTela(nome_arq);
  else printf("Falha no processamento do arquivo.\n");
  
  return true;
}
