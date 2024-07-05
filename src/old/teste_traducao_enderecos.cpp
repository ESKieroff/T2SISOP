#include <iostream>
#include <fstream> // Para manipulação de arquivos
#include "SimuladorMemoriaPaginada.h"

using namespace std;

int main()
{
    // Inicialização do simulador
    SimuladorMemoriaPaginada simulador;
    simulador.inicializar(10, 12, 2); // Exemplo de inicialização com valores arbitrários

    // Vetor de endereços virtuais para teste
    vector<int> enderecosVirtuais = {0, 512, 1024, 1536, 2048};

    // Abrir arquivo de log para escrita
    ofstream logfile("log_traducao_enderecos.txt");
    if (!logfile.is_open())
    {
        cerr << "Erro ao abrir arquivo de log!" << endl;
        return 1;
    }

    // Tradução de endereços e escrita no log
    logfile << "Tradução de Endereços Virtuais para Físicos:" << endl;
    for (int endereco : enderecosVirtuais)
    {
        int enderecoFisico = simulador.traduzirEndereco(endereco);
        logfile << "Endereço Virtual " << endereco << " -> Endereço Físico " << enderecoFisico << endl;
    }

    // Exibir estado da memória no log
    simulador.exibirEstado(logfile);

    // Fechar o arquivo de log
    logfile.close();

    // Esperar pela interação do usuário antes de sair
    cout << "Teste concluído. Pressione qualquer tecla para sair..." << endl;
    cin.get(); // Aguarda qualquer entrada do teclado

    return 0;
}
