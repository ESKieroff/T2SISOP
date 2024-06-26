#include <iostream>
#include <fstream>
#include "SimuladorMemoriaPaginada.h"

int main()
{
    // Configuração inicial do simulador
    SimuladorMemoriaPaginada simulador;
    simulador.inicializar(12, 14, 8); // Tamanho da memória virtual = 12 bits, física = 14 bits, página = 8 bits

    std::cout << "Teste de Tamanho de Página Diferente:\n";

    // Mapeamento de endereços virtuais para a mesma moldura física
    int enderecosVirtuais[] = {0, 512, 1024, 1536, 2048};
    int numEnderecos = sizeof(enderecosVirtuais) / sizeof(enderecosVirtuais[0]);

    for (int i = 0; i < numEnderecos; ++i)
    {
        int enderecoVirtual = enderecosVirtuais[i];
        int enderecoFisico = simulador.traduzirEndereco(enderecoVirtual);
        std::cout << "Endereço Virtual " << enderecoVirtual << " -> Endereço Físico " << enderecoFisico << "\n";
    }

    // Abrir arquivo de log para escrita
    std::ofstream logfile("log_tamanho_pagina.txt");
    if (!logfile.is_open())
    {
        std::cerr << "Erro ao abrir arquivo de log!" << std::endl;
        return 1;
    }

    // Tradução de endereços e escrita no log
    logfile << "Teste de Tamanho de Página Diferente:\n";
    for (int endereco : enderecosVirtuais)
    {
        int enderecoFisico = simulador.traduzirEndereco(endereco);
        logfile << "Endereço Virtual " << endereco << " -> Endereço Físico " << enderecoFisico << "\n";
    }

    // Exibir estado atual da tabela de páginas e da memória física no arquivo de log
    simulador.exibirEstado(logfile);

    logfile.close();

    std::cout << "Teste concluído. Pressione qualquer tecla para sair..." << std::endl;
    std::cin.get();

    return 0;
}
