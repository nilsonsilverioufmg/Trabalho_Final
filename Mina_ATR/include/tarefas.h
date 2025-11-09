#ifndef TAREFAS_H
#define TAREFAS_H

#include <string>
#include <mutex>

// Declaração antecipada da classe BufferCircular para evitar dependência circular
class BufferCircular;

// --- Declarações das Funções de Tarefa ---
// As referências abaixo são usadas para evitar cópias desnecessárias do buffer

//Lê dados dos sensores, filtra e coloca no buffer
void tarefa_tratamento_sensores(BufferCircular& buffer, const std::string& placa);

//Processa comandos lógicos com base nos dados do buffer
void tarefa_logica_comando(BufferCircular& buffer);

//Controla a navegação do caminhão com base na tarefa_logica_comando e transforma
//em comandos de atuadores
void tarefa_controle_navegacao(BufferCircular& buffer);

//Monitora falhas externas (via MQTT) e atualiza o estado global
//mtx_estado protege o acesso às variáveis globais
void tarefa_monitor_falhas();

//Coleta dados do buffer e prepara para IPC, log e MQTT
void tarefa_coletor_dados(BufferCircular& buffer);

//Interface local que lê o estado (via IPC) e comunica com a interface gráfica
void tarefa_interface_local();


//incluir void tarefa_planejamento_rota(); // Se formos implementar

#endif // TAREFAS_H