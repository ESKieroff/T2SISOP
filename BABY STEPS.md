# BABY STEPS FOR DUMMIES 

## Tutorial: Simulador de Sistema de Gerência de Memória Paginada em C++

Este tutorial apresenta a implementação do simulador de sistema de gerência de memória paginada em C++. O documento está dividido em três seções: a implementação da classe principal `SimuladorMemoriaPaginada`, a implementação dos casos de teste e a configuração do `Makefile`.

### Classe Principal: `SimuladorMemoriaPaginada`

#### Arquivo: `SimuladorMemoriaPaginada.h`

Este arquivo define a interface da classe `SimuladorMemoriaPaginada`.

```cpp
#ifndef SIMULADOR_MEMORIA_PAGINADA_H
#define SIMULADOR_MEMORIA_PAGINADA_H

#include <vector>
#include <ostream>

class SimuladorMemoriaPaginada
{
public:
    SimuladorMemoriaPaginada(int n = 0, int m = 0, int p = 0);
    void inicializar(int tamanhoMemoriaVirtual, int tamanhoMemoriaFisica, int tamanhoPagina);
    int traduzirEndereco(int enderecoVirtual, std::ofstream &logfile);
    int getCapacidadeMemoriaFisica() const;
    int getCapacidadeMemoriaVirtual() const;
    void exibirEstado(std::ostream &output);
    void exibirEstatisticas(std::ofstream &logfile) const;

private:
    std::vector<std::vector<int>> tabelaPaginas;
    std::vector<int> memoriaFisica;
    int tamanhoPagina;
    int tamanhoMemoriaVirtual;
    int hits;
    int misses;
    std::vector<std::pair<int, int>> ordemPaginas;
    int encontrarMolduraLivre();
};

#endif 

```

1. **Construtor e Métodos Públicos**:
   - `SimuladorMemoriaPaginada(int n, int m, int p)`: Construtor que inicializa o simulador.
   - `void inicializar(int tamanhoMemoriaVirtual, int tamanhoMemoriaFisica, int tamanhoPagina)`: Inicializa o simulador com os tamanhos de memória e página.
   - `int traduzirEndereco(int enderecoVirtual, std::ofstream &logfile)`: Traduz um endereço virtual para um endereço físico, registrando eventos em `logfile`.
   - `void exibirEstado(std::ostream& output)`: Exibe o estado da tabela de páginas e da memória física.
   - `int getCapacidadeMemoriaFisica() const;`: Retorna a capacidade total da memória física em bytes.
   - `int getCapacidadeMemoriaVirtual() const;`: Retorna a capacidade total da memória virtual em bytes.
   - `void exibirEstatisticas(std::ofstream &logfile) const;`: Exibe estatísticas como hits e misses em `logfile`.

2. **Atributos Privados**:
   - `std::vector<std::vector<int>> tabelaPaginas`: Tabela de páginas de dois níveis para mapeamento.
   - `std::vector<int> memoriaFisica`: Vetor representando as molduras da memória física.
   - `int tamanhoPagina`: Tamanho da página em bits.
   - `int tamanhoMemoriaVirtual;`: Tamanho total da memória virtual em bits.
   - `int hits;`: Contador de hits durante traduções de endereços.
   - `int misses;`: Contador de misses durante traduções de endereços.
   - `std::vector<std::pair<int, int>> ordemPaginas;`: Ordem das páginas acessadas para gerenciamento de substituição.

3. **Método Privado**:
   - `int encontrarMolduraLivre()`: Encontra e retorna uma moldura livre na memória física para alocação de página.


#### Arquivo: `SimuladorMemoriaPaginada.cpp`

Este arquivo implementa os métodos definidos na interface.

