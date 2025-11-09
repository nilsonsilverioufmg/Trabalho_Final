#ifndef BUFFERCIRCULAR_H
#define BUFFERCIRCULAR_H

#include <vector>
#include <mutex>
#include <condition_variable>
#include "dados.h" // Dados do veículo (posição, temperatura, etc.)

#include <pthread.h>
#include <thread>
/* Buffer Circular Thread-Safe (Produtor-Consumidor) componente
central para comunicação entre threads do caminhão.*/

class BufferCircular {
public:
    // Construtor: especifica o tamanho máximo do buffer
    BufferCircular(size_t tamanho);

    // Coloca um item no buffer. Bloqueia se estiver cheio.
    void put(const DadosVeiculo& dados);

    // Retira um item do buffer. Bloqueia se estiver vazio.
    DadosVeiculo get();

    // Impede cópia e atribuição para evitar problemas
    BufferCircular(const BufferCircular&) = delete;
    BufferCircular& operator=(const BufferCircular&) = delete;

private:
    std::vector<DadosVeiculo> buffer_; // Onde os dados são armazenados
    size_t cabeca_ = 0; // Índice de onde ler (get)
    size_t cauda_ = 0;  // Índice de onde escrever (put)
    size_t contador_ = 0; // Número de itens atualmente no buffer
    size_t tamanho_max_; // Tamanho máximo do buffer

    // Primitivas de sincronização
    std::mutex mtx_;                     // Protege o acesso ao buffer
    std::condition_variable cond_produtor_; // Produtores esperam aqui se buffer cheio
    std::condition_variable cond_consumidor_; // Consumidores esperam aqui se buffer vazio
};

#endif 