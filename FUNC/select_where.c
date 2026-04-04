#include "../CONTEXTO/contexto.h"

/*=============FUNÇÕES OCULTAS============*/

/* Percorre o arquivo f, imprimindo todas as estruturas que são iguais aos valores do filtro nos campos determinados.
- Recebe uma estrutura de flags que indica os campos a serem comparados, uma estrutura que representa um registro de dados que possui os valores esperados para cada campo, um ponteiro para uma flag que indica se há ao menos um registro que corresponde ao filtro e o arquivo a ser percorrido.
- Retorna se a execução ocorreu corretamente (true, se sim; false, se não). */
bool func_select_where(FLAG_FIELD flags, DATA *filtro, bool *encontrado_um, FILE *f);

/*===============INTERFACE===============*/

void func_select_where_interface(){
    // Indica se a funcionalidade foi executada com sucesso
    bool ok = true; 
    // Indica se ao menos um registro correspondeu a consulta
    bool encontrado_um;
    // Consideramos que esse tamanho é suficiente para o nome do arquivo
    char nome_arq[TAM_BUFFER+1];
    // Número de buscas e número de critérios de seleção
    int n, m;
    // Ponteiro para o arquivo binário
    FILE *f;
    // Guarda o valor esperado dos campos
    DATA *filtro = data_create();
    // Guarda os campos que serão comparados
    FLAG_FIELD flags;

    // Lendo e abrindo arquivo
    scanf(" %s", nome_arq);
    f = fopen(nome_arq, "rb+"); // Modo rb+, para ser necessário que o arquivo exista anteriormente
    if(f == NULL) ok = true;

    if(ok){
        scanf("%d", &n);

        // Para cada busca, vamos ler os critérios e percorrer o arquivo
        for(int i = 0; i < n && ok; i++){
            encontrado_um = false;
            // Recebendo filtro
            scanf("%d", &m);
            ok = func_where_input(&flags, filtro, m);
            // Fazendo busca
            ok = ok && func_select_where(flags, filtro, &encontrado_um, f);

            // Impressão no caso de nenhum registro coresponder a consulta
            if(ok && !encontrado_um) printf("Registro inexistente.\n");
        }

    }

    if(f != NULL) fclose(f), f = NULL;
    if(filtro != NULL) data_delete(&filtro);


    if(!ok) printf("Falha no processamento do arquivo.\n");
}

/*===============PRINCIPAL===============*/

bool func_select_where(FLAG_FIELD flags, DATA *filtro, bool *encontrado_um, FILE *f){
    if(filtro == NULL || f == NULL) return true;
    // Verifica se a função foi executada com sucesso
    bool ok = true;
    // Verifica se o arquivo está consistente
    bool consistente = false;
    // Valor que sinaliza o fim do arquivo
    int proxRRN = -1;
    // Útil para acessar o campo proxRRN e, assim, saber o final do arquivo
    HEADER *h = header_create();
    if(h == NULL) ok = false;
    // Interface para ler os registros, verificar os campos e imprimir se necessário
    DATA *d = data_create();
    if(d == NULL) ok = false;

    if(h != NULL){
        // Confirmando que se o arquivo está consistente
        ok = ok && header_load_field(h, STATUS, f);
        consistente = header_get_status(h) == '1';

        // Recuperando o campo proxRRN
        ok = ok && header_load_field(h, PROXRRN, f);
        proxRRN = header_get_proxRRN(h);
        
        // Não iremos precisar mais dessa estrutura
        ok = ok && header_delete(&h);
    }


    if(ok && consistente){
        // Iterando pelos registros de dados
        for(uint i = 0; i < proxRRN && ok; i++){
            // Carregando registro de RRN = i
            ok = data_load_all(d, i, f);
            // Conferindo se a estrutura é igual ao filtro nos campos determinados
            if(ok && func_where_compare(flags, d, filtro)){
                *encontrado_um = true;
                data_print(d);
            }
        }
        printf("\n");
    }

    if(d != NULL) data_delete(&d);

    return ok;
}