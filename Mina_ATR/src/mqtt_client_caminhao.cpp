#include <iostream>
#include <mqtt/async_client.h>
#include "BufferCircular.h"
#include "utils.h"
#include "tarefas.h"
#include "dados.h"
#include <sstream>


static const std::string BROKER = "tcp://127.0.0.1:1883";
static const std::string CLIENT_ID = "SimuladorMina";
static const std::string TOPIC = "mina/caminhao/estado";

void tarefa_coletor_dados(BufferCircular& buffer);


mqtt::async_client client(BROKER, CLIENT_ID);

void mqtt_enviar_estado(const std::string& msg) {
    try {
        mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, msg);
        pubmsg->set_qos(1);
        client.publish(pubmsg);
    } catch (const mqtt::exception& e) {
        std::cerr << "[MQTT] Erro ao publicar: " << e.what() << std::endl;
    }
}

void mqtt_iniciar() {
    try {
        auto connOpts = mqtt::connect_options_builder()
            .clean_session()
            .finalize();

        client.connect(connOpts)->wait();
        std::cout << "[MQTT] Conectado ao broker.\n";
    }
    catch (const mqtt::exception& e) {
        std::cerr << "[MQTT] Falha ao conectar: " << e.what() << std::endl;
    }
}
