#include <Arduino.h>
#include "HX711.h"
#include <LiquidCrystal.h>
#include <BigNumbers.h>

// Data pin and clock pin
byte pinData = 3;
byte pinClk = 2;
byte pinTare = 7; // Tare pin

HX711 loadcell;
HX711 scale;

// Parameter to calibrate weight and sensor, different for different sensors
float calibration_factor = 106026; // Put some value and adjust it through serial monitor

// LCD connections
LiquidCrystal lcd(8, 9, 10, 11, 12, 13); // Change these pins according to your LCD wiring
BigNumbers bigNum(&lcd);                 // construct BigNumbers object, passing to it the name of our LCD object

void setup()
{
    Serial.begin(9600);
    lcd.begin(16, 2); // Initialize the LCD
    bigNum.begin();   // set up BigNumbers
    lcd.clear();      // clear display
    scale.begin(pinData, pinClk);

    // Apply the calibration
    scale.set_scale();

    // Initializing the tare
    scale.tare(); // Reset the scale to 0

    long zero_factor = scale.read_average(); // Get a baseline reading
    Serial.print("Zero factor: ");           // This can be used to remove the need to tare the scale. Useful in permanent scale projects.
    Serial.println(zero_factor);

    // Set tare pin mode to input with pull-up resistor
    pinMode(pinTare, INPUT_PULLUP);
}

void loop()
{
    // Check if tare pin is pressed (LOW)
    if (digitalRead(pinTare) == LOW)
    {
        // Tare the scale
        scale.tare();
        delay(5); // Small delay for stability
    }

    scale.set_scale(calibration_factor); // Adjust to this calibration factor

    float weight = scale.get_units();
    Serial.println(weight);

    int weightInt = weight * 1000; // Convert to grams

    // Print to LCD
    bigNum.displayLargeInt(weightInt, 0, 5, false);

    if (weight < float(0))
    {
        lcd.setCursor(0, 0);
        lcd.print("__");
        lcd.setCursor(0, 1);
        lcd.print("--");
    }

    delay(250); // Delay for stability, adjust as needed
}
