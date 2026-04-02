#include "func.h"

#define TAM_LINHA 200

/*=============FUNÇÕES OCULTAS============*/

/* Percorre um arquivo .csv com os campos esperados separados por vírgulas, escrevendo um arquivo binário de acordo com as especificações do trabalho. 
- Recebe um ponteiro para o arquivo .csv (arquivo de entrada), um ponterio para o arquivo binário (arquivo de saída) e um ponteiro para a estrutura que representa o registro de cabeçalho. 
- Retorna ocorreu erro na execução (true, se sim; false, se não). */
bool func_create(FILE *fe, FILE *fs, HEADER *h);

/* Perocrre uma linha, dividindo-a por vírgula.
- Recebe um ponteiro para a string da linha e um ponteiro para um vetor de ponteiros de char, o qual receberá em cada posição um parte da linha delimitada por vírgulas (ou \n). 
- Retorna ocorreu erro na execução (true, se sim; false, se não). 

As strings apontados por dest perderam sua referência, portanto não a use caso dest aponte para um espcaço alocado dinamicamente (haverá memory leak). */
bool func_division_line(char *linha, char *dest[NMR_CAMPOS]);

/* Atribui para a estrutura os valores correspondentes a cada campo.
- Recebe uma string em que está o valor, outra string em que está o determinado o nome do campo daquele valor e um ponteiro para a estrutura que representa um registro de dados. 
- Retorna se ocorreu erro na execução (true, se sim; false, se não).*/
bool func_attribute_value(char *valor, char *campo, DATA *d);

/* Grava um registro de dados no próximo RRN disponível no arquivo.
- Recebe um ponteiro para a estrutura que representa um registro de dados, um ponteiron para a estrutura que representa o arquivo de cabeçalho e um ponteiro para o arquivo em que está ambos registros.
- Retorna se ocorreu erro na execução (true, se sim; false, se não). */
bool func_insert_end(DATA *d, HEADER *h, FILE *bin);

/* Desaloca um vetor de strings de tamanho NMR_CAMPOS. 
- Recebe o vetor de strings a ser desalocado. */
void func_desaloca_strs(char *str[NMR_CAMPOS]);

/*===============INTERFACE===============*/

void func_create_interface(){
    // Indica se a funcionalidade foi executada com sucesso
    bool erro = false;
    // Consideramos que o nome do arquivo não será maior que 30 bytes
    char nome_arq[31]; 
    // Ponteiro para o arquivo de entrada
    FILE *fe;
    // Ponteiro para o arquivo de saída 
    FILE *fs;
    // Interface para o registro de cabeçalho
    HEADER *h = header_create();
    if(h == NULL) erro = true;

    // Lendo o nome do arquivo de entrada
    scanf(" %30s", nome_arq);
    fe = fopen(nome_arq, "r"); // Arquivo de entrada é csv, por isso lido no modo texto
    if(fe == NULL) erro = true;

    // Lendo o nome do arquivo de saída
    scanf(" %30s", nome_arq);

    fs = fopen(nome_arq, "rb+"); // Arquivo de saída deve ser binário
    if(fs == NULL) erro = true;

    // A maior linha do arquivo é a primeiro com 99 caracteres
    if(!erro){
        // Mudando o status para inconsistente
        header_set_status(h, '0');
        // Salvando o registro de cabeçalho inicial
        header_save_all(h, fs);
        erro = func_create(fe, fs, h); 
        // Mudando o status do arquivo para consistente
        header_set_status(h, '1');
        
        // Salvando o registro de cabeçalho correspondente arquivo binário 
        header_save_all(h, fs);
    }
    
    if(fs != NULL) fclose(fs), fs = NULL;
    if(fe != NULL) fclose(fe), fe = NULL;
    if(h != NULL) header_delete(&h);

    if(erro) printf("Falha no processamento do arquivo.\n");
    else BinarioNaTela(nome_arq);

}

/*===============PRINCIPAL===============*/

