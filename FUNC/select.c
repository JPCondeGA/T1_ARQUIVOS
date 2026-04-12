#include "func.h"

/*=============FUNÇÕES OCULTAS============*/

/* Percorre o arquivo imprimindo todos os seus registros não logicamente removidos.
  - Recebe um ponteiro para estrutura que representa um registro de dados para recuperar os registros, uma flag que indica se ao menos um registro foi impresso e um ponteiro para arquivo.
  - Retorna true, se a execução ocorreu corretamente; false, caso contrário. */
bool func_select(DATA *d, bool *existe_registro, FILE *f);

/*===============INTERFACE===============*/

void func_select_interface(){
    bool ok = true; // Verifica se a execução ocorreu corretamente
    bool existe_registro = false; // Detecta caso não há registros para imprimir.
    char nome_arq[TAM_BUFFER]; // Consideramos que o nome do arquivo não será maior que TAM_BUFFER-1

    // Alocações
    DATA* d = data_criar(); // Inicializando estrutura que representa um registro de dados para percorrer os registros do arquivo
    HEADER *h = header_criar(); // Incializando estrutura que representa o registro de cabeçalho para ver a consistência do arquivo
    if(d == NULL) ok = false; // Se houver falha na alocação

    // Recebendo arquivo binário
    scanf(" %30s", nome_arq); // Lendo nome do arquivo
    FILE* f = fopen(nome_arq, "rb"); // Abre o arquivo no modo de leitura binário. -> não precisa mudar o status do arquivo
    if(f == NULL) ok = false; // Se houve falha na abertura do arquivo, retorna "false".

    ok = ok && cntx_checa_consistencia(h, true, f); // Checando status do arquivo (se h == NULL, tem verificação interna)
    header_apagar(&h); // Não iremos precisar mais dessa estrutura

    if(ok){
      ok = func_select(d,&existe_registro, f); // Percorrendo arquivo
      if(!existe_registro) printf("Registro inexistente.\n"); // Em caso de arquivo sem registros
    }
    else printf("Falha no processamento do arquivo.\n"); // Em caso de algum erro

    // Desalocações
    if(f != NULL) fclose(f), f = NULL; 
    data_apagar(&d); // Verificação interna
}

/*===============PRINCIPAL===============*/

bool func_select(DATA *d, bool *existe_registro, FILE *f){
    bool ok = true; // Verifica se a execução foi correta 
    int aux = 0; // Ajuda a detectar campos nulos.
    char* nomes; // data_get_nome_est() e data_get_nome_lin() retornam arrays dinâmicos que precisam ser desalocados após uso.
    uint RRN = 0; // Demarca qual o próximo registro que deve ser lido.
    
    while(ok){
      
      ok = data_carregar(d, RRN, f); // Mesmo se a leitura falhar (no fim do arquivo), não é retorna false
      if (feof(f)) break; //Checa por END OF FILE depois de ler do arquivo.
      
      // Imprimindo registros
      if (ok && data_get_removido(d) != '1'){ //Pula registros removidos
        cntx_imprimir_data(d); // Para todos os campos: Verifica se está marcado como -1 (para tamanho fixo) ou 0 (para tamanho variável) e imprime "NULO" se sim, ou o valor do campo se não.
        *existe_registro = true; // Encontrou ao menos um registro
      }
      
      RRN++;  //Atualiza o contador de registros.
    }

    return ok;
}
