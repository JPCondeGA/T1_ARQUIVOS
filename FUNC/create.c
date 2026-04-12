#include "../CONTEXTO/contexto.h"

#define TAM_LINHA 201

/*=============FUNÇÕES OCULTAS============*/

/*=-=-Principal-=-=*/

/* Percorre um arquivo .csv com os campos esperados separados por vírgulas, escrevendo um arquivo binário de acordo com as especificações do trabalho. 
- Recebe um ponteiro para o arquivo .csv (arquivo de entrada), um ponterio para o arquivo binário (arquivo de saída),  um ponteiro para a estrutura que representa o registro de cabeçalho, um ponteiro para a estrutura que representa um registro de dados que será usado para salvar os valores no arquivo e dois ponteiros para ávore, uma do tipo de ordenação por nome outra por pares
- Retorna se executou corretamente (true, se sim; false, se não). */
bool func_create(FILE *fe, FILE *fs, HEADER *h, DATA *d, ARVORE *arv_nome, ARVORE *arv_pares);

/*=-=-Auxiliar-=-=*/

/* Perocrre uma linha, dividindo-a por vírgula.
- Recebe um ponteiro para a string da linha e um ponteiro para um vetor de ponteiros de char (de tamanho NMR_CAMPOS), o qual receberá em cada posição um parte da linha delimitada por vírgulas (ou \n). 
- Retorna se executou corretamente (true, se sim; false, se não).
 */
bool func_dividir_linha(char *linha, char *dest[NMR_CAMPOS]);

/* Grava um registro de dados no próximo RRN disponível.
    - Recebe ponteiro para estrutura que representa um registro de dados, um ponteiro para estrturua que representa uo registro de cabeçalho e ponteiro para o arquivo em que está ambos registros. 
    - Retorna se a execução ocorreu corretamente (true, se sim; false, se não). */
bool cntx_insert_end(DATA *d, HEADER *h, FILE *bin);


/* Desaloca um vetor de strings de tamanho NMR_CAMPOS. 
- Recebe o vetor de strings a ser desalocado. */
void func_desalocar_strs(char *str[NMR_CAMPOS]);


/*===============INTERFACE===============*/

void func_create_interface(){
    bool ok = true; // Indica se a funcionalidade foi executada com sucesso
    char nome_arq[TAM_BUFFER]; // Consideramos que o nome do arquivo não será maior que TAM_BUFFER-1
    FILE *fe; // Ponteiro para o arquivo de entrada
    FILE *fs; // Ponteiro para o arquivo de saída 

    // Alocações
    HEADER *h = header_criar(); // Interface para o registro de cabeçalho
    DATA *d = data_criar(); // Interface com o registro de dados
    ARVORE *ar_nomes = avl_criar_arvore(false); // AVL para contar o número de estações
    ARVORE *ar_pares = avl_criar_arvore(true); // AVL para contar o número de pares

    if(ar_nomes == NULL || ar_pares == NULL || h == NULL || d == NULL) ok = false; // Verificando se as alocações ocorreu corretamente

    // Recebendo arquivo de entrada
    scanf(" %30s", nome_arq); // Lendo o nome do arquivo de entrada
    fe = fopen(nome_arq, "r"); // Arquivo de entrada é csv, por isso lido no modo texto
    if(fe == NULL) ok = false; // Se o arquivo não existir

    // Recebendo o arquivo de saóda
    scanf(" %30s", nome_arq); // Lendo o nome do arquivo de saída
    fs = fopen(nome_arq, "wb"); // Arquivo de saída deve ser binário (vamos considerar que o arquivo já está criado)
    if(fs == NULL) ok = false; // Se o arquivo não for aberto corretamente

    // Não precisamos verificar a consistência do arquivo, pois estamos criando um novo

    // Se não ocorreu nenhum erro até então
    if(ok){ 
        // Manipulando cabeçalho
        header_set_status(h, '0'); // Mudando o status para inconsistente
        header_salvar(h, fs); // Salvando o registro de cabeçalho inicial

        ok = func_create(fe, fs, h, d, ar_nomes, ar_pares); 

        header_set_status(h, '1'); // Mudando o status do arquivo para consistente
        header_salvar(h, fs);  // Salvando o registro de cabeçalho preenchido
    }
    
    // Desalocando
    if(fs != NULL) fclose(fs), fs = NULL;
    if(fe != NULL) fclose(fe), fe = NULL;
    header_apagar(&h); // Verificação interna
    data_apagar(&d);
    avl_apagar_arvore(&ar_nomes);
    avl_apagar_arvore(&ar_pares);

    // Impriimindo saída
    if(!ok) printf("Falha no processamento do arquivo.\n");
    else BinarioNaTela(nome_arq);
}


/*===============PRINCIPAL===============*/

