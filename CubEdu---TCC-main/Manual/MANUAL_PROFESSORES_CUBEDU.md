# Manual de Boas Práticas do CubEdu para Professores

## 1. Finalidade

Este documento orienta a preparação, condução e encerramento de atividades didáticas com o **CubEdu**, plataforma educacional baseada em CubeSat. O protótipo usa uma **NodeMCU V3 ESP8266MOD** como OBC, sensores de payload e ADS, servidor HTTP local e telemetria via Wi-Fi.

O firmware do repositório realiza leituras a cada 5 segundos, apresenta o estado dos sensores, disponibiliza um painel web e fornece dados em JSON.

## 2. Arquitetura de referência

| Subsistema | Elementos |
| --- | --- |
| OBC e TT&C | NodeMCU V3 ESP8266MOD, Wi-Fi em modo Access Point e servidor HTTP |
| Payload | DHT11, GUVA-S12SD, BH1750 e BMP180 |
| ADS | MPU6050 |
| EPS | Duas baterias em 2S1P e conversor LM2596 |

### Pinagem de referência

| Função | Pino da NodeMCU |
| --- | --- |
| DHT11 | D5 / GPIO14 |
| GUVA-S12SD | A0 |
| SDA | D2 / GPIO4 |
| SCL | D1 / GPIO5 |
| Entrada regulada | VIN |

O barramento I²C é compartilhado pelo BH1750, BMP180 e MPU6050 e opera, conforme a documentação do projeto, em 100 kHz.

## 3. Preparação do laboratório

Antes da aula:

1. Separe os kits e atribua um número de patrimônio ou identificação a cada conjunto.
2. Inspecione cabos, conectores, soldas, isolamento e polaridade.
3. Verifique se não existem fios desencapados ou parafusos soltos.
4. Teste cada NodeMCU com um cabo USB de dados conhecido.
5. Compile e envie previamente a versão do firmware usada na aula.
6. Confirme a criação da rede `CubEdu_TCC`, o painel web e pelo menos duas atualizações de telemetria.
7. Registre a versão do firmware, do pacote ESP8266 e das bibliotecas.
8. Mantenha um protótipo funcional de reserva e jumpers sobressalentes.
9. Disponibilize extintor e procedimentos institucionais adequados ao laboratório. Em atividades com células de lítio, use recipiente de armazenamento apropriado e mantenha supervisão contínua.

## 4. Configuração recomendada do computador

- Arduino IDE 2;
- pacote **esp8266 by ESP8266 Community**;
- placa selecionada: **NodeMCU 1.0 (ESP-12E Module)**;
- Monitor Serial: **115200 baud**;
- cabo USB com transmissão de dados;
- permissões para instalação de driver USB, quando necessário.

URL do Gerenciador de Placas:

```text
https://arduino.esp8266.com/stable/package_esp8266com_index.json
```

### Bibliotecas a validar

Confira os `#include` da versão atual do firmware antes da aula. A versão descrita no repositório utiliza módulos que normalmente requerem:

- DHT sensor library, da Adafruit;
- Adafruit Unified Sensor;
- BH1750;
- Adafruit BMP085 Library, para o BMP180;
- Adafruit MPU6050;
- Adafruit BusIO;
- `Wire`, `ESP8266WiFi` e `ESP8266WebServer`, fornecidas pelo core do ESP8266.

É recomendável manter um arquivo `VERSIONS.md` ou `libraries.txt` com versões testadas. Isso evita que uma atualização automática introduza incompatibilidades durante o semestre.

## 5. Validação elétrica do EPS

### 5.1 Antes de conectar a OBC

1. Desconecte a saída do LM2596 da NodeMCU.
2. Energize o conversor com o conjunto de baterias.
3. Meça a saída com multímetro.
4. Ajuste o LM2596 para a tensão definida no projeto antes de conectá-lo ao pino VIN. Para uma NodeMCU alimentada pelo VIN, adote o valor validado na placa específica, normalmente 5,0 V no protótipo, sem exceder os limites do fabricante da versão utilizada.
5. Desenergize o conversor.
6. Confira novamente a polaridade.
7. Somente então conecte a saída regulada ao VIN e ao GND.

Não permita que estudantes ajustem o LM2596 energizado e conectado à OBC.

### 5.2 Uso simultâneo de USB e alimentação externa

Como existem diferentes clones de NodeMCU e diferentes circuitos de proteção, adote como procedimento padrão:

