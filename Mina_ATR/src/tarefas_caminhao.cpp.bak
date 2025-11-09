// src/tarefas_caminhao.cpp

// --- INCLUDES ESSENCIAIS ---
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <numeric> // Para std::accumulate
#include <string>
#include <random>
#include <cmath>    // Para std::max, std::min
#include <cstdlib>  // Para system("clear") / system("cls")
#include <mutex>    // Para std::mutex e std::lock_guard

#include "tarefas.h"         // Inclui as declarações que criamos
#include "dados.h"           // Inclui as structs DadosVeiculo, MemoriaCompartilhada
#include "BufferCircular.h"  // PRECISA incluir a DEFINIÇÃO COMPLETA da classe aqui
#include "utils.h"           // Para gerar_placa_mercosul() (embora não usemos aqui diretamente)

// --- Declaração das Variáveis Globais (definidas em main_caminhao.cpp) ---
// É crucial usar 'extern' para indicar que estas variáveis são definidas em outro arquivo.
extern BufferCircular buffer_principal; // Referencia o buffer global
extern std::string g_placa_caminhao;
extern bool g_modo_automatico;
extern bool g_falha_ativa;
extern bool g_alerta_proximidade;
extern std::mutex mtx_estado;

// --- Implementação das Tarefas ---

// --- Tarefa: Tratamento de Sensores (Produtor do Buffer) ---
void tarefa_tratamento_sensores(BufferCircular& buffer, const std::string& placa) {
    DadosVeiculo dados_atuais{placa, 0.0, 0.0, 0.0, 60.0, {}}; // Inicializa timestamp
    const size_t MEDIA_MOVEL_N = 5; // Ordem do filtro
    std::vector<double> historico_x(MEDIA_MOVEL_N, 0.0);
    std::vector<double> historico_y(MEDIA_MOVEL_N, 0.0);
    size_t indice_historico = 0;

    // Gerador de números aleatórios para simular ruído
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> ruido_pos(-0.5, 0.5);
    std::uniform_real_distribution<double> temp_dist(70.0, 130.0); // Simula temp entre 70 e 130

    while (true) {
        // --- 1. Simula Leitura Bruta do Sensor (com ruído) ---
        // Na Etapa 2: Ler dados do MQTT aqui
        double x_bruto = dados_atuais.posicao_x + ruido_pos(generator);
        double y_bruto = dados_atuais.posicao_y + ruido_pos(generator);
        dados_atuais.temperatura = temp_dist(generator); // Simula variação de temperatura

        // --- 2. Aplica Filtro de Média Móvel ---
        historico_x[indice_historico] = x_bruto;
        historico_y[indice_historico] = y_bruto;
        indice_historico = (indice_historico + 1) % MEDIA_MOVEL_N;

        double x_filtrado = std::accumulate(historico_x.begin(), historico_x.end(), 0.0) / MEDIA_MOVEL_N;
        double y_filtrado = std::accumulate(historico_y.begin(), historico_y.end(), 0.0) / MEDIA_MOVEL_N;

        // --- 3. Prepara Dados para o Buffer ---
        dados_atuais.posicao_x = x_filtrado;
        dados_atuais.posicao_y = y_filtrado;
        // Simula movimento simples (ex: anda para frente no eixo X)
        dados_atuais.posicao_x += 1.0;
        dados_atuais.timestamp = std::chrono::system_clock::now(); // Pega timestamp atual

        // --- 4. Coloca no Buffer ---
        buffer.put(dados_atuais); // Usa o objeto buffer passado como parâmetro
        std::cout << "[Sensores] Placa " << placa << " - Dados filtrados (X=" << dados_atuais.posicao_x
                  << ", T=" << dados_atuais.temperatura << ") enviados ao buffer." << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Produz a cada 500ms
    }
}

