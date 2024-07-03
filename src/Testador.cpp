#include "SimuladorMemoriaPaginada.h"
#include <iostream>
#include <fstream>
#include <string>

// Função para ler parâmetros do arquivo config.txt
bool lerParametrosDoArquivo(int &tamanhoMemoriaVirtual, int &tamanhoMemoriaFisica, int &tamanhoPagina)
{
    std::ifstream arquivo("config.txt");
    if (!arquivo.is_open())
    {
        std::cerr << "Erro ao abrir arquivo config.txt!\n";
        return false;
    }

    arquivo >> tamanhoMemoriaVirtual >> tamanhoMemoriaFisica >> tamanhoPagina;
    arquivo.close();

    return true;
}

// Função para ler parâmetros via terminal
void lerParametrosViaTerminal(int &tamanhoMemoriaVirtual, int &tamanhoMemoriaFisica, int &tamanhoPagina)
{
    std::cout << "Digite o tamanho da memória virtual, memória física e tamanho da página:\n";
    std::cin >> tamanhoMemoriaVirtual >> tamanhoMemoriaFisica >> tamanhoPagina;
}

// Função para escolher entre ler do arquivo ou via terminal
void escolherParametros(int &tamanhoMemoriaVirtual, int &tamanhoMemoriaFisica, int &tamanhoPagina)
{
    char escolha;
    std::cout << "Deseja ler os parâmetros do arquivo config.txt? (S/N): ";
    std::cin >> escolha;

    if (escolha == 'S' || escolha == 's')
    {
        if (!lerParametrosDoArquivo(tamanhoMemoriaVirtual, tamanhoMemoriaFisica, tamanhoPagina))
        {
            std::cerr << "Falha ao ler parâmetros do arquivo. Por favor, insira manualmente.\n";
            lerParametrosViaTerminal(tamanhoMemoriaVirtual, tamanhoMemoriaFisica, tamanhoPagina);
        }
    }
    else
    {
        lerParametrosViaTerminal(tamanhoMemoriaVirtual, tamanhoMemoriaFisica, tamanhoPagina);
    }
}

// Função para testar todas as funcionalidades do simulador
void testarSimulador(int tamanhoMemoriaVirtual, int tamanhoMemoriaFisica, int tamanhoPagina)
{
    // Configuração inicial do simulador
    SimuladorMemoriaPaginada simulador(tamanhoMemoriaVirtual, tamanhoMemoriaFisica, tamanhoPagina);

    // Exemplo de uso do simulador
    std::cout << "Teste de tradução de endereços:\n";
    int enderecosVirtuais[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int numEnderecos = sizeof(enderecosVirtuais) / sizeof(enderecosVirtuais[0]);

    for (int i = 0; i < numEnderecos; ++i)
    {
        int enderecoFisico = simulador.traduzirEndereco(enderecosVirtuais[i]);
        std::cout << "Endereço virtual " << enderecosVirtuais[i] << " traduzido para endereço físico " << enderecoFisico << "\n";
    }

    // Exibir estado atual da tabela de páginas e da memória física no console
    simulador.exibirEstado(std::cout);
}

int main()
{
    int tamanhoMemoriaVirtual, tamanhoMemoriaFisica, tamanhoPagina;

    // Escolher como obter os parâmetros
    escolherParametros(tamanhoMemoriaVirtual, tamanhoMemoriaFisica, tamanhoPagina);

    // Testar todas as funcionalidades do simulador com os parâmetros obtidos
    testarSimulador(tamanhoMemoriaVirtual, tamanhoMemoriaFisica, tamanhoPagina);

    return 0;
}
