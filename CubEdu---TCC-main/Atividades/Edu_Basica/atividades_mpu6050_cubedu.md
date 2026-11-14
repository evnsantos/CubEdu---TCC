# Atividades Educacionais com o MPU6050 no CubEdu

Este material apresenta duas atividades práticas com o sensor inercial MPU6050 integrado à plataforma CubEdu. A primeira foi planejada para o Ensino Médio e introduz os conceitos de aceleração, rotação e orientação. A segunda foi planejada para o Ensino Superior e propõe o desenvolvimento de um algoritmo para estimar atitude, velocidade e trajetória a partir dos dados coletados pelo sensor.

> **Observação:** o MPU6050 possui acelerômetro e giroscópio de três eixos, mas não possui magnetômetro. Por esse motivo, a rolagem e a arfagem podem ser corrigidas com a direção da gravidade, enquanto a guinada obtida pela integração do giroscópio acumula deriva e não representa um rumo absoluto.

---

# Atividade 1 — Como um satélite identifica sua orientação?

## Público-alvo

Estudantes do Ensino Médio.

## Duração sugerida

Duas aulas de 50 minutos.

## Objetivo

Compreender como acelerômetros e giroscópios podem ser utilizados para identificar movimentos e estimar a orientação de uma plataforma, relacionando as medições do MPU6050 ao comportamento de um satélite.

## Objetivos específicos

Ao final da atividade, o estudante deverá ser capaz de:

- reconhecer os eixos X, Y e Z do MPU6050;
- diferenciar aceleração de velocidade angular;
- relacionar a inclinação do CubEdu aos ângulos de rolagem e arfagem;
- interpretar os sinais e as unidades apresentados pelo sensor;
- identificar ruídos e pequenas variações nas medições;
- compreender por que o MPU6050 não determina sozinho uma direção absoluta.

## Contextualização

Um satélite precisa conhecer sua orientação para apontar painéis solares, antenas, câmeras e outros instrumentos. Sensores inerciais permitem medir acelerações e rotações sem a necessidade de câmeras ou referências visuais externas.

Nesta atividade, o CubEdu representará um pequeno satélite. O MPU6050 será utilizado para observar como a plataforma responde a inclinações e rotações em torno de seus três eixos.

## Conceitos principais

### Acelerômetro

O acelerômetro mede a aceleração específica nos eixos X, Y e Z. Quando o CubEdu está parado, o sensor ainda identifica a ação aparente da gravidade. Dependendo da orientação da plataforma, um dos eixos apresentará valor próximo de `+9,81 m/s²` ou `-9,81 m/s²`.

### Giroscópio

O giroscópio mede a velocidade angular, normalmente em graus por segundo (`°/s`). Quando o CubEdu gira em torno de um eixo, a leitura correspondente aumenta ou diminui conforme o sentido da rotação.

### Ângulos de atitude

- **Roll ou rolagem (`φ`)**: rotação em torno do eixo X;
- **Pitch ou arfagem (`θ`)**: rotação em torno do eixo Y;
- **Yaw ou guinada (`ψ`)**: rotação em torno do eixo Z.

## Materiais necessários

- 1 plataforma CubEdu;
- 1 ESP8266;
- 1 sensor MPU6050;
- cabos de conexão;
- computador ou celular conectado à interface do CubEdu;
- superfície plana;
- apoio inclinado, régua ou transferidor, se disponível.

## Preparação

1. Posicione o CubEdu sobre uma superfície plana.
2. Ligue a plataforma e aguarde a inicialização do MPU6050.
3. Evite movimentar o conjunto durante os primeiros segundos, caso o firmware realize calibração inicial.
4. Verifique se estão disponíveis as leituras:
   - `Ax`, `Ay` e `Az`, em `m/s²`;
   - `Gx`, `Gy` e `Gz`, em `°/s`;
   - roll e pitch, em graus, quando calculados pelo firmware.

## Procedimento experimental

### Etapa 1 — Reconhecimento dos eixos

1. Mantenha o CubEdu nivelado e registre `Ax`, `Ay` e `Az`.
2. Incline a plataforma para a direita e observe qual leitura muda de forma predominante.
3. Incline a plataforma para a esquerda e compare o sinal da leitura.
4. Incline a plataforma para frente e para trás.
5. Gire a plataforma sobre a mesa e observe o eixo predominante do giroscópio.

