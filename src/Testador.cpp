#include "SimuladorMemoriaPaginada.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

// Função para ler parâmetros do arquivo config.txt
bool lerParametrosDoArquivo(const std::string &nomeArquivo, std::vector<std::tuple<int, int, int>> &parametros)
{
    // Abre o arquivo de configuração
    std::ifstream arquivo(nomeArquivo.c_str());
    if (!arquivo.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << "!\n";
        return false;
    }

    // Lê os parâmetros do arquivo
    int tamanhoMemoriaVirtual, tamanhoMemoriaFisica, tamanhoPagina;
    while (arquivo >> tamanhoMemoriaVirtual >> tamanhoMemoriaFisica >> tamanhoPagina)
    {
        // Armazena os parâmetros lidos em um vetor de tuplas
        parametros.emplace_back(tamanhoMemoriaVirtual, tamanhoMemoriaFisica, tamanhoPagina);
    }

    // Fecha o arquivo
    arquivo.close();
    return true;
}

// Função para testar a tradução de endereços virtuais para físicos
void testeTraducaoEnderecos(SimuladorMemoriaPaginada &simulador, std::ofstream &logfile)
{
    logfile << "Tradução de Endereços Virtuais para Físicos:\n";

    // Define alguns endereços virtuais de teste
    std::vector<int> enderecosVirtuais = {0, 512, 1024, 1536, 2048};

    // Tradução e registro no logfile
    for (int endereco : enderecosVirtuais)
    {
        int enderecoFisico = simulador.traduzirEndereco(endereco);
        logfile << "Endereço Virtual " << endereco << " -> Endereço Físico " << enderecoFisico << "\n";
    }

    // Exibe o estado atual do simulador no logfile
    simulador.exibirEstado(logfile);
}

// Função para testar a tradução com diferentes tamanhos de página
void testeTamanhoPagina(SimuladorMemoriaPaginada &simulador, std::ofstream &logfile)
{
    logfile << "Teste de Tamanho de Página Diferente:\n";

    // Define alguns endereços virtuais de teste
    int enderecosVirtuais[] = {0, 512, 1024, 1536, 2048};
    int numEnderecos = sizeof(enderecosVirtuais) / sizeof(enderecosVirtuais[0]);

    // Tradução e registro no logfile
    for (int i = 0; i < numEnderecos; ++i)
    {
        int enderecoVirtual = enderecosVirtuais[i];
        int enderecoFisico = simulador.traduzirEndereco(enderecoVirtual);
        logfile << "Endereço Virtual " << enderecoVirtual << " -> Endereço Físico " << enderecoFisico << "\n";
    }

    // Exibe o estado atual do simulador no logfile
    simulador.exibirEstado(logfile);
}

// Função para testar a reutilização de molduras
void testeReutilizacaoMolduras(SimuladorMemoriaPaginada &simulador, std::ofstream &logfile)
{
    logfile << "Teste de Reutilização de Molduras:\n";

    // Define alguns endereços virtuais de teste
    int enderecosVirtuais[] = {0, 512, 1024, 1536, 2048};
    int numEnderecos = sizeof(enderecosVirtuais) / sizeof(enderecosVirtuais[0]);

    // Tradução e registro no logfile
    for (int i = 0; i < numEnderecos; ++i)
    {
        int enderecoVirtual = enderecosVirtuais[i];
        int enderecoFisico = simulador.traduzirEndereco(enderecoVirtual);
        logfile << "Endereço Virtual " << enderecoVirtual << " -> Endereço Físico " << enderecoFisico << "\n";
    }

    // Exibe o estado atual do simulador no logfile
    simulador.exibirEstado(logfile);
}

// Função para testar a capacidade total da memória física
void testeCapacidadeMemoria(SimuladorMemoriaPaginada &simulador, std::ofstream &logfile)
{
    int capacidadeMemoriaFisica = simulador.getCapacidadeMemoriaFisica();
    bool falha = false;

    logfile << "Teste de Capacidade Total de Memória Física:\n";

    // Testa a tradução de endereços virtuais até a capacidade máxima da memória física
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

    // Exibe o estado atual do simulador no logfile
    simulador.exibirEstado(logfile);
}

// Função para executar os testes com parâmetros específicos
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

    // Executa os testes específicos
    testeCapacidadeMemoria(simulador, logfile);
    testeReutilizacaoMolduras(simulador, logfile);
    testeTamanhoPagina(simulador, logfile);
    testeTraducaoEnderecos(simulador, logfile);

    logfile.close();
}

// Função principal
int main()
{
    const std::string nomeArquivo = "config.txt";

    // Vetor para armazenar os parâmetros lidos do arquivo
    std::vector<std::tuple<int, int, int>> parametros;
    if (!lerParametrosDoArquivo(nomeArquivo, parametros))
    {
        return 1;
    }

    int testeNumero = 1;

    // Executa os testes para cada conjunto de parâmetros lidos
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
