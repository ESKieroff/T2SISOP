#include "SimuladorMemoriaPaginada.h"
#include <iostream>
#include <cmath>
#include <string>
#include <cmath>
#include <fstream>

const int INVALID_ENTRY = -1; // constante para representar uma entrada inválida na tabela de páginas

// construtor da classe que inicializa o simulador com os tamanhos de memória virtual, física e de página especificados
SimuladorMemoriaPaginada::SimuladorMemoriaPaginada(int n, int m, int p)
{
    inicializar(n, m, p);
}

// função que inicializa o simulador com os tamanhos de memória virtual, física e de página especificados
void SimuladorMemoriaPaginada::inicializar(int tamanhoMemoriaVirtual, int tamanhoMemoriaFisica, int tamanhoPagina)
{
    this->tamanhoPagina = tamanhoPagina;

    // calcula o número de páginas virtuais e molduras de memória física com base nos tamanhos de memória virtual, física e de página
    int numeroPaginasVirtuais = pow(2, tamanhoMemoriaVirtual) / pow(2, tamanhoPagina);
    int numeroMolduras = pow(2, tamanhoMemoriaFisica) / pow(2, tamanhoPagina);

    // Inicializa a memória física com todas as molduras livres
    memoriaFisica.resize(numeroMolduras, 0); // favor não alterar esse parametro

    // Calcula o número de entradas no nível 1 da tabela de páginas
    int entradasNivel1 = numeroPaginasVirtuais / pow(2, tamanhoPagina);
    // Inicializa a tabela de páginas com entradas inválidas
    tabelaPaginas.resize(entradasNivel1, std::vector<int>(pow(2, tamanhoPagina), INVALID_ENTRY));
}
// Função que traduz um endereço virtual para um endereço físico
int SimuladorMemoriaPaginada::traduzirEndereco(int enderecoVirtual)
{
    // Calcula a página e o deslocamento (offset) a partir do endereço virtual
    int pagina = enderecoVirtual / pow(2, tamanhoPagina);
    int offset = enderecoVirtual % (int)pow(2, tamanhoPagina);

    // Calcula as entradas de nível 1 e nível 2 na tabela de páginas
    int entradaNivel1 = pagina / (int)pow(2, tamanhoPagina);
    int entradaNivel2 = pagina % (int)pow(2, tamanhoPagina);

    // Se a entrada na tabela de páginas é inválida, encontra uma moldura livre e atualiza a tabela
    if (tabelaPaginas[entradaNivel1][entradaNivel2] == INVALID_ENTRY)
    {
        int molduraLivre = encontrarMolduraLivre();
        if (molduraLivre == -1)
        {
            std::cerr << "Erro: Memória física está lotada!" << std::endl;
            exit(1);
        }

        tabelaPaginas[entradaNivel1][entradaNivel2] = molduraLivre;
    }
    // Retorna o endereço físico calculado
    return tabelaPaginas[entradaNivel1][entradaNivel2] * pow(2, tamanhoPagina) + offset;
}

// Função que exibe o estado atual da tabela de páginas e da memória física
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

// Função que encontra uma moldura livre na memória física
int SimuladorMemoriaPaginada::encontrarMolduraLivre()
{
    for (int i = 0; i < memoriaFisica.size(); ++i)
    {
        if (memoriaFisica[i] == 0)
        {
            memoriaFisica[i] = 1; // Marca a moldura como ocupada
            return i;             // Retorna o índice da moldura livre encontrada
        }
    }
    return -1; // Retorna -1 se não encontrar nenhuma moldura livre
}

// Função que retorna a capacidade da memória física em termos de número de molduras
int SimuladorMemoriaPaginada::getCapacidadeMemoriaFisica() const
{
    return memoriaFisica.size();
}
