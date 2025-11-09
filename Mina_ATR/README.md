Trabalho Final – Automação em Tempo Real (ATR)

Sistema Embarcado de Automação para Caminhão Autônomo em Mina de Ferro

Descrição do Projeto:
    - O projeto propõe o desenvolvimento de um sistema embarcado em C++ responsável pelo monitoramento, controle e comunicação de um caminhão fora-de-estrada que opera em uma mina de ferro a céu aberto.

O sistema simula um veículo autônomo que executa o ciclo operacional:
    - carregamento → transporte → descarga → retorno

Durante o trajeto, sensores embarcados informam variáveis físicas e de segurança; o sistema embarcado processa os dados, detecta falhas, executa ações de controle e envia informações para a central de gestão via MQTT.

Contexto Operacional:
    - Em uma mina de ferro a céu aberto, dezenas de caminhões autônomos transportam minério em rotas pré-definidas.
    - Cada veículo possui sensores que monitoram posição, temperatura, nível de combustível, sistemas elétricos e hidráulicos, e atuadores para aceleração, direção, frenagem e descarga.

O sistema embarcado proposto é uma simulação fiel dessa operação, implementando o comportamento interno de cada caminhão autônomo.

Licença:            
    - Este é um projeto acadêmico desenvolvido para fins educacionais na disciplina Automação em Tempo Real. Não representa sistema real de mineração.
