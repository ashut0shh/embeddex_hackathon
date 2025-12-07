#include <Wire.h>
#include "MAX30105.h"   // SparkFun MAX3010x library

MAX30105 sensor;

void setup() {
  Serial.begin(9600);
  delay(100);

  Serial.println("Initializing MAX30102 / MAX30105...");

  if (!sensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("Sensor not found! Check wiring.");
    while (1);
  }

  // ---- Sensor Configuration ----
  sensor.setup();                // default config

  sensor.setLEDMode(2);          // IR only
  sensor.setPulseAmplitudeIR(0xAF);   // Strong IR LED (0xAF works well on MH-ET LIVE)
  sensor.setPulseAmplitudeRed(0x00);  // Turn off Red

  sensor.setSampleRate(100);     // 100 samples/sec
  sensor.setPulseWidth(411);     // Max resolution (411 us)
  sensor.setADCRange(16384);     // High range
  
  Serial.println("START");
}

void loop() {
  long ir = sensor.getIR();   // Read IR value

  // Print always (continuous output)
  if (ir > 50) {              // avoid zero/noise when no finger
    Serial.println(ir);
  }

  delay(10); // ~100Hz
}
