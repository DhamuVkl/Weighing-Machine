#include <Arduino.h>
#include "HX711.h"

// data pin and clock pin
byte pinData = 3;
byte pinClk = 2;

HX711 scale;
// Parameter to calibrate weight and sensor, different for different sensors
float calibration_factor = 106326; // put some value and adjust it through serial monitor

void setup()
{
    Serial.begin(9600);

    scale.begin(pinData, pinClk);
    // apply the calibration
    scale.set_scale();
    // initializing the tare.
    scale.tare(); // Reset the scale to 0

    long zero_factor = scale.read_average(); // Get a baseline reading
    Serial.print("Zero factor: ");           // This can be used to remove the need to tare the scale. Useful in permanent scale projects.
    Serial.println(zero_factor);
}

void loop()
{

    scale.set_scale(calibration_factor); // Adjust to this calibration factor

    Serial.print("Reading: ");
    Serial.print(scale.get_units(), 3);
    Serial.print(" kgs"); // Change this to grams and re-adjust the calibration factor if you follow SI units like a sane person
    Serial.println();
}