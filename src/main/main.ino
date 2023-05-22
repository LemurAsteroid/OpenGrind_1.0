#include <WiFi.h>
#include <ESPAsyncWebSrv.h>

#include "Grinder.h"
#include "Dosage.h"
#include "WebServer.h"

const char* ssid = "AlexanderMegos";
const char* password = "@Sean19Bailey96!";

Grinder *grinder;
Dosage *dosage;

// State Machine
enum States
{
  NORMAL,
  GRINDING
};
uint8_t state = NORMAL;

AsyncWebServer server(80);

void setup()
{
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRootPage);
  server.on("/submit", HTTP_GET, handleSubmit);

  dosage = new Dosage();
  grinder = new Grinder();

  server.begin();
}

void loop()
{

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

    delay(500); // show 0.0 on display for a longer time

    state = NORMAL;
    break;
  }
}
