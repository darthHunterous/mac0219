# 01 - Estados, Ações, Históricos e Propriedades
### Estado
* O Estado de um programa concorrente consiste nos valores das suas variáveis em um dado momento, tanto as declaradas pelo programador quanto as implícitas (program counter de cada processo)
* Cada processo no programa concorrente executa da maneira independente
* Um processo executa uma sequência de declarações
* Uma declaração é composta por uma sequência de uma ou mais **ações atômicas**, ações que examinam ou mudam o estado do programa de maneira **indivisível**
    * Exemplo de ação atômica: instruções de máquina que armazenam ou carregam palavras da memória de maneira ininterrupta

### Histórico
* A execução de um programa concorrente resulta na intercalação de ações atômicas executas por cada processo
* Uma execução específica de um programa concorrente pode ser vista como um **histórico**: s0 -> s1 -> ... -> sn
    * s0 é o estado inicial
    * Os outros sn estados são estados subsequentes, com transições feitas por ações atômicas de alteração de estado
* Execução paralela também pode ser modelada como um histórico linear, pois o efeito de executar certas ações em paralelo é equivalente a executar de maneira serial
    * Pois o efeito de mudança de estado causado por uma ação atômica é indivisível e portanto não é afetado por outras ações atômicas executadas ao mesmo tempo
* O número de históricos para um programa concorrente cresce exponencialmente pois cada ação atômica pode ser intercalada, formando uma enorme árvore de possibilidades
* O papel da **sincronização** é restringir os históricos possíveis de um programa concorrentes àqueles que são desejáveis
    * **Exclusão mútua** combina ações atômicas com as sequências de ações nas **seções críticas**, que não podem ser intercaladas com ações de outros processos que referenciem as mesmas variáveis
    * **Sincronização de condição** atrasa uma ação até que o estado satisfaça uma condição booleana
    * Ambas formas de sincronização fazem com que os processos sejam atrasados, restringindo o conjunto de ações atômicas que podem ser executadas a seguir

### Propriedade
* Propriedade de um programa é um atributo verdadeiro a todas possibilidades de histórico desse programa e portanto de todas suas execuções
* **Propriedade de Segurança**: quando o programa nunca entra em um estado ruim, em que algumas variáveis adotam valores indesejados
    * **Corretude parcial**: se o estado final do programa é correto, assumindo que ele termina
    * **Exclusão mútua** é um exemplo de propriedade de segurança pois um estado ruim seria um em que dois processos executam ações em diferentes seções críticas ao mesmo tempo
* **Propriedade de Vivacidade**: quando o programa eventualmente entra em um bom estado, em que as variáveis têm valores desejáveis
    * **Terminação**: se todos os laços e procedimentos do programa se encerram, ou seja, se o comprimento de todo histórico é finito
    * A entrada eventual em uma seção crítica é um exemplo de propriedade de vivacidade em um programa concorrente, um estado desejável é aquele em que um processo executa dentro de sua seção crítica
* **Corretude total**: combina a *corretude parcial* e a *terminação*, ou seja, o programa sempre termina e com a resposta correta
* **Satisfação de propriedades**: como demonstrar isto em um programa?
    * **Testes/debugging**: rodar e ver o que acontecer
        * Enumerar alguns históricos possíveis e verificar se são aceitáveis
        * **Desvantagem**: cada teste considera apenas um histórico de execução e um número limitado de testes muito provavelmente não demonstrará a ausência de históricos ruins
    * **Raciocínio Operacional**: análise exaustiva de casos
        * Enumeração de todos históricos possíveis a partir da intercalação das ações atômicas de cada processo
            * Programa com **n** processos, cada um executando **m** ações atômicas terá `(n * m )! / (m! ^ n)` históricos diferentes
            * Um programa de 3 processos que executam apenas duas ações atômicas terá 90 históricos diferentes
    * **Raciocínio Assertivo**: análise abstrata
        * Asserções lógicas (predicados) são usadas para representar conjuntos de estado do programa
        * Ações atômicas são consideradas **transformadores de predicado** pois alteram o estado que satisfaz um predicado para o que satisfaça outro predicado
        * Raciocínio assertivo leva a uma representação compacta de estados e suas transformações, permitindo análise de um programa, proporcional ao número de ações atômicas presentes nele
<br><br>
### Dificuldade em Debug de Programas Paralelos
* Difícil parar todos processos e examinar seus estados
* Cada execução gera um histórico diferente