# 01 - Essência da Computação Concorrente
* Programa concorrente tem dois ou mais processos, sendo cada processo sequencial
    * Programas concorrentes têm **várias threads de controle**
* Processos trabalham em conjunto através de **comunicação**: variáveis compartilhadas ou por mensagens
    * **Variável compartilhada:** um processos escreve na variável lida por outro
    * **Mensagem**: um processo envia mensagem para outro
* **Sincronização** entre processos
    * **Exclusão mútua**
        * Garantir que **seções críticas** não executem ao mesmo tempo
    * **Sincronização condicional**
        * Atrasar um processo até que determinada condição se torne verdadeira