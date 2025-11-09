#ifndef MQTT_CLIENT_CAMINHAO_H
#define MQTT_CLIENT_CAMINHAO_H

#include <string>
#include "BufferCircular.h"

// Inicializa o cliente MQTT
void mqtt_iniciar();

// Envia uma mensagem JSON com o estado atual
void mqtt_enviar_estado(const std::string& msg);


#endif
