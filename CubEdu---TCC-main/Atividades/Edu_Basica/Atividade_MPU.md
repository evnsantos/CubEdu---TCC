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

### Etapa 3 - Integração OBC e ADS

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