### Etapa 2 — Teste de posições conhecidas

Coloque o CubEdu nas posições abaixo e registre os dados após a estabilização.

| Posição | Ax (m/s²) | Ay (m/s²) | Az (m/s²) | Roll (°) | Pitch (°) |
|---|---:|---:|---:|---:|---:|
| Horizontal |  |  |  |  |  |
| Inclinado para a direita |  |  |  |  |  |
| Inclinado para a esquerda |  |  |  |  |  |
| Inclinado para frente |  |  |  |  |  |
| Inclinado para trás |  |  |  |  |  |

### Etapa 3 — Movimento lento e movimento rápido

1. Incline o CubEdu lentamente até aproximadamente 30°.
2. Retorne à posição horizontal.
3. Repita o movimento de forma mais rápida.
4. Compare as leituras do acelerômetro e do giroscópio.

Durante movimentos rápidos, o acelerômetro mede a combinação entre a gravidade e a aceleração do movimento. Por isso, o ângulo calculado apenas com o acelerômetro pode apresentar perturbações.

### Etapa 4 — Simulação de uma manobra

Execute a sequência:

1. permaneça nivelado por 5 segundos;
2. incline o CubEdu para a direita;
3. mantenha a posição por 5 segundos;
4. retorne ao nível;
5. incline o CubEdu para frente;
6. mantenha a posição por 5 segundos;
7. retorne ao nível.

Descreva como roll e pitch se comportaram durante cada etapa.

## Resultados esperados

- Em repouso, as leituras do giroscópio devem permanecer próximas de zero, embora pequenas oscilações sejam normais.
- Quando a plataforma está parada, o módulo da aceleração deve ficar próximo da gravidade local:

```text
|a| = sqrt(Ax² + Ay² + Az²) ≈ 9,81 m/s²
```

- A inclinação lateral deve alterar principalmente o roll.
- A inclinação frontal deve alterar principalmente o pitch.
- Uma rotação sobre a mesa deve ser percebida principalmente no eixo Z do giroscópio.

## Questões para análise

1. Qual eixo ficou mais alinhado à gravidade quando o CubEdu estava horizontal?
2. O que aconteceu com o sinal da aceleração ao inverter a inclinação?
3. Por que o giroscópio apresentou pequenas leituras mesmo quando o sistema estava parado?
4. Qual sensor respondeu de forma mais evidente a uma rotação rápida?
5. Por que o acelerômetro pode apresentar um ângulo incorreto durante um movimento brusco?
6. Por que não é possível obter um rumo absoluto confiável usando somente o MPU6050?
7. Quais dispositivos de um satélite dependem do conhecimento da orientação?

## Desafio adicional

Crie uma regra para classificar automaticamente a orientação do CubEdu:

```text
Se roll > +20°: inclinado para a direita
Se roll < -20°: inclinado para a esquerda
Se pitch > +20°: inclinado para frente
Se pitch < -20°: inclinado para trás
Caso contrário: aproximadamente nivelado
```

Os sinais podem mudar conforme a orientação física do sensor. A turma deverá verificar experimentalmente a convenção adotada.

---

# Atividade 2 — Reconstrução de atitude e trajetória com dados do CubEdu

## Público-alvo

Estudantes do Ensino Superior, especialmente de Engenharia Aeroespacial, Mecatrônica, Eletrônica, Controle e Automação ou áreas relacionadas.

## Duração sugerida

De quatro a seis horas, divididas entre coleta, programação, processamento e análise.

## Objetivo

Desenvolver um programa capaz de importar dados do MPU6050 coletados pelo CubEdu, calibrar as medições, estimar a atitude por fusão sensorial e reconstruir uma trajetória aproximada no referencial local.

## Objetivos específicos

Ao final da atividade, o estudante deverá ser capaz de:

- interpretar dados brutos de uma unidade de medição inercial;
- estimar e corrigir os biases do acelerômetro e do giroscópio;
- calcular roll e pitch com dados do acelerômetro;
- integrar velocidades angulares;
- implementar um filtro complementar;
- construir uma matriz de rotação corpo para navegação;
- remover a contribuição da gravidade;
- integrar aceleração para obter velocidade e posição;
- avaliar deriva, ruído e limitações da navegação inercial de baixo custo.

## Problema proposto

O CubEdu realizará uma missão curta sobre uma trajetória conhecida. Os dados do MPU6050 deverão ser registrados e posteriormente processados por um programa desenvolvido pela equipe.

