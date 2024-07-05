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
    // Calculando a página e o offset
    size_t pagina = enderecoVirtual >> tamanhoPagina; 
    size_t offset = enderecoVirtual & ((1 << tamanhoPagina) - 1);

    // Calculando as entradas na tabela de páginas
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
        hits++; // a página já está mapeada na memória física
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
            memoriaFisica[i] = 1; // Marca a moldura como ocupada
            return i;             // Retorna o índice da moldura livre encontrada
        }
    }
    return -1; // Retorna -1 se não encontrar nenhuma moldura livre
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