# MAC0219 - MiniEP3
### Édio Cerati Neto - NUSP 9762678

* A solução para o problema apresentado foi inspirado no código disponibilizado no Paca, que calcula uma aproximação para Pi com Pthreads através de integração
* Optei por utilizar o `mmap` para poder compartilhar o pedaço de memória equivalente a uma variável do tipo `double` entre os diferentes processos, responsável por armazenar o somatório da área correspondente a um quarto de círculo (ou seja, um quarto do valor de pi dado o raio `1.0` adotado)
* Para evitar condições de corrida ao atualizar o valor parcial da integração por cada processo, utilizou-se um semáforo, responsável por garantir a atualização correta da soma
* O programa funciona corretamente para qualquer um dos casos referentes aos argumentos `NUM_PROCESSOS` e `NUM_PONTOS`:
    * `NUM_PROCESSOS > NUM_PONTOS`
    * `NUM_PROCESSOS == NUM_PONTOS`
    * `NUM_PROCESSOS < NUM_PONTOS`
    * No caso em que `NUM_PROCESSOS < NUM_PONTOS`, os pontos a serem calculados são distribuídos em blocos contíguos para os processos,  resultando em um ponto por processo quando `NUM_PROCESSOS == NUM_PONTOS`
    * Quando `NUM_PROCESSOS > NUM_PONTOS`, a quantidade `NUM_PONTOS` de processos recebe trabalho (um ponto por processo nesse caso) e o restante dos processos fica ocioso, através da execução de `exit(0)`
* O processo pai aguarda o término da execução de todos outros processos filhos para entrar imprimir em `stdout` o valor de PI calculado, desalocar a memória referente ao `mmap` e desalocar o semáforo utilizado

### Exemplo de execução
* Após compilar com `make` e rodar com `./pi_process 16 1000000` (ou seja, 16 processos e 1 milhão de pontos) obtemos um bom resultado para PI aproximado: `3.14159265393423003943`, apresentando erro apenas a partir da décima casa decimal

### Principais desafios
* Inicialmente, o desafio foi entender o funcionamento dos processos e utilização do `fork()` e como deve-se proceder para se inicializar um número qualquer de processos ao invés de ficar restrito a uma quantidade em potências de dois
* Depois foi necessário entender o funcionamento de `mmap` para permitir uma porção de memória compartilhada
* Por fim, garantir o funcionamneto correto do programa através da implementação da seção crítica e da distribuição correta da carga de trabalho entre os processos, desalocando e encerrando tudo corretamente ao término