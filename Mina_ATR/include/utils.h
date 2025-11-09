#ifndef UTILS_H
#define UTILS_H

#include <string>

#include <pthread.h>
#include <thread>

// Gera uma string aleatória no formato aproximado de placa Mercosul
// Melhora essa string para gerar placas no formato LLLNLNN
std::string gerar_placa_mercosul();

// Outras funções utilitárias podem ser adicionadas aqui


#endif // UTILS_H