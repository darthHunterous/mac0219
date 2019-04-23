# 02 - Paralelizando o Grep
* Programa paralelizável: se as partes do programa são independentes, ou seja, não sobrescrevem as mesma variáveis
<br><br>
* **2.1 - Independência de Processos Paralelos**: duas partes de um programa são independetes se a interseção de seus **conjunto de escrita** é vazia
    * **Conjunto de escrita**: variáveis na qual o programa escreve
* **Variável**: qualquer valor lido ou escrito atomicamente, desde variáveis simples a elementos individuais de arrays e estruturas
<br><br>
### Paralelismo no Grep
```c
string line;
read line of input from stdin into line;
while(!EOF) {
    look for pattern in line;
    if (pattern is in line)
        write line;
    read next line of input;
}
```
* A leitura da primeira linha **precisa** ser executada antes de qualquer outra
* Uma linha **não pode** ser impressa antes de se procurar pelo padrão nela, portanto as duas primeiras linhas do laço não são paralelizáveis
* **Porém**, podemos ler a próxima linha enquanto se procura por um padrão na linha anterior
    * Solução errada
        ```c
        string line;
        read line of input from stdin into line;
        while(!EOF) {
            co look for pattern in line;
                if (pattern is in line)
                    write line;
            ||  read next line of input;
            oc;
        }
        ```
        * Os dois braços do processo concorrent **co** não são independentes pois o primeiro processo lê `line` e o segundo escreve em `line`, ou seja, se o segundo processo for mais rápido, irá sobrescrever a linha antes desta ser examinada pelo primeiro processo
            * Nova tentiva de solução, também equivocada
                ```c
                string line1, line2;
                read a line of input from stdin into line1;
                while (!EOF) {
                    co look for pattern in line1;
                    if (pattern is in line1)
                        write line1;
                    // read next line of input into line2;
                    oc;
                }
                ```
                * O primeiro processo irá sempre procurar o padrão em `line1` e o segundo sempre escrever em `line2`, que acaba sendo nunca examinada
            * Solução correta, porém ineficiente
                ```c
                string line1, line2;
                read a line of input from stdin into line1;
                while (!EOF) {
                    co look for pattern in line1;
                    if (pattern is in line1)
                        write line1;
                    // read next line of input into line2;
                    oc;
                    line1 = line2;
                }
                ```
                * Ao final do loop, line1 recebe line2 e permite a análise, pelo primeiro processo, da linha que acabou de ser lida pelo segundo processo
                * **Ineficiência**
                    * Cópia de `line2` em `line1`, que pode ser de muitos caracteres
                    * `co` no corpo do loop implica que a cada iteração dois processos são criados, executados e depois destruídos, sendo este o overhead mais crítico
* Solução mais eficiente
    * Colocar os loops dentro de cada braço do `co`, estabelecendo uma execução produtor-consumidor que se comunicam através de um buffer compartilhado
    ```c
    string buffer  # one line of input
    bool done = false;  # signal termination

    co  # process 01: finding patterns
        string line1;
        while(true) {
            wait for buffer to be full or done to be true;
            if (done) break;
            line1 = buffer;
            signal that buffer is empty;
            look for pattern in line1;
            if (pattern is in line1)
                write line1;
        }
    //  # process 02: read new lines
        string line2;
        while (true) {
            read next line of input into line2;
            if (EOF) {
                done = true;
                break;
            }
            wait for buffer to be empty;
            buffer = line2;
            signal that buffer is full;
        }
    oc;
    ```