#include <iostream>
#include <fstream>
#include "SimuladorMemoriaPaginada.h"

int main()
{
    // Configuração inicial do simulador
    SimuladorMemoriaPaginada simulador;
    simulador.inicializar(12, 14, 4); // Tamanho da memória virtual = 12 bits, física = 14 bits, página = 4 bits

    std::cout << "Teste de Reutilização de Molduras:\n";

    // Mapeamento de endereços virtuais para a mesma moldura física
    int enderecosVirtuais[] = {0, 512, 1024, 1536, 2048};
    int numEnderecos = sizeof(enderecosVirtuais) / sizeof(enderecosVirtuais[0]);

    for (int i = 0; i < numEnderecos; ++i)
    {
        int enderecoVirtual = enderecosVirtuais[i];
        int enderecoFisico = simulador.traduzirEndereco(enderecoVirtual, logfile);
        std::cout << "Endereço Virtual " << enderecoVirtual << " -> Endereço Físico " << enderecoFisico << "\n";
    }

    // Abrir arquivo de log para escrita
    std::ofstream logfile("log_reutilizacao_molduras.txt");
    if (!logfile.is_open())
    {
        std::cerr << "Erro ao abrir arquivo de log!" << std::endl;
        return 1;
    }

    // Escrita no arquivo de log
    logfile << "Teste de Reutilização de Molduras:\n";
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
