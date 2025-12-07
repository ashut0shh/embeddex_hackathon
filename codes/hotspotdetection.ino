#include <SoftwareSerial.h>
#include <math.h>

// ===================== PIN DEFINITIONS =====================
const int TH_BASE = A0;   // baseline thermistor
const int TH_H1   = A1;   // hotspot 1
const int TH_H2   = A2;   // hotspot 2
const int TH_H3   = A3;   // hotspot 3

const int FSR_PIN = 4;    // digital FSR input (using pull-up)
const int LED_PIN = 13;   // LED indicator for foot presence

SoftwareSerial BT(2, 3);   // RX, TX

// ===================== THERMISTOR CONSTANTS =====================
const float SERIES_RESISTOR = 10000.0;
const float NOMINAL_RESISTANCE = 10000.0;
const float NOMINAL_TEMP = 25.0;
const float BETA_COEFFICIENT = 3950.0;

// ******** HOTSPOT THRESHOLD = 2.2°C ********
const float HOTSPOT_THRESHOLD = 2.2;

// ===================== FSR DEBOUNCE =====================
const unsigned long DEBOUNCE_MS = 50;
bool lastState = false;
bool stableState = false;
unsigned long lastChangeMillis = 0;

// Send "foot detected" once
bool lastFootState = false;

// Remember last hotspot state
bool lastHotspotState = false;

// ===================== READ THERMISTOR =====================
float readThermistor(int pin) {
  int adc = analogRead(pin);
  float resistance = (1023.0 / adc - 1.0) * SERIES_RESISTOR;

  float steinhart = resistance / NOMINAL_RESISTANCE;
  steinhart = log(steinhart);
  steinhart /= BETA_COEFFICIENT;
  steinhart += 1.0 / (NOMINAL_TEMP + 273.15);
  steinhart = 1.0 / steinhart;
  steinhart -= 273.15;

  return steinhart;
}

// ===================== SETUP =====================
void setup() {
  pinMode(FSR_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  BT.begin(9600);

  stableState = (digitalRead(FSR_PIN) == LOW);
  lastState = stableState;
  lastFootState = !stableState;

  Serial.println("Foot Thermal System + Bluetooth Ready");
  BT.println("Bluetooth Ready");
}

// ===================== LOOP =====================
void loop() {

  // -------------------- FSR DEBOUNCE --------------------
  bool raw = (digitalRead(FSR_PIN) == LOW);

  if (raw != lastState) {
    lastChangeMillis = millis();
    lastState = raw;
  }

  if ((millis() - lastChangeMillis) > DEBOUNCE_MS) {
    if (stableState != lastState) {
      stableState = lastState;
    }
  }

  // INVERTED LOGIC
  bool footPresent = !stableState;
  digitalWrite(LED_PIN, footPresent ? HIGH : LOW);

  // ------------------ FOOT DETECTED ONCE ------------------
  if (footPresent && !lastFootState) {
    BT.println("Foot Detected");
    Serial.println("Sent via BT: Foot Detected");
  }
  lastFootState = footPresent;

  // -------------------- READ TEMPS --------------------
  float t_base = readThermistor(TH_BASE);
  float t_h1   = readThermistor(TH_H1);
  float t_h2   = readThermistor(TH_H2);
  float t_h3   = readThermistor(TH_H3);

  float diff1 = t_h1 - t_base;
  float diff2 = t_h2 - t_base;
  float diff3 = t_h3 - t_base;

  // -------------------- SERIAL MONITOR --------------------
  Serial.println("============ FOOT THERMAL MONITOR ============");
  Serial.print("Foot Present: "); Serial.println(footPresent ? "YES" : "NO");

  Serial.print("Base Temp: "); Serial.println(t_base);

  // ---- Print actual temps + differences ----
  Serial.print("H1 Temp: "); Serial.print(t_h1);
  Serial.print("  | Diff: "); Serial.println(diff1);

  Serial.print("H2 Temp: "); Serial.print(t_h2);
  Serial.print("  | Diff: "); Serial.println(diff2);

  Serial.print("H3 Temp: "); Serial.print(t_h3);
  Serial.print("  | Diff: "); Serial.println(diff3);

  // -------------------- HOTSPOT DETECTION --------------------
  bool hotspotNow = false;
  String message = "";

  if (footPresent) {
    if (diff1 >= HOTSPOT_THRESHOLD) { hotspotNow = true; message += "H1 "; }
    if (diff2 >= HOTSPOT_THRESHOLD) { hotspotNow = true; message += "H2 "; }
    if (diff3 >= HOTSPOT_THRESHOLD) { hotspotNow = true; message += "H3 "; }

    // Always show on serial monitor
    if (hotspotNow) {
      Serial.print("HOTSPOT DETECTED AT: ");
      Serial.println(message);
    } else {
      Serial.println("No hotspot detected.");
    }

    // ---------- ONLY SEND BT IF HOTSPOT JUST APPEARED ----------
    if (hotspotNow && !lastHotspotState) {
      BT.print("HOTSPOT DETECTED: ");
      BT.println(message);
      Serial.println("Sent via BT (once): HOTSPOT DETECTED");
    }
  }

  // Update hotspot memory state
  lastHotspotState = hotspotNow;

  Serial.println("----------------------------------------------");
  delay(400);
}
