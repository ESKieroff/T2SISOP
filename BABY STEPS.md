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

class SimuladorMemoriaPaginada {
public:
    SimuladorMemoriaPaginada(int n = 0, int m = 0, int p = 0);
    void inicializar(int tamanhoMemoriaVirtual, int tamanhoMemoriaFisica, int tamanhoPagina);
    int traduzirEndereco(int enderecoVirtual);
    void exibirEstado(std::ostream& output);

private:
    std::vector<std::vector<int>> tabelaPaginas;
    std::vector<int> memoriaFisica;
    int tamanhoPagina;

    int encontrarMolduraLivre();
};

#endif // SIMULADOR_MEMORIA_PAGINADA_H
```

1. **Construtor e Métodos Públicos**:
   - `SimuladorMemoriaPaginada(int n, int m, int p)`: Construtor que inicializa o simulador.
   - `void inicializar(int tamanhoMemoriaVirtual, int tamanhoMemoriaFisica, int tamanhoPagina)`: Método para inicializar o simulador com os tamanhos de memória e página.
   - `int traduzirEndereco(int enderecoVirtual)`: Método para traduzir um endereço virtual para um endereço físico.
   - `void exibirEstado(std::ostream& output)`: Método para exibir o estado da tabela de páginas e da memória física.

2. **Atributos Privados**:
   - `std::vector<std::vector<int>> tabelaPaginas`: Tabela de páginas de dois níveis.
   - `std::vector<int> memoriaFisica`: Vetor representando as molduras da memória física.
   - `int tamanhoPagina`: Tamanho da página em bits.

3. **Método Privado**:
   - `int encontrarMolduraLivre()`: Método para encontrar uma moldura livre na memória física.

#### Arquivo: `SimuladorMemoriaPaginada.cpp`

Este arquivo implementa os métodos definidos na interface.

```cpp
#include "SimuladorMemoriaPaginada.h"
#include <iostream>
#include <cmath>

const int INVALID_ENTRY = -1;

SimuladorMemoriaPaginada::SimuladorMemoriaPaginada(int n, int m, int p) {
    inicializar(n, m, p);
}

void SimuladorMemoriaPaginada::inicializar(int tamanhoMemoriaVirtual, int tamanhoMemoriaFisica, int tamanhoPagina) {
    this->tamanhoPagina = tamanhoPagina;

    int numeroPaginasVirtuais = pow(2, tamanhoMemoriaVirtual) / pow(2, tamanhoPagina);
    int numeroMolduras = pow(2, tamanhoMemoriaFisica) / pow(2, tamanhoPagina);

    memoriaFisica.resize(numeroMolduras, 0);

    int entradasNivel1 = numeroPaginasVirtuais / pow(2, tamanhoPagina);
    tabelaPaginas.resize(entradasNivel1, std::vector<int>(pow(2, tamanhoPagina), INVALID_ENTRY));
}

int SimuladorMemoriaPaginada::traduzirEndereco(int enderecoVirtual) {
    int pagina = enderecoVirtual / pow(2, tamanhoPagina);
    int offset = enderecoVirtual % (int)pow(2, tamanhoPagina);

    int entradaNivel1 = pagina / (int)pow(2, tamanhoPagina);
    int entradaNivel2 = pagina % (int)pow(2, tamanhoPagina);

    if (tabelaPaginas[entradaNivel1][entradaNivel2] == INVALID_ENTRY) {
        int molduraLivre = encontrarMolduraLivre();
        if (molduraLivre == -1) {
            std::cerr << "Erro: Memória física está lotada!" << std::endl;
            exit(1);
        }

        tabelaPaginas[entradaNivel1][entradaNivel2] = molduraLivre;
    }

    return tabelaPaginas[entradaNivel1][entradaNivel2] * pow(2, tamanhoPagina) + offset;
}

void SimuladorMemoriaPaginada::exibirEstado(std::ostream &output) {
    std::cout << "Conteúdo da Tabela de Páginas:" << std::endl;
    for (size_t i = 0; i < tabelaPaginas.size(); ++i) {
        for (size_t j = 0; j < tabelaPaginas[i].size(); ++j) {
            if (tabelaPaginas[i][j] != INVALID_ENTRY) {
                std::cout << "Página " << i * pow(2, tamanhoPagina) + j << " -> Moldura " << tabelaPaginas[i][j] << std::endl;
            }
        }
    }

    std::cout << "\nEstado da Memória Física:" << std::endl;
    for (size_t i = 0; i < memoriaFisica.size(); ++i) {
        std::cout << "Moldura " << i << ": " << (memoriaFisica[i] == 0 ? "Livre" : "Ocupada") << std::endl;
    }
}