```cpp
#include "SimuladorMemoriaPaginada.h"
#include <iostream>
#include <cmath>
#include <string>
#include <cmath>
#include <fstream>

const int INVALID_ENTRY = -1;

SimuladorMemoriaPaginada::SimuladorMemoriaPaginada(int n, int m, int p)
{
    inicializar(n, m, p);
}

void SimuladorMemoriaPaginada::inicializar(int tamanhoMemoriaVirtual, int tamanhoMemoriaFisica, int tamanhoPagina)
{
    this->tamanhoPagina = tamanhoPagina;
    this->tamanhoMemoriaVirtual = tamanhoMemoriaVirtual;
    hits = 0;
    misses = 0;
    ordemPaginas.clear();

    size_t numeroPaginasVirtuais = (1ULL << tamanhoMemoriaVirtual) >> tamanhoPagina;
    size_t numeroMolduras = (1ULL << tamanhoMemoriaFisica) >> tamanhoPagina;

    memoriaFisica.resize(numeroMolduras, 0);

    size_t entradasNivel1 = numeroPaginasVirtuais >> tamanhoPagina;
    tabelaPaginas.resize(entradasNivel1, std::vector<int>(1 << tamanhoPagina, INVALID_ENTRY));
}

int SimuladorMemoriaPaginada::traduzirEndereco(int enderecoVirtual, std::ofstream &logfile)
{

    size_t pagina = enderecoVirtual >> tamanhoPagina; 
    size_t offset = enderecoVirtual & ((1 << tamanhoPagina) - 1);

    size_t entradaNivel1 = pagina >> tamanhoPagina;   
    size_t entradaNivel2 = pagina & ((1 << tamanhoPagina) - 1); 

    if (tabelaPaginas[entradaNivel1][entradaNivel2] == INVALID_ENTRY)
    {
        misses++;
        std::cerr << "Page fault!" << std::endl;
        logfile << "Page fault: página " << pagina << " não está na memória física.\n";

        int molduraLivre = encontrarMolduraLivre();
        if (molduraLivre == -1)
        {
            auto paginaAntiga = ordemPaginas.front();
            ordemPaginas.erase(ordemPaginas.begin());

            size_t entradaAntigaNivel1 = paginaAntiga.first;
            size_t entradaAntigaNivel2 = paginaAntiga.second;
            molduraLivre = tabelaPaginas[entradaAntigaNivel1][entradaAntigaNivel2];
            tabelaPaginas[entradaAntigaNivel1][entradaAntigaNivel2] = INVALID_ENTRY;

            logfile << "Substituição de página: removendo página "
                    << (entradaAntigaNivel1 << tamanhoPagina) + entradaAntigaNivel2
                    << " da moldura "
                    << molduraLivre << ".\n";
        }

        tabelaPaginas[entradaNivel1][entradaNivel2] = molduraLivre;
        ordemPaginas.push_back(std::make_pair(entradaNivel1, entradaNivel2));
        logfile << "A página " << pagina << " foi carregada na moldura " << molduraLivre << ".\n";
    }
    else
    {
        hits++; 
    }

    return (tabelaPaginas[entradaNivel1][entradaNivel2] << tamanhoPagina) + offset;
}

void SimuladorMemoriaPaginada::exibirEstado(std::ostream &output)
{
    std::cout << "Conteúdo da Tabela de Páginas:" << std::endl;
    for (size_t i = 0; i < tabelaPaginas.size(); ++i)
    {
        for (size_t j = 0; j < tabelaPaginas[i].size(); ++j)
        {
            if (tabelaPaginas[i][j] != INVALID_ENTRY)
            {
                std::cout << "Página " << i * pow(2, tamanhoPagina) + j << " -> Moldura " << tabelaPaginas[i][j] << std::endl;
            }
        }
    }

    std::cout << "\nEstado da Memória Física:" << std::endl;
    for (size_t i = 0; i < memoriaFisica.size(); ++i)
    {
        std::cout << "Moldura " << i << ": " << (memoriaFisica[i] == 0 ? "Livre" : "Ocupada") << std::endl;
    }
}

int SimuladorMemoriaPaginada::encontrarMolduraLivre()
{
    for (int i = 0; i < memoriaFisica.size(); ++i)
    {
        if (memoriaFisica[i] == 0)
        {
            memoriaFisica[i] = 1; 
            return i; 
        }
    }
    return -1; 
}

int SimuladorMemoriaPaginada::getCapacidadeMemoriaFisica() const
{
    return memoriaFisica.size();
}

int SimuladorMemoriaPaginada::getCapacidadeMemoriaVirtual() const
{
    return tamanhoMemoriaVirtual;
}

void SimuladorMemoriaPaginada::exibirEstatisticas(std::ofstream &logfile) const
{
    logfile << "Estatísticas:\n";
    logfile << "Hits: " << hits << "\n";
    logfile << "Misses: " << misses << "\n";
}
```

