#include <Arduino.h>
#include "HX711.h"
#include <LiquidCrystal.h>
#include <BigNumbers.h>

const byte pinData = 3;
const byte pinClk = 2;
const byte pinTare = 7;

HX711 scale;

const long calibration_factor = 106500;

LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
BigNumbers bigNum(&lcd);

const int numReadings = 8;
int readings[numReadings];
int index = 0;
int total = 0;
int average = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  bigNum.begin();
  lcd.clear();
  scale.begin(pinData, pinClk);
  scale.set_scale(calibration_factor);
  scale.tare();

  pinMode(pinTare, INPUT_PULLUP);

  for (int i = 0; i < numReadings; ++i) {
    readings[i] = 0;
  }
}

void loop() {
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  const unsigned long interval = 5; // Adjust as needed

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (digitalRead(pinTare) == LOW) {
      scale.tare();
    }

    int weight = scale.get_units() * 1000;

    total = total - readings[index];
    readings[index] = weight;
    total = total + readings[index];
    index = (index + 1) % numReadings;

    average = total / numReadings;

    if ((average & 1) == 0) {
      bigNum.displayLargeInt(average, 0, 5, false);
    }

    if (weight < 0) {
      lcd.setCursor(0, 0);
      lcd.print("__");
      lcd.setCursor(0, 1);
      lcd.print("--");
    }
  }
}
