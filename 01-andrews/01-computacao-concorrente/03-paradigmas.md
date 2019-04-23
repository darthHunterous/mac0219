# 03 - Paradigmas de Programação Concorrente
* Paralelismo iterativo
* Paralelismo recursivo
* Produtores e consumidores (pipelines)
* Clientes e servidores
* Pares interativos

### Paralelismo Iterativo
* Programas com vários processos idênticos, cada um contendo laços, sendo cada processo então, um programa iterativo
* Cada processo trabalha em conjunto para resolver um único problema
* Comunicam e sincronizam através de variáveis compartilhadas ou transmissão de mensagens

### Paralelismo Recursivo
* Usado quando o programa tem um ou mais procedimentos recursivos, cujas chamadas são independentes, ou seja, cada procedimento trabalha em partes independentes dos dados compartilhados

### Produtores e Consumidores
* Processos que se comunicam, organizados em uma **pipeline** através da qual flui a informação
* Cada processo na pipeline é um **filtro** que consome a saída do seu antecessor e produz uma saída para seu sucessor

### Clientes e Servidores
* Um processo cliente requisita um serviço e aguarda por uma resposta
* Um servidor fica aguardando por requisições dos clientes

### Pares interativos
* Programas distribuídos com vários processos que executam o mesmo código e que trocam mensagens para finalização de tarefas