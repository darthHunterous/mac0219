# 2.5 - Sincronização Produtor-Consumidor
* Problema exemplo: cópia de um array do produtor para o consumidor
    * O processo produtor possui um array local de inteiros `a[n]` enquanto que o consumidor tem `b[n]`
    * Como os arrays não são compartilhados, precisam usar variáveis compartilhadas para comunicação, por exemplo um inteiro `buf`
    * Sendo `p` e `c` os números de itens depositados e consumidos, respectivamente. A condição de sincronização é:
        * `c <= p <= c+1`
        * Ou seja, os valores de `c` e `p` podem diferir apenas por um no máximo, significando que o produtor depositou um elemento a mais do que foi consumido
    * Para sincronizar pode-se usar um `await` para esperar o buffer estar vazio ou cheio
        * Quando `p == c`, o buffer está vazio (elemento anterior foi consumido)
        * Quando `p > c`, o buffer está cheio
    * Código de exemplo
        ```c
        int buf, p = 0, c = 0;

        process Producer {
            int a[n];
            while (p < n) {
                < await (p == c); >
                buf = a[p];
                p = p+1; 
            }
        }

        process Consumer {
            int b[n];
            while (c < n) {
                < await (p > c); >
                b[c] = buf;
                c = c+1;
            }
        }
        ```
        * **Busy waiting** ou **spinning**: quando um processo fica ocupado checando constantemente por uma condição ser satisfeita (caso do **await**)
        