O programa deverá:

1. importar os dados do experimento;
2. estimar os biases dos sensores;
3. calcular a atitude da plataforma;
4. transformar a aceleração do sistema do corpo para um referencial local;
5. retirar a componente gravitacional;
6. calcular velocidade e deslocamento por integração numérica;
7. apresentar gráficos da atitude, velocidade e trajetória;
8. comparar a trajetória estimada com a trajetória de referência;
9. discutir as fontes de erro.

## Materiais necessários

- plataforma CubEdu com ESP8266 e MPU6050;
- computador para aquisição e processamento;
- fita métrica;
- fita adesiva para marcar a trajetória;
- cronômetro;
- carrinho, suporte deslizante ou plataforma que mantenha o CubEdu aproximadamente nivelado;
- GNU Octave, MATLAB ou Python com NumPy, Pandas e Matplotlib.

## Arquivo de dados

O firmware deverá registrar, no mínimo, as colunas abaixo:

```csv
tempo_ms,ax_ms2,ay_ms2,az_ms2,gx_degs,gy_degs,gz_degs
0,0.03,-0.05,9.79,0.12,-0.08,0.05
...
```

Recomenda-se uma frequência de aquisição entre **50 Hz e 100 Hz**. A frequência realmente obtida deve ser calculada a partir dos timestamps, sem assumir que o período entre todas as amostras é perfeitamente constante.

## Missão experimental

### Trajetória sugerida: percurso retangular

Marque no chão um retângulo de `2,0 m × 1,0 m`.

1. Mantenha o CubEdu parado por 10 segundos para calibração.
2. Desloque a plataforma por 2,0 m em linha reta.
3. Pare por 3 segundos.
4. Faça uma rotação próxima de 90°.
5. Desloque a plataforma por 1,0 m.
6. Repita o processo até retornar ao ponto inicial.
7. Mantenha o CubEdu parado por mais 10 segundos.

> Para reduzir perturbações, a plataforma deve permanecer aproximadamente nivelada. Movimentos manuais bruscos tornam a integração da aceleração ainda mais sensível a erros.

## Etapa 1 — Importação e validação dos dados

O programa deverá:

- ler o arquivo CSV;
- verificar valores ausentes ou duplicados;
- converter `tempo_ms` para segundos;
- calcular `dt` entre amostras consecutivas;
- apresentar média, desvio-padrão, mínimo e máximo de cada canal;
- mostrar a frequência média e a variação do período de amostragem.

Calcule:

```text
dt[k] = t[k] - t[k-1]
```

Amostras com `dt <= 0` devem ser identificadas e tratadas.

## Etapa 2 — Calibração estática

Utilize os primeiros 10 segundos, durante os quais o CubEdu permaneceu parado.

### Bias do giroscópio

```text
bgx = média(Gx_parado)
bgy = média(Gy_parado)
bgz = média(Gz_parado)
```

Correção:

```text
Gx_corr = Gx - bgx
Gy_corr = Gy - bgy
Gz_corr = Gz - bgz
```

### Bias do acelerômetro

A correção depende da orientação inicial. Se o eixo Z estiver alinhado à gravidade:

```text
bax = média(Ax_parado)
bay = média(Ay_parado)
baz = média(Az_parado) - g
```

Com `g = 9,80665 m/s²` como valor de referência:

```text
Ax_corr = Ax - bax
Ay_corr = Ay - bay
Az_corr = Az - baz
```

Antes de aplicar essa equação, a equipe deverá confirmar o sinal da gravidade e a orientação física dos eixos.

## Etapa 3 — Atitude pelo acelerômetro

Para uma convenção compatível com o código de referência, calcule:

```text
roll_acc  = atan2(-Ay, Az)
pitch_acc = atan2(Ax, sqrt(Ay² + Az²))
```

Os resultados podem ser convertidos de radianos para graus.

Essas estimativas são mais úteis em repouso ou baixa dinâmica, pois acelerações lineares também alteram os valores medidos.

## Etapa 4 — Atitude pelo giroscópio

Em uma implementação simplificada e para ângulos moderados:

```text
roll_gyro[k]  = roll[k-1]  + Gx_corr[k] × dt[k]
pitch_gyro[k] = pitch[k-1] + Gy_corr[k] × dt[k]
yaw_gyro[k]   = yaw[k-1]   + Gz_corr[k] × dt[k]
```