- para programar, desconecte a alimentação externa e use apenas USB;
- para o ensaio de autonomia, retire o USB e use somente o EPS;
- só permita alimentação simultânea se o modelo específico tiver seu caminho de potência verificado eletricamente e documentado.

Essa regra reduz o risco de corrente reversa entre o computador e o regulador externo.

### 5.3 Baterias

- Use duas células equivalentes, de mesma química, capacidade, estado de carga e histórico.
- Não misture células novas e usadas.
- Inspecione deformação, aquecimento, perfuração e danos no isolamento.
- Não carregue as células pelo CubEdu, pois o protótipo não possui carregador integrado.
- Retire as baterias para carregamento em carregador apropriado para a química e a configuração utilizadas.
- Não deixe células carregando sem supervisão.
- Não permita curto-circuito, inversão de polaridade ou contato com objetos metálicos.
- Interrompa o uso em caso de inchaço, aquecimento anormal, odor ou dano mecânico.
- Se o conjunto for realmente 2S, recomenda-se proteção/BMS 2S e carregamento balanceado. A ausência desses itens deve ser explicitamente tratada na análise de risco e no procedimento do laboratório.

## 6. Verificação da montagem

Use esta sequência antes de energizar:

- [ ] GND comum entre OBC, sensores e fonte;
- [ ] DHT11 em D5 / GPIO14;
- [ ] GUVA-S12SD em A0;
- [ ] SDA em D2 / GPIO4;
- [ ] SCL em D1 / GPIO5;
- [ ] tensão de alimentação compatível com cada módulo;
- [ ] polaridade do LM2596 confirmada;
- [ ] saída do LM2596 medida;
- [ ] ausência de curto entre alimentação e GND;
- [ ] jumpers firmes e sem partes condutoras expostas;
- [ ] bateria desconectada durante a programação USB.

A entrada A0 merece atenção: o limite elétrico depende da implementação da placa NodeMCU e do divisor presente no módulo. Valide a faixa de saída do GUVA-S12SD para a placa usada, sem assumir que todo clone aceita o mesmo valor.

## 7. Sequência didática sugerida

Para reduzir falhas e facilitar o diagnóstico, conduza a integração em blocos:

1. **OBC isolada:** energia por USB, Blink, Monitor Serial e identificação da porta.
2. **Barramento I²C:** execute um scanner I²C e registre os endereços detectados.
3. **Sensores individuais:** valide um sensor por vez.
4. **OBC + sensores:** integre BH1750, BMP180 e MPU6050 no mesmo barramento.
5. **Entradas dedicadas:** adicione DHT11 e GUVA-S12SD.
6. **TT&C:** valide a rede Wi-Fi, o endereço IP e o acesso ao painel.
7. **EPS:** conecte o LM2596 previamente ajustado e realize o teste sem USB.
8. **Sistema completo:** execute aquisição, dashboard e registro de falhas.
9. **Autonomia:** realize somente após validações funcionais e inspeção das baterias.

Essa ordem preserva a metodologia de testar cada bloco separadamente antes da integração.

## 8. Envio e teste do firmware

1. Abra `Firmware_V1_Op.ino`.
2. Confirme a placa **NodeMCU 1.0 (ESP-12E Module)**.
3. Selecione a porta correta.
4. Compile antes de conectar o protótipo completo.
5. Envie o firmware usando somente a alimentação USB.
6. Abra o Monitor Serial em 115200 baud.
7. Registre mensagens de inicialização, IP, sensores detectados, memória livre e reinicializações.
8. Aguarde pelo menos duas janelas de atualização, equivalentes a aproximadamente 10 segundos.
9. Conecte um dispositivo à rede `CubEdu_TCC`.
10. Acesse o endereço exibido no Monitor Serial. O IP padrão de um ESP8266 em Access Point costuma ser `192.168.4.1`, mas o valor impresso pelo firmware deve prevalecer.
11. Verifique o painel e, se aplicável, o endpoint JSON informado pelo firmware.

## 9. Critérios mínimos de aceitação

Considere o kit apto para aula quando:

- compilar sem erros;
- realizar upload sem reinicializações repetitivas;
- criar a rede Wi-Fi em até 30 segundos;
- permitir acesso ao painel por pelo menos um dispositivo;
- atualizar a telemetria em intervalos próximos de 5 segundos;
- identificar corretamente os sensores presentes;
- manter leituras sem `NaN` persistente, valores congelados ou saltos incompatíveis;
- operar por pelo menos 15 minutos no ensaio preliminar sem aquecimento anormal;
- não apresentar queda excessiva da tensão regulada sob carga.

