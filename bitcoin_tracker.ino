#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define CD_URL "http://api.coindesk.com/v1/bpi/currentprice.json"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const char* ssid = "";  # REDE -WIFI
const char* passwd = ""; # SENHA DA REDE WIFI

WiFiClient client;
HTTPClient http;


#define OLED_RESET -1/
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void printMessage(String value) {
  display.clearDisplay();
  display.setTextSize(2);             
  display.setTextColor(SSD1306_WHITE);        
  display.setCursor(0, 0);

  display.println("  BTC|USD  ");
  display.setTextSize(2);
  display.print(" "); display.println(value);
  display.display();
}

void updateBitcoinValue(void) {
  http.begin(CD_URL);
  int statusCode = http.GET();
  if (statusCode < 0) {
    Serial.println("Error - " + statusCode);
    return;
  }

  if (statusCode != HTTP_CODE_OK) {
    return;
  }

  String payload = http.getString();

  const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
  DynamicJsonBuffer jsonBuffer(capacity);

  JsonObject& root = jsonBuffer.parseObject(payload);
  if (!root.success()) {
  Serial.println("Parsing falho");
  } else {

    String valor = root["bpi"]["USD"]["rate"];
    valor.remove(valor.length()-1); valor.remove(valor.length()-1);
    Serial.println(valor);
    printMessage(valor);
  }



  http.end();

}


void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 allocation failed\n");
    for (;;);
  }


  WiFi.begin(ssid, passwd);
  Serial.print("Conectando ao WiFi...");
  int i = 1;

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(i); Serial.print("..."); i++;

  };

  Serial.println("\n");
  Serial.println("Conectado!");
  Serial.print("Endereco IP: "); Serial.print(WiFi.localIP());

}

void loop() {
  updateBitcoinValue();
  delay(3000);
}
