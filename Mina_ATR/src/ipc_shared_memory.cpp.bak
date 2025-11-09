/* Esse arquivo funcionará como uma memória compartilhada entre processos, ou seja, 
o simulador_mina funcionará como um sensor para o caminhão_embarcado (produtor/consumidor)
Esta implementação usará a API de Memória Compartilhada do POSIX (ambos os processos 
acessam a mesma área da RAM diretamente, sem cópias intermediárias — só precisam de 
sincronização.) por ser mais simples e eficiente que Sockets ou Pipes (cópias entre
o espaço do usuário e o espaço do kernel a cada envio/recebimento)
Dificuldade: Sincronização: Se o Processo A estiver escrevendo ao mesmo tempo que 
o Processo B está lendo, os dados podem ser corrompidos
Possibilidade de solução: Usar um Semáforo Posix que funcione como um mutex,
garantindo que apenas um processo acesse a área compartilhada por vez.*/