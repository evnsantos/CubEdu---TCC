# Instalação e configuração

Este documento apresenta os procedimentos necessários para preparar o ambiente de desenvolvimento, compilar o firmware e gravá-lo na placa NodeMCU ESP8266 utilizada no projeto CubEdu.

## 1. Requisitos

Para executar o procedimento, são necessários:

- computador com porta USB;
- Arduino IDE;
- placa NodeMCU V3 ESP8266MOD;
- cabo micro-USB com suporte à transmissão de dados;
- firmware do projeto CubEdu;
- acesso à Internet para instalação inicial das bibliotecas;
- sensores utilizados no protótipo.

> Alguns cabos micro-USB realizam apenas alimentação. Caso a placa não seja reconhecida pelo computador, teste outro cabo com suporte à transmissão de dados.

## 2. Ambiente de desenvolvimento

O firmware foi desenvolvido utilizando:

- Arduino IDE;
- placa NodeMCU 1.0, ESP-12E Module;
- framework Arduino para ESP8266;
- Monitor Serial configurado em 115200 baud.

## 3. Instalação da Arduino IDE

1. Baixe e instale a Arduino IDE.
2. Execute a Arduino IDE após a instalação.
3. Aguarde a inicialização completa do programa.

A versão utilizada deve possuir suporte ao Gerenciador de Placas e ao Gerenciador de Bibliotecas.

## 4. Instalação do pacote ESP8266

Para programar a NodeMCU, é necessário instalar o pacote de placas ESP8266.

Na Arduino IDE:

1. Abra o menu `Arquivo`.
2. Acesse `Preferências`.
3. Localize o campo de URLs adicionais para o Gerenciador de Placas.
4. Adicione a URL do pacote ESP8266.
5. Confirme a alteração.
6. Abra o menu `Ferramentas`.
7. Acesse `Placa`.
8. Abra o `Gerenciador de Placas`.
9. Pesquise por:

```text
esp8266
```

10. Instale o pacote denominado:

```text
esp8266 by ESP8266 Community
```

11. Aguarde a conclusão da instalação.

## 5. Seleção da placa

Após instalar o pacote ESP8266:

1. Abra o menu `Ferramentas`.
2. Acesse `Placa`.
3. Selecione a família de placas ESP8266.
4. Escolha:

```text
NodeMCU 1.0 (ESP-12E Module)
```

Essa opção é compatível com a NodeMCU V3 ESP8266MOD utilizada no projeto.

## 6. Bibliotecas necessárias

As seguintes bibliotecas são utilizadas pelo firmware:

- `Wire`;
- `ESP8266WiFi`;
- `ESP8266WebServer`;
- `DHT sensor library`;
- `Adafruit Unified Sensor`;
- `BH1750`;
- `Adafruit MPU6050`;
- `Adafruit BMP085 Library`.

As bibliotecas `Wire`, `ESP8266WiFi` e `ESP8266WebServer` são disponibilizadas com o ambiente e o pacote ESP8266.

As demais bibliotecas podem ser instaladas pelo Gerenciador de Bibliotecas da Arduino IDE.

## 7. Instalação das bibliotecas

Na Arduino IDE:

1. Abra o menu `Ferramentas`.
2. Selecione `Gerenciar Bibliotecas`.
3. Pesquise e instale individualmente cada biblioteca.

### 7.1 DHT11

Pesquise por:

```text
DHT sensor library
```

Instale:

```text
DHT sensor library by Adafruit
```

### 7.2 Biblioteca unificada de sensores

Pesquise por:

```text
Adafruit Unified Sensor
```

Instale:

```text
Adafruit Unified Sensor by Adafruit
```

Essa biblioteca é uma dependência utilizada por diferentes sensores do projeto.

### 7.3 BH1750

Pesquise por:

```text
BH1750
```

Instale:

```text
BH1750 by Christopher Laws
```

### 7.4 MPU6050

Pesquise por:

```text
Adafruit MPU6050
```

Instale:

```text
Adafruit MPU6050 by Adafruit
```

Caso a Arduino IDE solicite a instalação de dependências adicionais, confirme a instalação de todas.

### 7.5 BMP180

Pesquise por:

```text
Adafruit BMP085 Library
```

Instale:

```text
Adafruit BMP085 Library by Adafruit
```

Apesar do nome, essa biblioteca também oferece suporte ao sensor BMP180 utilizado no projeto.

## 8. Abertura do firmware

1. Faça o download ou clone o repositório do CubEdu.
2. Localize a pasta do firmware final.
3. Abra o arquivo com extensão `.ino`.
4. Verifique se o nome da pasta é igual ao nome do arquivo principal.

Exemplo:

```text
firmware_final/
└── firmware_final.ino
```

A Arduino IDE pode solicitar a criação de uma pasta quando o arquivo `.ino` estiver isolado. Nesse caso, permita que o programa organize o arquivo.

## 9. Configuração da rede Wi-Fi

No início do firmware, localize:

```cpp
const char* WIFI_SSID = "CubEdu_TCC";
const char* WIFI_PASSWORD = "Teste2026";
```

O nome e a senha podem ser alterados antes da gravação.

A senha deve possuir pelo menos 8 caracteres para que o ESP8266 consiga criar uma rede protegida.

Se o repositório for público, evite armazenar senhas pessoais ou reutilizadas em outros serviços.

## 10. Conexão da NodeMCU

