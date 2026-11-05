# CubEdu

Firmware desenvolvido para um protótipo educacional de CubeSat, responsável pela aquisição de dados dos sensores, processamento pela OBC e disponibilização da telemetria por meio de uma rede Wi-Fi local.

O projeto foi desenvolvido como parte do Trabalho de Conclusão de Curso de **Gabriel Santos Evangelista**, na Universidade de Brasília, Faculdade de Ciências e Tecnologias em Engenharia.

## Visão geral

O CubEdu utiliza uma placa NodeMCU V3 baseada no ESP8266 como computador de bordo, ou OBC. A placa realiza a leitura dos sensores do Payload e do subsistema de determinação de atitude, organiza os dados de telemetria e cria uma rede Wi-Fi em modo Access Point.

Após se conectar à rede criada pelo ESP8266, o usuário pode acessar uma interface web hospedada no próprio microcontrolador. A página apresenta as leituras e o estado dos componentes, sem necessidade de conexão com a Internet.

O firmware também disponibiliza os dados em formato JSON para acesso por outras aplicações.

## Funcionalidades

- Leitura periódica dos sensores
- Comunicação I²C entre a OBC e os módulos digitais
- Leitura analógica do sensor GUVA-S12SD
- Cálculo da temperatura média do protótipo
- Cálculo de roll e pitch a partir do acelerômetro
- Criação de uma rede Wi-Fi local
- Servidor HTTP executado no ESP8266
- Dashboard para visualização da telemetria
- API de telemetria em formato JSON
- Impressão dos dados no Monitor Serial
- Atualização dos dados a cada 5 segundos
- Indicação do estado dos sensores
- Monitoramento da memória livre da OBC
- Contagem de pacotes de telemetria
- Exibição do tempo de funcionamento
- Contagem de dispositivos conectados à rede

## Arquitetura do sistema

O projeto está organizado nos seguintes subsistemas:

### OBC

A placa NodeMCU V3 ESP8266MOD funciona como computador de bordo. A OBC inicializa os sensores, coleta as medições, processa os dados e executa o servidor web.

### Payload

O Payload é composto pelos seguintes sensores:

- DHT11, para temperatura e umidade relativa
- GUVA-S12SD, para estimativa da radiação ultravioleta
- BH1750, para medição de iluminância
- BMP180, para temperatura, pressão atmosférica e altitude barométrica

### ADS

O subsistema de determinação de atitude utiliza o MPU6050 para obtenção de:

- aceleração nos eixos X, Y e Z;
- velocidade angular nos eixos X, Y e Z;
- temperatura interna do circuito integrado;
- estimativa dos ângulos de roll e pitch.

### TT&C

O subsistema de telecomunicação, rastreamento e comando utiliza o Wi-Fi integrado ao ESP8266.

A placa funciona em modo Access Point e cria uma rede local. Um computador ou dispositivo móvel pode se conectar diretamente à OBC e acessar a página de telemetria.

### EPS

A alimentação do protótipo utiliza:

- duas baterias LiPo em configuração 2S1P;
- conversor DC-DC LM2596;
- saída regulada conectada ao pino VIN da NodeMCU.

O firmware apresenta a configuração do EPS no dashboard, mas não realiza medição digital da tensão ou da corrente das baterias.

## Componentes utilizados

| Subsistema | Componente | Função |
|---|---|---|
| OBC e TT&C | NodeMCU V3 ESP8266MOD | Processamento, Wi-Fi e servidor web |
| Payload | DHT11 | Temperatura e umidade |
| Payload | GUVA-S12SD | Estimativa de radiação UV |
| Payload | BH1750 | Iluminância |
| Payload | BMP180 | Temperatura, pressão e altitude |
| ADS | MPU6050 | Aceleração, velocidade angular e atitude |
| EPS | LM2596 | Regulação da tensão de alimentação |
| EPS | Baterias LiPo 2S1P | Armazenamento de energia |

## Conexões

| Componente | Pino da NodeMCU | Interface |
|---|---|---|
| DHT11 | D5 / GPIO14 | Digital |
| GUVA-S12SD | A0 | Analógica |
| BH1750 | D2 e D1 | I²C |
| BMP180 | D2 e D1 | I²C |
| MPU6050 | D2 e D1 | I²C |
| LM2596 | VIN | Alimentação |

### Barramento I²C

| Sinal | Pino da NodeMCU |
|---|---|
| SDA | D2 / GPIO4 |
| SCL | D1 / GPIO5 |

O barramento I²C é configurado para operar em 100 kHz. Os sensores BH1750, BMP180 e MPU6050 compartilham os mesmos sinais SDA e SCL.

## Ambiente de desenvolvimento

O firmware foi desenvolvido utilizando:

- Arduino IDE
- Placa NodeMCU 1.0, ESP-12E Module
- Framework Arduino para ESP8266
- Monitor Serial configurado em 115200 baud
