
# Sistemas Operacionais
André Sacilotto Santos
Florensa D'Ávila Dimer
Leonardo Vargas Schilling
Rodrigo Oliveira Rosa
2024/1

# Relatório: Simulador de Sistema de Gerência de Memória Paginada

## Abstract

Este relatório descreve o desenvolvimento de um simulador em C++ para um sistema de gerência de memória paginada. O simulador traduz endereços virtuais em endereços físicos utilizando uma tabela de páginas de dois níveis e implementa alocação dinâmica de molduras na memória física conforme necessário.

## Palavras-chave

Sistema Operacional, Memória Paginada, Tabela de Páginas, C++, Simulação.

## Introdução

A gerência de memória é uma funcionalidade crucial dos sistemas operacionais modernos, permitindo o uso eficiente da memória disponível através da técnica de paginação. Neste trabalho, desenvolvemos um simulador em C++ para explorar os conceitos fundamentais da memória paginada, onde endereços virtuais são mapeados em endereços físicos utilizando uma tabela de páginas de dois níveis.

## Solução (Desenvolvimento)

### Implementação

A implementação consiste em uma classe `SimuladorMemoriaPaginada` que permite a configuração dos parâmetros de tamanho da memória virtual, memória física e tamanho da página durante a inicialização. A estrutura de dados inclui uma tabela de páginas de dois níveis (`nivel1` e `nivel2`) e um vetor para representar a memória física. A tradução de endereços virtuais para físicos é realizada pela função `traduzirEndereco`, que aloca molduras físicas conforme necessário e mantém o estado atualizado da tabela de páginas e da memória física.

### Detalhes de Implementação

A classe `SimuladorMemoriaPaginada` utiliza vetores e operações matemáticas simples para calcular os índices de página e offset, além de gerenciar a alocação dinâmica de molduras na memória física. A tabela de páginas é inicializada com valores `-1` para indicar entradas não utilizadas, enquanto o vetor de memória física é inicializado com zeros para indicar molduras livres.

## Resultados (Testes)

### Conjunto de Testes Realizados

1. **Teste Básico de Tradução de Endereços**:
   - Verificação da correta tradução de endereços virtuais para físicos.
   - Confirmação do estado atualizado da tabela de páginas e da memória física após cada tradução.

2. **Teste de Capacidade Total de Memória Física**:
   - Avaliação do comportamento do simulador ao tentar alocar mais molduras do que disponível na memória física.
   - Verificação do tratamento adequado de erro quando a memória física está lotada.

3. **Teste de Reutilização de Molduras**:
   - Verificação de que endereços virtuais que mapeiam para a mesma moldura física reutilizam a mesma moldura sem remapeamento.

### Exemplo de Saída

```
Teste 1:
Parametros: Memoria Virtual = 16, Memoria Fisica = 8, Tamanho Pagina = 2
Teste de Capacidade Total de Memória Física:
Page fault: página 0 não está na memória física.
A página 0 foi carregada na moldura 0.
Alocando endereço virtual 0 -> endereço físico 0
Alocando endereço virtual 1 -> endereço físico 1
Alocando endereço virtual 2 -> endereço físico 2
Alocando endereço virtual 3 -> endereço físico 3
Alocando endereço virtual 4 -> endereço físico 2004
...
Page fault: página 4 não está na memória física.
A página 4 foi carregada na moldura 1.
Alocando endereço virtual 16 -> endereço físico 4
...
Alocando endereço virtual 63 -> endereço físico 51
Todos os 64 endereços virtuais alocados com sucesso.
Teste de Reutilização de Molduras:
Endereço Virtual 2 -> Endereço Físico 2
Endereço Virtual 4 -> Endereço Físico 2004
Endereço Virtual 2 -> Endereço Físico 2
Endereço Virtual 7 -> Endereço Físico 2007
Endereço Virtual 5 -> Endereço Físico 2005
Endereço Virtual 0 -> Endereço Físico 0
Teste de Tamanho de Página Diferente:
...
Endereço Virtual 4 -> Endereço Físico 2004
Endereço Virtual 13 -> Endereço Físico 2005
Endereço Virtual 0 -> Endereço Físico 0
Tradução de Endereços Virtuais para Físicos:
Endereço Virtual 2 -> Endereço Físico 2
Endereço Virtual 2 -> Endereço Físico 2
Endereço Virtual 0 -> Endereço Físico 0
Endereço Virtual 14 -> Endereço Físico 2006
Endereço Virtual 0 -> Endereço Físico 0
Estatísticas:
Hits: 68
Misses: 13
Fim da bateria de teste. 1

```

## Considerações Finais

O trabalho foi desafiador, e aprender uma linguagem enquanto desenvolve um problema é enriquecedor. Houve necessidade de buscarmos soluções externas, e tivemos até o último momento da entrega dificuldade em compreender conceitos abordados em aula para colocá-los em prática. Pode parecer trivial, mas é difícil implementar "miss" e "hit" pensando em uma estrutura enxuta de testes. No apanhado geral, somou para a equipe. 

Rodrigo - Essa é a segunda disciplina em que tenho que trabalhar com C++ e estou adorando. Apesar da dificuldade inicial e muitos conceitos ou funcionalidades que acabam saindo em sua maioria de fóruns na internet ou ajuda de ia, estou aprendendo bastante e pretendo me profissionalizar na linguagem para que possa incluir no meu portifólio. Não considero uma das melhores, mas consigo tolerar mais que java, infinitamente mais. 

Muito grato por você dedicar seu precioso tempo para ler. :smile: