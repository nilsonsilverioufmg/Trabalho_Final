#include <iostream>
#include <thread>
#include <chrono>


int main() {
    std::cout << "Iniciando Simulador da Mina..." << std::endl;

    // --- Lógica Principal do Simulador (Etapa 2) ---
    // 1. Inicializar cliente MQTT
    // 2. Iniciar thread(s) para simular caminhões e detectar colisões
    // 3. Publicar dados de sensores e alertas via MQTT

    std::cout << "Simulador em execução (esqueleto). Pressione Ctrl+C para sair." << std::endl;

    // Loop principal simplificado para Etapa 1 (apenas para compilar)
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0; 
}