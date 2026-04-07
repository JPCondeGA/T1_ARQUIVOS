#include "../CONTEXTO/contexto.h"

bool func_update(){
  // Verifica se a função foi executada corretamente
  bool ok = true;
  // Consideramos que o nome do arquivo não será maior que 30 bytes
  char nome_arq[TAM_BUFFER];
  // Lendo o nome do arquivo de entrada
  scanf(" %30s", nome_arq);
  // Ponteiro para o arquivo de entrada
  FILE* f = fopen(nome_arq, "rb+"); //Abre o arquivo no modo de leitura e escritura binário.
  if(f == NULL) ok = false;
  
  HEADER* h; //Registro de cabeçalho.
  DATA *d, *filtro, *carimbo; //Registro de dados, registro auxiliar para realizar a busca do usuário e registro auxiliar para guardar os valores que o usuário quer substitutuir.

  filtro = data_create();
  if(filtro == NULL) ok = false;
  carimbo = data_create();
  if(carimbo == NULL) ok = false;
  ok = ok && func_inicializar(&h, &d);
  

  if(ok){
    // Carregando o cabeçalho
    header_load_all(h, f);
    // Inicialização da funcionalidade finalizada!
    header_set_status(h, '0'); // Arquivo foi aberto para escrita e seu status deve ser atualizado.
    header_save_field(h, STATUS, f); // Salva a mudança no registro de cabeçalho.
    
    FLAG_FIELD flag_filtro, flag_carimbo; //Marcam quais campos possuem filtros ativos para a busca e quais campos serão alterados.
    ok = func_init_flag_field(&flag_filtro);
  
    uint proxRRN = header_get_proxRRN(h);  // Demarca até qual registro existe.
    char* nomeest; // Variável auxiliar para receber data_get_nome_est().
    uint n; // Guarda a quantidade de alterações a serem feitas.
    uint m; // Guarda a quantidade de campos que serão usados para a busca
    uint p; // Guarda a quantidade de campos que serão alterados
    char campo[TAM_BUFFER]; // Guarda nomes de campos do registro. Os maiores nomes de campo possuem 15 caracteres + '/0', portanto tamanho 16.
    char valor[TAM_BUFFER]; //Guarda valores que podem estar salvos em campos de registro.
    scanf("%d", &n);
  
    for(int i = 0; i < n; i++){
    
      // Inicializando todos campos com false
      func_init_flag_field(&flag_filtro); 
      func_init_flag_field(&flag_carimbo); 
      
      // Lendo critérios de seleção
      scanf("%d", &m);
      func_where_input(&flag_filtro, filtro, m);
      
      // Lendo valores a serem atualizados
      scanf("%d", &p);
      func_where_input(&flag_carimbo, carimbo, p);
      
      for(uint j = 0; j < proxRRN; j++){

        data_load_all(d, j, f);

        if (feof(f)) break; //Checa por END OF FILE depois de ler do arquivo.
        
        if (data_get_removido(d) != '1'){ //Pula registros removidos
          if (func_where_compare(flag_filtro, d, filtro)){ //Se encontrar um registro que se encaixa no filtro, inicia a lógica de alteração.
                
            func_copy_data(&flag_carimbo, d, carimbo); //Copia todos os campos em "carimbo" marcados na flag em "d" 
            data_save_all(d, j, f); //Salva as alterações do registro no arquivo.

            // Não precisamos verificar o número de estações e pares, segundo as orientações dos monitores
            
          }
        }
      }
    }
  }
  
  header_set_status(h, '1'); //Atualiza o status do arquivo, agora que foi regularizado novamente.
  header_save_field(h, STATUS, f); //Salva apenas os campos que haviam possibilidade de alteração.

  if(f != NULL) fclose(f), f = NULL; //Fecha o arquivo e salva as alterações.
  
  //Desaloca todas as estruturas (verificações internas)
  data_delete(&d);
  data_delete(&filtro);
  data_delete(&carimbo);
  header_delete(&h);
  //"flags_filtro" e "flags_carimbo" não precisam ser apagadas porque são structs estáticas, não dinâmica.
  
  if(ok) BinarioNaTela(nome_arq); //Execução da funcionalidade foi terminada.
  else printf("Falha no processamento do arquivo.\n");
  return ok;
}