Para uma implementação mais completa, utilize a matriz cinemática dos ângulos de Euler:

```text
[φ_dot]   [1  sinφ tanθ   cosφ tanθ] [p]
[θ_dot] = [0  cosφ       -sinφ     ] [q]
[ψ_dot]   [0  sinφ/cosθ   cosφ/cosθ] [r]
```

A formulação possui singularidade quando `cos(θ)` se aproxima de zero. A missão deve evitar pitch próximo de `±90°`, ou a equipe deverá utilizar quatérnios.

## Etapa 5 — Filtro complementar

Combine a resposta rápida do giroscópio com a referência de baixa frequência do acelerômetro:

```text
roll[k]  = α × roll_gyro[k]  + (1 - α) × roll_acc[k]
pitch[k] = α × pitch_gyro[k] + (1 - α) × pitch_acc[k]
```

Teste pelo menos três valores:

```text
α = 0,90
α = 0,98
α = 0,995
```

A equipe deverá justificar a escolha final comparando ruído, atraso e deriva. A guinada será obtida somente pela integração do giroscópio e, portanto, acumulará erro.

## Etapa 6 — Transformação para o referencial local

Adote um referencial local definido pela equipe, por exemplo:

- eixo N: direção inicial do movimento;
- eixo E: direção lateral direita;
- eixo D: direção vertical para baixo.

Construa a matriz de rotação com a sequência 3-2-1:

```text
Cnb = Rz(yaw) × Ry(pitch) × Rx(roll)
```

Transforme a aceleração medida no corpo:

```text
a_nav = Cnb × a_corpo
```

Em seguida, retire a contribuição da gravidade conforme a convenção adotada. Para NED:

```text
a_linear = a_nav - [0, 0, g]ᵀ
```

A equipe deverá verificar os sinais colocando o sistema parado. Após a compensação, as três componentes da aceleração linear devem permanecer próximas de zero.

## Etapa 7 — Detecção de repouso e ZUPT

A dupla integração amplifica rapidamente pequenos erros. Para limitar a deriva, implemente uma correção ZUPT, sigla para *Zero-Velocity Update*.

Considere o CubEdu parado quando, por pelo menos 0,3 s:

```text
||a|| estiver entre 9,61 e 10,01 m/s²
```

E simultaneamente:

```text
||ω|| < 3 °/s
```

Quando a condição de repouso for satisfeita:

```text
velocidade = [0, 0, 0]
```

Os limites são valores iniciais e deverão ser ajustados a partir do ruído observado no sensor utilizado.

## Etapa 8 — Integração numérica

Utilize preferencialmente a regra do trapézio:

```text
v[k] = v[k-1] + 0,5 × (a[k] + a[k-1]) × dt[k]
```

```text
p[k] = p[k-1] + 0,5 × (v[k] + v[k-1]) × dt[k]
```

Compare o resultado com a integração retangular e discuta as diferenças.

## Etapa 9 — Resultados obrigatórios

O relatório ou notebook deverá apresentar:

1. gráficos de `Ax`, `Ay` e `Az` antes e depois da calibração;
2. gráficos de `Gx`, `Gy` e `Gz` antes e depois da calibração;
3. roll e pitch calculados pelo acelerômetro;
4. roll, pitch e yaw calculados pelo giroscópio;
5. roll e pitch obtidos pelo filtro complementar;
6. aceleração transformada para o referencial local;
7. velocidade estimada em cada eixo;
8. posição estimada em cada eixo;
9. trajetória 2D no plano horizontal;
10. comparação entre posição final estimada e posição final real;
11. erro de fechamento da trajetória;
12. análise das principais fontes de erro.

## Métricas de desempenho

### Erro de fechamento

Para uma trajetória que termina no ponto inicial:

```text
erro_fechamento = sqrt(x_final² + y_final²)
```

### Erro relativo ao percurso

Para o retângulo de `2,0 m × 1,0 m`, o percurso total é `6,0 m`:

```text
erro_relativo = erro_fechamento / 6,0 × 100%
```

### Erro de atitude em repouso

Compare os ângulos estimados no início e no fim:

```text
deriva_roll  = roll_final - roll_inicial
deriva_pitch = pitch_final - pitch_inicial
deriva_yaw   = yaw_final - yaw_inicial
```

## Estrutura mínima do programa

