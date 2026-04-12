#include "../CONTEXTO/contexto.h"

/*=============FUNÇÕES OCULTAS============*/

/* Percorre os registros de dados do arquivo e atualiza os campos especificados com os valores dados dos registros que correspondem aos outros valores dados dos campos determinados. 
  - Recebe uma estrutura que indica os campos usados como critério, outra estrutura dessas que indica quais campos serão alterados, um ponteiro para estrutura que representa um registro de dados para percorrer o arquivo, um outro ponteiro para estrutura desse tipo com os valores de critério dos campos, um outro ponteiro para estrutura desse tipo com os valores novos dos campos e um ponteiro para o arquivo. 
  - Retorno */
bool func_update(FLAG_FIELD flag_filtro, FLAG_FIELD flag_carimbo, DATA *d, DATA *filtro, DATA *carimbo, FILE *f);


/*=============INTERFACE============*/

void func_update_interface(){
  bool ok = true; // Verifica se a função foi executada corretamente
  char nome_arq[TAM_BUFFER]; // Consideramos que o nome do arquivo não será maior que TAM_BUFFER-1
  
  // Lendo o nome do arquivo de entrada
  scanf(" %30s", nome_arq);
  // Ponteiro para o arquivo de entrada
  FILE* f = fopen(nome_arq, "rb+"); //Abre o arquivo no modo de leitura e escritura binário.
  if(f == NULL) ok = false;
  
  HEADER* h; // Registro de cabeçalho.
  DATA *d, *filtro, *carimbo; // Registro de dados, registro auxiliar para realizar a busca do usuário e registro auxiliar para guardar os valores que o usuário quer substitutuir.

  // Alocações
  filtro = data_criar();
  carimbo = data_criar();
  if(filtro == false || carimbo == NULL) ok = false;

  ok = ok && cntx_inicializar(&h, &d);
  
  // Checando a consistência do arquivo
  if(f != NULL && h != NULL){ // Verificando se as condições para isso é possível
    header_carregar_campo(h, STATUS, false, f); // Vamos precisar só desse campo (o cursor já está no começo)
    ok = ok && cntx_checa_consistencia(h, false, false, NULL); // Checando consistência já carregada (não importa o move, pois não iremos carregar)
  }

  // Inicialização da funcionalidade finalizada!
  if(ok){
    // Alterando consistência do arquivo  
    cntx_altera_consistencia(h, INCONSISTENTE, true, f); // Cursor precisa voltar
    
    FLAG_FIELD flag_filtro, flag_carimbo; // Marcam quais campos possuem filtros ativos para a busca e quais campos serão alterados.
  
    uint n; // Guarda a quantidade de alterações a serem feitas.
    uint m; // Guarda a quantidade de campos que serão usados para a busca
    uint p; // Guarda a quantidade de campos que serão alterados
    
    scanf("%d", &n); // Lendo a quantidade de updates
  
    // Para cada update iremos percorrer o arquivo
    for(int i = 0; i < n; i++){
    
      // Inicializando todos campos com false
      cntx_init_flag(&flag_filtro); 
      cntx_init_flag(&flag_carimbo); 
      
      // Lendo critérios de seleção
      scanf("%d", &m);

      ok = cntx_where_input(&flag_filtro, filtro, m);
      
      // Lendo valores a serem atualizados
      scanf("%d", &p);
      ok = ok && cntx_where_input(&flag_carimbo, carimbo, p);
      
      if(ok) func_update(flag_filtro, flag_carimbo, d, filtro, carimbo, f); // Percorrendo o arquivo e atualizando os registros que passarem no teste
      
    }
  }
  
  // Atualizando o cabeçalho
  header_set_status(h, CONSISTENTE); // Atualiza o status do arquivo, agora que foi regularizado novamente.
  header_salvar_campo(h, STATUS, true, f); // Salva apenas os campos que haviam possibilidade de alteração (precisa mover o cursor)

  if(f != NULL) fclose(f), f = NULL; //Fecha o arquivo e salva as alterações.
  
  //Desaloca todas as estruturas (verificações internas)
  data_apagar(&d);
  data_apagar(&filtro);
  data_apagar(&carimbo);
  header_apagar(&h);

  //"flags_filtro" e "flags_carimbo" não precisam ser apagadas porque são structs estáticas, não dinâmica.
  
  if(ok) BinarioNaTela(nome_arq); //Execução da funcionalidade foi terminada.
  else printf("Falha no processamento do arquivo.\n");
}


/*=============PRINCIPAL============*/

bool func_update(FLAG_FIELD flag_filtro, FLAG_FIELD flag_carimbo, DATA *d, DATA *filtro, DATA *carimbo, FILE *f){
  bool ok = true; // Verifica se a execução ocorreu corretamente 
  bool fim; // Indica se houve erro de leitura e, consequentemente, o fim do arquivo
  uint RRN = 0; // Guarda o RRN do registro atual

  while(ok){
    // Lendo o registro atual
    if(RRN == 0) fim = data_carregar(d, RRN, true, f); // Somente na primeira iteração que o cursor vai estar posicionado no primeiro registro
    else fim = data_carregar(d, RRN, false, f);

    if (!fim) break; // Checa por END OF FILE depois de ler do arquivo.
    
    if (data_get_removido(d) != '1'){ // Pula registros removidos
      if (cntx_where_compare(flag_filtro, d, filtro)){ // Se encontrar um registro que se encaixa no filtro, inicia a lógica de alteração.
        
        ok = cntx_copiar_data(&flag_carimbo, d, carimbo); //Copia todos os campos em "carimbo" marcados na flag em "d" 
        
        fseek(f, -TAM_DATA, SEEK_CUR); // Voltando cursor (carregar moveu ele para o próximo registro)

        if(ok) data_salvar(d, RRN, false, f); //Salva as alterações do registro no arquivo.

        // Não precisamos verificar o número de estações e pares, segundo as orientações dos monitores; mas se precisamos seria algo similar ao delete
        
      }
    }

    RRN++; // Passando para o próximo registro
  }
  
  return ok;
}
