#include <WiFi.h>
#include <PubSubClient.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"
#include "Base64.h"

const char* ssid = "admin";
const char* password = "a/@1234567";
const char* mqtt_server = "broker.mqttdashboard.com";
const unsigned int mqtt_port = 1883;
#define MQTT_USER               ""
#define MQTT_PASSWORD           ""
#define MQTT_PUBLISH_TOPIC    "camera/test"
#define MQTT_SUBSCRIBE_TOPIC    "camera/test/get"

WiFiClient espClient;
PubSubClient client(espClient);



//ESP32-CAM 安信可模組腳位設定
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

//自訂指令參數值
String command = "";
byte receiveState = 0;
byte cmdState = 1;
byte pState = 1;
byte questionState = 0;
byte equalState = 0;
byte semicolonState = 0;

String feedback = "";

unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 150;

void executeCommand(String cmd) {
  if (cmd == "LEFT") {
  Serial.println("Got LEFT");
  }
  else if (cmd == "RIGHT") {
    Serial.println("Got RIGHT");
  }
  else if (cmd == "UP") {
    Serial.println("Got UP");
  }
  else if (cmd == "DOWN") {
    Serial.println("Got DOWN");
  }
  else if (cmd == "STILL") {
    Serial.println("Got STILL");
  }
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  //關閉電源不穩就重開機的設定

  Serial.begin(115200);
  randomSeed(micros());

  initCamera();
  initWiFi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  client.setBufferSize(2048);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  else {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      feedback = sendImage();
      sendText(feedback);
    }
  }
  client.loop();
}




void initWiFi() {
  WiFi.mode(WIFI_STA);

  for (int i = 0; i < 2; i++) {
    WiFi.begin(ssid, password);

    delay(1000);
    Serial.println("");
    Serial.print("Connecting to ");
    Serial.println(ssid);

    long int StartTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      if ((StartTime + 5000) < millis()) break;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("");
      Serial.println("STAIP address: ");
      Serial.println(WiFi.localIP());
      Serial.println("");

      pinMode(2, OUTPUT);
      for (int j = 0; j < 5; j++) {
        digitalWrite(2, HIGH);
        delay(100);
        digitalWrite(2, LOW);
        delay(100);
      }
      break;
    }
  }

  if (WiFi.status() != WL_CONNECTED) {    //若連線失敗
    pinMode(2, OUTPUT);
    for (int k = 0; k < 2; k++) {
      digitalWrite(2, HIGH);
      delay(1000);
      digitalWrite(2, LOW);
      delay(1000);
    }
  }
}
