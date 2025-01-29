#include "Grinder.h"
#include "Definitions.h"
#include <EEPROM.h>

Grinder::Grinder() {
    pinMode(GPIO_GRINDER, OUTPUT);
    pinMode(START_BTN, INPUT_PULLUP);
    digitalWrite(GPIO_GRINDER, LOW);
}

bool Grinder::startBtnPressed() {
    return !digitalRead(START_BTN); // Negated because PULLUP
}

unsigned long Grinder::getTargetTime() {
    return targetTime;
}

void Grinder::on(double targetTime) {
    this->targetTime = millis() + targetTime * 1000;
    Serial.println("Grinder on");
    digitalWrite(GPIO_GRINDER, HIGH);
}

void Grinder::off() {
    Serial.println("Grinder off");
    digitalWrite(GPIO_GRINDER, LOW);
}