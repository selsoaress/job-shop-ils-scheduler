## Problema de Escalonamento Job Shop (Job Shop Scheduling Problem – JSSP)

O **Job Shop Scheduling Problem (JSSP)** é um problema clássico de otimização combinatória cujo objetivo é programar a execução de um conjunto de tarefas em máquinas limitadas, respeitando restrições tecnológicas e de capacidade.

### Definição formal

- Temos um conjunto de **jobs** \( J = \{1, \dots, n\} \).
- Cada job \( j \) é composto por uma sequência ordenada de **operações**  
  \[
  O_{j1}, O_{j2}, \dots, O_{j m_j}
  \]
- Cada operação:
  - Deve ser processada em **uma máquina específica**;
  - Possui um **tempo de processamento** conhecido;
  - Não pode ser interrompida (sem preempção).
- Cada máquina:
  - Pode processar **apenas uma operação por vez**.

### Objetivo clássico

O objetivo mais comum é minimizar o **makespan**, isto é, o tempo total necessário para completar todos os jobs:
\[
\min C_{\max}
\]

---

## Modelagem via Grafo Direcionado Acíclico (DAG)

Uma forma poderosa de representar o JSSP é por meio de um **grafo direcionado acíclico**, que explicita as dependências temporais entre as operações.

### Nós do grafo

- Cada **nó** representa uma operação \( O_{jk} \).
- Opcionalmente, adiciona-se:
  - Um nó **fonte** (início);
  - Um nó **sorvedouro** (fim).

---

### Arestas do grafo

Existem dois tipos principais de arestas:

#### 1. Arestas de precedência tecnológica (fixas)

Representam a ordem interna de cada job.

Se a operação \( O_{jk} \) deve ocorrer antes de \( O_{j(k+1)} \), então:
\[
O_{jk} \rightarrow O_{j(k+1)}
\]

Essas arestas são **obrigatórias** e sempre presentes no grafo.

---

#### 2. Arestas de conflito de máquina (decisórias)

Representam a ordem entre operações que competem pela **mesma máquina**.

Se duas operações \( O_a \) e \( O_b \) usam a mesma máquina, então **uma deve preceder a outra**, mas essa ordem é uma **decisão de escalonamento**:

- Ou: \( O_a \rightarrow O_b \)
- Ou: \( O_b \rightarrow O_a \)

A escolha dessas arestas define uma solução específica do problema.

---

## Aciclicidade e viabilidade

- O grafo completo (precedência + conflitos de máquina) **deve ser acíclico**.
- Um ciclo no grafo implica uma **inconsistência temporal**, ou seja, uma solução inviável.
- Assim:
  - **Toda solução viável do JSSP corresponde a um DAG**
  - **Todo DAG válido induz um escalonamento viável**

---

## Avaliação do makespan via caminho crítico

Uma vez construído o DAG:

- Atribui-se a cada nó um peso igual ao tempo de processamento da operação.
- O **makespan** é dado pelo **comprimento do maior caminho da fonte ao sorvedouro**:
\[
C_{\max} = \max_{\text{caminhos}} \sum \text{tempos das operações}
\]

Esse cálculo equivale a encontrar o **caminho crítico** no DAG.

---

## Por que essa modelagem é importante?

- Permite:
  - Avaliar rapidamente soluções via caminho crítico;
  - Detectar inviabilidade por ciclos;
  - Aplicar heurísticas e metaheurísticas (ILS, tabu, SA, etc.) como **modificações locais nas arestas de máquina**.
- É a base de várias abordagens clássicas:
  - Shifting Bottleneck
  - Busca local por reversão de arcos
  - Modelagens híbridas com programação matemática

---

## Intuição resumida

> Escalonar um job shop é escolher orientações para os conflitos de máquina  
> de modo que o grafo resultante seja acíclico  
> e tenha o menor caminho crítico possível.
