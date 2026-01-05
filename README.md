## Job Shop Scheduling Problem

Este projeto trata do **Job Shop Scheduling Problem (JSSP)**, um problema clássico de otimização combinatória cujo objetivo é definir a ordem de execução de operações em máquinas limitadas, respeitando restrições tecnológicas e de capacidade.

Cada job é composto por uma sequência fixa de operações. Cada operação deve ser processada em uma máquina específica, possui um tempo de processamento conhecido e não pode ser interrompida. Cada máquina pode processar apenas uma operação por vez. O objetivo considerado é minimizar o tempo total de conclusão do escalonamento (makespan).

---

## Modelagem por grafo

O problema é modelado por meio de um **grafo direcionado acíclico (DAG)**. Cada operação é representada por um nó, e as dependências temporais entre operações são representadas por arestas direcionadas.

Existem dois tipos de dependências no grafo. O primeiro tipo corresponde às **restrições tecnológicas**, que impõem a ordem fixa das operações dentro de um mesmo job. Essas dependências são obrigatórias e independem do escalonamento adotado.

O segundo tipo corresponde aos **conflitos de máquina**. Operações que utilizam a mesma máquina não podem ser executadas simultaneamente, sendo necessário decidir uma ordem de processamento entre elas. Essas decisões definem a orientação das arestas de máquina e caracterizam uma solução específica do problema.

Para que um escalonamento seja viável, o grafo resultante deve ser acíclico. A presença de ciclos indica uma inconsistência temporal e torna a solução inviável.

---

## Avaliação da solução

Uma vez construído o grafo, o makespan é obtido a partir do caminho crítico, isto é, o maior caminho no DAG considerando os tempos de processamento das operações. Esse valor é utilizado como critério de avaliação das soluções.

---

## Construção inicial

A solução inicial é construída por uma heurística gulosa baseada na regra **SPT (Shortest Processing Time)**. Em cada máquina, as operações disponíveis são ordenadas de acordo com o menor tempo de processamento, respeitando sempre as restrições de precedência dos jobs. Essa etapa gera um escalonamento inicial viável.

---

## Busca local

A busca local utiliza como movimento básico a troca de **operações adjacentes em uma mesma máquina**. Após o swap, é realizado um ajuste ao longo da cadeia produtiva do job, garantindo que a ordem das operações seja preservada e que o grafo permaneça acíclico.

---

## Perturbação

A etapa de perturbação consiste na troca de **operações não consecutivas em uma mesma máquina**. Esse movimento promove alterações mais intensas no escalonamento e tem como objetivo permitir a saída de ótimos locais. Após a perturbação, o escalonamento é novamente ajustado para restaurar a viabilidade.

---

## Visão geral da abordagem

De forma resumida, a abordagem adotada neste projeto segue os seguintes passos:

- construção inicial gulosa via SPT;
- exploração do espaço de soluções por swaps em máquinas;
- ajustes para garantir viabilidade do grafo;
- avaliação das soluções por meio do caminho crítico.
