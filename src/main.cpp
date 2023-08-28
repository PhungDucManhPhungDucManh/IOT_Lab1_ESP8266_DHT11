#include <Arduino.h>
#include <DHT.h>
#include <EspMQTTClient.h>

void onConnectionEstablished();
void sendData();

const String CHANNEL_ID = "2251747";

unsigned long nextTime = 0;
int delayTime = 10000;

DHT dht11(D5, DHT11);

EspMQTTClient mqttClient(
    "Mang",
    "12345678",
    "mqtt3.thingspeak.com",
    "DzYjCQkhOTcmIhQtHAIFBQY",
    "LYfxWWWSHJLiEFLb0jzgJgEP",
    "DzYjCQkhOTcmIhQtHAIFBQY",
    1883);

void setup()
{
  Serial.begin(9600);
  dht11.begin();
}

void loop()
{
  mqttClient.loop();
  sendData();
}

void onConnectionEstablished()
{
  Serial.println("\nConnected to MQTT server.");
}

void sendData()
{
  if (millis() < nextTime || !mqttClient.isConnected())
    return;

  float doAm = dht11.readHumidity();
  float nhietDo = dht11.readTemperature();

  if (isnan(doAm) || isnan(nhietDo))
  {
    return;
  }

  String topic = "channels/" + CHANNEL_ID + "/publish";
  String payload = "field1=" + String(nhietDo) + "&field2=" + String(doAm);

  if (mqttClient.publish(topic, payload))
  {
    Serial.print(F("Nhiệt độ: "));
    Serial.print(nhietDo);
    Serial.print(F("°C Độ ẩm: "));
    Serial.print(doAm);
    Serial.print(F("%  "));
    Serial.println();
  }

  nextTime += delayTime;

}