1. **Constantes e Construtor**:
   - `const int INVALID_ENTRY = -1`: Define uma constante para entradas inválidas na tabela de páginas.
   - `SimuladorMemoriaPaginada::SimuladorMemoriaPaginada(int n, int m, int p)`: Construtor que inicializa o simulador com os parâmetros fornecidos.

2. **Inicialização**:
   - `void SimuladorMemoriaPaginada::inicializar(int tamanhoMemoriaVirtual, int tamanhoMemoriaFisica, int tamanhoPagina)`: Inicializa o simulador com os tamanhos de memória e página, e configura a tabela de páginas e a memória física.

3. **Tradução de Endereços**:
   - `int SimuladorMemoriaPaginada::traduzirEndereco(int enderecoVirtual)`: Traduz um endereço virtual para um endereço físico, alocando uma moldura se necessário.

4. **Exibição de Estado**:
   - `void SimuladorMemoriaPaginada::exibirEstado(std::ostream &output)`: Exibe o estado atual da tabela de páginas e da memória física.

5. **Busca de Moldura Livre**:
   - `int SimuladorMemoriaPaginada::encontrarMolduraLivre()`: Procura uma moldura livre na memória física e a marca como ocupada.

6. **Obtem a Capacidade de Memória Física**:
   - `int SimuladorMemoriaPaginada::getCapacidadeMemoriaFisica() const`: Obtém o valor da memória física total.

7. **Obtem a Capacidade de Memória Virtual**:
   - `int SimuladorMemoriaPaginada::getCapacidadeMemoriaVirtual() const`: Obtém o valor da memória virtual total.

8. **Exibe Dados sobre Hit e Miss**:
   - `void SimuladorMemoriaPaginada::exibirEstatisticas(std::ofstream &logfile) const`: Exibe os resultados de contadores de hits e misses da bateria de testes.

### Casos de Teste

#### Arquivo: `Testador.cpp`

Este arquivo implementa uma bateria de testes para todos os métodos da classe principal.

