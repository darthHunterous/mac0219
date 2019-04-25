# 2.6 - Sinopse de Semântica Axiomática
* A base para o raciocínio assertivo é a **lógica de programação**, um sistema de lógica formal que facilita fazer declarações precisas sobre a execução do programa

### 2.6.1 - Sistemas de Lógica Formal
* Definidos por:
    * Conjunto de **símbolos**
    * Conjunto de **fórmulas** construídas a partir desses símbolos
        * Fórmulas são sequências bem formadas de símbolos
    * Conjunto de fórmulas distintas, os **axiomas**
        * Fórmulas especiais que são assumidas como verdadeiras a *priori*
    * Conjunto de **regras de inferência**
        * Especificam como derivar novas fórmulas verdadeiras a partir de outros axiomas e fórmulas
        * Da forma (<sup>H<sub>1</sub>, H<sub>2</sub>, ..., H<sub>n</sub></sup>)/<sub>C</sub> , onde H<sub>i</sub> são as hipóteses e C é a conclusão
        * Se todas hipóteses são verdadeiras, pode-se deduzir que a conclusão também é
        * Hipóteses e conclusões são fórmulas
* **Prova**: sequência de linhas, cada uma sendo um axioma ou derivado de linhas anteriores através da aplicação de uma regra de inferência
* **Teorema**: qualquer linha de uma prova, portanto são axiomas ou obtidos através da aplicação de uma regra de inferência a partir de outro teorema
* **Interpretação**: mapeia cada fórmula para verdadeiro ou falso
* Uma lógica é **sólida** a respeito de uma interpretação se todos seus axiomas e regras de inferência são **sólida**
    * Um axioma é **sólida** se ele mapeia para verdadeiro
    * Uma regra de inferência é **sólida** se sua conclusão mapeia para verdadeiro, assumindo todas suas hipóteses mapeando para verdadeiro
* Se uma lógica é **sólida**, então todos teoremas são verdadeiros sobre o domínio. Nesse caso, a interpretação é um **modelo** para a lógica
* **Completude** é o dual da **solidez**
    * Uma lógica é **completa** a respeito de uma interpretação se uma fórmula mapeada para verdadeiro é um teorema, ou seja, toda fórmula é provável na lógica
    * Portanto, se **FATOS** é o conjunto de afirmações verdadeiras expressáveis como fórmulas em uma lógica e **TEOREMAS** é o conjunto de teoremas nessa lógica, a **solidez** significa que **TEOREMAS** &sube; **FATOS** e da **completude** vem que **FATOS** &sube; **TEOREMAS**
* Uma lógica **completa** e **sólida** permite que todas afirmações verdadeiras expressáveis nela sejam passíveis de prova
<br><br>
* **Teorema da Incompletude de Gödel**: qualquer lógica que contenha aritmética não pode ser **completa**. Porém uma lógica que estende outra pode ser **relativamente completa**, ou seja, não introduz incompletude além da inerente à lógica que foi estendida
* Completude relativa é suficiente para a lógica de programação, uma vez que as propriedades aritméticas são certamente verdadeiras

### 2.6.2 - Lógica de Programação
* Sistema de Lógica Formal que permite provar propriedades de um programa, sendo usada a *PL* (Programming Logic)
* **PL**:
    * Símbolos: predicados, chaves e declarações da linguagem
    * Fórmulas: triplas da forma `{P} S {Q}`
        * P e Q especificam relações entre os valores das variáveis do programa
        * S é a lista de declarações
        * **2.4 - Interpretação da tripla**: é verdadeira se quando a execução de `S` começa, o estado satisfaz `P` e quando termina, o estado satisfaz `Q`
            * Também chamada de **corretude parcial**, que é uma propriedade de segurança, ou seja, se o estado inicial satisfaz `P`, assumindo que `S` termina, o estado final irá satisfazer `Q`
            * **Corretude total**: corretude parcial junto com terminação, ou seja, todos históricos são finitos
        * P e Q são chamadas de **asserções** pois garantem que o estado do programa precise satisfazê-los para que a interpretação da tripla seja verdadeira, caracterizando um estado aceitável do programa
            * P é **pré-condição** de S, pois é a condição que precisa ser satisfeita para a execução de S
            * Q é **pós-condição**, sendo o estado que resulta da execução de S, se esta se encerra
            * Asserções especiais são **true**, todos estados do programa e **false**, nenhum estado
