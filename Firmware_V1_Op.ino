/*
  ============================================================
  
  ============================================================
  Titulo: Firmware V1 
  Autor: Gabriel Santos Evangelista
  Data: Segundo Semestre de 2026
  Descricao: Codigo completo de coleta, transmissao e vizualização dos dados. 
  Criado para ter uma interface amigavel na WEB.
  Nota: UI e UX desenvolvidas com auxilio de IA.
  ============================================================

  CONEXOES:
    DHT11       -> D5 / GPIO14
    GUVA-S12SD  -> A0
    BH1750      -> I2C
    BMP180      -> I2C
    MPU6050     -> I2C
    NodeMCU V3 ESP8266MOD
    Comunicacao Wi-Fi em modo Access Point

  BARRAMENTO I2C:
    SDA -> D2 / GPIO4
    SCL -> D1 / GPIO5

  REDE WI-FI:
    SSID: CubEdu_TCC
    Senha: Teste2026

  DASHBOARD:
    http://192.168.4.1

  API JSON:
    http://192.168.4.1/api/telemetry

  ATUALIZACAO:
    A cada 5 segundos
  ============================================================
*/


#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <BH1750.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_MPU6050.h>

#define I2C_SDA D2
#define I2C_SCL D1

#define DHT_PIN D5
#define DHT_TYPE DHT11

#define GUVA_PIN A0
#define STATUS_LED LED_BUILTIN

DHT dht(DHT_PIN, DHT_TYPE);
BH1750 lightMeter;
Adafruit_BMP085 bmp;
Adafruit_MPU6050 mpu;

ESP8266WebServer server(80);

const char* WIFI_SSID = "CubEdu_TCC";
const char* WIFI_PASSWORD = "Teste2026";

const unsigned long UPDATE_INTERVAL = 5000;

const float ADC_MAX_VOLTAGE = 3.3;
const float ADC_MAX_VALUE = 1023.0;
const float GUVA_UV_FACTOR = 10.0;
const float SEA_LEVEL_PRESSURE_PA = 101325.0;

unsigned long previousUpdateMillis = 0;
unsigned long telemetrySequence = 0;

bool wifiOnline = false;
bool dhtOnline = false;
bool bh1750Online = false;
bool bmpOnline = false;
bool mpuOnline = false;

struct Telemetry {
  float dhtTemperature;
  float dhtHumidity;

  int guvaRaw;
  float guvaVoltage;
  float uvIndex;

  float illuminance;

  float bmpTemperature;
  float pressureHpa;
  float altitude;

  float accelerationX;
  float accelerationY;
  float accelerationZ;

  float gyroX;
  float gyroY;
  float gyroZ;

  float mpuTemperature;
  float roll;
  float pitch;

  float averageTemperature;
  uint8_t temperatureSensorCount;

  unsigned long uptime;
  uint32_t freeHeap;
  uint8_t wifiClients;
};

Telemetry telemetry;

String jsonFloat(float value, uint8_t decimals = 2) {
  if (isnan(value) || isinf(value)) {
    return "null";
  }

  return String(value, decimals);
}

bool initializeWiFi() {
  Serial.println();
  Serial.println("Iniciando Wi-Fi...");

  WiFi.persistent(false);

  WiFi.mode(WIFI_OFF);
  delay(300);

  WiFi.mode(WIFI_AP);
  delay(300);

  bool started = WiFi.softAP(
    WIFI_SSID,
    WIFI_PASSWORD
  );

  if (!started) {
    Serial.println("Falha ao criar a rede Wi-Fi.");
    return false;
  }

  delay(500);

  Serial.println("Rede Wi-Fi criada.");

  Serial.print("SSID: ");
  Serial.println(WIFI_SSID);

  Serial.print("Senha: ");
  Serial.println(WIFI_PASSWORD);

  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  return true;
}

