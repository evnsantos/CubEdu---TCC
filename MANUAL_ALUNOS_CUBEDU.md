# Manual de Montagem e Uso do CubEdu para Alunos

## 1. Objetivo

Este manual apresenta um passo a passo simples para montar, conectar e testar o protótipo educacional **CubEdu**, baseado na placa **NodeMCU V3 ESP8266MOD**.

O sistema lê os sensores, processa os dados e cria uma rede Wi-Fi local. Após a conexão, as medições podem ser visualizadas em uma página aberta no navegador, sem necessidade de Internet.

## 2. Materiais principais

- 1 NodeMCU V3 ESP8266MOD;
- 1 sensor DHT11;
- 1 sensor GUVA-S12SD;
- 1 sensor BH1750;
- 1 sensor BMP180;
- 1 sensor MPU6050;
- 1 conversor DC-DC LM2596, previamente ajustado pelo professor;
- 2 baterias LiPo/Li-ion na configuração 2S1P, montadas e supervisionadas pelo professor;
- cabos jumper e estrutura do CubEdu;
- cabo USB de dados compatível com a NodeMCU;
- computador com Arduino IDE.

> **Importante:** alunos não devem montar, carregar, desmontar ou alterar o conjunto de baterias sem supervisão.

## 3. Cuidados antes de começar

1. Trabalhe sobre uma mesa limpa, seca e sem objetos metálicos soltos.
2. Desligue todas as fontes de energia antes de alterar os fios.
3. Não conecte nem retire sensores com o CubEdu energizado.
4. Confira três vezes os pinos `VCC`, `GND`, `SDA` e `SCL`.
5. Nunca una diretamente `VCC` e `GND`.
6. Não altere o parafuso de ajuste do LM2596.
7. Durante a programação por USB, mantenha a alimentação pelas baterias desconectada, salvo autorização expressa do professor.
8. Se houver aquecimento, cheiro, fumaça ou comportamento incomum, desconecte a alimentação e avise o professor.

## 4. Instalação do Arduino IDE

1. Acesse a página oficial: <https://www.arduino.cc/en/software>.
2. Baixe e instale o **Arduino IDE 2** para o sistema operacional do computador.
3. Abra o Arduino IDE.
4. Entre em **Arquivo > Preferências**.
5. Em **URLs adicionais para Gerenciadores de Placas**, adicione:

```text
https://arduino.esp8266.com/stable/package_esp8266com_index.json
```

6. Abra o **Gerenciador de Placas**.
7. Pesquise por `esp8266`.
8. Instale **esp8266 by ESP8266 Community**.
9. Em **Ferramentas > Placa**, selecione **NodeMCU 1.0 (ESP-12E Module)**.

## 5. Instalação das bibliotecas

No Arduino IDE, abra o **Gerenciador de Bibliotecas** e instale as bibliotecas exigidas pelo arquivo `Firmware_V1_Op.ino`:

- DHT sensor library, da Adafruit;
- Adafruit Unified Sensor;
- BH1750;
- Adafruit BMP085 Library, compatível com BMP180;
- Adafruit MPU6050;
- Adafruit BusIO.

As bibliotecas `Wire`, `ESP8266WiFi` e `ESP8266WebServer` acompanham o pacote da placa ESP8266.

## 6. Ligações do protótipo

Faça as ligações apenas com o sistema desligado.

| Componente | Ligação na NodeMCU | Tipo |
| --- | --- | --- |
| DHT11 | D5 / GPIO14 | Digital |
| GUVA-S12SD | A0 | Analógica |
| BH1750 | SDA em D2 e SCL em D1 | I²C |
| BMP180 | SDA em D2 e SCL em D1 | I²C |
| MPU6050 | SDA em D2 e SCL em D1 | I²C |
| LM2596 | VIN, somente após ajuste pelo professor | Alimentação |

### Barramento I²C

- `SDA` → D2 / GPIO4;
- `SCL` → D1 / GPIO5;
- todos os módulos devem compartilhar o `GND`;
- siga a tensão de alimentação definida na montagem entregue pelo professor.

## 7. Abrir e enviar o firmware

1. Baixe o repositório do CubEdu ou o arquivo `Firmware_V1_Op.ino`.
2. Abra o arquivo no Arduino IDE.
3. Conecte somente a NodeMCU ao computador com um cabo USB de dados.
4. Em **Ferramentas > Porta**, escolha a porta que apareceu após conectar a placa.
5. Confirme a placa **NodeMCU 1.0 (ESP-12E Module)**.
6. Clique em **Verificar**.
7. Se a compilação terminar sem erros, clique em **Carregar**.
8. Aguarde a mensagem de conclusão antes de retirar o cabo.

## 8. Primeiro teste

1. Abra o **Monitor Serial**.
2. Configure a velocidade para **115200 baud**.
3. Pressione o botão `RST` da NodeMCU, se necessário.
4. Observe as mensagens de inicialização e o estado de cada sensor.
5. Aguarde pelo menos 5 segundos para a primeira atualização completa.

Se um sensor aparecer como indisponível:

- desligue o sistema;
- confira `VCC` e `GND`;
- confira `SDA` em D2 e `SCL` em D1;
- verifique se os jumpers estão firmes;
- peça ajuda antes de trocar fios.

## 9. Acesso ao painel do CubEdu

1. Após a inicialização, procure no computador ou celular a rede Wi-Fi **CubEdu_TCC**.
2. Conecte-se à rede usando as informações apresentadas pelo professor ou pelo Monitor Serial.
3. Desative temporariamente os dados móveis se o celular insistir em trocar de rede.
4. Abra o endereço informado no Monitor Serial no navegador. Em montagens ESP8266 no modo Access Point, o endereço normalmente é `192.168.4.1`, mas confirme no terminal.
5. Observe a atualização da telemetria a cada 5 segundos.

## 10. Encerramento da atividade

1. Registre as medições solicitadas pelo professor.
2. Feche o Monitor Serial e o Arduino IDE.
3. Desconecte primeiro a alimentação externa.
4. Depois, retire o cabo USB segurando pelo conector, nunca pelo fio.
5. Não guarde o protótipo com fios soltos ou baterias conectadas.
6. Informe qualquer falha observada.

## 11. Problemas comuns

| Problema | Verificação rápida |
| --- | --- |
| A porta não aparece | Troque o cabo USB por um cabo de dados e reconecte a placa |
| Erro de compilação | Confira se a placa ESP8266 e todas as bibliotecas foram instaladas |
| Erro no envio | Feche o Monitor Serial, confirme a porta e pressione `RST` |
| Rede Wi-Fi não aparece | Reinicie a placa e leia a mensagem no Monitor Serial |
| Sensor indisponível | Desligue e confira alimentação e sinais do sensor |
| Página não abre | Confirme a conexão à rede CubEdu_TCC e o endereço mostrado no terminal |

## 12. Regra principal

> Sempre altere a montagem com o sistema desligado. Em caso de dúvida, pare a atividade e chame o professor.
