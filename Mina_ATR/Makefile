# Makefile para o Trabalho Final de ATR - Projeto Mina_ATR

# Compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -std=c++17 -Wall -g -Iinclude

# -------------------------------------------------------------
# Ajuste importante para GitHub Codespaces / Ubuntu 24.04
# Adiciona caminhos padrão onde libs locais podem ser instaladas
# (ex: /usr/local/lib quando compila manualmente a Paho MQTT C++)
# -------------------------------------------------------------
LDFLAGS = -L/usr/local/lib -L/lib/x86_64-linux-gnu -lpthread -lrt -lpaho-mqttpp3 -lpaho-mqtt3cs

# --- Configuração dos Executáveis ---

# 1. Sistema Embarcado do Caminhão
TARGET_CAMINHAO = caminhao_embarcado
SRCDIR_CAMINHAO = src
SOURCES_CAMINHAO_NO_MAIN = $(filter-out $(SRCDIR_CAMINHAO)/main_caminhao.cpp, $(wildcard $(SRCDIR_CAMINHAO)/*.cpp))
OBJECTS_CAMINHAO = $(SOURCES_CAMINHAO_NO_MAIN:.cpp=.o) $(SRCDIR_CAMINHAO)/main_caminhao.o

# 2. Simulador da Mina
TARGET_SIMULADOR = simulador_mina
SRCDIR_SIMULADOR = simulador
SOURCES_SIMULADOR_NO_MAIN = $(filter-out $(SRCDIR_SIMULADOR)/main_simulador.cpp, $(wildcard $(SRCDIR_SIMULADOR)/*.cpp))
OBJECTS_SIMULADOR = $(SOURCES_SIMULADOR_NO_MAIN:.cpp=.o) $(SRCDIR_SIMULADOR)/main_simulador.o

# 3. Gestão da Mina
TARGET_GESTAO = gestao_mina
SRCDIR_GESTAO = gestao
SOURCES_GESTAO_NO_MAIN = $(filter-out $(SRCDIR_GESTAO)/main_gestao.cpp, $(wildcard $(SRCDIR_GESTAO)/*.cpp))
OBJECTS_GESTAO = $(SOURCES_GESTAO_NO_MAIN:.cpp=.o) $(SRCDIR_GESTAO)/main_gestao.o

# --- Regras de Compilação ---

all: $(TARGET_CAMINHAO) $(TARGET_SIMULADOR) $(TARGET_GESTAO)

$(TARGET_CAMINHAO): $(OBJECTS_CAMINHAO)
	$(CXX) $(OBJECTS_CAMINHAO) -o $(TARGET_CAMINHAO) $(LDFLAGS)

$(TARGET_SIMULADOR): $(OBJECTS_SIMULADOR)
	$(CXX) $(OBJECTS_SIMULADOR) -o $(TARGET_SIMULADOR) $(LDFLAGS)

$(TARGET_GESTAO): $(OBJECTS_GESTAO)
	$(CXX) $(OBJECTS_GESTAO) -o $(TARGET_GESTAO) $(LDFLAGS)

# --- Regras genéricas para compilar .cpp -> .o ---
$(SRCDIR_CAMINHAO)/%.o: $(SRCDIR_CAMINHAO)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRCDIR_SIMULADOR)/%.o: $(SRCDIR_SIMULADOR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRCDIR_GESTAO)/%.o: $(SRCDIR_GESTAO)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


teste:
	g++ src/teste_mqtt.cpp -o teste_mqtt $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(SRCDIR_CAMINHAO)/*.o $(SRCDIR_SIMULADOR)/*.o $(SRCDIR_GESTAO)/*.o
	rm -f $(TARGET_CAMINHAO) $(TARGET_SIMULADOR) $(TARGET_GESTAO)

.PHONY: all clean
