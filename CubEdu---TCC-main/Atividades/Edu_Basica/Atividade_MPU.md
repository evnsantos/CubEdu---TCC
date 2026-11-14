# Atividade: Como o CubEdu Identifica sua Orientação?

## Objetivo

Utilizando o sensor **MPU6050**, os alunos deverão coletar dados de aceleração e velocidade angular para identificar a orientação e os movimentos da plataforma CubEdu. Ao final da atividade, os participantes serão capazes de reconhecer os eixos do sensor, relacionar as medições aos movimentos realizados e interpretar os ângulos de rolagem e arfagem.

---

## Público-Alvo

- Ensino Médio

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
- Régua ou transferidor
- Apoio para inclinar a plataforma
- Folha de registro de dados

---

## Cronograma da Atividade

| Etapa | Atividade | Duração sugerida |
|---|---|---:|
| 1 | Desenvolvimento e compreensão do código | 25 min |
| 2 | Teste inicial da OBC | 10 min |
| 3 | Integração entre OBC e MPU6050 | 15 min |
| 4 | Montagem do kit | 10 min |
| 5 | Coleta de dados | 25 min |
| 6 | Interpretação dos movimentos | 20 min |
| 7 | Discussão e conclusão | 15 min |
| **Total** |  | **120 min** |

---

### Etapa 1 - Desenvolvimento do Código

Desenvolva um programa capaz de realizar a leitura do acelerômetro e do giroscópio presentes no MPU6050.

Como base, os alunos poderão:

- utilizar o código disponibilizado no repositório do projeto;
- adaptar exemplos encontrados na biblioteca do sensor;
- desenvolver seu próprio código.

O programa deverá exibir no Monitor Serial:

- aceleração no eixo X, representada por `Ax`;
- aceleração no eixo Y, representada por `Ay`;
- aceleração no eixo Z, representada por `Az`;
- velocidade angular no eixo X, representada por `Gx`;
- velocidade angular no eixo Y, representada por `Gy`;
- velocidade angular no eixo Z, representada por `Gz`.

As acelerações deverão ser apresentadas em `m/s²` e as velocidades angulares em `°/s`.

---

### Etapa 2 - Teste Inicial da OBC

Antes de conectar o sensor, os alunos deverão:

1. compilar o código na Arduino IDE;
2. realizar o upload para a ESP8266;
3. verificar se o programa executa corretamente;
4. confirmar o funcionamento da comunicação serial;
5. verificar se mensagens de inicialização são exibidas no Monitor Serial.

---

### Etapa 3 - Integração OBC e Payload

Após validar o funcionamento da OBC:

1. desligar a alimentação da plataforma;
2. conectar o MPU6050 ao barramento I²C;
3. conferir as conexões de alimentação, SDA e SCL;
4. ligar novamente a plataforma;
5. verificar se as seis medições são exibidas corretamente;
6. manter o sensor parado por alguns segundos e observar as pequenas oscilações das leituras.

---

### Etapa 4 - Montagem do Kit

Com os testes anteriores concluídos:

1. montar o conjunto completo na estrutura do CubEdu;
2. organizar os cabos e conexões;
3. verificar a direção dos eixos X, Y e Z indicada no módulo;
4. posicionar a estrutura sobre uma superfície plana;
5. verificar novamente o correto funcionamento do sistema.

---

### Etapa 5 - Coleta de Dados

Posicione o CubEdu em cinco condições diferentes. Aguarde aproximadamente **5 segundos** para a estabilização das leituras antes de registrar cada resultado.

| Medição | Posição do CubEdu | Ax (m/s²) | Ay (m/s²) | Az (m/s²) | Gx (°/s) | Gy (°/s) | Gz (°/s) |
|---|---|---:|---:|---:|---:|---:|---:|
| 1 | Horizontal |  |  |  |  |  |  |
| 2 | Inclinado para a direita |  |  |  |  |  |  |
| 3 | Inclinado para a esquerda |  |  |  |  |  |  |
| 4 | Inclinado para frente |  |  |  |  |  |  |
| 5 | Inclinado para trás |  |  |  |  |  |  |

Em seguida, faça uma rotação lenta da plataforma sobre a mesa e observe qual eixo do giroscópio apresenta a maior alteração.

---

### Etapa 6 - Interpretação dos Movimentos

Quando o CubEdu está parado, o módulo da aceleração deve apresentar valor próximo ao da aceleração da gravidade.

```text
|a| = sqrt(Ax² + Ay² + Az²)
```

O resultado esperado é aproximadamente:

```text
|a| ≈ 9,81 m/s²
```

Para estimar os ângulos de rolagem e arfagem, podem ser utilizadas as seguintes expressões:

#### Rolagem

```text
roll = atan2(-Ay, Az)
```

#### Arfagem

```text
pitch = atan2(Ax, sqrt(Ay² + Az²))
```

Os resultados inicialmente estarão em radianos. Para convertê-los em graus:

```text
ângulo_graus = ângulo_radianos × 180 / π
```

Registre os valores calculados:

| Posição | Roll (°) | Pitch (°) | Interpretação |
|---|---:|---:|---|
| Horizontal |  |  |  |
| Inclinado para a direita |  |  |  |
| Inclinado para a esquerda |  |  |  |
| Inclinado para frente |  |  |  |
| Inclinado para trás |  |  |  |

> Os sinais dos ângulos podem mudar conforme a orientação física do MPU6050 na estrutura. Os alunos deverão determinar experimentalmente qual sinal corresponde a cada sentido.

---

## Discussão

Após realizar as medições e os cálculos, responda às seguintes questões:

1. Qual eixo ficou mais alinhado à gravidade quando o CubEdu estava na posição horizontal?
2. O que aconteceu com o sinal das acelerações quando a inclinação foi invertida?
3. Qual eixo do giroscópio apresentou maior alteração durante a rotação sobre a mesa?
4. Por que o giroscópio apresenta pequenas leituras mesmo quando a plataforma está parada?
5. Por que o acelerômetro continua medindo aproximadamente `9,81 m/s²` quando o sistema está em repouso?
6. Qual é a diferença entre aceleração e velocidade angular?
7. Como um satélite pode utilizar informações de orientação durante uma missão?
8. Por que o MPU6050, sem auxílio de outro sensor, não fornece um rumo absoluto confiável?

---

## Conclusão

Nesta atividade, os alunos utilizaram conceitos de programação, movimentos, aceleração e rotação para compreender como uma plataforma pode identificar sua orientação. A partir dos dados do MPU6050, foi possível reconhecer os eixos do sistema, observar a influência da gravidade e estimar os ângulos de rolagem e arfagem do CubEdu.

Além disso, a atividade demonstrou que sensores reais apresentam ruídos e limitações, sendo necessário interpretar os dados antes de utilizá-los em sistemas de orientação.

---

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