int SimuladorMemoriaPaginada::encontrarMolduraLivre() {
    for (int i = 0; i < memoriaFisica.size(); ++i) {
        if (memoriaFisica[i] == 0) {
            memoriaFisica[i] = 1; // Marca a moldura como ocupada
            return i;             // Retorna o índice da moldura livre encontrada
        }
    }
    return -1; // Retorna -1 se não encontrar nenhuma moldura livre
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

### Casos de Teste

#### Arquivo: `teste_capacidade_memoria.cpp`

Este arquivo testa a capacidade total da memória física.

```cpp
#include <iostream>
#include <fstream>
#include "SimuladorMemoriaPaginada.h"

void aguardarTecla() {
    std::cout << "Pressione qualquer tecla para sair...";
    std::cin.ignore(); // Ignorar o próximo caractere no buffer (que seria o Enter)
    std::cin.get();    // Esperar até que o usuário pressione Enter
}

int main() {
    // Configuração inicial do simulador
    SimuladorMemoriaPaginada simulador;
    simulador.inicializar(12, 14, 4); // Tamanho da memória virtual = 12 bits, física = 14 bits, página = 4 bits

    std::cout << "Teste de Capacidade Total de Memória Física:\n";

    // Preencher gradualmente a memória física até a capacidade total
    int capacidadeMemoriaFisica = 1 << 14; // Capacidade total da memória física (14 bits)
    std::ofstream logfile("log_capacidade_memoria.txt");
    if (!logfile.is_open()) {
        std::cerr << "Erro ao abrir arquivo de log!" << std::endl;
        return 1;
    }

    bool falha = false;

    for (int enderecoVirtual = 0; enderecoVirtual < capacidadeMemoriaFisica; ++enderecoVirtual) {
        int enderecoFisico = simulador.traduzirEndereco(enderecoVirtual);

        if (enderecoFisico == -1) {
            std::cout << "Falha ao alocar moldura para o endereço virtual " << enderecoVirtual << ". Capacidade excedida.\n";
            logfile << "Falha ao alocar moldura para o endereço virtual " << enderecoVirtual << ". Capacidade excedida.\n";
            falha = true;
            break;
        }

        // Simulação de preenchimento da memória física
        if (enderecoVirtual % 1000 == 

0) {
            std::cout << "Endereço virtual " << enderecoVirtual << " mapeado para endereço físico " << enderecoFisico << ".\n";
            logfile << "Endereço virtual " << enderecoVirtual << " mapeado para endereço físico " << enderecoFisico << ".\n";
        }
    }

    if (!falha) {
        std::cout << "Memória física preenchida até a capacidade total com sucesso.\n";
        logfile << "Memória física preenchida até a capacidade total com sucesso.\n";
    }

    simulador.exibirEstado(std::cout);
    simulador.exibirEstado(logfile);

    logfile.close();

    aguardarTecla();

    return 0;
}
```

1. **Função `aguardarTecla`**:
   - `void aguardarTecla()`: Espera até que o usuário pressione Enter para sair.

2. **Função `main`**:
   - **Configuração**: Inicializa o simulador com tamanhos de memória virtual, física e de página.
   - **Teste de Capacidade**: Preenche gradualmente a memória física até a capacidade total, registrando o mapeamento de endereços virtuais para físicos em um arquivo de log.
   - **Exibição de Estado**: Exibe o estado final do simulador no console e no arquivo de log.

#### Arquivo: `teste_traducao_enderecos.cpp`

Este arquivo testa a tradução de endereços virtuais para endereços físicos.

```cpp
#include <iostream>
#include <fstream>
#include "SimuladorMemoriaPaginada.h"

void aguardarTecla() {
    std::cout << "Pressione qualquer tecla para sair...";
    std::cin.ignore(); // Ignorar o próximo caractere no buffer (que seria o Enter)
    std::cin.get();    // Esperar até que o usuário pressione Enter
}

int main() {
    // Configuração inicial do simulador
    SimuladorMemoriaPaginada simulador;
    simulador.inicializar(12, 14, 4); // Tamanho da memória virtual = 12 bits, física = 14 bits, página = 4 bits

    std::cout << "Teste de Tradução de Endereços:\n";

    // Sequência de endereços virtuais para testar
    int enderecosVirtuais[] = {0x001, 0x010, 0x100, 0x200, 0x0100, 0x0200, 0x0300, 0x0400};
    std::ofstream logfile("log_traducao_enderecos.txt");
    if (!logfile.is_open()) {
        std::cerr << "Erro ao abrir arquivo de log!" << std::endl;
        return 1;
    }

    for (int enderecoVirtual : enderecosVirtuais) {
        int enderecoFisico = simulador.traduzirEndereco(enderecoVirtual);
        std::cout << "Endereço virtual " << enderecoVirtual << " mapeado para endereço físico " << enderecoFisico << ".\n";
        logfile << "Endereço virtual " << enderecoVirtual << " mapeado para endereço físico " << enderecoFisico << ".\n";
    }

    simulador.exibirEstado(std::cout);
    simulador.exibirEstado(logfile);

    logfile.close();

    aguardarTecla();

    return 0;
}
```

1. **Função `aguardarTecla`**:
   - `void aguardarTecla()`: Espera até que o usuário pressione Enter para sair.

2. **Função `main`**:
   - **Configuração**: Inicializa o simulador com tamanhos de memória virtual, física e de página.
   - **Teste de Tradução**: Testa a tradução de uma sequência de endereços virtuais para endereços físicos, registrando os resultados em um arquivo de log.
   - **Exibição de Estado**: Exibe o estado final do simulador no console e no arquivo de log.


### Makefile

O `Makefile` é usado para compilar os arquivos de origem e gerar os executáveis dos testes.

#### Arquivo: `Makefile`

```Makefile
CC = g++
CFLAGS = -Wall -std=c++11

all: teste_capacidade_memoria teste_traducao_enderecos

teste_capacidade_memoria: teste_capacidade_memoria.o SimuladorMemoriaPaginada.o
	$(CC) $(CFLAGS) -o teste_capacidade_memoria teste_capacidade_memoria.o SimuladorMemoriaPaginada.o

teste_traducao_enderecos: teste_traducao_enderecos.o SimuladorMemoriaPaginada.o
	$(CC) $(CFLAGS) -o teste_traducao_enderecos teste_traducao_enderecos.o SimuladorMemoriaPaginada.o

SimuladorMemoriaPaginada.o: SimuladorMemoriaPaginada.cpp SimuladorMemoriaPaginada.h
	$(CC) $(CFLAGS) -c SimuladorMemoriaPaginada.cpp

teste_capacidade_memoria.o: teste_capacidade_memoria.cpp SimuladorMemoriaPaginada.h
	$(CC) $(CFLAGS) -c teste_capacidade_memoria.cpp

teste_traducao_enderecos.o: teste_traducao_enderecos.cpp SimuladorMemoriaPaginada.h
	$(CC) $(CFLAGS) -c teste_traducao_enderecos.cpp

clean:
	rm -f *.o teste_capacidade_memoria teste_traducao_enderecos
```

1. **Variáveis**:
   - `CC = g++`: Define o compilador C++ a ser usado.
   - `CFLAGS = -Wall -std=c++11`: Define as flags de compilação (habilitar todos os avisos e usar a versão C++11).

2. **Alvos**:
   - `all`: Alvo padrão que compila os testes `teste_capacidade_memoria` e `teste_traducao_enderecos`.
   - `teste_capacidade_memoria`: Compila e liga os arquivos objeto necessários para o teste de capacidade de memória.
   - `teste_traducao_enderecos`: Compila e liga os arquivos objeto necessários para o teste de tradução de endereços.

3. **Compilação dos Arquivos Objeto**:
   - `SimuladorMemoriaPaginada.o`: Compila `SimuladorMemoriaPaginada.cpp`.
   - `teste_capacidade_memoria.o`: Compila `teste_capacidade_memoria.cpp`.
   - `teste_traducao_enderecos.o`: Compila `teste_traducao_enderecos.cpp`.

4. **Limpeza**:
   - `clean`: Remove todos os arquivos objeto e executáveis gerados.

### BUGS

Se você chegou até aqui na leitura (duvido muito) e executou todo o processo, deve ter percebido que o terminal fecha abruptamente nos casos de teste da capacidade de memória e tradução de endereços. Não encontrei o problema.

Também deve ter percebido que a função "aguardaTecla" precisa de ajustes, porque só permite o ENTER nos casos em que funciona.
De resto está rodando belíssimamente. 

A depender do ambiente e das libs, o comando "make" não irá funcionar.

Alguém poderia testar no Linux, pra sabermos se o terminal vai aceitar de boa ou teremos os mesmos problemas.

Caso alguém se sinta confortável, refatora por gentileza este arquivo e os demais para EN-US pra ficar bonitinho no Github. 