// Atua como o "cérebro" do sistema de gestão da mina.
//
//  Responsabilidades:
//  1. Manter o estado atualizado de toda a frota de caminhões em um mapa (`std::map`) protegido por mutex (`mtx_frota`).
//  2. Fornecer o método `run_interface_loop()` que roda em uma thread separada para desenhar a interface de console (TUI) com
//     o status de todos os veículos.
//  3. Fornecer o método `atualizar_status_caminhao()` para ser chamado pelo callback do MQTT (em outra thread) e atualizar 
//     o mapa de forma segura.
//  4. Fornecer métodos para enviar comandos (ex: `enviar_comando_rota`)
//     que formatam e publicam mensagens MQTT para os caminhões.