void initializeSensors() {
  Serial.println();
  Serial.println("Inicializando sensores...");

  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(100000);

  dht.begin();

  bh1750Online = lightMeter.begin(
    BH1750::CONTINUOUS_HIGH_RES_MODE
  );

  bmpOnline = bmp.begin(
    BMP085_ULTRAHIGHRES
  );

  mpuOnline = mpu.begin();

  if (mpuOnline) {
    mpu.setAccelerometerRange(
      MPU6050_RANGE_8_G
    );

    mpu.setGyroRange(
      MPU6050_RANGE_500_DEG
    );

    mpu.setFilterBandwidth(
      MPU6050_BAND_21_HZ
    );
  }

  delay(2000);

  Serial.print("BH1750: ");
  Serial.println(
    bh1750Online ? "OK" : "Falha"
  );

  Serial.print("BMP180: ");
  Serial.println(
    bmpOnline ? "OK" : "Falha"
  );

  Serial.print("MPU6050: ");
  Serial.println(
    mpuOnline ? "OK" : "Falha"
  );
}

void readDHT11() {
  float temperature =
    dht.readTemperature();

  float humidity =
    dht.readHumidity();

  if (
    !isnan(temperature) &&
    !isnan(humidity)
  ) {
    telemetry.dhtTemperature =
      temperature;

    telemetry.dhtHumidity =
      humidity;

    dhtOnline = true;
  } else {
    telemetry.dhtTemperature = NAN;
    telemetry.dhtHumidity = NAN;

    dhtOnline = false;
  }
}

void readGUVA() {
  unsigned long sum = 0;

  for (uint8_t i = 0; i < 10; i++) {
    sum += analogRead(GUVA_PIN);
    delay(2);
  }

  telemetry.guvaRaw =
    sum / 10;

  telemetry.guvaVoltage =
    telemetry.guvaRaw *
    (ADC_MAX_VOLTAGE / ADC_MAX_VALUE);

  telemetry.uvIndex =
    telemetry.guvaVoltage *
    GUVA_UV_FACTOR;
}

void readBH1750() {
  if (!bh1750Online) {
    telemetry.illuminance = NAN;
    return;
  }

  float value =
    lightMeter.readLightLevel();

  if (
    !isnan(value) &&
    !isinf(value) &&
    value >= 0
  ) {
    telemetry.illuminance = value;
  } else {
    telemetry.illuminance = NAN;
  }
}

void readBMP180() {
  if (!bmpOnline) {
    telemetry.bmpTemperature = NAN;
    telemetry.pressureHpa = NAN;
    telemetry.altitude = NAN;

    return;
  }

  telemetry.bmpTemperature =
    bmp.readTemperature();

  telemetry.pressureHpa =
    bmp.readPressure() / 100.0;

  telemetry.altitude =
    bmp.readAltitude(
      SEA_LEVEL_PRESSURE_PA
    );
}

void readMPU6050() {
  if (!mpuOnline) {
    telemetry.accelerationX = NAN;
    telemetry.accelerationY = NAN;
    telemetry.accelerationZ = NAN;

    telemetry.gyroX = NAN;
    telemetry.gyroY = NAN;
    telemetry.gyroZ = NAN;

    telemetry.mpuTemperature = NAN;
    telemetry.roll = NAN;
    telemetry.pitch = NAN;

    return;
  }

  sensors_event_t acceleration;
  sensors_event_t gyro;
  sensors_event_t temperature;

  mpu.getEvent(
    &acceleration,
    &gyro,
    &temperature
  );

  telemetry.accelerationX =
    acceleration.acceleration.x;

  telemetry.accelerationY =
    acceleration.acceleration.y;

  telemetry.accelerationZ =
    acceleration.acceleration.z;

  telemetry.gyroX =
    gyro.gyro.x;

  telemetry.gyroY =
    gyro.gyro.y;

  telemetry.gyroZ =
    gyro.gyro.z;

  telemetry.mpuTemperature =
    temperature.temperature;

  telemetry.roll =
    atan2(
      telemetry.accelerationY,
      telemetry.accelerationZ
    ) *
    180.0 / PI;

  telemetry.pitch =
    atan2(
      -telemetry.accelerationX,
      sqrt(
        telemetry.accelerationY *
        telemetry.accelerationY +

        telemetry.accelerationZ *
        telemetry.accelerationZ
      )
    ) *
    180.0 / PI;
}

