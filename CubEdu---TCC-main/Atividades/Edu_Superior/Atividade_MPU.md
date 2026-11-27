# Atividade: Reconstruindo a Trajetória do CubEdu

## Objetivo

Utilizando os dados coletados pelo sensor MPU6050, os alunos deverão desenvolver uma cadeia simplificada de navegação inercial capaz de estimar atitude, velocidade e trajetória da plataforma CubEdu.

A atividade será dividida em dois encontros:

- **Aula 1:** preparação do sistema embarcado, aquisição de dados, calibração e validação dos sensores;
- **Aula 2:** execução da missão completa, estimativa da trajetória e análise dos resultados.

---

# Aula 1 - Preparação e Validação da Unidade Inercial

## Objetivos da Aula

Ao final desta etapa, os alunos deverão ser capazes de:

- configurar o MPU6050;
- coletar dados do acelerômetro e giroscópio;
- registrar dados em formato CSV;
- calcular os bias dos sensores;
- implementar o filtro complementar;
- validar a estimativa de atitude;
- compreender as limitações da navegação inercial.

## Duração

**2 horas**

---

## Materiais Necessários

- Plataforma CubEdu
- OBC (ESP8266)
- Sensor MPU6050
- Computador com Arduino IDE
- Cabo USB para programação
- MATLAB, GNU Octave ou Python

---

## Cronograma da Aula 1

| Etapa | Atividade | Duração |
|---------|---------|---------:|
| 1 | Desenvolvimento do código de aquisição | 40 min |
| 2 | Calibração dos sensores | 30 min |
| 3 | Implementação do filtro complementar | 30 min |
| 4 | Testes de validação | 20 min |
| **Total** | | **120 min** |

---

## Etapa 1 - Desenvolvimento do Código de Aquisição

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

Recomenda-se uma frequência mínima de aquisição de:

```text
50 Hz
```

correspondente a:

```text
Δt ≈ 0,02 s
```

---

## Etapa 2 - Calibração dos Sensores

Após conectar o MPU6050 à OBC:

1. posicione o CubEdu sobre uma superfície plana;
2. mantenha a plataforma parada;
3. registre 10 segundos de dados;
4. calcule os bias dos sensores;
5. implemente as correções necessárias.

### Bias do Giroscópio

```text
bias_Gx = média(Gx)
bias_Gy = média(Gy)
bias_Gz = média(Gz)
```

### Correção do Giroscópio

```text
Gx_corrigido = Gx - bias_Gx
Gy_corrigido = Gy - bias_Gy
Gz_corrigido = Gz - bias_Gz
```

### Bias do Acelerômetro

Considerando o eixo Z alinhado com a gravidade:

```text
bias_Ax = média(Ax)
bias_Ay = média(Ay)
bias_Az = média(Az) - 9,81
```

### Correção do Acelerômetro

```text
Ax_corrigido = Ax - bias_Ax
Ay_corrigido = Ay - bias_Ay
Az_corrigido = Az - bias_Az
```

---

## Etapa 3 - Implementação do Filtro Complementar

Inicialmente determine os ângulos utilizando apenas o acelerômetro.

### Roll

```text
roll_acc = atan2(-Ay, Az)
```

### Pitch

```text
pitch_acc = atan2(Ax, sqrt(Ay² + Az²))
```

Em seguida, integre os dados do giroscópio:

```text
roll_gyro[k] = roll[k-1] + Gx_corrigido[k] × dt[k]
```

```text
pitch_gyro[k] = pitch[k-1] + Gy_corrigido[k] × dt[k]
```

```text
yaw[k] = yaw[k-1] + Gz_corrigido[k] × dt[k]
```

Combine as estimativas utilizando um filtro complementar:

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

---

## Etapa 4 - Testes de Validação

Execute os seguintes testes.

### Teste A - Inclinação da Plataforma

Incline o CubEdu:

- para frente;
- para trás;
- para a esquerda;
- para a direita.

Observe o comportamento dos ângulos de roll e pitch.

### Teste B - Rotação da Plataforma

Rotacione lentamente a plataforma em torno do eixo vertical.

Observe o comportamento do ângulo:

```text
yaw
```

Discuta:

- a deriva causada pelo giroscópio;
- a ausência de um magnetômetro para correção da orientação.

---

## Entregáveis da Aula 1

Cada grupo deverá possuir ao final da aula:

- código de aquisição funcionando;
- arquivo CSV de teste;
- bias calculados;
- filtro complementar implementado;
- gráficos de aceleração;
- gráficos de velocidade angular;
- gráficos de roll, pitch e yaw.

---

# Aula 2 - Navegação Inercial e Reconstrução da Trajetória

## Objetivos da Aula

Ao final desta etapa, os alunos deverão ser capazes de:

- transformar acelerações entre referenciais;
- remover o efeito da gravidade;
- estimar velocidade;
- estimar posição;
- aplicar ZUPT;
- avaliar os erros da navegação inercial.

## Duração

**2 horas**

---

## Materiais Necessários

- CubEdu completo montado;
- MPU6050 calibrado;
- computador para processamento dos dados;
- fita métrica;
- fita adesiva para demarcar a trajetória;
- arquivo CSV obtido durante o experimento.

---

## Cronograma da Aula 2

| Etapa | Atividade | Duração |
|---------|---------|---------:|
| 1 | Preparação da missão | 15 min |
| 2 | Execução da missão | 20 min |
| 3 | Transformação para referencial local | 25 min |
| 4 | Remoção da gravidade | 10 min |
| 5 | Estimativa da velocidade | 15 min |
| 6 | Estimativa da trajetória | 15 min |
| 7 | Avaliação dos resultados | 20 min |
| **Total** | | **120 min** |

---

## Etapa 1 - Preparação da Missão

Utilizando o sistema validado na Aula 1:

1. verificar a comunicação com o MPU6050;
2. confirmar a aplicação dos bias calculados;
3. verificar a taxa de aquisição;
4. iniciar a coleta de dados.

---

## Etapa 2 - Execução da Missão

Demarque uma trajetória reta de:

```text
1 metro
```

Procedimento:

1. mantenha o CubEdu parado durante 10 s;
2. movimente a plataforma por aproximadamente 1 m;
3. realize um deslocamento suave e nivelado;
4. pare no ponto final;
5. mantenha a plataforma parada por mais 10 s;
6. salve os dados em formato CSV.

> A trajetória retilínea foi escolhida para permitir a execução da atividade dentro do tempo disponível. Como extensão, o experimento pode ser repetido em trajetórias retangulares ou curvas.

---

## Etapa 3 - Transformação para Referencial Local

Utilizando os ângulos estimados de roll, pitch e yaw, construa a matriz de rotação:

```text
C
```

e transforme as acelerações medidas no referencial do sensor para o referencial local:

```text
a_local = C × a_corpo
```

---

## Etapa 4 - Remoção da Gravidade

Remova a componente gravitacional:

```text
a_linear = a_local - [0, 0, 9,81]ᵀ
```

Quando o CubEdu estiver parado, a aceleração linear deve permanecer próxima de zero.

---

## Etapa 5 - Estimativa da Velocidade

Calcule a velocidade utilizando integração trapezoidal:

```text
v[k] = v[k-1] + 0,5 × (a[k] + a[k-1]) × dt[k]
```

Nos intervalos em que a plataforma estiver parada, aplique a técnica de velocidade nula:

```text
v = 0 m/s
```

Esta técnica é conhecida como **ZUPT (Zero Velocity Update)**.

---

## Etapa 6 - Estimativa da Trajetória

Calcule a posição:

```text
p[k] = p[k-1] + 0,5 × (v[k] + v[k-1]) × dt[k]
```

Gere:

- posição ao longo do percurso;
- velocidade ao longo do tempo;
- gráfico da trajetória estimada;
- comparação com a distância real percorrida.

---

## Etapa 7 - Avaliação dos Resultados

Preencha a seguinte tabela:

| Parâmetro | Resultado |
|-----------|-----------:|
| Frequência média de aquisição (Hz) | |
| Bias de Gx (°/s) | |
| Bias de Gy (°/s) | |
| Bias de Gz (°/s) | |
| Distância real (m) | 1,00 |
| Distância estimada (m) | |
| Erro absoluto (m) | |
| Erro relativo (%) | |
| Velocidade final estimada (m/s) | |

### Erro Absoluto

```text
Erro absoluto = |distância estimada - distância real|
```

### Erro Relativo

```text
Erro relativo = (Erro absoluto / distância real) × 100
```

---

## Resultados Obrigatórios

Ao final da atividade os grupos deverão apresentar:

- gráfico das acelerações nos três eixos;
- gráfico das velocidades angulares nos três eixos;
- gráfico de roll, pitch e yaw;
- gráfico da velocidade estimada;
- gráfico da posição estimada;
- representação da trajetória;
- comparação entre distância real e distância estimada.

---

## Discussão

1. Qual foi o bias identificado em cada eixo do giroscópio?
2. A frequência de aquisição permaneceu constante durante todo o experimento?
3. Qual foi o efeito do filtro complementar sobre roll e pitch?
4. O ângulo de yaw apresentou deriva? Por quê?
5. A velocidade final estimada foi próxima de zero após a aplicação da ZUPT?
6. Qual foi o erro entre a distância estimada e a distância real?
7. Como erros de atitude afetam a remoção da gravidade?
8. Por que pequenos erros de aceleração crescem rapidamente após integrações sucessivas?
9. Qual foi o efeito observado com a aplicação da ZUPT?
10. Quais sensores poderiam ser incorporados para melhorar a estimativa da trajetória?

---

## Limitações da Atividade

O MPU6050 possui acelerômetro e giroscópio, mas não possui magnetômetro. Dessa forma, o ângulo de yaw é obtido exclusivamente pela integração do giroscópio, acumulando deriva ao longo do tempo.

Além disso, pequenos erros de calibração, orientação e remoção da gravidade são amplificados durante as integrações necessárias para obtenção de velocidade e posição. Por essa razão, a trajetória calculada deve ser interpretada como uma estimativa experimental e não como uma medida exata da posição.

Em sistemas reais, unidades inerciais normalmente são combinadas com sensores adicionais, como:

- magnetômetros;
- receptores GNSS;
- câmeras;
- sensores solares;
- sensores de estrelas;
- referências externas de posição.

---

## Conclusão

Nesta atividade, os alunos desenvolveram uma cadeia simplificada de navegação inercial utilizando dados do MPU6050 e da plataforma CubEdu.

Durante a primeira aula foram realizadas a integração do sensor, a aquisição de dados, a calibração dos bias e a implementação do filtro complementar. Na segunda aula foram executadas as etapas de navegação inercial, transformação de referenciais, compensação da gravidade, estimativa de velocidade, reconstrução da trajetória e análise dos erros.

A comparação entre a trajetória real e a trajetória estimada permitiu observar os efeitos do ruído, do bias, dos erros de orientação e da deriva dos sensores inerciais. Dessa forma, a atividade integrou conceitos de programação, álgebra linear, processamento de sinais e navegação inercial por meio de uma aplicação prática com a plataforma CubEdu.

---
** Glosário

C =

       | cosψ·cosθ    cosψ·sinθ·sinφ - sinψ·cosφ    cosψ·sinθ·cosφ + sinψ·sinφ |
       |                                                                       |
       | sinψ·cosθ    sinψ·sinθ·sinφ + cosψ·cosφ    sinψ·sinθ·cosφ - cosψ·sinφ |
       |                                                                       |
       |   -sinθ                 cosθ·sinφ                    cosθ·cosφ        |
---

## Referências

- COSTA, D. de O. *Introdução à Navegação Assistida - Aula 6*. 2025. Faculdade de Ciências e Tecnologias em Engenharia, Universidade de Brasília (UnB). Material de apoio à disciplina de Engenharia Aeroespacial e Eletrônica.

- COSTA, D. de O. *Acelerômetros e Aceleração Linear: Corpo ao NED - Aula 11*. 2026. Faculdade de Ciências e Tecnologias em Engenharia, Universidade de Brasília (UnB).

- COSTA, D. de O. *Elementos de Sistemas de Navegação - Aula 19: Exercícios*. 2026. Faculdade de Ciências e Tecnologias em Engenharia, Universidade de Brasília (UnB).

- COSTA, D. de O. *Início da Fusão Sensorial: Acelerômetro + Girômetro - Aula 17*. 2026. Material de aula da disciplina Elementos de Sistemas de Navegação, Curso de Engenharia Aeroespacial, Universidade de Brasília (UnB).

- COSTA, D. de O. *Trabalho Final ESN - 1/2026*. 2026. Documento orientador do Trabalho Final da disciplina Elementos de Sistemas de Navegação.

- ROGERS, R. M. *Applied Mathematics in Integrated Navigation Systems*. 3. ed. Reston, VA: American Institute of Aeronautics and Astronautics, 2007. ISBN 9781563479274.
