#ifndef SIMULADOR_MEMORIA_PAGINADA_H
#define SIMULADOR_MEMORIA_PAGINADA_H

#include <vector>
#include <ostream>

class SimuladorMemoriaPaginada
{
public:
    SimuladorMemoriaPaginada(int n = 0, int m = 0, int p = 0);
    void inicializar(int tamanhoMemoriaVirtual, int tamanhoMemoriaFisica, int tamanhoPagina);
    int traduzirEndereco(int enderecoVirtual);
    int getCapacidadeMemoriaFisica() const;
    int getCapacidadeMemoriaVirtual() const;
    void exibirEstado(std::ostream &output);
    void exibirEstatisticas(std::ofstream &logfile) const;

private:
    std::vector<std::vector<int>> tabelaPaginas;
    std::vector<int> memoriaFisica;
    int tamanhoPagina;
    int tamanhoMemoriaVirtual;
    int hits;
    int misses;


    int encontrarMolduraLivre();
};

#endif // SIMULADOR_MEMORIA_PAGINADA_H