void calculateAverageTemperature() {
  float sum = 0;
  uint8_t count = 0;

  if (
    dhtOnline &&
    !isnan(telemetry.dhtTemperature)
  ) {
    sum += telemetry.dhtTemperature;
    count++;
  }

  if (
    bmpOnline &&
    !isnan(telemetry.bmpTemperature)
  ) {
    sum += telemetry.bmpTemperature;
    count++;
  }

  if (
    mpuOnline &&
    !isnan(telemetry.mpuTemperature)
  ) {
    sum += telemetry.mpuTemperature;
    count++;
  }

  telemetry.temperatureSensorCount =
    count;

  if (count > 0) {
    telemetry.averageTemperature =
      sum / count;
  } else {
    telemetry.averageTemperature = NAN;
  }
}

void collectTelemetry() {
  digitalWrite(STATUS_LED, LOW);

  readDHT11();
  yield();

  readGUVA();
  yield();

  readBH1750();
  yield();

  readBMP180();
  yield();

  readMPU6050();
  yield();

  calculateAverageTemperature();

  telemetry.uptime =
    millis();

  telemetry.freeHeap =
    ESP.getFreeHeap();

  if (wifiOnline) {
    telemetry.wifiClients =
      WiFi.softAPgetStationNum();
  } else {
    telemetry.wifiClients = 0;
  }

  telemetrySequence++;

  digitalWrite(STATUS_LED, HIGH);
}

