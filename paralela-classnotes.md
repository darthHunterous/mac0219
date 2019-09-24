# Ben-Ari, Andrews - Cap. 02

### Programação com variáveis compartilhadas
* Programa concorrente consiste em um número finito de *processos*. Cada processo é escrito usando um conjunto de finito de instruções atômicas
* O estado de um programa é o conteúdo de suas variáveis em um dado momento
    * Variáveis
        * Explícitas
        * Implícitas
            * Registradores
            * Ponteiros de controle
* Um processo executa uma sequência de instruções
    * Instrução: sequência de uma ou mais *ações atômicas*
    * *Ações atômicas*
        * Indivisíveis
        * Examinar ou alterar o estado de um programa
* A execução de um programa concorrente consiste de sequência de instruções atômicas intercaladas
* História: uma sequência de execução das ações atômicas
* **Papel da sincronização**
    * Restringir conjunto de histórias possíveis a um conjunto desejável
    * **Exclusão mútua**: criar seções que parecem atômicas
    * **Condição de sincronização:** atrasar um ação até que seja válida uma condição
* O estado de um algoritmo concorrente corresponde aos estados das variáveis e aos respectivos ponteiros de controle dos processos
* Dados dois estados s1 e s2, existe uma transição entre s1 e s2 se alguma das intruções apontadas pelos ponteiros de controle muda de s1 para s2
* **Diagrama de Estados do problema dos sapos**
    * Sapos machos pulam pra direita e fêmeas pra esquerda
    * Em negrito, estados terminais (em que nenhum sapo consegue pular)
    * Inicial: MMOFF
        * **OMMFF**
        * MOMFF
            * **OMMFF**
            * MFMOF
                * MFMFO
                * MFOMF
        * MMFOF
            * MOFMF
                * OMFMF
                * MFOMF
        * **MMFFO**
* Exercício de Exemplo
    * Encontrar o máximo de um vetor com inteiros positivos
        ```c
        int m = 0;
        forall[i = 0 to n-1] {
            if(a[i] > m)
                < if(a[i] > m)
                m = a[i] >
        }
        ```