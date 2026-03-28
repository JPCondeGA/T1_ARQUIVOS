# Structs
## Registro de Cabeçalho
- status
- topo
- proxRRN
- NmrEstacoes -> quantidade de estações com nome diferente
- NmrPares -> quantidade de linhas
-> importante para diminuir o número acesso a disco

## Registro de Dados
- removido
- prox
- codEstacao
- codLinha
- codProxEstacao
- distEstProx
- codLinhaIntegra
- codEstIntegra
- tamNomeEstacao
- nomeEstacao
- tamNomeLinha
- nomeLinha
-> importante para diminuir o número acesso a disco

# Funções
## Escrever Resgistro de Cabeçalho
- Função: só guarda no arquivo
- TAD ou mexer nos campos livremente?

## Escrever Registro de Dados
- Função: só guarda no arquivo
- Função: só lê do arquivo
- Função: receber nomeEstacao
- Função: receber nomeLinha
- Função: colocar lixo
- TAD ou mexer nos campos livremente

## TAD Árvore
- Função: inicializa a árvore lendo o arquivo binário ou csv

## Create
- Função: ler linha e escrever nos campos corretos
- Repetir a função de cima até EOF
- Ignorar a primeira linha 

## Select
- 
