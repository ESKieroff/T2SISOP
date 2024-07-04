#ifndef SIMULADOR_MEMORIA_PAGINADA_H
#define SIMULADOR_MEMORIA_PAGINADA_H

#include <vector>
#include <ostream>

// Definição da classe SimuladorMemoriaPaginada
class SimuladorMemoriaPaginada {
public:
    // Construtor da classe que inicializa o simulador com parâmetros opcionais
    SimuladorMemoriaPaginada(int n = 0, int m = 0, int p = 0);

    // Método para inicializar o simulador com os tamanhos da memória virtual, física e das páginas
    void inicializar(int tamanhoMemoriaVirtual, int tamanhoMemoriaFisica, int tamanhoPagina);

    // Método para traduzir um endereço virtual para um endereço físico
    int traduzirEndereco(int enderecoVirtual);

    // Método para obter a capacidade da memória física em termos de número de molduras
    int getCapacidadeMemoriaFisica() const;

    // Método para exibir o estado atual da tabela de páginas e da memória física
    void exibirEstado(std::ostream& output);

private:
    // Tabela de páginas representada como um vetor de vetores de inteiros
    std::vector<std::vector<int>> tabelaPaginas;

    // Memória física representada como um vetor de inteiros
    std::vector<int> memoriaFisica;

    // Tamanho da página em bits
    int tamanhoPagina;

    // Método para encontrar uma moldura livre na memória física
    int encontrarMolduraLivre();
};

#endif // SIMULADOR_MEMORIA_PAGINADA_H
