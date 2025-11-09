#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "Iniciando Sistema de Gestão da Mina..." << std::endl;

    //teste de threads
    std::cout << "Gestão em execução. Pressione Ctrl+C para sair." << std::endl;

    // Loop principal simplificado (pode ser substituído pelo loop do MQTT)
    while (true) {
         std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}