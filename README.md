
# Simulador de Sistema de Gerência de Memória Paginada

Este é um simulador em C++ de um sistema de gerência de memória paginada, onde endereços virtuais são traduzidos em endereços físicos usando uma tabela de páginas de dois níveis. O simulador foi desenvolvido como parte de um projeto acadêmico de estudo.

## Requisitos do Sistema

- **Compilador C++**: Recomenda-se um compilador C++ compatível com C++11 ou superior.
- **Sistema Operacional**: Compatível com ambientes Unix/Linux ou Windows.

## Como Rodar

### Compilação
1. Clone o repositório:

   ```
   git clone https://github.com/ESKieroff/T2SISOP.git
   ```

2. Navegue até o diretório do projeto:

   ```
   cd T2SISOP
   ```

3. Compile o código fonte usando Makefile:

   ```
   make
   ```

### Execução

4. Execute o programa de testes:

   ```
   ./Testador
   ```

5. Verifique os logs gerados para cada conjunto de parâmetros de teste. Os logs estarão nos arquivos `log_*.txt`.

## Funcionalidades

- Tradução de endereços virtuais para físicos usando uma tabela de páginas de dois níveis.
- Alocação dinâmica de molduras de memória física conforme necessidade.
- Verificação de lotação da memória física e tratamento de erros associados.
- Exibição do estado atual da tabela de páginas e da memória física após cada tradução de endereço.

## Exemplos de Uso

### Exemplo 1: Configuração Padrão

Configuração com tamanho da memória virtual = 12 bits, tamanho da memória física = 14 bits, tamanho da página = 4 bits.

### Exemplo 2: Teste de Capacidade

Executar um conjunto de endereços virtuais suficientes para preencher completamente a memória física e verificar o tratamento correto de memória lotada.

## Limpeza

Para remover os arquivos compilados e os logs gerados, utilize o comando:

```
make clean
```

## Contribuições

Contribuições são bem-vindas! Sinta-se à vontade para enviar pull requests com melhorias no código ou na documentação.

## Licença

Este projeto está licenciado sob a [MIT License](LICENSE).

