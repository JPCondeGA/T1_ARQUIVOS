#include "../CONTEXTO/contexto.h"

/*=============FUNÇÕES OCULTAS============*/

/* Percorre um arquivo comparando os campos do seu registro campos com os critérios passados; se passar nos critérios, remove logicamente aquele registro
  - Recebe uma estrutura de flags que indica os campos a ser comparados, um ponteiro para a estrutura que representa um registro de dados para percorrer o arquivo, outro registro desse tipo que tem os valores de critério, um ponteiro para a estrutura que representa o registro de cabeçalho para devidas manipulações, dois ponteiros para árvore (uma ordenada pelo nome, outra ordenada por pares), uma flag que indica se é para instanciar a árvore e um ponteiro para arquivo*/
void func_delete(FLAG_FIELD flags, DATA *d, DATA *filtro, HEADER *h, ARVORE* ar_nomes, ARVORE* ar_pares, bool primeira, FILE *f);

/*=============INTERFACE============*/

void func_delete_interface(){
  bool ok = true; // Verifica se a funcionalidade foi executada com sucesso
  char nome_arq[TAM_BUFFER]; // Consideramos que o nome do arquivo não será maior que TAM_BUFFER

  HEADER* h; // Registro de cabeçalho.
  DATA *d, *filtro; // Registro de dados e registro auxiliar para realizar a busca do usuário.
  ARVORE *ar_nomes, *ar_pares; // Árvores que irão guardar quantas estações e pares de códigos de estação diferentes existem no registro de dados, respectivamente.

  // Lendo o nome do arquivo de entrada
  scanf(" %30s", nome_arq);
  // Ponteiro para o arquivo de entrada
  FILE* f = fopen(nome_arq, "rb+"); // Abre o arquivo no modo de leitura e escritura binário.
  if(f == NULL) ok = false; // Verificando alocação

  // Alocações
  filtro = data_criar();
  ar_nomes = avl_criar_arvore(false);
  ar_pares = avl_criar_arvore(true);
  if(filtro == NULL || ar_nomes == NULL || ar_pares == NULL) ok = false;
  ok = ok && cntx_inicializar(&h, &d);
  
  // Verificando consistência
  if(f != NULL && h != NULL){ // É possível verificar a consistência do arquivo
    header_carregar(h, false, f); // Já estamos no primeiro byte
    ok = ok && cntx_checa_consistencia(h, false, false, NULL); // O campo status já está na estrutura (como não precisa carregar, o move não importa)
  }

  //Inicialização da funcionalidade finalizada!
  if(ok){ // Checando consistência
    cntx_altera_consistencia(h, INCONSISTENTE, true, f); // Mudando o status do arquivo para inconsistente (o cursor deve ser movido)
    
    FLAG_FIELD flags; //Marca quais campos possuem filtros ativos. 
    uint n; //Guarda a quantidade de remoções a serem feitas.
    uint m; //Guarda a quantidade de filtros que serão adicionados.
    char campo[TAM_BUFFER]; //Guarda o campo a ser procurado para remoção. Os maiores nomes de campo possuem 15 caracteres + '/0', portanto tamanho 16.
    char valor[TAM_BUFFER]; //Guarda o valor que será procurado no registro de dados.
    bool primeira = true; // Checa se a primeira passagem pelo arquivo (para criar as árvore)
   
    scanf("%d", &n); // Lendo quantidade de remoções
    
    // Para cada remoção, vamos ler os critérios e percorrer o arquivo
    for(int i = 0; i < n && ok; i++){
    
      scanf("%d", &m); // Lendo quantidade de campos do filtro
      cntx_init_flag(&flags); // Inicializando todos campos com false

      ok = cntx_where_input(&flags, filtro, m); // Recebendo os valores do filtro
      if(ok) func_delete(flags, d, filtro, h, ar_nomes, ar_pares, primeira, f);
      
      primeira = false; // Passou da primeira
    }
  
    // Setando os números de estações e pares a partir da AVL
    header_set_nmr_estacoes(h, avl_get_n(ar_nomes));
    header_set_nmr_pares_estacao(h, avl_get_n(ar_pares)); 
    header_set_status(h, CONSISTENTE); // Atualiza o status do arquivo, agora que foi regularizado novamente

    //Salvando o cabeçalho
    header_salvar(h, true, f); // O cursor deve ser movido
  }

  if(f != NULL) fclose(f), f = NULL; // Salva o arquivo
  
  // Desalocando estruturas
  avl_apagar_arvore(&ar_nomes);
  avl_apagar_arvore(&ar_pares);
  data_apagar(&d);
  data_apagar(&filtro);
  header_apagar(&h);
  //"flags" não precisa ser apagada porque é uma struct estática, não dinâmica.

  // Imprimindo resultado
  if(ok) BinarioNaTela(nome_arq);
  else printf("Falha no processamento do arquivo.\n");
}