```cpp
#include "SimuladorMemoriaPaginada.h"
#include "Testador.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <random>


// Função para ler parâmetros do arquivo config.txt
bool lerParametrosDoArquivo(const std::string &nomeArquivo, std::vector<std::tuple<int, int, int>> &parametros)
{

    std::ifstream arquivo(nomeArquivo.c_str());
    if (!arquivo.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << "!\n";
        return false;
    }

    int tamanhoMemoriaVirtual, tamanhoMemoriaFisica, tamanhoPagina;
    while (arquivo >> tamanhoMemoriaVirtual >> tamanhoMemoriaFisica >> tamanhoPagina) // enquanto houver parametros para ler
    {
        parametros.emplace_back(tamanhoMemoriaVirtual, tamanhoMemoriaFisica, tamanhoPagina); // adiciono a tupla com os parametros na lista
    }

    arquivo.close();
    return true;
}

void testeTraducaoEnderecos(SimuladorMemoriaPaginada &simulador, std::ofstream &logfile)
{
    logfile << "Tradução de Endereços Virtuais para Físicos:\n";

    int capacidadeMemoriaVirtual = simulador.getCapacidadeMemoriaVirtual();
    std::vector<int> enderecosVirtuais = gerarEnderecosVirtuaisAleatorios(5, capacidadeMemoriaVirtual);

    for (int endereco : enderecosVirtuais)
    {
        int enderecoFisico = simulador.traduzirEndereco(endereco, logfile);
        logfile << "Endereço Virtual " << endereco << " -> Endereço Físico " << enderecoFisico << "\n";
    }

    simulador.exibirEstado(logfile);
}

void testeTamanhoPagina(SimuladorMemoriaPaginada &simulador, std::ofstream &logfile)
{
    logfile << "Teste de Tamanho de Página Diferente:\n";

    int capacidadeMemoriaVirtual = simulador.getCapacidadeMemoriaVirtual();
    std::vector<int> enderecosVirtuais = gerarEnderecosVirtuaisAleatorios(5, capacidadeMemoriaVirtual);

    int numEnderecos = sizeof(enderecosVirtuais) / sizeof(enderecosVirtuais[0]);

    for (int i = 0; i < numEnderecos; ++i)
    {
        int enderecoVirtual = enderecosVirtuais[i];
        int enderecoFisico = simulador.traduzirEndereco(enderecoVirtual, logfile);
        logfile << "Endereço Virtual " << enderecoVirtual << " -> Endereço Físico " << enderecoFisico << "\n";
    }

    simulador.exibirEstado(logfile);
}

void testeReutilizacaoMolduras(SimuladorMemoriaPaginada &simulador, std::ofstream &logfile)
{
    logfile << "Teste de Reutilização de Molduras:\n";

    int capacidadeMemoriaVirtual = simulador.getCapacidadeMemoriaVirtual();
    std::vector<int> enderecosVirtuais = gerarEnderecosVirtuaisAleatorios(5, capacidadeMemoriaVirtual);
    
    int numEnderecos = sizeof(enderecosVirtuais) / sizeof(enderecosVirtuais[0]);

    for (int i = 0; i < numEnderecos; ++i)
    {
        int enderecoVirtual = enderecosVirtuais[i];
        int enderecoFisico = simulador.traduzirEndereco(enderecoVirtual, logfile);
        logfile << "Endereço Virtual " << enderecoVirtual << " -> Endereço Físico " << enderecoFisico << "\n";
    }

    simulador.exibirEstado(logfile);
}

void testeCapacidadeMemoria(SimuladorMemoriaPaginada &simulador, std::ofstream &logfile)
{
    int capacidadeMemoriaFisica = simulador.getCapacidadeMemoriaFisica();
    bool falha = false;

    logfile << "Teste de Capacidade Total de Memória Física:\n";

    for (int enderecoVirtual = 0; enderecoVirtual < capacidadeMemoriaFisica; ++enderecoVirtual)
    {
        int enderecoFisico = simulador.traduzirEndereco(enderecoVirtual, logfile);

        if (enderecoFisico == -1)
        {
            logfile << "Falha ao alocar moldura para o endereço virtual " << enderecoVirtual << ". Capacidade excedida.\n";
            falha = true;
            break;
        }

        logfile << "Alocando endereço virtual " << enderecoVirtual << " -> endereço físico " << enderecoFisico << "\n";
    }

    if (!falha)
    {
        logfile << "Todos os " << capacidadeMemoriaFisica << " endereços virtuais alocados com sucesso.\n";
    }

    simulador.exibirEstado(logfile);
}

void executarTestes(const std::tuple<int, int, int> &parametros, int testeNumero)
{
    int tamanhoMemoriaVirtual = std::get<0>(parametros);
    int tamanhoMemoriaFisica = std::get<1>(parametros);
    int tamanhoPagina = std::get<2>(parametros);

    std::ofstream logfile("log_" + std::to_string(testeNumero) + ".txt");
    if (!logfile.is_open())
    {
        std::cerr << "Erro ao abrir arquivo log.txt!\n";
        return;
    }
    logfile << "Teste " << testeNumero << ":\n";
    logfile << "Parametros: Memoria Virtual = " << tamanhoMemoriaVirtual
            << ", Memoria Fisica = " << tamanhoMemoriaFisica
            << ", Tamanho Pagina = " << tamanhoPagina << "\n";

    SimuladorMemoriaPaginada simulador;
    simulador.inicializar(tamanhoMemoriaVirtual, tamanhoMemoriaFisica, tamanhoPagina);

    testeCapacidadeMemoria(simulador, logfile);
    testeReutilizacaoMolduras(simulador, logfile);
    testeTamanhoPagina(simulador, logfile);
    testeTraducaoEnderecos(simulador, logfile);
    simulador.exibirEstatisticas(logfile);

    logfile.close();
}

std::vector<int> gerarEnderecosVirtuaisAleatorios (int numEnderecos, int tamanhoMemoriaVirtual)
{
    std::vector<int> enderecosVirtuais;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, tamanhoMemoriaVirtual - 1);

    for (int i = 0; i < numEnderecos; ++i)
    {
        enderecosVirtuais.push_back(dis(gen));
    }

    return enderecosVirtuais;
}


int main()
{
    const std::string nomeArquivo = "config.txt";

    std::vector<std::tuple<int, int, int>> parametros;
    if (!lerParametrosDoArquivo(nomeArquivo, parametros))
    {
        return 1;
    }

    int testeNumero = 1;

    for (const auto &[tamanhoMemoriaVirtual, tamanhoMemoriaFisica, tamanhoPagina] : parametros)
    {
        
        std::ofstream logfile("log_" + std::to_string(testeNumero) + ".txt");
        if (!logfile.is_open())
        {
            std::cerr << "Erro ao abrir arquivo log.txt!\n";
            continue;
        }
        logfile << "Teste " << testeNumero << ":\n";
        logfile << "Parametros: Memoria Virtual = " << tamanhoMemoriaVirtual
                << ", Memoria Fisica = " << tamanhoMemoriaFisica
                << ", Tamanho Pagina = " << tamanhoPagina << "\n";

        SimuladorMemoriaPaginada simulador;
        simulador.inicializar(tamanhoMemoriaVirtual, tamanhoMemoriaFisica, tamanhoPagina);

        testeCapacidadeMemoria(simulador, logfile);
        testeReutilizacaoMolduras(simulador, logfile);
        testeTamanhoPagina(simulador, logfile);
        testeTraducaoEnderecos(simulador, logfile);
        simulador.exibirEstatisticas(logfile);
        
        logfile << "Fim da bateria de teste. " << testeNumero << "\n";
        logfile.close();
        testeNumero++;
    }

    std::cout << "Todos os testes foram executados. Verifique os logs para mais detalhes." << std::endl;

    return 0;
}
```
1. **Teste de Tradução de Endereços Virtuais para Físicos**:
   - `void testeTraducaoEnderecos(SimuladorMemoriaPaginada &simulador, std::ofstream &logfile)`: Realiza a tradução de endereços virtuais para físicos e registra os resultados no arquivo de log.

