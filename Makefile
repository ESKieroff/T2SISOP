# Nome dos executáveis
EXEC_TRADUCAO = teste_traducao_enderecos
EXEC_CAPACIDADE = teste_capacidade_memoria
EXEC_REUTILIZACAO = teste_reutilizacao_molduras
EXEC_TAMANHO_PAGINA = teste_tamanho_pagina
EXEC_TESTADOR = Testador

# Compilador e opções
CXX = g++
CXXFLAGS = -std=c++17

# Diretório dos arquivos fonte e cabeçalho
SRC_DIR = src
INC_DIR = include

# Arquivos fonte
SRC = $(SRC_DIR)/SimuladorMemoriaPaginada.cpp \
	$(SRC_DIR)/teste_capacidade_memoria.cpp \
	$(SRC_DIR)/teste_reutilizacao_molduras.cpp \
	$(SRC_DIR)/teste_tamanho_pagina.cpp \
	$(SRC_DIR)/teste_traducao_enderecos.cpp \
	$(SRC_DIR)/Testador.cpp

# Objetos gerados
OBJS = $(SRC_DIR)/SimuladorMemoriaPaginada.o \
	$(SRC_DIR)/teste_capacidade_memoria.o \
	$(SRC_DIR)/teste_reutilizacao_molduras.o \
	$(SRC_DIR)/teste_tamanho_pagina.o \
	$(SRC_DIR)/teste_traducao_enderecos.o \
	$(SRC_DIR)/Testador.o

# Todos os alvos
all: $(EXEC_TRADUCAO) $(EXEC_CAPACIDADE) $(EXEC_REUTILIZACAO) $(EXEC_TAMANHO_PAGINA) $(EXEC_TESTADOR)

# Regras de compilação para os executáveis
$(EXEC_TRADUCAO): $(SRC_DIR)/$(EXEC_TRADUCAO).o $(SRC_DIR)/SimuladorMemoriaPaginada.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EXEC_CAPACIDADE): $(SRC_DIR)/$(EXEC_CAPACIDADE).o $(SRC_DIR)/SimuladorMemoriaPaginada.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EXEC_REUTILIZACAO): $(SRC_DIR)/$(EXEC_REUTILIZACAO).o $(SRC_DIR)/SimuladorMemoriaPaginada.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EXEC_TAMANHO_PAGINA): $(SRC_DIR)/$(EXEC_TAMANHO_PAGINA).o $(SRC_DIR)/SimuladorMemoriaPaginada.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EXEC_TESTADOR): $(SRC_DIR)/$(EXEC_TESTADOR).o $(SRC_DIR)/SimuladorMemoriaPaginada.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regra geral para compilação de objetos
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/SimuladorMemoriaPaginada.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Limpar arquivos intermediários e executáveis
clean:
	rm -f $(SRC_DIR)/*.o
	rm -f $(OBJS)
	rm -f $(EXEC_TRADUCAO) $(EXEC_CAPACIDADE) $(EXEC_REUTILIZACAO) $(EXEC_TAMANHO_PAGINA) $(EXEC_TESTADOR)

# Marca os alvos 'all' e 'clean' como não arquivos
.PHONY: all clean