// --- Tarefa: Lógica de Comando (Consumidor do Buffer, Gerencia Estado) ---
void tarefa_logica_comando(BufferCircular& buffer) { // Precisa do buffer
    while (true) {
        DadosVeiculo dados = buffer.get(); // Bloqueia se o buffer estiver vazio

        { // Escopo do lock_guard
            std::lock_guard<std::mutex> lock(mtx_estado); // Trava o mutex

            // Lógica de Falha por Temperatura
            if (dados.temperatura > 120.0 && !g_falha_ativa) {
                std::cout << "[Lógica] ALERTA: Superaquecimento! Ativando falha. Placa: " << dados.placa << std::endl;
                g_falha_ativa = true;
                g_modo_automatico = false; // Falha grave -> Modo Manual
            }

            // Na Etapa 2: Ler comandos da Interface Local (via IPC) aqui
            // Na Etapa 2: Ler alertas de anticolisão (via MQTT/global)

            std::cout << "[Lógica] Placa " << dados.placa << " - Processou. Modo: "
                      << (g_modo_automatico ? "Auto" : "Manual") << ", Falha: " << (g_falha_ativa ? "SIM" : "NÃO")
                      << std::endl;

        } // Mutex é liberado aqui

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Processa a cada 100ms
    }
}

// --- Tarefa: Controle de Navegação (Consumidor do Buffer, Atua) ---
void tarefa_controle_navegacao(BufferCircular& buffer) { // Precisa do buffer
     while (true) {
        DadosVeiculo dados = buffer.get(); // Lê do buffer
        bool local_modo_auto;
        bool local_falha;
        bool local_alerta_prox;

        { // Escopo mínimo para ler o estado global
            std::lock_guard<std::mutex> lock(mtx_estado);
            local_modo_auto = g_modo_automatico;
            local_falha = g_falha_ativa;
            local_alerta_prox = g_alerta_proximidade; // Lendo o estado de alerta
        } // Mutex liberado

        if (local_modo_auto && !local_falha && !local_alerta_prox) {
            // Lógica de controle SIMULADA
            // Na Etapa 2: Ler setpoint real vindo do Planejamento de Rota
            double setpoint_x = 1000.0; // Exemplo: ir para X=1000
            double erro_x = setpoint_x - dados.posicao_x;
            double o_aceleracao = std::max(0.0, std::min(100.0, erro_x * 0.1)); // Controle P simples
            // double o_direcao = 0.0; // Manter reto

            std::cout << "[Navegação] Placa " << dados.placa << " - Modo Auto. Indo para X=" << setpoint_x
                      << ". Acel: " << o_aceleracao << "%" << std::endl;
            // Na Etapa 2: Publicar o_aceleracao e o_direcao via MQTT para o simulador
        }
        else if (local_alerta_prox) {
             std::cout << "[Navegação] Placa " << dados.placa << " - ALERTA ANTICOLISÃO! Parando." << std::endl;
             // Na Etapa 2: Publicar aceleração 0 ou negativa
        }
        else {
            std::cout << "[Navegação] Placa " << dados.placa << " - Controle desligado (Manual/Falha)." << std::endl;
            // Garantir atuadores em estado seguro (aceleração 0)
            // Na Etapa 2: Publicar aceleração 0
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Ciclo de controle rápido
    }
}

// --- Tarefa: Monitoramento de Falhas (Atualiza Estado de Falha) ---
void tarefa_monitor_falhas() { // Não precisa do buffer diretamente
    // Na Etapa 2: Assinar MQTT para falhas elétricas/hidráulicas simuladas
    bool falha_simulada_eletrica = false;
    int contador = 0;

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5)); // Verifica a cada 5s
        contador++;

        // Simula uma falha elétrica intermitente
        if (contador > 5 && contador % 10 == 0) { // Começa após 25s, ocorre a cada 50s
            falha_simulada_eletrica = true;
            std::cout << "[Monitor Falha] Falha elétrica simulada: ON" << std::endl;
        } else if (contador > 5 && contador % 13 == 0) { // Desliga 15s depois
             falha_simulada_eletrica = false;
             std::cout << "[Monitor Falha] Falha elétrica simulada: OFF" << std::endl;
        }

        { // Escopo do lock
            std::lock_guard<std::mutex> lock(mtx_estado);

            // Verifica se há alguma falha simulada ativa
            bool alguma_falha_externa = falha_simulada_eletrica; // || falha_hidraulica_lida_do_mqtt;

            if (alguma_falha_externa && !g_falha_ativa) {
                // Se detectou uma nova falha externa e o sistema não estava em falha
                std::cout << "[Monitor Falha] Ativando estado de falha devido a falha externa." << std::endl;
                g_falha_ativa = true;
                g_modo_automatico = false; // Falha -> Modo Manual
            }
            // OBS: A falha de temperatura é tratada na Lógica de Comando.
            // O rearme da falha (g_falha_ativa = false) virá da Interface Local na Etapa 2.
        } // Mutex liberado
    }
}

