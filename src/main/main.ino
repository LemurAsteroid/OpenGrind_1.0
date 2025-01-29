#include "Grinder.h"
#include "Dosage.h"
#include "Secrets.h"

#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL48qFJy-9a"
#define BLYNK_TEMPLATE_NAME "CoffeeGrinder"
#define BLYNK_AUTH_TOKEN "UZB3UdBYLpotbObpxFQl8RSkKi6xoOeq"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = SSID;
char pass[] = PASSWORD;

Grinder *grinder;
Dosage *dosage;

BlynkTimer timer;

// State Machine
enum States
{
  NORMAL,
  GRINDING
};
uint8_t state = NORMAL;

void setup()
{
  Serial.begin(115200);

  dosage = new Dosage();
  grinder = new Grinder();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Blynk.virtualWrite(V0, dosage->singleDoseTime);
  Blynk.virtualWrite(V1, dosage->doubleDoseTime);

  timer.setInterval(5L, mainLoop);
}

void mainLoop(){
  switch (state)
  {
  case NORMAL:
    // start grinding
    if (grinder->startBtnPressed())
    {
      Serial.println("btn pressed");
      state = GRINDING;
      break;
    }
    break;

  case GRINDING:
    Serial.println("Grinding");
    grinder->on(dosage->singleDoseSelected ? dosage->singleDoseTime : dosage->doubleDoseTime);
    while (millis() < grinder->getTargetTime())
    {
      // grinder->getTargetTime() - millis();
    }
    grinder->off();

    state = NORMAL;
    break;
  }
}

void loop()
{
  Blynk.run();
  timer.run();
}

BLYNK_WRITE(V0)
{
  dosage->singleDoseTime = param.asDouble();
}

BLYNK_WRITE(V1)
{
  dosage->doubleDoseTime = param.asDouble();
}