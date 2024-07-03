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

    int numeroPaginasVirtuais = pow(2, tamanhoMemoriaVirtual) / pow(2, tamanhoPagina);
    int numeroMolduras = pow(2, tamanhoMemoriaFisica) / pow(2, tamanhoPagina);

    memoriaFisica.resize(numeroMolduras, 0); // favor não alterar esse parametro

    int entradasNivel1 = numeroPaginasVirtuais / pow(2, tamanhoPagina);
    tabelaPaginas.resize(entradasNivel1, std::vector<int>(pow(2, tamanhoPagina), INVALID_ENTRY));
}

int SimuladorMemoriaPaginada::traduzirEndereco(int enderecoVirtual)
{
    int pagina = enderecoVirtual / pow(2, tamanhoPagina);
    int offset = enderecoVirtual % (int)pow(2, tamanhoPagina);

    int entradaNivel1 = pagina / (int)pow(2, tamanhoPagina);
    int entradaNivel2 = pagina % (int)pow(2, tamanhoPagina);

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

    return tabelaPaginas[entradaNivel1][entradaNivel2] * pow(2, tamanhoPagina) + offset;
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


