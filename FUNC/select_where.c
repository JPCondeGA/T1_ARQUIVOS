#include "../CONTEXTO/contexto.h"

/*=============FUNÇÕES OCULTAS============*/

/* Percorre o arquivo f, imprimindo todas as estruturas que são iguais aos valores do filtro nos campos determinados.
- Recebe uma estrutura de flags que indica os campos a serem comparados, uma estrutura que representa um registro de dados que possui os valores esperados para cada campo, um ponteiro para uma flag que indica se há ao menos um registro que corresponde ao filtro e um ponteiro para o arquivo a ser percorrido.
- Retorna se a execução ocorreu corretamente (true, se sim; false, se não). */
bool func_select_where(FLAG_FIELD flags, DATA *filtro, DATA *d, bool *existe_registro, FILE *f);


/*===============INTERFACE===============*/

void func_select_where_interface(){
    bool ok = true; // Indica se a funcionalidade foi executada com sucesso
    bool existe_registro; // Indica se ao menos um registro correspondeu a consulta
    char nome_arq[TAM_BUFFER]; // Consideramos que esse tamanho é suficiente para o nome do arquivo
    int n, m; // Número de buscas e número de critérios de seleção
    FILE *f; // Ponteiro para o arquivo binário
    FLAG_FIELD flags; // Guarda indicações de quais campos serão comparados

    // Alocações
    DATA *filtro = data_criar(); // Guarda o valor esperado dos campos
    DATA *d = data_criar(); // Guarda o registro atual do laço
    HEADER *h = header_criar();  // Para acessar o campo status

    // Lendo e abrindo arquivo
    scanf(" %s", nome_arq);
    f = fopen(nome_arq, "rb"); // Abrindo em modo leitura (não precisamsos escrver)
    if(f == NULL || filtro == NULL || d == NULL) ok = true; // Verificando alocações

    ok = ok && cntx_checa_consistencia(h, true, false, f); // Verificando consistência do arquivo -> cursor vai para o primeiro byte do primeiro registro da dados

    if(ok){
        scanf("%d", &n);

        // Para cada busca, vamos ler os critérios e percorrer o arquivo
        for(int i = 0; i < n && ok; i++){
            existe_registro = false;
            // Recebendo filtro
            scanf("%d", &m);
            ok = cntx_where_input(&flags, filtro, m);

            ok = ok && func_select_where(flags, filtro, d, &existe_registro, f); // Fazendo busca

            if(ok && !existe_registro) printf("Registro inexistente.\n"); // Impressão no caso de nenhum registro coresponder a consulta
            printf("\n");
        }

    }

    // Desalocações
    if(f != NULL) fclose(f), f = NULL;
    data_apagar(&filtro);
    data_apagar(&d);
    header_apagar(&h);

    if(!ok) printf("Falha no processamento do arquivo.\n"); // Em caso de algum erro
}

/*===============PRINCIPAL===============*/

bool func_select_where(FLAG_FIELD flags, DATA *filtro, DATA *d, bool *existe_registro, FILE *f){
    if(filtro == NULL || f == NULL) return true;
    bool ok = true; // Verifica se a função foi executada com sucesso
    bool fim; // Verifica se ocorreu falha na leitura
    uint RRN = 0;

    // Iterando pelos registros de dados
    while(ok){
        // Carregando registro
        if(RRN == 0) fim = data_carregar(d, RRN, true, f); // Precisa mover o cursor para o início (só a primeira vez estará lá)
        else fim = data_carregar(d, RRN, false, f); // Nesse caso não precisamo mover o cursor
        // move = false, pois já estamos sobre o o byte

        // Se a leitura falhar (no fim do arquivo), retorna false
        if(!fim) break;

        // Conferindo se a estrutura é igual ao filtro nos campos determinados
        if(ok && cntx_where_compare(flags, d, filtro)){
            *existe_registro = true;
            cntx_imprimir_data(d);
        }

        RRN++;
    }
    
    return ok;
}