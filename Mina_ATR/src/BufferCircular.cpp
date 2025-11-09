#include "BufferCircular.h"

// Construtor do Buffer Circular
BufferCircular::BufferCircular(size_t tamanho) :
    buffer_(tamanho),               // Aloca o vetor com o tamanho desejado
    tamanho_max_(tamanho) {}        // Guarda o tamanho máximo

// Método para adicionar um item (Produtor)
void BufferCircular::put(const DadosVeiculo& dados) {
    // Usa um unique_lock para gerenciar o mutex de forma mais flexível
    // nenhuma outra thread pode acessar o buffer enquanto este lock estiver ativo
    std::unique_lock<std::mutex> lock(mtx_);

    // Espera (liberando o lock) enquanto o buffer estiver cheio
    cond_produtor_.wait(lock, [this]() {
        // A condição para continuar (e não esperar) é: buffer NÃO está cheio
        return contador_ < tamanho_max_;
    });

    // ----- Seção Crítica -----
    buffer_[cauda_] = dados; // Coloca o dado na posição = onde o produtor escreve
    cauda_ = (cauda_ + 1) % tamanho_max_; // Avança a cauda circularmente
    contador_++; // Incrementa o número de itens
    // ----- Fim da Seção Crítica -----

    // Libera o lock (automaticamente ao sair do escopo do unique_lock)
    lock.unlock(); 

    // Notifica UM consumidor (thread) que pode estar dormindo
    cond_consumidor_.notify_one();
}

// Método para thread retirar um item (Consumidor)
DadosVeiculo BufferCircular::get() {
    // Adquire o lock do mutex
    std::unique_lock<std::mutex> lock(mtx_);

    // Espera (liberando o lock) enquanto o buffer estiver vazio
    cond_consumidor_.wait(lock, [this]() {
        // A condição para continuar (e não esperar) é: buffer NÃO está vazio
        return contador_ > 0;
    });

    // ----- Seção Crítica -----
    DadosVeiculo dados = buffer_[cabeca_]; // Pega o dado da posição da cabeça = onde o consumidor lê
    cabeca_ = (cabeca_ + 1) % tamanho_max_; // Avança a cabeça circularmente
    contador_--; // Decrementa o número de itens
    // ----- Fim da Seção Crítica -----

    // Libera o lock antes de notificar a thread produtora
    lock.unlock(); 

    // Acorda uma thread que pode estar dormindo e notificando que há espaço no buffer
    cond_produtor_.notify_one();

    return dados; // Retorna o dado lido
}