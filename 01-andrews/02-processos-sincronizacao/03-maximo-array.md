# 03 - Encontrar o máximo de um array
* Pode ser expresso com predicados lógicos da forma:
    * `(∀ j: 1 <= j <= n: m >= a[j]) ^ (∃ j: 1 <= j <= n: m == a[j])`
        * O primeiro predicado diz que o máximo **m** deve ser pelo menos tão grande quanto todo valor no array
        * O segundo predicado diz que **m** deve ser equivalente a algum valor no array
    ```c
    int m = 0;
    for [i = 0 to n-1] {
        if (a[i] > m)
            m = a[i];
    }
    ```
    * **Solução Incorreta**
        ```c
        int m = 0;
        co [i = 0 to n-1] {
            if (a[i] > m)
                m = a[i];
        }
        ```
        * Os processos não são independentes pois leem e escrevem na variável `m`
        * Se os processos executam simultaneamente, todos irão comparar `a[i]` com `m` ao mesmo tempo, portanto todos atualizarão o valor de `m`, que receberá o valor do último processo que tentou atualizá-lo, ou seja, receberá o valor de maneira desordenada e, muito provavelmente, incorreta, pois escrever numa variável da memória ocorre de maneira atômica
    * **Solução - Correta mas Ineficiente**
        ```c
        int m = 0;
        co [i = 0 to n-1] {
            < if (a[i] > m)
                m = a[i]; >
        }
        ```
        * Os brackets angulares garantem a execução do `if` de maneira atômica, um de cada vez, garantindo a consistência da solução do programa, mas torna-se um programa similar ao sequencial, exceto que cada processo executa de maneira arbitrária (desordenada), um por vez
    * A solução correta deve garantir que o valor de **m** é atualizado de maneira atômica e que **m** é o máximo correto
    * Colocar brackets angulares em `< m = a[i]; >` não faz diferença, pois a escrita de uma variável já se dá de maneira atômica no hardware, portanto essa alteração no programa concorrente não faria diferença, com os processos ainda fazendo comparações de maneira desordenada
    * A melhor solução combina ambos programas, pois é seguro fazer comparações em paralelo por serem apenas leitura, mas também faz-se necessário assegurar que **m** é de fato o máximo ao final do programa
        ```c
        int m = 0;
        co [i = 0 to n-1] {
            if (a[i] > m)
                <if (a[i] > m)
                    m = a[i]; >
        }
        ```
        * **Checagem dupla**: após um processo atualizar **m** é provável que vários outros processos não necessitem executar o corpo do **if**, diminuindo gradativamente a probabilidade de que os processos sequer entrem na segunda checagem, evitando o atraso de contenção para que o segundo **if** seja executado atomicamente

### Pontos Chave desta Seção
* **Um**: sincronização é necessária para obter respostas corretas de processos que leiam e escrevam em variáveis compartilhadas
* **Dois**: brackets angulares são usados para especificar ações atômicas, que são instâncias de seções críticas
* **Três**: double check é útil para atualizar uma variável compartilhada, até porque, no caso em que a primeira checagem já seja falsa, a segunda não é feita
