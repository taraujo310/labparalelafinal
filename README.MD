Link para o relatório: https://docs.google.com/document/d/10xsd2_WL34t82U4ci-9-ONHzg_LAK1xe_SHfRfaErtM/edit?usp=sharing

# Análise da implementação paralela dos algoritmos de ordenação Merge Sort e Bubble Sort utilizando MPI e OpenMP

## Introdução

O presente relatório apresenta uma comparação entre os algoritmos de ordenação bubble sort e merge sort em um vetor de inteiros com 1024 chaves e implementação paralela usando MPI e OpenMP .

## Escolha dos Algoritmos e Implementação

Os algoritmos foram escolhidos por suas formas diferentes de executar a ordenação: enquanto o bubble sort compara e troca, o merge sort segue o paradigma de dividir para conquistar. Os dois paradigmas refletem na implementação, i.e., é mais fácil implementar um método de dividir para conquistar utilizando uma memória compartilhada.

 A implementação do merge sort com OpenMP se baseia na propriedade que as duas partes divididas do vetor podem ser ordenadas independentemente. Para isso, utilizei a cláusula de seções na recursão do algoritmo.

Já no merge sort com MPI, dividi o vetor em pedaços menores e enviei cada pedaço para um processo por meio do scatter. Cada processo realiza uma versão sequencial do algoritmo e, ao final, todas as partes são mescladas num único vetor.

A implementação do bubble sort com OpenMP não é a mais eficiente. Utilizei apenas um a cláusula parallel for para paralelizar o algoritmo. Pesquisas mostraram que há versões mais eficientes do mesmo algoritmo porém mais complexas pois é necessário lidar com a condição de corrida entre as threads. Tal complexidade era esperada e por isso optei pela versão menos eficiente, porém mais simples.

Contudo, o bubble sort com MPI também segue um modelo parecido com o do merge sort: divide o vetor em pedaços menores, cada processo realiza uma versão sequencial do algoritmo e então todas as partes são mescladas resultando num único vetor ordenado.

Dentre as implementações com MPI, a mais difícil foi o merge sort. Precisei utilizar duas versões diferentes de merge: uma para a mesclagem final e outra para o próprio algoritmo do merge sort. Isso não foi necessário no código do bubble sort.

## Resultados

Para o benchmark, criei um script que executa 10 vezes cada versão, no intuito de poder calcular uma média das execuções. Contudo, os resultados foram muito consistentes, não havendo tal necessidade. Seguem os resultados em segundos:

| Rodada | Mergesort MPI | Mergesort OPENMP | Bubblesort MPI | Bubblesort OPENMP |
| ------ | ------------- | ---------------- | -------------- | ----------------- |
| 1 | 0.07 | 0.00 | 0.06 | 0.00 |
| 2 | 0.06 | 0.00 | 0.06 | 0.00 |
| 3 | 0.07 | 0.00 | 0.06 | 0.00 |
| 4 | 0.06 | 0.00 | 0.06 | 0.01 |
| 5 | 0.06 | 0.00 | 0.06 | 0.01 |
| 6 | 0.07 | 0.00 | 0.06 | 0.00 |
| 7 | 0.06 | 0.00 | 0.06 | 0.00 |
| 8 | 0.06 | 0.00 | 0.06 | 0.00 |
| 9 | 0.07 | 0.00 | 0.06 | 0.00 |
| 10 | 0.06 | 0.00 | 0.06 | 0.00 |

Pode-se verificar que as versões com OpenMP são mais eficientes e isto também é esperado dado o funcionamento desta interface. 

No intuito de analisar a diferença real entre os dois algoritmos, realizei uma execução com 143360 chaves. Os resultados seguem:

| Rodada | Mergesort MPI | Mergesort OPENMP | Bubblesort MPI | Bubblesort OPENMP |
| ------ | ------------- | ---------------- | -------------- | ----------------- |
| 1 | 0.10 | 0.11 | 6.87 | 37.66 |
| 2 | 0.10 | 0.11 | 7.55 | 34.88 |
| 3 | 0.10 | 0.11 | 6.78 | 36.11 |
| 4 | 0.10 | 0.13 | 7.47 | 33.78 |
| 5 | 0.10 | 0.11 | 8.55 | 35.08 |
| 6 | 0.10 | 0.13 | 7.08 | 36.21 |
| 7 | 0.12 | 0.11 | 7.18 | 35.97 |
| 8 | 0.11 | 0.15 | 6.41 | 35.83 |
| 9 | 0.10 | 0.12 | 6.45 | 45.39 |
| 10 |0.10 | 0.13 | 7.54 | 51.60 |

Duas coisas aparecem de interessante nesta rodada. Primeiramente, a diferença entre os algoritmos se evidenciam: enquanto o bubble sort é um algoritmo reconhecido por sua simplicidade porém ineficiência, o merge sort é reconhecido por sua eficiência no pior caso.

A segunda, e mais interessante para este relatório, é a evidência da implementação ineficiente do algoritmo bubble sort com OpenMP, com uma média de tempo de execução de 38.25 segundos, 5 vezes mais demorado comparado ao mesmo algoritmo com MPI que teve um média de execução de 7.19 segundos.
