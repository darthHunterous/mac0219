# 05 - Paralelismo Recursivo
* Programa recursivo pode ser parlelizado quando efetua múltiplas chamadas recursivas independentes, ou seja, se o conjunto de escrita dos procedimentos é disjunto
* Conjunto de escrita disjunto
    * Se o procedimento não referencia variáveis globais, ou apenas as lê

### Programa para aproximação de valor de Integral por Área
```c
double fleft = f(a);
double fright;
double area = 0.0;
double width = (b-a) / INTERVALS;

for [x=(a+width) to b by width] {
    fright = f(x);
    area = area + (fleft + fright) * width / 2;
    fleft = fright;
}
```

* No cálculo da quadratura adaptativa, divide-se a área de uma curva à esquerda e direita, cuja nova adaptação gera duas chamadas recursivas, que podem ser chamadas de maneira paralela
    * Porém como cada nova thread irá gerar duas novas chamadas recursivas, podemos acabar tendo processos demais