# Atividade: Reconstruindo a Trajetória do CubEdu

## Objetivo

Utilizando os dados coletados pelo sensor **MPU6050**, os alunos deverão desenvolver um programa para estimar a atitude, a velocidade e a trajetória aproximada da plataforma CubEdu. Ao final da atividade, os participantes serão capazes de calibrar uma unidade de medição inercial, aplicar um filtro complementar, transformar acelerações entre referenciais e analisar os erros gerados pela integração numérica.

---

## Público-Alvo

- Ensino Superior

---

## Duração

- 2 horas

---

## Materiais Necessários

- Plataforma CubEdu
- OBC (ESP8266)
- Sensor MPU6050
- Computador com Arduino IDE
- Cabo USB para programação
- MATLAB, GNU Octave ou Python
- Fita métrica
- Fita adesiva para demarcar a trajetória
- Carrinho ou suporte para movimentar o CubEdu
- Planilha eletrônica ou arquivo CSV para armazenar os dados

---

## Cronograma da Atividade

| Etapa | Atividade | Duração sugerida |
|---|---|---:|
| 1 | Preparação do código de aquisição | 15 min |
| 2 | Integração e calibração inicial | 15 min |
| 3 | Execução da missão e coleta dos dados | 20 min |
| 4 | Importação e tratamento dos dados | 15 min |
| 5 | Estimativa da atitude | 20 min |
| 6 | Estimativa da velocidade e trajetória | 20 min |
| 7 | Análise e discussão | 15 min |
| **Total** |  | **120 min** |

---

### Etapa 1 - Desenvolvimento do Código de Aquisição

Desenvolva um programa capaz de coletar os dados do acelerômetro e do giroscópio do MPU6050.

O programa deverá registrar:

- tempo da medição em milissegundos;
- aceleração nos eixos X, Y e Z em `m/s²`;
- velocidade angular nos eixos X, Y e Z em `°/s`.

Os dados deverão ser exibidos no Monitor Serial no formato CSV:

```csv
tempo_ms,ax_ms2,ay_ms2,az_ms2,gx_degs,gy_degs,gz_degs
0,0.03,-0.05,9.79,0.12,-0.08,0.05
```

Recomenda-se uma frequência de aquisição mínima de **50 Hz**. Isso corresponde a um intervalo aproximado de:

```text
Δt = 1 / 50 = 0,02 s
```

---

### Etapa 2 - Teste e Calibração Inicial

Após conectar o MPU6050 à OBC:

1. posicione o CubEdu sobre uma superfície plana;
2. mantenha a plataforma parada;
3. inicie o programa de aquisição;
4. registre **10 segundos** de dados sem movimentar o conjunto;
5. utilize essas amostras para calcular o bias de cada eixo.

#### Bias do Giroscópio

```text
bias_Gx = média(Gx em repouso)
bias_Gy = média(Gy em repouso)
bias_Gz = média(Gz em repouso)
```

#### Correção do Giroscópio

```text
Gx_corrigido = Gx - bias_Gx
Gy_corrigido = Gy - bias_Gy
Gz_corrigido = Gz - bias_Gz
```

Considerando o eixo Z alinhado à gravidade, o bias do acelerômetro pode ser estimado por:

```text
bias_Ax = média(Ax em repouso)
bias_Ay = média(Ay em repouso)
bias_Az = média(Az em repouso) - 9,81
```

Antes de aplicar a correção, confirme a orientação e o sinal do eixo Z.

---

### Etapa 3 - Execução da Missão

Demarque uma trajetória reta de **1 metro**.

1. mantenha o CubEdu parado por 10 segundos;
2. movimente a plataforma em linha reta por 1 metro;
3. realize o movimento de forma suave e mantenha a estrutura nivelada;
4. pare no ponto final;
5. mantenha o CubEdu parado por mais 10 segundos;
6. salve os dados coletados em um arquivo CSV.

> A trajetória reta foi escolhida para permitir a realização da atividade em 2 horas. Como desafio posterior, o experimento poderá ser repetido em uma trajetória retangular.

---

### Etapa 4 - Importação e Tratamento dos Dados

O programa de análise deverá:

1. importar o arquivo CSV;
2. converter o tempo de milissegundos para segundos;
3. calcular o intervalo entre amostras;
4. remover linhas duplicadas ou inválidas;
5. aplicar a correção de bias;
6. gerar gráficos dos dados do acelerômetro e do giroscópio.

O intervalo de tempo deverá ser calculado para cada amostra:

```text
dt[k] = t[k] - t[k-1]
```

Não assuma que todos os intervalos são exatamente iguais.

---

### Etapa 5 - Estimativa da Atitude

Calcule inicialmente os ângulos de roll e pitch utilizando o acelerômetro:

```text
roll_acc = atan2(-Ay, Az)
```

```text
pitch_acc = atan2(Ax, sqrt(Ay² + Az²))
```

Em seguida, integre as velocidades angulares:

```text
roll_gyro[k] = roll[k-1] + Gx_corrigido[k] × dt[k]
```

```text
pitch_gyro[k] = pitch[k-1] + Gy_corrigido[k] × dt[k]
```

```text
yaw[k] = yaw[k-1] + Gz_corrigido[k] × dt[k]
```

Combine acelerômetro e giroscópio por meio de um filtro complementar:

```text
roll[k] = α × roll_gyro[k] + (1 - α) × roll_acc[k]
```

```text
pitch[k] = α × pitch_gyro[k] + (1 - α) × pitch_acc[k]
```

Utilize inicialmente:

```text
α = 0,98
```

O valor de `α` poderá ser alterado para comparar o comportamento do filtro.

---

### Etapa 6 - Estimativa da Trajetória