```text
1. Ler o arquivo CSV
2. Validar timestamps e amostras
3. Calcular dt
4. Estimar os biases no trecho inicial parado
5. Corrigir acelerômetro e giroscópio
6. Calcular roll e pitch pelo acelerômetro
7. Integrar as velocidades angulares
8. Aplicar o filtro complementar
9. Montar a matriz corpo → referencial local
10. Transformar a aceleração
11. Remover a gravidade
12. Detectar períodos de repouso
13. Aplicar ZUPT
14. Integrar aceleração e velocidade
15. Gerar gráficos e métricas
16. Exportar os resultados
```

## Tabela para registro dos resultados

| Parâmetro | Sem correção | Com calibração | Com filtro e ZUPT |
|---|---:|---:|---:|
| Erro de fechamento (m) |  |  |  |
| Erro relativo (%) |  |  |  |
| Deriva de roll (°) |  |  |  |
| Deriva de pitch (°) |  |  |  |
| Deriva de yaw (°) |  |  |  |
| Velocidade final (m/s) |  |  |  |

## Questões para discussão

1. Qual foi o bias identificado em cada eixo?
2. A frequência de amostragem permaneceu constante?
3. Como o valor de `α` alterou a resposta do filtro complementar?
4. O acelerômetro forneceu ângulos confiáveis durante acelerações lineares?
5. Quanto a guinada derivou durante o experimento?
6. Qual foi o efeito da compensação da gravidade sobre a trajetória?
7. Qual foi o efeito da ZUPT sobre a velocidade e o erro de fechamento?
8. A integração trapezoidal melhorou o resultado em relação à retangular?
9. Por que pequenos erros de atitude geram grandes erros de posição?
10. Quais sensores ou referências externas poderiam melhorar o sistema?

## Limitações esperadas

A trajetória estimada não deverá coincidir perfeitamente com a trajetória real. As principais causas são:

- bias residual do acelerômetro e do giroscópio;
- ruído de medição;
- variação do período de amostragem;
- desalinhamento físico entre o sensor e a estrutura;
- erros nos ângulos de atitude;
- compensação incompleta da gravidade;
- vibração e acelerações bruscas;
- deriva da guinada pela ausência de magnetômetro;
- acúmulo de erro produzido pela dupla integração.

A divergência não deve ser tratada apenas como falha. A análise da deriva é parte central da atividade e demonstra por que sistemas reais combinam IMU com GNSS, magnetômetros, câmeras, sensores solares ou outras referências externas.

## Entregáveis

Cada equipe deverá entregar:

- arquivo CSV original coletado pelo CubEdu;
- código-fonte comentado;
- gráficos solicitados;
- tabela com as métricas;
- representação da trajetória estimada e da trajetória real;
- análise técnica dos erros;
- proposta de melhoria do sistema.

## Critérios de avaliação

| Critério | Peso |
|---|---:|
| Aquisição e organização dos dados | 10% |
| Calibração dos sensores | 15% |
| Estimativa de atitude | 20% |
| Transformação de referenciais e compensação da gravidade | 20% |
| Integração, ZUPT e trajetória | 20% |
| Análise crítica dos resultados | 15% |

## Desafios avançados

As equipes que concluírem a atividade principal poderão implementar uma ou mais extensões:

1. substituir os ângulos de Euler por quatérnios;
2. usar filtro passa-baixas antes da integração;
3. desenvolver detecção de repouso adaptativa;
4. comparar filtro complementar e filtro de Kalman;
5. adicionar um magnetômetro para corrigir a guinada;
6. comparar a trajetória inercial com dados de GNSS;
7. utilizar a telemetria Wi-Fi do CubEdu para processar os dados em tempo real;
8. exportar os resultados em um novo CSV com atitude, velocidade e posição.

---

# Relação entre as duas atividades

A atividade do Ensino Médio apresenta os princípios físicos e a interpretação direta dos dados. A atividade do Ensino Superior utiliza os mesmos sinais para construir uma cadeia de navegação inercial:

```text
MPU6050
   ↓
Calibração
   ↓
Estimativa de atitude
   ↓
Rotação corpo → referencial local
   ↓
Compensação da gravidade
   ↓
Aceleração linear
   ↓
Integração
   ↓
Velocidade e trajetória aproximadas
```

Dessa forma, as duas propostas utilizam a mesma plataforma, mas apresentam níveis diferentes de abstração, programação e análise matemática.