bool func_create(FILE *fe, FILE *fs, HEADER *h, DATA *d, ARVORE *ar_nomes, ARVORE *ar_pares){
    bool ok = true; // Verifica se o procedimento finalizou corretamente
    char linha[TAM_LINHA]; // Consideramos que uma linha terá, no máximo TAM_LINHA-1 caracteres
    char *campos[NMR_CAMPOS]; // Guarda o nome dos campos em ordem que aparece no cabeçalho do .csv
    char *valores[NMR_CAMPOS]; // Guarda os valores de cada campo na ordem dos campos
    char *aux; // Guarda a string retornada pelo data_get_nome_estacao

    // Lendo a primeira linha
    if(fgets(linha, TAM_LINHA, fe) != NULL){ // fgets vai até o primeiro \n
        ok = func_dividir_linha(linha, campos); // Divindo nome dos campos
    }

    // Lendo próximas linhas
    while(ok && fgets(linha, TAM_LINHA, fe) != NULL){ // Identificaremos o final do arquivo pelo retorno do fgets
        ok = func_dividir_linha(linha, valores); // Separando os valores por campo

        // Preenchendo a estrutura que representa um registro de dados em ordem dos campos
        for(int i = 0; i < NMR_CAMPOS && ok; i++){
            ok = cntx_atribuir_valor_data(valores[i], campos[i], d);
        }

        // Colocando nome na árvore - os outros dois parâmetros podem ser quaisquer inteiros
        ok = ok && avl_inserir(ar_nomes, aux = data_get_nome_estacao(d), 0, 0);  // Nome nunca vai ser nulo
        free(aux); // A string é copiada para árvore, podemos desalocar
        
        // Colocando par de inteiros na árvore - não importa a string que eu passo como parâmetro
        if(data_get_cod_prox_estacao(d) != -1) // Código da estação nunca vai ser nulo -> pares (x, NULO) não são contados
            ok = ok && avl_inserir(ar_pares, NULL, data_get_cod_estacao(d), data_get_cod_prox_estacao(d));

        ok = ok && cntx_insert_end(d, h, fs); // Salvando dados no próximo RRN disponível (já verificamos que nenhum dos ponteiros é inválido)

        func_desalocar_strs(valores); // Desalocando strings que representam os valores atuais
    }

    func_desalocar_strs(campos);

    // Atualizando o número de estações e número de pares do cabeçalho com base na AVL
    header_set_nmr_estacoes(h, avl_get_n(ar_nomes));
    header_set_nmr_pares_estacao(h, avl_get_n(ar_pares));

    return ok;
}

/*===============AUXILIAR===============*/

bool func_dividir_linha(char linha[TAM_LINHA], char *dest[NMR_CAMPOS]){
    bool ok = true;  // Verifica se o procedimento finalizou corretamente
    int campo = 0; // Campo em que estamos (= quantidade de vírgulas)
    char buffer[TAM_BUFFER]; // Armazena temporariamente o valor de um campo
    int j = 0; // Iterador pelo buffe
    int i; // Iterador da linhar
    
    // Percorrendo linha
    for(i = 0; i < TAM_LINHA; i++){
        // Se for qualquer um desses caracteres, acabamos de terminar de ler um valor de um dos campos.
        // (os últimos valores de cada linha pode ter o \r, por isso essa verificação).
        if(linha[i] == ',' || linha[i] == '\n' || linha[i] == '\r'){
            buffer[j] = '\0'; // Finalizando string

            // Alocando espaço para a string de destino (o buffer será copiado para ela) 
            dest[campo] = (char*)malloc(sizeof(char)*(j+1)); // + 1 para o terminador
            // Caso a alocação não foi bem sucedida
            if(dest[campo] == NULL){
                ok = false;
                break;
            }

            strcpy(dest[campo], buffer); // Copiando valor para string de destino

            campo++; // Passamos para o próximo campo 
            if(campo >= NMR_CAMPOS) break; // Se já passamos por todos campos, acabamos o processo
            j = 0; // Senão, iremmos começar na posição 0 do buffer de novo
        }
        // Se não é um \n ou um ',', é um caractere válido; mas precisamos tomar cuidado com o tamanho do buffer
        else if(j < 30){
            buffer[j] = linha[i]; // Copiando o valor da linha para o buffer
            j++;
        }
    }
    // Nesse caso, a linha não teve quebra de linha, ou seja, o arquivo não segue o padrão esperado
    if(i == TAM_LINHA) ok = false;

    return ok;
}

bool cntx_insert_end(DATA *d, HEADER *h, FILE *f){
    if(d == NULL || h == NULL || f == NULL) return false; // Verificando se os ponteiros são inválidos

    data_salvar(d, header_get_proxRRN(h), f); // Salvando o que está no data na posição correspondente ao proxRRN no arquivo 
    header_set_proxRRN(h, header_get_proxRRN(h) + 1); // Incrementando RRN

    return true;
}

void func_desalocar_strs(char *str[NMR_CAMPOS]){
    // Percorrendo vetor
    for(int i = 0; i < NMR_CAMPOS; i++){
        if(str[i] != NULL) free(str[i]), str[i] = NULL; // Se apontar para um lugar válido, desaloca e seta NULL
    }
}



