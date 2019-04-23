# 04 - Ações Atômicas e Await
* Quando processos interagem, nem todas intercalações são desejadas, sendo a sincronização necessária para prevenir tais intercalações
    * **Exclusão mútua**: combinar ações atômicas em ações compostas
    * **Sincronização de condições**: atrasar a execução do processo até que um certo predicado seja satisfeito

### 2.4.1 - Atomicidade Fine-Grained
* Ação atômica está ligada a uma transformação de estado indivisível, ou seja qualquer estado intermediário que existir não está visível aos outros processos
* **Ação atômica fine-grained**: implementada diretamente no hardware
* Em um programa sequencial, atribuições de variáveis aparentam ser atômicas, porém em programas concorrentes, atribuições podem ser implementadas por uma sequência de instruções de máquina fine-grained
* Exemplo de programa
    ```c
    int y = 0;
    int z = 0;

    co x = y + z;
    //
    y = 1;
    z = 2;
    oc;
    ```
    * Se `x = y + z` é implementado através do carregamento do valor de um registrador que contém y e depois somando z nesse valor, o valor final poderia ser 0, 1, 2 ou 3 pois os valor obtidos como resultado dependem de até onde o segundo processo tiver executado. Ademais, o valor final de `x` pode ser 2, mesmo que não seja possível parar o programa e ver um estado em que `y+z` valha 2
<br><br>
* Características de um computador analisado
    * Tipos básicos são armazenados na memória e lidos e escritos como ações atômicas
    * A manipulação de valores se dá carregando-os para registradores, operando neles e depois armazenando na memória novamente
    * Cada processo tem seus próprios registradores, seja havendo conjuntos distintos de registradores ou por **context switch** (salvando e restaurando valores dos registradores quando um processo diferente executa)
    * Resultado intermediários de uma expressão complexa são armazenados em registradores ou em memória que o processo não tenha acesso
* Com tais características acima, se uma expressão de um processo não referencia uma variável alterada por outro processo, a avaliação da expressão aparentará ser atômica, mesmo que requeira várias ações atômicas fine-grained, pois:
    * Nenhum valor do qual a expressão depende pode mudar enquanto ela é avaliada
    * Nenhum outro processo pode acessar valores temporários enquanto a expressão é avaliada
* O mesmo vale para atribuições do tipo `x = expression`, se não referencia variáveis alteradas por outro processo aparentará ser atômico
* A maior parte dos comandos em programas concorrentes não cumprem as condições acima, porém na maior parte das vezes cumprem a **propriedade at-most-once**
    * **Propriedade At-Most-Once**: se uma atribuição ou expressão contém no máximo uma **referência crítica** e não é lida por outro processo
    * **Referência Crítica**: referência a um valor que é alterado por outro processo
* Cumprindo a propriedade At-Most-Once, a atribuição aparentará ser atômica, pois a única variável compartilhada será lida ou escrita apenas uma vez, não sendo o processo capaz de identificar como a variável é atualizada, vendo um valor legítimo
<br><br>
* Programa sem referência crítica
    ```c
    int x = 0, y = 0;
    co x = x + 1;
    //
    y = y + 1;
    oc;
    ```
    * Como não há referências críticas, o valor final de `x` e `y` são 1
<br><br>    
* Programa que satisfaz At-Most-Once
    ```c
    int x = 0, y = 0;
    co x = y + 1;
    //
    y = y + 1;
    oc;
    ```
    * O primeiro processo possui uma referência crítica a `y`, porém o segundo processo não referencia `x` (do primeiro processo) e não possui referências críticas, portanto o valor final de `x` é 1 ou 2 e o de `y` é 1
    * O primeiro processo irá ler `y` antes ou depois deste ser incrementado pelo segundo, mas em um programa concorrente não conseguimos determinar isso pois a ordem de execução é não-determinística
<br><br>
* Programa que não satisfaz At-Most-Once
    ```c
    int x = 0, y = 0;
    co x = x + 1;
    //
    y =  x + 1;
    oc;
    ```
    * Cada processo contém uma referência crítica e atribui a uma variável lida pelo outro
    * Os valores de `x` e `y` poderiam ser 1 e 2, 2 e 1 ou até mesmo 1 e 1 (se os processos lerem `x` e `y` antes de atribuir a eles)

### 2.4.2 - Declaração Await
* Se uma declaração não satisfaz a **at-most-once**, ela precisa ser executada atomicamente, geralmente sequências de ações precisam ser executadas de maneira atômica
* Utilização de sincronização para construir uma **ação atômica coarse-grained**, ou seja, uma sequência de *ações fine-grained*, que aparentam ser indivisíveis
* Um exemplo seria um banco de dados que contém os valores `x` e `y`, que devem ser os mesmos, ou seja, se um processo examinar ambos, não deve encontrar valores diferentes, para isso, se um processo alterar `x`, deve alterar `y` como parte da mesmo ação atômica
* Outro exemplo, de uma lista ligada com apontadores para a cabeça e o fim, inserção e remoção mútua podem gerar conflitos, como no caso dela conter apenas um elemento, por isso inserção e remoção precisam ser atômicas. Outro caso seria se ela está vazia, onde para remover, deve-se atrasar até que um elemento tenha sido inserido
<br><br>
* **Notações do Livro**
    * **Ações atômicas**: entre brackets angulares `<e>`
    * **Sincronização**: `await`
        * `<await (B) S;>`
            * B é uma expressão booleana com a condição para o atraso
            * O `await` encontra-se entre brackets para indicar que é atômico
            * B é garantido como verdadeiro quando S executar
            * O estado interno de S não é visível aos outros processos
        * Exemplo de await
            * `<await (s > 0) s = s-1;>`
        * A forma geral do `await` especifica tanto **exclusão mútua** quanto **sincronização de condição**
            * Para especificar apenas **exclusão mútua**, usaremos `<s;>`
                * Incrementar `x` e `y` atomicamente
                    * `< x = x+1; y = y+1; >`
                        * O estado interno não é visível para outros processos que referenciem `x` e `y`, por exemplo um estado em que `x` tenha sido incrementado e `y` não
        * Para especificar apenas **sincronização de condição**:
            * `<await (B);>`
                * Atrasar processo até que uma contagem seja positiva: `<await (count > 0);>`
    * **Spin Loop**: `while (not B);`
        * Loop com o corpo vazio, que fica executando até que a condição B seja verdadeira, ou seja, equivalente a `<await (B);>`, desde que B satisfaça a **at-most-once**
* **Ação atômica incondicional**: não possui uma condição de atraso B, podendo executar imediatamente
    * Exemplos são as ações implementadas em hardware (**fine-grained**), expressões em brackets angulares e **awaits** com a condição de guarda sempre true ou omitida
* **Ação atômicas condicional:** **await** com uma condição B, que só pode se tornar verdadeira com a ação de outros processos
    * Um processo esperando por uma ação atômica condicional pode esperar por um tempo arbitrariamente longo, portanto