* Para a interpretação **2.4** ser um modelo para PL, os axiomas e regras de inferência de PL precisam ser sólidos em relação a **2.4**, assegurando que todos teoremas prováveis em PL são sólidos
    * A tripla `{x == 0} x = x+1; {x == 1}` deve ser um teorema
    * A tripla `{x == 0} x = x+1; {y == 1}` não é um teorema pois a atribuição de na variável **x** não garante o estado de **y**
* **Axioma da atribuição**: {P<sub>x &larr; e</sub>} x = e {P}
    * {P<sub>x &larr; e</sub>}: substitua todas ocorrência livres da variável **x** no predicado **P** pela expressão **e**
    * Tal axioma diz que para uma atribuição resultar em um estado satisfazendo o predicado **P**, então o estado anterior deve satisfazer **P** com a variável **x** textualmente substituída pela expressão **e**
    * Exemplo do axioma: `{1 == 1} x = 1; {x == 1}`
<br><br>
* **Regras de inferência em PL**:
    * **Regra da Composição**: `{P} S1 {Q}` e `{Q} S2 {R}` resultam em `{P} S1; S2 {R}`
    * **Regra do If**: `{P ^ B} S {Q}` e (P &and; &not;B) &rarr; Q resultam em `{P} if (B) S; {Q}`
    * **Regra do While**: `{I ^ B} S {I}` resulta em `{I} while(B) S; {I ^ ¬B}`
    * **Regra da Consequência**: P\` &rarr; P, `{P} S {Q}` e Q\` &rarr; Q resulta em `{P'} S {Q'}`

### 2.6.3 - Semântica da Execução Concorrente
* **Regra do Await**: `{P ^ B} S {Q}` segue `{P} <await (B) S;> {Q}`
    * A hipótese diz que se a execução de S começa quando P e B são verdadeiros, com S terminando, Q será verdadeiro
    * A conclusão diz que o `await` gera o estado Q se começar no estado P, supondo que o await termina sua execução
* **Regra da declaração `co`**: se `{Pi} Si {Qi}` são livres de interferência, então temos `{P1 ^ ... ^ Pn}`, `co S1; // ... // Sn; oc` e `{Q1 ^ ... ^ Qn}`
    * Para ser válido, os processos não podem interferir uns aos outros
* **Exemplo de programa concorrente**
    ```c
    {x == 0}
    co <x = x+1;> // <x = x+2;> oc
    {x == 3}
    ```
    * Se x inicia como zero, claramente irá terminar a execução como três, mas como descobrir o que é válido em cada processo?
    * Não se pode assumir com qual valor de x o processo se inicia pois a ordem de execução é não determinística, portanto o que é realmente válido está de acordo com o programa abaixo
        ```c
        {x == 0}
        co {x == 0 ∨ x == 2}
           <x = x+1;>
           {x == 1 ∨ x == 3}
        //
           {x == 0 ∨ x == 1}
           <x = x+2;>
           {x == 2 ∨ x == 3}
        oc
        {x == 3}
        ```
<br><br>
* **2.5 - Não Interferência**
    * Seja `a` uma **ação de atribuição** (declaração de atribuição ou **await** com atribuições) em um processo e `pre(a)` sua pré-condição
    * Seja `C` uma **asserção crítica** (pré ou pós condição fora de um **await**) em outro processo
    * Então `a` não interfere com `C` se `{C ^ pre(a)} a {C}`
    * Em suma, a asserção crítica `C` é invariante em relação à ação de atribuição `a`