String buildTelemetryJson() {
  String json;

  json.reserve(1800);

  json += "{";

  json += "\"sequence\":";
  json += String(telemetrySequence);
  json += ",";

  json += "\"uptime_ms\":";
  json += String(telemetry.uptime);
  json += ",";

  json += "\"payload\":{";

  json += "\"dht11\":{";

  json += "\"online\":";
  json += dhtOnline ? "true" : "false";
  json += ",";

  json += "\"temperature_c\":";
  json += jsonFloat(
    telemetry.dhtTemperature,
    1
  );
  json += ",";

  json += "\"humidity_percent\":";
  json += jsonFloat(
    telemetry.dhtHumidity,
    1
  );

  json += "},";

  json += "\"guva\":{";

  json += "\"online\":true,";

  json += "\"adc_raw\":";
  json += String(telemetry.guvaRaw);
  json += ",";

  json += "\"voltage_v\":";
  json += jsonFloat(
    telemetry.guvaVoltage,
    3
  );
  json += ",";

  json += "\"uv_index_estimated\":";
  json += jsonFloat(
    telemetry.uvIndex,
    2
  );

  json += "},";

  json += "\"bh1750\":{";

  json += "\"online\":";
  json += bh1750Online ? "true" : "false";
  json += ",";

  json += "\"illuminance_lux\":";
  json += jsonFloat(
    telemetry.illuminance,
    2
  );

  json += "},";

  json += "\"bmp180\":{";

  json += "\"online\":";
  json += bmpOnline ? "true" : "false";
  json += ",";

  json += "\"temperature_c\":";
  json += jsonFloat(
    telemetry.bmpTemperature,
    2
  );
  json += ",";

  json += "\"pressure_hpa\":";
  json += jsonFloat(
    telemetry.pressureHpa,
    2
  );
  json += ",";

  json += "\"altitude_m\":";
  json += jsonFloat(
    telemetry.altitude,
    2
  );

  json += "}";

  json += "},";

  json += "\"combined\":{";

  json += "\"average_temperature_c\":";
  json += jsonFloat(
    telemetry.averageTemperature,
    2
  );
  json += ",";

  json += "\"temperature_sensor_count\":";
  json += String(
    telemetry.temperatureSensorCount
  );

  json += "},";

  json += "\"ads\":{";
  json += "\"mpu6050\":{";

  json += "\"online\":";
  json += mpuOnline ? "true" : "false";
  json += ",";

  json += "\"acceleration_ms2\":{";

  json += "\"x\":";
  json += jsonFloat(
    telemetry.accelerationX,
    3
  );
  json += ",";

  json += "\"y\":";
  json += jsonFloat(
    telemetry.accelerationY,
    3
  );
  json += ",";

  json += "\"z\":";
  json += jsonFloat(
    telemetry.accelerationZ,
    3
  );

  json += "},";

  json += "\"gyro_rads\":{";

  json += "\"x\":";
  json += jsonFloat(
    telemetry.gyroX,
    3
  );
  json += ",";

  json += "\"y\":";
  json += jsonFloat(
    telemetry.gyroY,
    3
  );
  json += ",";

  json += "\"z\":";
  json += jsonFloat(
    telemetry.gyroZ,
    3
  );

  json += "},";

  json += "\"temperature_c\":";
  json += jsonFloat(
    telemetry.mpuTemperature,
    2
  );
  json += ",";

  json += "\"roll_deg\":";
  json += jsonFloat(
    telemetry.roll,
    2
  );
  json += ",";

  json += "\"pitch_deg\":";
  json += jsonFloat(
    telemetry.pitch,
    2
  );

  json += "}";

  json += "},";

  json += "\"eps\":{";
  json += "\"converter\":\"LM2596\",";
  json += "\"batteries\":\"LiPo 2S1P\",";
  json += "\"digital_telemetry\":false";
  json += "},";

  json += "\"obc\":{";

  json += "\"board\":\"NodeMCU V3 ESP8266MOD\",";

  json += "\"free_heap_bytes\":";
  json += String(telemetry.freeHeap);
  json += ",";

  json += "\"wifi_clients\":";
  json += String(telemetry.wifiClients);
  json += ",";

  json += "\"ssid\":\"";
  json += WIFI_SSID;
  json += "\",";

  json += "\"ip\":\"";

  if (wifiOnline) {
    json += WiFi.softAPIP().toString();
  } else {
    json += "N/D";
  }

  json += "\"";

  json += "}";

  json += "}";

  return json;
}