2. **Teste de Tamanho de Página Diferente**:
   - `void testeTamanhoPagina(SimuladorMemoriaPaginada &simulador, std::ofstream &logfile)`: Testa a tradução de endereços virtuais com diferentes tamanhos de página e registra os resultados no arquivo de log.

3. **Teste de Reutilização de Molduras**:
   - `void testeReutilizacaoMolduras(SimuladorMem

oriaPaginada &simulador, std::ofstream &logfile)`: Testa a reutilização de molduras na memória física e registra os resultados no arquivo de log.

4. **Teste de Capacidade Total de Memória Física**:
   - `void testeCapacidadeMemoria(SimuladorMemoriaPaginada &simulador, std::ofstream &logfile)`: Testa a capacidade total da memória física para alocação de endereços virtuais e registra os resultados no arquivo de log.

5. **Executar Testes**:
   - `void executarTestes(const std::tuple<int, int, int> &parametros, int testeNumero)`: Executa uma série de testes com base nos parâmetros fornecidos e gera um arquivo de log para cada teste realizado.

6. **Gerar Endereços Virtuais Aleatórios**:
   - `std::vector<int> gerarEnderecosVirtuaisAleatorios(int numEnderecos, int tamanhoMemoriaVirtual)`: Gera um vetor de endereços virtuais aleatórios dentro do tamanho da memória virtual especificada.

7. **Função Principal**:
   - `int main()`: Função principal que coordena a leitura dos parâmetros do arquivo de configuração, a execução dos testes para cada conjunto de parâmetros e a geração de arquivos de log correspondentes.

Este código implementa uma estrutura robusta para testar a funcionalidade da classe `SimuladorMemoriaPaginada` em diferentes cenários configuráveis através do arquivo `config.txt`.

### Makefile

O `Makefile` é usado para compilar os arquivos de origem e gerar os executáveis dos testes.

#### Arquivo: `Makefile`