/*=============PRINCIPAL============*/

void func_delete(FLAG_FIELD flags, DATA *d, DATA *filtro, HEADER *h, ARVORE* ar_nomes, ARVORE* ar_pares, bool primeira, FILE *f){
  uint proxRRN = header_get_proxRRN(h); // Demarca o fim do arquivo
  char* nome_estacao; // Variável auxiliar para receber data_get_nome_est()

  for(uint RRN = 0; RRN < proxRRN; RRN++){
        // Carregando o registro atual
        if(RRN == 0 && !primeira) data_carregar(d, RRN, true, f); // Se estamos no primeiro RRN e não na primeira, então o cursor está no final do arquivo e precisamos movê-lo 
        else data_carregar(d, RRN , false, f);

        if (feof(f)) break; //Checa por END OF FILE depois de ler do arquivo.

        if (data_get_removido(d) != '1'){ // Pula registros removidos

          // Se encontrar um registro que se encaixa no filtro, o remove logicamente
          if (cntx_where_compare(flags, d, filtro)){ 
            data_set_removido(d, '1'); // Marca seu registro como logicamente removido.
            data_set_proximo(d, header_get_topo(h)); // Salva o último valor do topo em "próximo".
            header_set_topo(h, RRN); // Empilha o registro no cabeçalho.
            
            // Movendo ponteiro para início daquele registro
            fseek(f, -TAM_DATA, SEEK_CUR);

            //Salva apenas os campos que foram alterados.
           data_salvar_removido_proximo(d, RRN, false, f); // Não precisamos mover o cursor, movemos em cima com SEEK_CUR (melhor que SEEK_SET) -> cursor vai para o próximo registro
            
            //O header é capaz de mudar mais vezes, portanto é salvo fora do laço.

            // Removendo da árvore (Se não for a primeira vez não precisamos verificar se o nó está)
            if(!primeira){
              nome_estacao = data_get_nome_estacao(d); // Pegando o nome do registro
              avl_remover(ar_nomes, nome_estacao, 0, 0); // Removendo o nome
              free(nome_estacao); nome_estacao = NULL; // Desalocando
              if(data_get_cod_prox_estacao(d) != -1) // Pares com nulo não são colocados na árvore
                avl_remover(ar_pares, NULL, data_get_cod_estacao(d), data_get_cod_prox_estacao(d)); // Removendo o par
            }
          }

          // Se for a primeira, adicionamos os nós que não foram removidos
          else if(primeira){ 
            nome_estacao = data_get_nome_estacao(d); // Pegando o nome do registro 
            avl_inserir(ar_nomes, nome_estacao, 0, 0); // Inserindo na AVL de nomes
            free(nome_estacao); nome_estacao = NULL; // O nome é copiado para o nó, podemos desalocar
            if(data_get_cod_prox_estacao(d) != -1) // Par (cod_estacao, NULL) não é válido para colocar na AVL 
              avl_inserir(ar_pares, NULL, data_get_cod_estacao(d), data_get_cod_prox_estacao(d)); // Inserindo na AVL de pares numéricos
          }
        }
      }
}