Utilizando roll, pitch e yaw, construa uma matriz de rotação para transformar a aceleração do referencial do sensor para um referencial local.

```text
a_local = C × a_corpo
```

Em seguida, remova a componente da gravidade. Considerando um referencial local com o eixo vertical positivo para cima:

```text
a_linear = a_local - [0, 0, 9,81]ᵀ
```

Confira a convenção utilizada no programa. Quando a plataforma estiver parada, a aceleração linear deverá permanecer próxima de zero.

Calcule a velocidade utilizando integração trapezoidal:

```text
v[k] = v[k-1] + 0,5 × (a[k] + a[k-1]) × dt[k]
```

Em seguida, calcule a posição:

```text
p[k] = p[k-1] + 0,5 × (v[k] + v[k-1]) × dt[k]
```

Quando o CubEdu estiver parado no início ou no fim do percurso, aplique uma correção de velocidade nula:

```text
v = 0 m/s
```

Essa técnica é conhecida como **ZUPT**, ou atualização de velocidade nula.

---

### Etapa 7 - Análise dos Resultados

Preencha a tabela:

| Parâmetro | Resultado |
|---|---:|
| Frequência média de aquisição (Hz) |  |
| Bias de Gx (°/s) |  |
| Bias de Gy (°/s) |  |
| Bias de Gz (°/s) |  |
| Distância real (m) | 1,00 |
| Distância estimada (m) |  |
| Erro absoluto (m) |  |
| Erro relativo (%) |  |
| Velocidade final estimada (m/s) |  |

Calcule o erro absoluto:

```text
Erro absoluto = |distância estimada - distância real|
```

Calcule o erro relativo:

```text
Erro relativo = (Erro absoluto / distância real) × 100
```

#### Exemplo

Distância real:

```text
1,00 m
```

Distância estimada:

```text
1,18 m
```

Erro absoluto:

```text
Erro absoluto = |1,18 - 1,00| = 0,18 m
```

Erro relativo:

```text
Erro relativo = (0,18 / 1,00) × 100 = 18 %
```

---

## Resultados Obrigatórios

O programa desenvolvido deverá apresentar:

- gráfico das acelerações nos três eixos;
- gráfico das velocidades angulares nos três eixos;
- gráfico de roll, pitch e yaw;
- gráfico da velocidade estimada;
- gráfico da posição estimada;
- representação da trajetória;
- comparação entre a distância real e a distância estimada.

---

## Discussão

Após realizar os cálculos, responda às seguintes questões:

1. Qual foi o bias identificado em cada eixo do giroscópio?
2. A frequência de aquisição permaneceu constante durante todo o experimento?
3. Qual foi o efeito do filtro complementar sobre roll e pitch?
4. A velocidade final estimada foi próxima de zero?
5. Qual foi o erro entre a distância estimada e a distância real?
6. Por que pequenos erros de aceleração aumentam após a integração?
7. Como um erro na estimativa da atitude afeta a remoção da gravidade?
8. Por que a estimativa de yaw apresenta deriva maior?
9. Qual foi o efeito da aplicação da ZUPT?
10. Quais sensores poderiam ser adicionados para melhorar a trajetória estimada?

---

## Limitações da Atividade

O MPU6050 possui acelerômetro e giroscópio, mas não possui magnetômetro. Dessa forma, o ângulo de yaw é obtido apenas pela integração do giroscópio e acumula deriva ao longo do tempo.

Além disso, pequenos erros de calibração, orientação e remoção da gravidade são acumulados durante a integração. Portanto, a trajetória calculada deverá ser interpretada como uma **estimativa experimental**, e não como uma medição exata da posição.

Em sistemas reais, unidades inerciais são combinadas com outros sensores, como:

- magnetômetros;
- receptores GNSS;
- câmeras;
- sensores solares;
- sensores de estrelas;
- referências externas de posição.

---

## Conclusão

Nesta atividade, os alunos desenvolveram uma cadeia simplificada de navegação inercial utilizando os dados do MPU6050. O processo envolveu aquisição, calibração, estimativa de atitude, transformação de referenciais, compensação da gravidade e integração numérica.

A comparação entre a trajetória real e a trajetória estimada permitiu observar os efeitos do ruído, do bias e da deriva. Dessa forma, a atividade relacionou programação, cálculo, álgebra linear, processamento de sinais e sistemas de navegação por meio de uma aplicação prática com a plataforma CubEdu.

---

## Referências

- COSTA, D. de O. *Introdução à Navegação Assistida - Aula 6*. 2025. Faculdade de Ciências e Tecnologias em Engenharia, Universidade de Brasília (UnB). Material de apoio à disciplina de Engenharia Aeroespacial e Eletrônica.
- COSTA, D. de O. *Acelerômetros e Aceleração Linear: Corpo ao NED - Aula 11*. 2026. Faculdade de Ciências e Tecnologias em Engenharia, Universidade de Brasília (UnB).
- COSTA, D. de O. *Elementos de Sistemas de Navegação - Aula 19: Exercícios*. 2026. Faculdade de Ciências e Tecnologias em Engenharia, Universidade de Brasília (UnB). Material de referência para o fluxo de navegação inercial.
- COSTA, D. de O. *Início da Fusão Sensorial: Acelerômetro + Girômetro - Aula 17*. 2026. Material de aula da disciplina Elementos de Sistemas de Navegação, Curso de Engenharia Aeroespacial, Universidade de Brasília (UnB).
- COSTA, D. de O. *Trabalho Final ESN - 1/2026*. 2026. Documento orientador do Trabalho Final da disciplina Elementos de Sistemas de Navegação.
- ROGERS, R. M. *Applied Mathematics in Integrated Navigation Systems*. 3. ed. Reston, VA: American Institute of Aeronautics and Astronautics, 2007. ISBN 9781563479274.