```Makefile
# Nome dos executáveis
EXEC_TESTADOR = Testador

# Compilador e opções
CXX = g++
CXXFLAGS = -std=c++17

# Diretório dos arquivos fonte e cabeçalho
SRC_DIR = src
INC_DIR = include

# Arquivos fonte
SRC = $(SRC_DIR)/SimuladorMemoriaPaginada.cpp \
	$(SRC_DIR)/Testador.cpp

# Objetos gerados
OBJS = $(SRC_DIR)/SimuladorMemoriaPaginada.o \
	$(SRC_DIR)/Testador.o

# Todos os alvos
all: $(EXEC_TESTADOR)

# Regras de compilação para os executáveis
$(EXEC_TESTADOR): $(SRC_DIR)/$(EXEC_TESTADOR).o $(SRC_DIR)/SimuladorMemoriaPaginada.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regra geral para compilação de objetos
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/SimuladorMemoriaPaginada.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Limpar arquivos intermediários e executáveis
clean:
	rm -f $(SRC_DIR)/*.o
	rm -f $(OBJS)
	rm -f $(EXEC_TESTADOR)

# Marca os alvos 'all' e 'clean' como não arquivos
.PHONY: all clean
```

1. **Variáveis**:
   - `CC = g++`: Define o compilador C++ a ser usado.
   - `CFLAGS = -Wall -std=c++17`: Define as flags de compilação (habilitar todos os avisos e usar a versão C++17).

2. **Alvos**:
   - `all`: Alvo padrão que compila o programa de testes `Testador`.

3. **Compilação dos Arquivos Objeto**:
   - `SimuladorMemoriaPaginada.o`: Compila `SimuladorMemoriaPaginada.cpp`.
   - `Testador.o`: Compila `Testador.cpp`.

4. **Limpeza**:
   - `clean`: Remove todos os arquivos objeto e executáveis gerados. Na verdade não remove, mas fica ali até descobrir como fazer funcionar.

### BUGS

Se você chegou até aqui na leitura (duvido muito) e executou todo o processo, deve ter percebido que o terminal fecha abruptamente nos casos de teste da capacidade de memória e tradução de endereços. Não encontrei o problema.

Também deve ter percebido que a função "aguardaTecla" precisa de ajustes, porque só permite o ENTER nos casos em que funciona.
De resto está rodando belíssimamente. 

A depender do ambiente e das libs, o comando "make" não irá funcionar.

Alguém poderia testar no Linux, pra sabermos se o terminal vai aceitar de boa ou teremos os mesmos problemas.

Caso alguém se sinta confortável, refatora por gentileza este arquivo e os demais para EN-US pra ficar bonitinho no Github. 

### UPDATE 1

Foram alteradas as configurações do arquivo "Testador" para encapsular toda parte de testes e gerar um arquivo de log para cada teste realizado. 
Com ele será possível ajustar os parâmetros no arquivo de teste "config.txt" indicando em inteiros os valores para "tamanhoMemoriaVirtual", "tamanhoMemoriaFisica" e "tamanhoPagina". 
Ao executar o testador, ele exibe o progresso no terminal e copia os dados da execução para o log correspondente. Assim torna-se possível revisitar os dados persistidos. 
Detalhes sobre a execução estão descritos no "README". 
Não será mais necessário utilizar os arquivos de teste individuais. Caso necessite, poderá usá-los fazendo pequenos ajustes, como incluir função para leitura do arquivo de configuração (exemplo de tentativa no "teste_capacidde_memoria").

### UPDATE 2

- alterada forma de cálculo dos endereços e molduras - estava usando potência que gera um double, e por algum motivo estava alocando memória física negativa. Com as alterações para deslocamento resolveu a maioria dos problemas. Próxima etapa tentar usar "long long" para as variáveis. 
- removidos do makefile os programas que não foram mais utilizados para testes e movidos para "old".
- alterada forma de executar os testes: antes era uma lista de endereços fixa; agora uma lista gerada randomicamente. 
- tentativa de configuração para adicionar tratamento de page fault e registro no log.
- tentativa de configuração para registrar hits e miss.
- ajustes de perfumaria no baby steps e relatório.