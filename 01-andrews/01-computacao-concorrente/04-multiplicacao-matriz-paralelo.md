# 04 - Multiplicação de Matriz
* Dividindo **P** processadores para uma matriz quadrada de tamanho **n**, sendo **n** divisível por **P**
```c
process worker[w=1 to P] {       # faixas paralelas
    int first = (w-1) * n/P;     # primeira linha da faixa
    int last = first + n/P - 1;  # ultimha linha da faixa
    for [i=first to last] {
        for [j=0 to n-1] {
            c[i,j] = 0.0;
            for[k=0 to n-1]
                c[i,j] = c[i,j] + a[i,k] * b[k,j];
        }
    }
}
```
* No programa acima, as **i** linhas da matriz são divididas em **P** faixas de **n/P** linhas cada

### Programa Paralelizável
* O programa precisa ter computações independentes, ou seja, conjuntos de escrita disjuntos