#include <iostream>
#include <fstream>
#include "SimuladorMemoriaPaginada.h"

void aguardarTecla()
{
    std::cout << "Pressione qualquer tecla para sair...";
    std::cin.ignore(); // Ignorar o próximo caractere no buffer (que seria o Enter)
    std::cin.get();    // Esperar até que o usuário pressione Enter
}

bool lerParametrosDoArquivo(const std::string &nomeArquivo, int &tamanhoMemoriaVirtual, int &tamanhoMemoriaFisica, int &tamanhoPagina)
{
    std::ifstream arquivo(nomeArquivo.c_str());
    if (!arquivo.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de configuração!" << std::endl;
        return false;
    }

    std::string chave;
    while (arquivo >> chave)
    {
        if (chave == "tamanhoMemoriaVirtual")
        {
            arquivo >> tamanhoMemoriaVirtual;
        }
        else if (chave == "tamanhoMemoriaFisica")
        {
            arquivo >> tamanhoMemoriaFisica;
        }
        else if (chave == "tamanhoPagina")
        {
            arquivo >> tamanhoPagina;
        }
    }

    arquivo.close();
    return true;
}

int main()
{
    // valores default - Tamanho da memória virtual = 12 bits, física = 14 bits, página = 4 bits
    int tamanhoMemoriaVirtual = 12; // Valor padrão
    int tamanhoMemoriaFisica = 14;  // Valor padrão
    int tamanhoPagina = 4;          // Valor padrão

    // Tentar ler os parâmetros do arquivo de configuração
    if (!lerParametrosDoArquivo("config.txt", tamanhoMemoriaVirtual, tamanhoMemoriaFisica, tamanhoPagina))
    {
        std::cout << "Usando valores padrao: "
        << "Memoria Virtual = " << tamanhoMemoriaVirtual
        << ", Memoria Fisica = " << tamanhoMemoriaFisica
        << ", Tamanho da Pagina = " << tamanhoPagina
        << std::endl;
    }

    // Configuração inicial do simulador
    // pega os valores lidos do arquivo ou os valores padrão caso não consiga ler o arquivo
    SimuladorMemoriaPaginada simulador;
    simulador.inicializar(tamanhoMemoriaVirtual, tamanhoMemoriaFisica, tamanhoPagina);

    std::cout << "Teste de Capacidade Total de Memória Física:\n";

    // Preencher gradualmente a memória física até a capacidade total
    int capacidadeMemoriaFisica = 1 << 14; // Capacidade total da memória física (14 bits)
    std::ofstream logfile("log_capacidade_memoria.txt");
    if (!logfile.is_open())
    {
        std::cerr << "Erro ao abrir arquivo de log!" << std::endl;
        return 1;
    }

    bool falha = false;

    for (int enderecoVirtual = 0; enderecoVirtual < capacidadeMemoriaFisica; ++enderecoVirtual)
    {
        int enderecoFisico = simulador.traduzirEndereco(enderecoVirtual);

        if (enderecoFisico == -1)
        {
            std::cout << "Falha ao alocar moldura para o endereço virtual " << enderecoVirtual << ". Capacidade excedida.\n";
            logfile << "Falha ao alocar moldura para o endereço virtual " << enderecoVirtual << ". Capacidade excedida.\n";
            falha = true;
            break;
        }

        // Simulação de preenchimento da memória física
        if (enderecoVirtual % 1000 == 0)
        { // Exibir a cada 1000 endereços virtuais para acompanhar o progresso
            std::cout << "Alocando endereço virtual " << enderecoVirtual << " -> endereço físico " << enderecoFisico << "\n";
        }

        // Simular escrita no log
        logfile << "Alocando endereço virtual " << enderecoVirtual << " -> endereço físico " << enderecoFisico << "\n";
    }

    if (!falha)
    {
        std::cout << "Todos os " << capacidadeMemoriaFisica << " endereços virtuais alocados com sucesso.\n";
        logfile << "Todos os " << capacidadeMemoriaFisica << " endereços virtuais alocados com sucesso.\n";
    }

    // Exibir estado atual da tabela de páginas e da memória física no console
    simulador.exibirEstado(std::cout);

    // Exibir estado atual da tabela de páginas e da memória física no arquivo de log
    simulador.exibirEstado(logfile);

    logfile.close();

    aguardarTecla();

    return 0;
}
