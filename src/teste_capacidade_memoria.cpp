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
        if (enderecoVirtual % 1000 == 0) { // Exibir a cada 1000 endereços virtuais para acompanhar o progresso
            std::cout << "Alocando endereço virtual " << enderecoVirtual << " -> endereço físico " << enderecoFisico << "\n";
        }

        // Simular escrita no log
        logfile << "Alocando endereço virtual " << enderecoVirtual << " -> endereço físico " << enderecoFisico << "\n";
    }

    if (!falha) {
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