Para ensaio de autonomia, use o requisito definido no TCC e registre horário inicial, horário final, tensão inicial/final e causa do encerramento.

## 10. Diagnóstico de falhas

### Porta serial ausente

- testar outro cabo USB de dados;
- testar outra porta USB;
- verificar o Gerenciador de Dispositivos;
- identificar o conversor USB-serial do clone, frequentemente CH340 ou CP210x, e instalar o driver oficial correspondente;
- evitar hubs USB sem alimentação.

### Erro de compilação

- conferir a placa selecionada;
- copiar o erro completo;
- verificar bibliotecas duplicadas;
- validar nomes e versões das bibliotecas;
- apagar instalações conflitantes apenas após registrar a configuração funcional.

### Erro no upload

- fechar o Monitor Serial;
- confirmar a porta;
- pressionar `RST`;
- retirar periféricos que possam afetar pinos de boot;
- testar a NodeMCU isolada.

### Sensores I²C ausentes

- desligar a alimentação;
- conferir SDA e SCL;
- verificar GND comum;
- executar scanner I²C;
- testar um módulo por vez;
- procurar endereços duplicados e pull-ups excessivos;
- manter fios curtos em montagem didática.

### Reinicializações ou mensagens de watchdog

- verificar queda de tensão na alimentação;
- medir a saída do LM2596 durante transmissão Wi-Fi;
- procurar curtos intermitentes;
- testar sem os sensores;
- registrar a causa de reset exibida no terminal;
- confirmar que o firmware não introduziu operações bloqueantes longas.

### Rede criada, mas página indisponível

- confirmar o IP no Monitor Serial;
- desativar temporariamente VPN, proxy e dados móveis;
- testar outro navegador;
- verificar se o dispositivo permaneceu conectado à rede sem Internet;
- testar o endpoint JSON separadamente.

## 11. Gestão de turma e rastreabilidade

Para cada kit, mantenha uma ficha com:

- identificação do kit;
- versão do hardware;
- hash ou tag do firmware;
- versões do core e bibliotecas;
- tensão ajustada do LM2596;
- identificação e capacidade nominal das baterias;
- data da inspeção;
- falhas observadas;
- componentes substituídos;
- responsável pela verificação.

Oriente os alunos a não modificar simultaneamente hardware e firmware. A cada teste, altere uma variável e registre o resultado.

## 12. Boas práticas de software

- Preserve uma versão estável do firmware em uma tag, por exemplo `v1.0-aula`.
- Mantenha credenciais e parâmetros didáticos em uma seção claramente identificada.
- Não publique senhas pessoais ou institucionais.
- Use nomes de arquivos sem espaços e uma pasta com o mesmo nome do arquivo `.ino` quando exigido pelo Arduino IDE.
- Registre dependências e versões.
- Inclua licença de software e de documentação no repositório.
- Mantenha exemplos menores para testes individuais dos sensores.
- Use Issues do GitHub para registrar defeitos reproduzíveis.

## 13. Encerramento e armazenamento

1. Desconecte a fonte externa.
2. Retire o USB.
3. Remova as baterias do protótipo, se o projeto mecânico permitir.
4. Inspecione aquecimento e danos.
5. Armazene as células de acordo com a política institucional e as recomendações do fabricante.
6. Guarde kits e cabos identificados.
7. Registre qualquer falha antes de devolver o kit ao estoque.

## 14. Checklist rápido do professor

### Antes da aula

- [ ] firmware compilado e testado;
- [ ] bibliotecas e pacote ESP8266 disponíveis;
- [ ] baterias inspecionadas;
- [ ] LM2596 ajustado e lacrado contra alteração acidental;
- [ ] cabo de dados validado;
- [ ] kit reserva disponível;
- [ ] procedimento de emergência conhecido.

### Durante a aula

- [ ] montagem sempre desenergizada;
- [ ] supervisão na conexão do EPS;
- [ ] nenhuma bateria sendo carregada no protótipo;
- [ ] alterações realizadas uma por vez;
- [ ] resultados registrados.

### Após a aula

- [ ] alimentação removida;
- [ ] baterias armazenadas corretamente;
- [ ] danos e falhas registrados;
- [ ] arquivos e resultados salvos;
- [ ] kits conferidos.
