#ifndef DADOS_H
#define DADOS_H

#include <thread>
#include <pthread.h> // Para mutex da shared memory
#include <string>    // Para a placa
#include <chrono>    // Para timestamp

/*Este arquivo contém as estruturas de dados que os diferentes componentes 
usarão para se comunicar, tanto entre as threads (internamente) quanto entre
os processos (via IPC).*/

/*Prefixo para o nome da Memória Compartilhada (IPC)
A placa será anexada a este nome para criar SHM único por caminhão*/
#define SHM_NAME_PREFIX "/cam_shm_"

// Estrutura de dados que trafega pelo Buffer Circular
struct DadosVeiculo {
    std::string placa;
    double posicao_x;
    double posicao_y;
    double angulo;
    double temperatura;
    // Usando time_point para um timestamp mais preciso
    std::chrono::time_point<std::chrono::system_clock> timestamp; 
};

// Estrutura da Memória Compartilhada para IPC com Interface Local
struct MemoriaCompartilhada {
    pthread_mutex_t mutex; // Mutex POSIX para sincronização INTER-PROCESSOS
    
    // Dados para a Interface Local (use tipos fixos se possível)
    char placa[10]; // Usar array de char é mais seguro em SHM
    bool modo_automatico;
    bool falha_ativa;
    bool alerta_proximidade; // Adicionando alerta
    double posicao_x;
    double posicao_y;
    double temperatura_atual; 
};

#endif // DADOS_H