/*1.Implementar o cliente MQTT.
2. Conectar-se ao broker (Mosquitto).
3. Usar a biblioteca Paho MQTT C++.
4. Em um loop (controlado por uma std::thread):
5. Chamar simulador_logica.atualizarEstado() para calcular a nova posição.
6. Chamar simulador_logica.getDadosJson() para obter os dados.
7. Publicar a string JSON recebida no tópico MQTT correto.
8. Gerenciar reconexão em caso de falha.*/




#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>
#include "mqtt/async_client.h"

// Configurações do broker
const std::string SERVER_ADDRESS{"tcp://localhost:1883"};
const std::string CLIENT_ID{"simulador_mina"};
const std::string TOPICO_SENSOR{"mina/sensor"};
const std::string TOPICO_COMANDO{"mina/comando"};
const int QOS = 1;

// Flag para controlar o loop
std::atomic<bool> running(true);

// Callback para receber mensagens
class callback : public virtual mqtt::callback {
public:
    void message_arrived(mqtt::const_message_ptr msg) override {
        std::cout << "[MQTT] Comando recebido no tópico '" 
                  << msg->get_topic() << "': " << msg->to_string() << std::endl;
    }

    void connection_lost(const std::string& cause) override {
        std::cerr << "[MQTT] Conexão perdida: " << cause << std::endl;
    }
};

// Função que publica dados de sensores a cada 1s
void publicar_sensores(mqtt::async_client& client) {
    int cont = 0;
    while (running) {
        std::string payload = "Temperatura: " + std::to_string(20 + cont % 10) +
                              " | Nível minério: " + std::to_string(50 + cont % 20);
        auto msg = mqtt::make_message(TOPICO_SENSOR, payload);
        msg->set_qos(QOS);

        try {
            client.publish(msg)->wait_for(std::chrono::seconds(5));
            std::cout << "[MQTT] Publicado: " << payload << std::endl;
        } catch (const mqtt::exception& exc) {
            std::cerr << "[MQTT] Erro ao publicar: " << exc.what() << std::endl;
        }

        cont++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);
    callback cb;
    client.set_callback(cb);

    mqtt::connect_options connOpts;
    connOpts.set_clean_session(true);

    try {
        std::cout << "[MQTT] Conectando ao broker..." << std::endl;
        client.connect(connOpts)->wait();
        std::cout << "[MQTT] Conectado!" << std::endl;

        // Inscrever-se no tópico de comandos
        client.start_consuming();
        client.subscribe(TOPICO_COMANDO, QOS)->wait();

        // Thread de publicação
        std::thread pubThread(publicar_sensores, std::ref(client));

        std::cout << "[MQTT] Pressione Ctrl+C para sair..." << std::endl;
        while (true) {
            // Mensagens recebidas
            auto msg = client.consume_message();
            if (msg) {
                std::cout << "[MQTT] Comando recebido: " << msg->to_string() << std::endl;
            }
        }

        running = false;
        pubThread.join();
        client.disconnect()->wait();
        std::cout << "[MQTT] Desconectado." << std::endl;

    } catch (const mqtt::exception& exc) {
        std::cerr << "[MQTT] Erro: " << exc.what() << std::endl;
        return 1;
    }

    return 0;
}
