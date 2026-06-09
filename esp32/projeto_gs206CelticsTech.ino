#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqtt_server = "3ce2272cc2d5473895749ecf5cdd0ff5.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;

const char* mqtt_user = "NODEREDGS26";
const char* mqtt_pass = "gAS2610359";

#define LED_VERMELHO 25
#define LED_VERDE 26

#define BTN_VERDE 16

#define DHT_BASE_PIN 5
#define DHT_TOPO_PIN 4

#define SOLO_PIN 34

DHT dhtBase(DHT_BASE_PIN, DHT22);
DHT dhtTopo(DHT_TOPO_PIN, DHT22);

LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiClientSecure espClient;
PubSubClient client(espClient);

void conectarWiFi() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conectando WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistema Pronto");
  lcd.setCursor(0, 1);
  lcd.print("Aguardando...");
}

void setup() {

  Serial.begin(115200);

  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(BTN_VERDE, INPUT_PULLUP);

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);

  dhtBase.begin();
  dhtTopo.begin();

  lcd.init();
  lcd.backlight();

  espClient.setInsecure();

  client.setServer(mqtt_server, mqtt_port);

  conectarWiFi();
}

void realizarLeitura() {

  digitalWrite(LED_VERDE, HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Coletando...");
  lcd.setCursor(0, 1);
  lcd.print("Sensores");

  delay(1000);

  int valorSolo = analogRead(SOLO_PIN);

  int solo = map(
    valorSolo,
    0,
    4095,
    0,
    100
  );

  float humBase = dhtBase.readHumidity();
  float humTopo = dhtTopo.readHumidity();
  float tempTopo = dhtTopo.readTemperature();

  if (isnan(humBase) || isnan(humTopo) || isnan(tempTopo)) {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erro Sensor");

    digitalWrite(LED_VERDE, LOW);

    delay(2000);

    return;
  }

  float evaporacao = abs(humBase - humTopo);

  String nivelEvaporacao;
  String irrigacao;

  if (evaporacao < 15) {
    nivelEvaporacao = "BAIXA";
  }
  else if (evaporacao < 30) {
    nivelEvaporacao = "MEDIA";
  }
  else {
    nivelEvaporacao = "ALTA";
  }

  if (solo < 40 || evaporacao > 30) {
    irrigacao = "SIM";
  }
  else {
    irrigacao = "NAO";
  }

  if (solo < 30 || evaporacao > 40) {
    digitalWrite(LED_VERMELHO, HIGH);
  }
  else {
    digitalWrite(LED_VERMELHO, LOW);
  }

  String jsonSolo =
    "{\"solo\":" +
    String(solo) +
    "}";

  String jsonClima =
    "{\"temp_topo\":" +
    String(tempTopo, 1) +
    ",\"hum_base\":" +
    String(humBase, 1) +
    ",\"hum_topo\":" +
    String(humTopo, 1) +
    "}";

  String jsonAnalise =
    "{\"evaporacao\":" +
    String(evaporacao, 1) +
    ",\"nivel_evaporacao\":\"" +
    nivelEvaporacao +
    "\",\"irrigacao\":\"" +
    irrigacao +
    "\"}";

  if (client.connect(
        "ESP32_CLIENT",
        mqtt_user,
        mqtt_pass
      )) {

    client.publish(
      "agricola/solo",
      jsonSolo.c_str()
    );

    client.publish(
      "agricola/clima",
      jsonClima.c_str()
    );

    client.publish(
      "agricola/analise",
      jsonAnalise.c_str()
    );

    Serial.println("=== MQTT ENVIADO ===");
    Serial.println(jsonSolo);
    Serial.println(jsonClima);
    Serial.println(jsonAnalise);

    client.disconnect();
  }
  else {

    Serial.println("Falha MQTT");
  }

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Solo:");
  lcd.print(solo);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.print(tempTopo, 1);
  lcd.print("C");

  delay(3000);

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Evap:");
  lcd.print(nivelEvaporacao);

  lcd.setCursor(0, 1);

  if (irrigacao == "SIM") {
    lcd.print("Irrigar Solo");
  }
  else {
    lcd.print("Condicao Boa");
  }

  delay(4000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistema Pronto");
  lcd.setCursor(0, 1);
  lcd.print("Aguardando...");

  digitalWrite(LED_VERDE, LOW);
}

void loop() {

  if (digitalRead(BTN_VERDE) == LOW) {

    delay(50);

    if (digitalRead(BTN_VERDE) == LOW) {

      realizarLeitura();

      while (digitalRead(BTN_VERDE) == LOW) {
        delay(10);
      }
    }
  }

  delay(100);
}