// --- Tarefa: Coletor de Dados (Consumidor, Prepara para IPC/Log/MQTT) ---
void tarefa_coletor_dados(BufferCircular& buffer) { // Precisa do buffer
     // Na Etapa 2: Inicializar memória compartilhada (IPC), cliente MQTT e arquivo de log aqui
    while (true) {
        DadosVeiculo dados = buffer.get(); // Pega dados recentes do buffer
        bool local_modo_auto;
        bool local_falha;
        bool local_alerta_prox;
        long long timestamp_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                                    dados.timestamp.time_since_epoch()).count(); // Pega timestamp dos dados

        { // Escopo para ler estado global
            std::lock_guard<std::mutex> lock(mtx_estado);
            local_modo_auto = g_modo_automatico;
            local_falha = g_falha_ativa;
            local_alerta_prox = g_alerta_proximidade;
        } // Mutex liberado

        // --- Operações da Etapa 2 ---

        // 1. Escrever no arquivo de log (Simulado com cout por enquanto)
        std::cout << "[Coletor] LOG Placa " << dados.placa << " - T:" << timestamp_ms
                  << " X:" << dados.posicao_x << " Y:" << dados.posicao_y
                  << " Modo:" << (local_modo_auto ? "A" : "M")
                  << " Falha:" << (local_falha ? "S" : "N")
                  << " Prox:" << (local_alerta_prox ? "S" : "N") << std::endl;

        // 2. Escrever na memória compartilhada (Simulado por enquanto)
        //    Na Etapa 2: Implementar escrita na SHM com mutex POSIX

        // 3. Publicar status via MQTT (Simulado por enquanto)
        //    Na Etapa 2: Formatar JSON e publicar via Paho MQTT

        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Coleta a cada 200ms
    }
}

// --- Tarefa: Interface Local (Lê Estado, Exibe no Console) ---
void tarefa_interface_local() { // Não precisa do buffer diretamente
     // Na Etapa 2: Conectar à memória compartilhada (IPC) aqui
    while (true) {
        // --- Leitura Simulada (Etapa 1) ---
        // Na Etapa 2: Ler da memória compartilhada aqui com mutex POSIX
        bool local_modo_auto;
        bool local_falha;
        bool local_alerta_prox;
        double local_x = -1.0, local_y = -1.0; // Valores da SHM (inicializados em -1)
        std::string local_placa = g_placa_caminhao; // Pega a placa global definida no main

        {
             std::lock_guard<std::mutex> lock(mtx_estado); // Usa mutex C++ para ler globais da Etapa 1
             local_modo_auto = g_modo_automatico;
             local_falha = g_falha_ativa;
             local_alerta_prox = g_alerta_proximidade;
             // Na Etapa 2, ler x e y da SHM em vez daqui
        }

        /* --- Exibição no Console ---
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif*/

        std::cout << "--- Interface Local Caminhão " << local_placa << " ---" << std::endl;
        // Na Etapa 2, ler pos_x e pos_y da SHM
        // std::cout << "Posição (X, Y): (" << local_x << ", " << local_y << ")" << std::endl;
        std::cout << "Modo Operação: " << (local_modo_auto ? "AUTOMÁTICO" : "MANUAL") << std::endl;
        std::cout << "Status Falha: " << (local_falha ? "FALHA ATIVA (!)" : "Normal") << std::endl;
        std::cout << "Alerta Proximidade: " << (local_alerta_prox ? "SIM (!)" : "NÃO") << std::endl;
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "Comandos: [M]anual, [A]uto, [R]earme (Não implementado)" << std::endl;

        // Na Etapa 2: Ler teclado aqui e enviar comando via IPC

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Atualiza a tela a cada segundo
    }
}

// --- Tarefa: Planejamento de Rota (Esqueleto) ---
// void tarefa_planejamento_rota() {
//     // Na Etapa 2: Conectar ao MQTT e assinar tópico de comando de rota
//     while(true) {
//         // Esperar por nova mensagem de rota
//         // Atualizar variável global de setpoint (protegida por mutex)
//         std::cout << "[Planejamento] Aguardando nova rota..." << std::endl;
//         std::this_thread::sleep_for(std::chrono::seconds(10));
//     }
// }