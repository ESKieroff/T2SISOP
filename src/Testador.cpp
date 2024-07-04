#include "SimuladorMemoriaPaginada.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>



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

    std::vector<int> enderecosVirtuais = {0, 512, 1024, 1536, 2048};

    for (int endereco : enderecosVirtuais)
    {
        int enderecoFisico = simulador.traduzirEndereco(endereco);
        logfile << "Endereço Virtual " << endereco << " -> Endereço Físico " << enderecoFisico << "\n";
    }

    simulador.exibirEstado(logfile);
}

void testeTamanhoPagina(SimuladorMemoriaPaginada &simulador, std::ofstream &logfile)
{
    logfile << "Teste de Tamanho de Página Diferente:\n";

    int enderecosVirtuais[] = {0, 512, 1024, 1536, 2048};
    int numEnderecos = sizeof(enderecosVirtuais) / sizeof(enderecosVirtuais[0]);

    for (int i = 0; i < numEnderecos; ++i)
    {
        int enderecoVirtual = enderecosVirtuais[i];
        int enderecoFisico = simulador.traduzirEndereco(enderecoVirtual);
        logfile << "Endereço Virtual " << enderecoVirtual << " -> Endereço Físico " << enderecoFisico << "\n";
    }

    simulador.exibirEstado(logfile);
}

void testeReutilizacaoMolduras(SimuladorMemoriaPaginada &simulador, std::ofstream &logfile)
{
    logfile << "Teste de Reutilização de Molduras:\n";

    int enderecosVirtuais[] = {0, 512, 1024, 1536, 2048};
    int numEnderecos = sizeof(enderecosVirtuais) / sizeof(enderecosVirtuais[0]);

    for (int i = 0; i < numEnderecos; ++i)
    {
        int enderecoVirtual = enderecosVirtuais[i];
        int enderecoFisico = simulador.traduzirEndereco(enderecoVirtual);
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
        int enderecoFisico = simulador.traduzirEndereco(enderecoVirtual);

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

    logfile.close();
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

        logfile << "Fim da bateria de teste. " << testeNumero << "\n";
        logfile.close();
        testeNumero++;
    }

    std::cout << "Todos os testes foram executados. Verifique os logs para mais detalhes." << std::endl;

    return 0;
}
