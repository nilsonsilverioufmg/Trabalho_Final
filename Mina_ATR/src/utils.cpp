#include "utils.h"
#include <random>
#include <string>
#include <sstream> // Para formatar a string

std::string gerar_placa_mercosul() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> letra_dist('A', 'Z');
    std::uniform_int_distribution<> num_dist('0', '9');

    std::stringstream ss;
    // Formato LLLNLNN
    ss << static_cast<char>(letra_dist(gen))
       << static_cast<char>(letra_dist(gen))
       << static_cast<char>(letra_dist(gen))
       << static_cast<char>(num_dist(gen))
       << static_cast<char>(letra_dist(gen)) // 4º caractere é letra
       << static_cast<char>(num_dist(gen))
       << static_cast<char>(num_dist(gen));

    return ss.str();
}

// Exemplo de função timestamp (poderia ir aqui)
// #include <chrono>
// long long current_timestamp_ms() {
//     return std::chrono::duration_cast<std::chrono::milliseconds>(
//                std::chrono::system_clock::now().time_since_epoch()
//            ).count();
// }