1. Conecte a NodeMCU ao computador utilizando um cabo micro-USB.
2. Aguarde o reconhecimento da placa.
3. Na Arduino IDE, abra o menu `Ferramentas`.
4. Acesse `Porta`.
5. Selecione a porta correspondente à NodeMCU.

No Windows, a porta normalmente é apresentada como `COM3`, `COM4` ou semelhante. No Linux, pode aparecer como `/dev/ttyUSB0`.

Se nenhuma porta aparecer:

- teste outro cabo micro-USB;
- teste outra porta USB do computador;
- reinicie a Arduino IDE;
- instale o driver correspondente ao conversor CH340 ou CP2102, conforme o modelo da placa.

## 11. Configurações recomendadas

No menu `Ferramentas`, utilize:

```text
Placa: NodeMCU 1.0 (ESP-12E Module)
Upload Speed: 115200
CPU Frequency: 80 MHz
Flash Size: 4 MB
Porta: porta correspondente à NodeMCU
```

Outras opções podem ser mantidas nos valores padrão fornecidos pelo pacote ESP8266.

## 12. Compilação e gravação

1. Clique no botão `Verificar` e aguarde a compilação.
2. Confirme a placa e a porta serial selecionadas.
3. Clique no botão `Carregar`.
4. Aguarde a compilação e a transferência.
5. Não desconecte o cabo durante o processo.

Erros relacionados a arquivos `.h` geralmente indicam que alguma biblioteca não foi instalada.

## 13. Monitor Serial

Após a gravação:

1. Abra o Monitor Serial.
2. Configure a velocidade para `115200 baud`.
3. Pressione o botão `RST` da NodeMCU.

A inicialização deverá apresentar informações semelhantes a:

```text
CubEdu
Trabalho de Conclusao de Curso

Iniciando Wi-Fi...
Rede Wi-Fi criada.
SSID: CubEdu_TCC
IP: 192.168.4.1
Servidor HTTP iniciado.
Sistema inicializado.
```

## 14. Conexão e acesso ao dashboard

1. Abra as configurações de Wi-Fi do computador ou celular.
2. Conecte-se à rede `CubEdu_TCC` com a senha definida no firmware.
3. Ignore o aviso de rede sem Internet, pois a comunicação é local.
4. Abra no navegador:

```text
http://192.168.4.1
```

Os dados são atualizados a cada 5 segundos.

## 15. Acesso à API JSON

A telemetria também pode ser consultada em:

```text
http://192.168.4.1/api/telemetry
```

## 16. Ligações do hardware

| Componente | Pino da NodeMCU | Interface |
|---|---|---|
| DHT11 | D5 / GPIO14 | Digital |
| GUVA-S12SD | A0 | Analógica |
| BH1750 | D2 e D1 | I²C |
| MPU6050 | D2 e D1 | I²C |
| BMP180 | D2 e D1 | I²C |
| LM2596 | VIN | Alimentação |

### Barramento I²C

| Sinal | Pino |
|---|---|
| SDA | D2 / GPIO4 |
| SCL | D1 / GPIO5 |

## 17. Ordem recomendada para os testes

1. Testar somente a NodeMCU pelo cabo USB.
2. Gravar o código básico de Wi-Fi.
3. Verificar a criação da rede.
4. Testar os sensores sem o Wi-Fi.
5. Conectar um sensor por vez.
6. Verificar as leituras no Monitor Serial.
7. Gravar o firmware integrado.
8. Acessar o dashboard.
9. Verificar a API JSON.
10. Realizar o ensaio com alimentação pelo EPS.

## 18. Alimentação do sistema

Durante a programação, a NodeMCU pode ser alimentada pela porta micro-USB.

Na configuração final, o sistema utiliza duas baterias LiPo em configuração 2S1P e um conversor LM2596 com saída regulada conectada ao pino VIN.

Antes de conectar o LM2596:

1. ligue o conversor às baterias;
2. meça a saída com um multímetro;
3. ajuste a saída para aproximadamente 5 V;
4. desligue a alimentação;
5. conecte a saída ao VIN e ao GND da NodeMCU;
6. ligue novamente o sistema.

Não conecte a saída das baterias diretamente ao pino de 3,3 V.

## 19. Solução de problemas

### Rede Wi-Fi não é criada

Verifique:

- se a senha possui pelo menos 8 caracteres;
- se o modo `WIFI_AP` está configurado;
- se a placa foi reiniciada após a gravação;
- se a alimentação está estável.

### Página não abre

Verifique:

- se o dispositivo está conectado à rede `CubEdu_TCC`;
- se o endereço utilizado é `192.168.4.1`;
- se os dados móveis do celular estão interferindo na conexão;
- se o servidor foi iniciado no Monitor Serial.

### Sensor aparece offline

Verifique:

- alimentação e GND do sensor;
- fios SDA e SCL;
- pinos D2 e D1;
- compatibilidade da tensão do módulo;
- instalação da biblioteca correta.

### Erro ao gravar o firmware

Verifique:

- placa selecionada;
- porta serial;
- cabo USB com suporte a dados;
- driver CH340 ou CP2102;
- velocidade de upload;
- disponibilidade da porta serial.

## 20. Conclusão da instalação

A instalação estará concluída quando:

- o firmware for gravado sem erros;
- a rede `CubEdu_TCC` estiver disponível;
- o endereço `192.168.4.1` abrir o dashboard;
- os sensores apresentarem valores;
- a API JSON responder;
- o Monitor Serial atualizar os dados a cada 5 segundos.
