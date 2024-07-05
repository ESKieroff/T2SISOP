# Nome dos executáveis
EXEC_TESTADOR = Testador

# Compilador e opções
CXX = g++
CXXFLAGS = -std=c++17

# Diretório dos arquivos fonte e cabeçalho
SRC_DIR = src
INC_DIR = include

# Arquivos fonte
SRC = $(SRC_DIR)/SimuladorMemoriaPaginada.cpp \
	$(SRC_DIR)/Testador.cpp

# Objetos gerados
OBJS = $(SRC_DIR)/SimuladorMemoriaPaginada.o \
	$(SRC_DIR)/Testador.o

# Todos os alvos
all: $(EXEC_TESTADOR)

# Regras de compilação para os executáveis
$(EXEC_TESTADOR): $(SRC_DIR)/$(EXEC_TESTADOR).o $(SRC_DIR)/SimuladorMemoriaPaginada.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regra geral para compilação de objetos
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/SimuladorMemoriaPaginada.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Limpar arquivos intermediários e executáveis
clean:
	rm -f $(SRC_DIR)/*.o
	rm -f $(OBJS)
	rm -f $(EXEC_TESTADOR)

# Marca os alvos 'all' e 'clean' como não arquivos
.PHONY: all clean