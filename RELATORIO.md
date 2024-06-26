
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
Tradução de Endereços Virtuais para Físicos:
Endereço Virtual 0 -> Endereço Físico ...
Endereço Virtual 512 -> Endereço Físico ...
Endereço Virtual 1024 -> Endereço Físico ...
...
```

## Considerações Finais

Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis vestibulum velit nec nunc auctor, sit amet convallis leo ultricies. Nulla facilisi. Donec id nisl et justo condimentum dapibus. Vivamus et metus eu arcu convallis volutpat. Sed commodo enim id risus ultricies, sit amet pretium leo consequat. Nullam non diam et magna tempor blandit eget ac eros. Sed a nulla convallis, dictum purus nec, consequat justo. Sed eget turpis neque. Cras eget libero a libero viverra lobortis sit amet nec ipsum. Sed ac tincidunt ex. Sed nec tortor urna. Ut dignissim nibh id justo lacinia, nec fermentum dolor gravida. Sed eget lectus ac risus feugiat accumsan. Vivamus vestibulum sit amet metus nec convallis. Sed eu tellus in justo consectetur aliquet. Vivamus ac erat sit amet nulla dapibus rutrum.

