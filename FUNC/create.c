#include "func.h"

/*=============FUNÇÕES OCULTAS============*/

/**/
bool func_create(FILE *fe, FILE *fs, bool *erro, HEADER *h);

/**/
void func_desaloca_strs(char *str[NMR_CAMPOS]);

/**/
void func_division_line(char *linha, char *dest[NMR_CAMPOS]);

/**/
void func_attribute_value(char *valor, char *campo, DATA *d);

/**/
void func_insert_end(DATA *d, HEADER *h, FILE *bin);

bool func_create_interface(){
    // Indica se a funcionalidade foi executada com sucesso
    bool erro = true;
    // Consideramos que o nome do arquivo não será maior que 30 bytes
    char nome_arq[31]; 
    // Ponteiro para o arquivo de entrada
    FILE *fe;
    // Ponteiro para o arquivo de saída 
    FILE *fs;
    // Interface para o registro de cabeçalho
    HEADER *h = header_create();


    // Lendo o nome do arquivo de entrada
    scanf(" %30s", nome_arq);
    fe = fopen(nome_arq, "r"); // Arquivo de entrada é csv, por isso lido no modo texto
    
    if(fe != NULL) printf("ok 1\n");
    // Lendo o nome do arquivo de saída
    scanf(" %30s", nome_arq);
    fs = fopen(nome_arq, "rb+"); // Arquivo de saída deve ser binário
    if(fe != NULL) printf("ok 2\n");
    // Mudando o status para inconsistente
    header_set_status(h, '0');
    // Salvando o registro de cabeçalho inicial
    header_save_all(h, fs);

    // A maior linha do arquivo é a primeiro com 99 caracteres
    if(fe == NULL || fs == NULL) erro = false;
    else{
        printf("ok 3\n");
        func_create(fe, fs, &erro, h); 
    }

    fclose(fs); fclose(fe);
    header_set_status(h, '1');

    // Salvando o registro de cabeçalho correspondente arquivo binário 
    header_save_all(h, fs);

    return erro;
}

void func_desaloca_strs(char *str[NMR_CAMPOS]){
    for(int i = 0; i < NMR_CAMPOS; i++){
        if(str[i] != NULL) free(str[i]), str[i] = NULL;
    }
}

void func_division_line(char *linha, char *dest[NMR_CAMPOS]){
    // Iterador pelo buffer
    int j = 0;
    // Campo em que estamos (= quantidade de vírgulas)
    int campo = 0;
    // Armazena temporariamente o valor de um campo
    char buffer[31];
    // Percorrendo a linha
    for(int i = 0; linha[i] != '\n'; i++){
        if(linha[i] == ','){
            buffer[j] = '\0'; // Finalizando string
            printf("%s\n", buffer);
            // Se o campo for nulo (vazio) 
            if(j == 0) dest[campo] = NULL;
            else dest[campo] = (char*)malloc(sizeof(char)*(j+1)); // + 1 para o terminador
            
            strcpy(dest[campo], buffer);
            // Passamos para o próximo campo 
            campo++; 
            // Iremmos começar na posição 0 do buffer de novo
            j = 0;
            continue;
        }
        // Se não é um \n ou um ',', é um caractere válido; mas precisamos tomar cuidado com o tamanho do buffer
        else if(j < 30){
            // Copiando o valor da linha para o buffer
            buffer[j] = linha[i];
        }
    }
}

void func_attribute_value(char *valor, char *campo, DATA *d){
    if(strcmp(campo, "CodEstacao") == 0)
        if(valor != NULL) data_set_cod_est(d, atoi(valor));
        else data_set_cod_est(d, -1);

    else if(strcmp(campo, "NomeEstacao") == 0)
        data_set_nome_est(d, valor);
        
    else if(strcmp(campo, "CodLinha") == 0)
        if(valor != NULL) data_set_cod_lin(d, atoi(valor));
        else data_set_cod_lin(d, -1);

    else if(strcmp(campo, "NomeLinha") == 0)
        data_set_nome_lin(d, valor);

    else if(strcmp(campo, "CodProxEst") == 0)
        if(valor != NULL) data_set_cod_prox(d, atoi(valor));
        else data_set_cod_prox(d, -1);

    else if(strcmp(campo, "DistanciaProxEst") == 0)
        if(valor != NULL) data_set_dist(d, atoi(valor));
        else data_set_dist(d, -1);

    else if(strcmp(campo, "CodLinhaInteg") == 0)
        if(valor != NULL) data_set_cod_lin_int(d, atoi(valor));
        else data_set_cod_lin_int(d, -1);

    else if(strcmp(campo, "CodEstacaoInteg") == 0)
        if(valor != NULL) data_set_cod_est_int(d, atoi(valor));
        else data_set_cod_est_int(d, -1);
}

void func_insert_end(DATA *d, HEADER *h, FILE *bin){
    if(d == NULL || h == NULL) return;

    data_save_all(d, header_get_proxRRN(h), bin);
    header_set_proxRRN(h, header_get_proxRRN(h) + 1);
}

bool func_create(FILE *fe, FILE *fs, bool *erro, HEADER *h){
    char linha[201];
    // Guarda o nome dos campos em ordem 
    char *campos[NMR_CAMPOS];
    char *valores[NMR_CAMPOS];
    // Interface com o registro de dados
    DATA *d;
    // Para contar o número de estações
    ARVORE *ar;

    // Lendo a primeira linha
    if(fgets(linha, 200, fe) != NULL){
        printf("ok 4\n");
        // Divindo nome dos campos
        func_division_line(linha, campos);
    }

    for(int i = 0; i < NMR_CAMPOS; i++) printf("%s\n", campos[i]);

    // Lendo próximas linhas
    /*while(fgets(linha, 100, fe) != NULL){
        // Separando os valores
        func_division_line(linha, valores);

        for(int i = 0; i < NMR_CAMPOS; i++){
            // Preenchendo a estrutura que representa um registro de dados
            func_attribute_value(valores[i], campos[i], d);
            avl_inserir(ar, data_get_nome_est(d));
        }

        // Salvando dados no próximo RRN
        func_insert_end(d, h, fs);

        func_desaloca_strs(valores);
    }*/

    func_desaloca_strs(campos);

    header_set_nmr_estacoes(h, avl_get_n(ar));

    return true;
}