bool func_create(FILE *fe, FILE *fs, HEADER *h){
    bool erro = false;
    // Consideramos que uma linha terá, no máximo 200 caracters
    char linha[TAM_LINHA+1];
    // Guarda o nome dos campos em ordem
    char *campos[NMR_CAMPOS];
    // Guarda os valores de cada campo na ordem dos campos
    char *valores[NMR_CAMPOS];
    // Interface com o registro de dados
    DATA *d = data_create();
    if(d == NULL) erro = true;
    // Para contar o número de estações
    ARVORE *ar_nomes = avl_criar_arvore(false);
    // Para contar o número de pares
    ARVORE *ar_pares = avl_criar_arvore(true);
    if(ar_nomes == NULL || ar_pares == NULL) erro = true;

    if(!erro){
        // Lendo a primeira linha
        if(fgets(linha, TAM_LINHA, fe) != NULL){
            // Divindo nome dos campos
            erro = func_division_line(linha, campos);
        }

        // Lendo próximas linhas
        while(!erro && fgets(linha, TAM_LINHA, fe) != NULL){
            // Separando os valores
            erro = func_division_line(linha, valores);

            for(int i = 0; !erro && i < NMR_CAMPOS; i++){
                // Preenchendo a estrutura que representa um registro de dados em ordem dos campos
                erro = func_attribute_value(valores[i], campos[i], d);
            }

            // Colocando nome na árvore (mesmo se o nome for NULL, a funçao avl_inserir trata) - os outros dois parâmetros podem ser quaisquer inteiros
            erro = erro || !avl_inserir(ar_nomes, data_get_nome_est(d), 0, 0);  // Nome nunca vai ser nulo
            // Colocando par de inteiros na árvore - não importa a string que eu passo como parâmetro
            if(data_get_cod_prox(d) != -1) // Código da estação nunca vai ser nulo
                erro = erro || !avl_inserir(ar_pares, NULL, data_get_cod_est(d), data_get_cod_prox(d));

            // Salvando dados no próximo RRN disponível (já verificamos que nenhum dos ponteiros é inválido)
            erro = erro || func_insert_end(d, h, fs);

            func_desaloca_strs(valores);
        }

        func_desaloca_strs(campos);

        // Atualizando cabeçalho
        header_set_nmr_estacoes(h, avl_get_n(ar_nomes));
        header_set_nmr_pares(h, avl_get_n(ar_pares));

    }

    if(d != NULL) data_delete(&d);
    if(ar_nomes != NULL) avl_apagar_arvore(&ar_nomes);
    if(ar_pares != NULL) avl_apagar_arvore(&ar_pares);

    return erro;
}

/*===============AUXILIAR===============*/

bool func_division_line(char linha[TAM_LINHA], char *dest[NMR_CAMPOS]){
    bool erro = false;
    // Iterador pelo buffer
    int j = 0;
    // Campo em que estamos (= quantidade de vírgulas)
    int campo = 0;
    // Armazena temporariamente o valor de um campo
    char buffer[31];
    // Percorrendo a linha
    int i;
    for(i = 0; i < TAM_LINHA; i++){
        // Os últimos valores de cada pode ter o \r, por isso essa verificação
        if(linha[i] == ',' || linha[i] == '\n' || linha[i] == '\r'){
            buffer[j] = '\0'; // Finalizando string

            // Alocando
            dest[campo] = (char*)malloc(sizeof(char)*(j+1)); // + 1 para o terminador
            // Caso a alocação não foi bem sucedida
            if(dest[campo] == NULL){
                erro = true;
                break;
            }
            // Copiando valor para string de destino
            strcpy(dest[campo], buffer);            
            // Passamos para o próximo campo 
            campo++; 
            if(campo >= NMR_CAMPOS) break;
            // Iremmos começar na posição 0 do buffer de novo
            j = 0;
            continue;
        }
        // Se não é um \n ou um ',', é um caractere válido; mas precisamos tomar cuidado com o tamanho do buffer
        else if(j < 30){
            // Copiando o valor da linha para o buffer
            buffer[j] = linha[i];
            j++;
        }
    }
    // Nesse caso, a linha não teve quebra de linha, ou seja, o arquivo não segue o padrão esperado
    if(i == TAM_LINHA) erro = true;

    return erro;
}

bool func_attribute_value(char *valor, char *campo, DATA *d){
    if(strcmp(campo, "CodEstacao") == 0){
        if(valor[0] != '\0') data_set_cod_est(d, atoi(valor));
        else data_set_cod_est(d, -1);
    }

    else if(strcmp(campo, "NomeEstacao") == 0){
        data_set_nome_est(d, valor);
    }
        
    else if(strcmp(campo, "CodLinha") == 0){
        if(valor[0] != '\0') data_set_cod_lin(d, atoi(valor));
        else data_set_cod_lin(d, -1);
    }

    else if(strcmp(campo, "NomeLinha") == 0){
        data_set_nome_lin(d, valor);
    }

    else if(strcmp(campo, "CodProxEst") == 0){
        if(valor[0] != '\0') data_set_cod_prox(d, atoi(valor));
        else data_set_cod_prox(d, -1);
    }

    else if(strcmp(campo, "DistanciaProxEst") == 0){
        if(valor[0] != '\0') data_set_dist(d, atoi(valor));
        else data_set_dist(d, -1);
    }

    else if(strcmp(campo, "CodLinhaInteg") == 0){
        if(valor[0] != '\0') data_set_cod_lin_int(d, atoi(valor));
        else data_set_cod_lin_int(d, -1);
    }

    else if(strcmp(campo, "CodEstacaoInteg") == 0){
        if(valor[0] != '\0') data_set_cod_est_int(d, atoi(valor));
        else data_set_cod_est_int(d, -1);
    }
    // O campo lido não corresponde a nenhum dos campos esperados
    else return true;

    return false;
}

void func_desaloca_strs(char *str[NMR_CAMPOS]){
    for(int i = 0; i < NMR_CAMPOS; i++){
        if(str[i] != NULL) free(str[i]), str[i] = NULL;
    }
}

bool func_insert_end(DATA *d, HEADER *h, FILE *bin){
    if(d == NULL || h == NULL || bin == NULL) return true;

    data_save_all(d, header_get_proxRRN(h), bin);
    header_set_proxRRN(h, header_get_proxRRN(h) + 1);

    return false;
}