const char DASHBOARD_PAGE[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html lang="pt-BR">

<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>CubEdu</title>

<style>
*{box-sizing:border-box}

:root{
--bg:#04111f;
--panel:#09233a;
--line:#205779;
--text:#eff8ff;
--muted:#9cb7cf;
--blue:#38c7ff;
--purple:#a879ff;
--green:#35e59a;
--orange:#ff9d18;
--red:#ff6680
}

body{
margin:0;
min-height:100vh;
font-family:Arial,sans-serif;
color:var(--text);
background:radial-gradient(circle at top,#0b3557,var(--bg) 58%)
}

header{
position:relative;
max-width:1540px;
margin:auto;
padding:25px 20px 22px;
text-align:center
}

h1{
margin:0;
color:#54caff;
font-size:35px
}

header p{
margin:6px 0 0;
color:#91c8ef;
font-size:18px;
font-weight:bold
}

.system{
position:absolute;
top:28px;
right:20px;
display:flex;
gap:18px;
align-items:center;
padding:12px 17px;
background:#071a2b;
border:1px solid var(--line);
border-radius:12px
}

.dot{
display:inline-block;
width:13px;
height:13px;
margin-right:8px;
border-radius:50%;
background:var(--green);
box-shadow:0 0 10px var(--green)
}

.online{color:var(--green)}
.offline{color:var(--red)}

.layout{
width:min(1540px,calc(100% - 30px));
margin:auto;
display:grid;
grid-template-columns:41% 59%;
gap:14px
}

.panel{
padding:16px;
background:rgba(7,31,52,.93);
border:1px solid var(--line);
border-radius:16px
}

.panel-title{
margin:0 0 12px;
padding-bottom:10px;
color:var(--blue);
border-bottom:1px solid var(--line);
font-size:23px
}

.component-table{
overflow:hidden;
border:1px solid var(--line);
border-radius:13px
}

.row{
display:grid;
grid-template-columns:1.2fr 1.05fr .55fr;
align-items:center;
min-height:58px;
padding:10px 18px;
border-bottom:1px solid var(--line)
}

.row:last-child{border-bottom:0}

.row.header{
min-height:52px;
color:#b6d8f5;
font-weight:bold
}

.row strong{font-size:16px}

.badge{
display:inline-block;
min-width:95px;
padding:7px 10px;
text-align:center;
border-radius:18px;
font-size:13px;
font-weight:bold;
background:#073d3a
}

.telemetry-grid{
display:grid;
grid-template-columns:1fr 1fr;
gap:14px
}

.subsystem{
padding:12px;
background:rgba(9,39,64,.84);
border:1px solid var(--line);
border-radius:13px
}

.subsystem h2{
margin:0 0 11px;
font-size:21px
}

.payload h2,.ttc h2{color:var(--blue)}
.ads h2{color:var(--purple)}
.eps h2{color:var(--orange)}
.obc h2{color:var(--green)}

.data{
display:grid;
overflow:hidden;
border:1px solid var(--line);
border-radius:10px
}

.data.two{
grid-template-columns:1fr 1fr
}

.data.three{
grid-template-columns:repeat(3,1fr)
}

.item{
min-height:66px;
padding:10px 13px;
border-right:1px solid var(--line);
border-bottom:1px solid var(--line)
}

.label{
display:block;
margin-bottom:6px;
color:var(--muted);
font-size:14px
}

.value{
color:#fff;
font-size:18px;
font-weight:bold
}

.wide{grid-column:1/-1}
.ttc{grid-column:1/-1}

.ttc .data{
grid-template-columns:1.2fr 1fr 1.2fr 1fr 1fr
}

.error{
display:none;
width:min(1540px,calc(100% - 30px));
margin:0 auto 12px;
padding:10px;
color:#ffd5dc;
background:#721c2a;
border-radius:9px;
text-align:center
}

footer{
width:min(1540px,calc(100% - 30px));
margin:20px auto 0;
padding:20px 10px 28px;
color:#7898b2;
border-top:1px solid var(--line);
text-align:center;
font-size:12px;
line-height:1.7
}

footer strong{color:#b9d7ec}

@media(max-width:1000px){
.system{
position:static;
width:max-content;
margin:16px auto 0
}

.layout{
grid-template-columns:1fr
}
}

@media(max-width:700px){
h1{font-size:29px}

.telemetry-grid{
grid-template-columns:1fr
}

.ttc{grid-column:auto}

.ttc .data,
.data.three{
grid-template-columns:1fr
}

.row{
grid-template-columns:1fr 1fr
}

.row span:last-child{
grid-column:1/-1;
margin-top:8px
}
}
</style>
</head>

<body>

<header>
<h1>CubEdu</h1>
<p>Trabalho de Conclusão de Curso</p>

<div class="system">
<span class="online">
<span class="dot"></span>
Sistema Online
</span>

<span>Atualização: 5 s</span>
</div>
</header>

<div id="errorBox" class="error">
Não foi possível atualizar a telemetria.
</div>

<main class="layout">

<section class="panel">

<h2 class="panel-title">
● Status dos Componentes
</h2>

<div class="component-table">

<div class="row header">
<span>Componente</span>
<span>Porta / Interface</span>
<span>Estado</span>
</div>

<div class="row">
<strong>BMP180</strong>
<span>I²C (D1, D2)</span>
<span id="bmpStatus" class="badge">--</span>
</div>

<div class="row">
<strong>BH1750</strong>
<span>I²C (D1, D2)</span>
<span id="bhStatus" class="badge">--</span>
</div>

<div class="row">
<strong>DHT11</strong>
<span>Digital (D5)</span>
<span id="dhtStatus" class="badge">--</span>
</div>

<div class="row">
<strong>GUVA-S12SD</strong>
<span>Analógico (A0)</span>
<span class="badge online">Online</span>
</div>

<div class="row">
<strong>MPU6050</strong>
<span>I²C (D1, D2)</span>
<span id="mpuStatus" class="badge">--</span>
</div>

<div class="row">
<strong>ESP8266</strong>
<span>Servidor Web / Wi-Fi</span>
<span class="badge online">Online</span>
</div>

<div class="row">
<strong>EPS</strong>
<span>LM2596 / LiPo 2S1P</span>
<span class="badge online">Configurado</span>
</div>

</div>
</section>

<section class="panel">

<h2 class="panel-title">
▥ Telemetria dos Subsistemas
</h2>

<div class="telemetry-grid">

<section class="subsystem payload">

<h2>Payload</h2>

<div class="data two">

<div class="item">
<span class="label">Temperatura média</span>

<span class="value">
<span id="averageTemperature">--</span> °C
</span>
</div>

<div class="item">
<span class="label">Pressão</span>

<span class="value">
<span id="pressure">--</span> hPa
</span>
</div>

<div class="item wide">
<span class="label">Iluminância</span>

<span class="value">
<span id="lux">--</span> lux
</span>
</div>

<div class="item">
<span class="label">Umidade</span>

<span class="value">
<span id="humidity">--</span> %
</span>
</div>

<div class="item">
<span class="label">Índice UV estimado</span>

<span class="value" id="uvIndex">
--
</span>
</div>

</div>
</section>

<section class="subsystem ads">

<h2>ADS</h2>

<div class="data">

<div class="item">
<span class="label">
Aceleração X / Y / Z
</span>

<span class="value" id="acceleration">
--
</span>
</div>

<div class="item">
<span class="label">
Velocidade angular X / Y / Z
</span>

<span class="value" id="gyro">
--
</span>
</div>

</div>

<div class="data three">

<div class="item">
<span class="label">Roll</span>

<span class="value">
<span id="roll">--</span>°
</span>
</div>

<div class="item">
<span class="label">Pitch</span>

<span class="value">
<span id="pitch">--</span>°
</span>
</div>

<div class="item">
<span class="label">
Altitude barométrica
</span>

<span class="value">
<span id="altitude">--</span> m
</span>
</div>

</div>
</section>

<section class="subsystem eps">

<h2>EPS</h2>

<div class="data">

<div class="item">
<span class="label">
Conversor DC-DC
</span>

<span class="value">
LM2596
</span>
</div>

<div class="item">
<span class="label">Baterias</span>

<span class="value">
LiPo 2S1P
</span>
</div>

</div>
</section>

<section class="subsystem obc">

<h2>OBC</h2>

<div class="data">

<div class="item">
<span class="label">
Memória livre
</span>

<span class="value">
<span id="heap">--</span> bytes
</span>
</div>

<div class="item">
<span class="label">
Pacotes enviados
</span>

<span class="value" id="sequence">
--
</span>
</div>

<div class="item">
<span class="label">
Tempo de funcionamento
</span>

<span class="value" id="uptime">
--
</span>
</div>

</div>
</section>

<section class="subsystem ttc">

<h2>TT&amp;C</h2>

<div class="data">

<div class="item">
<span class="label">Rede Wi-Fi</span>
<span class="value" id="ssid">--</span>
</div>

<div class="item">
<span class="label">Endereço IP</span>
<span class="value" id="ip">--</span>
</div>

<div class="item">
<span class="label">
Dispositivos conectados
</span>

<span class="value" id="clients">
--
</span>
</div>

<div class="item">
<span class="label">
Taxa de atualização
</span>

<span class="value">
5 segundos
</span>
</div>

<div class="item">
<span class="label">
Última atualização
</span>

<span class="value" id="lastUpdate">
--
</span>
</div>

</div>
</section>

</div>
</section>

</main>

<footer>
Trabalho de Conclusão de Curso desenvolvido pelo aluno
<strong>Gabriel Santos Evangelista</strong>
<br>

Orientador:
<strong>Prof. Dr. Manuel Nascimento Dias Barcelos Junior</strong>
<br>

Universidade de Brasília - FCTE
</footer>

<script>
const $=id=>document.getElementById(id);

const show=(value,decimals=2)=>
  value==null
  ?"N/D"
  :Number(value).toFixed(decimals);

function status(id,online){
  const element=$(id);

  element.textContent=
    online
    ?"● Online"
    :"● Offline";

  element.className=
    "badge "+
    (online?"online":"offline");
}

function formatTime(milliseconds){
  let totalSeconds=
    Math.floor(milliseconds/1000);

  let hours=
    Math.floor(totalSeconds/3600);

  let minutes=
    Math.floor(
      (totalSeconds%3600)/60
    );

  let seconds=
    totalSeconds%60;

  return [hours,minutes,seconds]
    .map(
      value=>
      String(value).padStart(2,"0")
    )
    .join(":");
}

async function updateTelemetry(){
  try{
    const response=await fetch(
      "/api/telemetry",
      {cache:"no-store"}
    );

    if(!response.ok){
      throw new Error();
    }

    const data=await response.json();

    $("errorBox").style.display="none";

    status(
      "bmpStatus",
      data.payload.bmp180.online
    );

    status(
      "bhStatus",
      data.payload.bh1750.online
    );

    status(
      "dhtStatus",
      data.payload.dht11.online
    );

    status(
      "mpuStatus",
      data.ads.mpu6050.online
    );

    $("averageTemperature").textContent=
      show(
        data.combined
          .average_temperature_c,
        2
      );

    $("pressure").textContent=
      show(
        data.payload
          .bmp180
          .pressure_hpa,
        1
      );

    $("lux").textContent=
      show(
        data.payload
          .bh1750
          .illuminance_lux,
        1
      );

    $("humidity").textContent=
      show(
        data.payload
          .dht11
          .humidity_percent,
        1
      );

    $("uvIndex").textContent=
      show(
        data.payload
          .guva
          .uv_index_estimated,
        2
      );

    const mpu=
      data.ads.mpu6050;

    $("acceleration").textContent=
      show(mpu.acceleration_ms2.x,2)+
      " / "+
      show(mpu.acceleration_ms2.y,2)+
      " / "+
      show(mpu.acceleration_ms2.z,2)+
      " m/s²";

    $("gyro").textContent=
      show(mpu.gyro_rads.x,3)+
      " / "+
      show(mpu.gyro_rads.y,3)+
      " / "+
      show(mpu.gyro_rads.z,3)+
      " rad/s";

    $("roll").textContent=
      show(mpu.roll_deg,2);

    $("pitch").textContent=
      show(mpu.pitch_deg,2);

    $("altitude").textContent=
      show(
        data.payload
          .bmp180
          .altitude_m,
        2
      );

    $("heap").textContent=
      Number(
        data.obc.free_heap_bytes
      ).toLocaleString("pt-BR");

    $("sequence").textContent=
      data.sequence;

    $("uptime").textContent=
      formatTime(data.uptime_ms);

    $("ssid").textContent=
      data.obc.ssid;

    $("ip").textContent=
      data.obc.ip;

    $("clients").textContent=
      data.obc.wifi_clients;

    $("lastUpdate").textContent=
      new Date()
        .toLocaleTimeString("pt-BR");

  }catch(error){
    $("errorBox").style.display="block";
  }
}

updateTelemetry();

setInterval(
  updateTelemetry,
  5000
);
</script>

</body>
</html>
)HTML";

void configureWebServer() {
  server.on("/", HTTP_GET, []() {
    server.send_P(
      200,
      "text/html; charset=utf-8",
      DASHBOARD_PAGE
    );
  });

  server.on(
    "/api/telemetry",
    HTTP_GET,
    []() {
      server.sendHeader(
        "Access-Control-Allow-Origin",
        "*"
      );

      server.sendHeader(
        "Cache-Control",
        "no-store"
      );

      server.send(
        200,
        "application/json; charset=utf-8",
        buildTelemetryJson()
      );
    }
  );

  server.on(
    "/api/status",
    HTTP_GET,
    []() {
      server.send(
        200,
        "application/json",
        "{\"status\":\"online\"}"
      );
    }
  );

  server.onNotFound([]() {
    server.send(
      404,
      "application/json",
      "{\"error\":\"route_not_found\"}"
    );
  });

  server.begin();

  Serial.println(
    "Servidor HTTP iniciado."
  );
}

void printTelemetry() {
  Serial.println();
  Serial.println("------------------------------");

  Serial.print("Pacote: ");
  Serial.println(telemetrySequence);

  Serial.print("Temperatura media: ");

  if (
    isnan(
      telemetry.averageTemperature
    )
  ) {
    Serial.println("N/D");
  } else {
    Serial.print(
      telemetry.averageTemperature,
      2
    );

    Serial.println(" C");
  }

  Serial.print("Umidade: ");

  if (isnan(telemetry.dhtHumidity)) {
    Serial.println("N/D");
  } else {
    Serial.print(
      telemetry.dhtHumidity,
      1
    );

    Serial.println(" %");
  }

  Serial.print("Pressao: ");

  if (isnan(telemetry.pressureHpa)) {
    Serial.println("N/D");
  } else {
    Serial.print(
      telemetry.pressureHpa,
      2
    );

    Serial.println(" hPa");
  }

  Serial.print("Iluminancia: ");

  if (isnan(telemetry.illuminance)) {
    Serial.println("N/D");
  } else {
    Serial.print(
      telemetry.illuminance,
      2
    );

    Serial.println(" lux");
  }

  Serial.print("Indice UV: ");
  Serial.println(
    telemetry.uvIndex,
    2
  );

  Serial.print("Clientes Wi-Fi: ");
  Serial.println(
    telemetry.wifiClients
  );

  Serial.print("Memoria livre: ");
  Serial.print(
    telemetry.freeHeap
  );
  Serial.println(" bytes");

  Serial.println("------------------------------");
}

void setup() {
  pinMode(
    STATUS_LED,
    OUTPUT
  );

  digitalWrite(
    STATUS_LED,
    HIGH
  );

  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("==============================");
  Serial.println("CubEdu");
  Serial.println("Trabalho de Conclusao de Curso");
  Serial.println("==============================");

  wifiOnline =
    initializeWiFi();

  if (wifiOnline) {
    configureWebServer();

    Serial.println();
    Serial.println(
      "Acesse: http://192.168.4.1"
    );
  }

  initializeSensors();

  collectTelemetry();

  previousUpdateMillis =
    millis();

  Serial.println();
  Serial.println(
    "Sistema inicializado."
  );
}

void loop() {
  if (wifiOnline) {
    server.handleClient();
  }

  unsigned long currentMillis =
    millis();

  if (
    currentMillis -
    previousUpdateMillis >=
    UPDATE_INTERVAL
  ) {
    previousUpdateMillis =
      currentMillis;

    collectTelemetry();
    printTelemetry();
  }

  yield();
}