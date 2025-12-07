# Project Flow: Non‑Invasive Glucose Sensing & Foot Ulcer Detection System

This document describes the complete flow of the system, from sensor placement to data transmission and visualization.

---

## 1. System Overview

The project is designed to monitor diabetic foot health using multiple sensors and estimate glucose variations non‑invasively. The system detects foot hotspots (risk of ulcer), measures viscosity‑based glucose changes, and transmits the data to a mobile application and MATLAB dashboard.

---

## 2. Hardware Components

* **LM35 Temperature Sensors / Thermistors** – Detect temperature distribution across the foot arch.
* **FSR (Force Sensitive Resistor) Pressure Sensor** – Detects presence or absence of the foot.
* **MH‑ET Live Sensor** – Used to estimate blood viscosity and correlate it to glucose spikes or drops.
* **Arduino (UNO)** – Main microcontroller to read and process sensor data.
* **Bluetooth Module (HC‑05)** – Wireless transmission to mobile app.
* **Power Supply and wiring**.

---

## 3. Core Objective

* Detect **hotspots** on the foot that indicate potential diabetic ulcers.
* Estimate **glucose variability** through viscosity measurement.
* Avoid unnecessary processing when the foot is not present.
* Display **temperature, heart rate, glucose, pressure, viscosity**, and **alerts** in:

  * Arduino Serial Monitor
  * Mobile app (via Bluetooth)
  * MATLAB graphical dashboard

---

## 4. Project Flow

### **Step 1: System Initialization**

1. Power on Arduino.
2. Initialize LM35 sensors, FSR sensor, MH‑ET Live sensor, and Bluetooth module.
3. MATLAB serial connection waits for data.

---

### **Step 2: Check Foot Presence Using FSR**

* FSR sensor continuously monitors pressure.
* **If pressure < threshold:**

  * Foot is absent → System remains idle.
  * No temperature or viscosity measurements taken.
  * No Bluetooth transmission.
* **If pressure ≥ threshold:**

  * Foot is detected → Proceed to measurements.

---

### **Step 3: Temperature Measurement (LM35 Array)**

1. Multiple LM35 sensors placed along the **arch of the foot**.
2. Arduino reads analog temperature values from each sensor.
3. Conversion from voltage to °C.
4. Detect temperature differences:

   * Compare each sensor temperature with baseline (arch reference temperature).
   * Identify hotspots where **ΔT > pre‑defined threshold (e.g., > 2°C)**.

---

### **Step 4: Hotspot Detection Logic**

* If **no hotspot detected**:

  * Arduino still shows all temperatures on serial monitor.
  * Data may or may not be sent to Bluetooth depending on user settings.
* If **hotspot detected**:

  * Highlight the LM35 index producing the hotspot.
  * Trigger alert message.
  * Send temperature packet to mobile app via Bluetooth.

---

### **Step 5: Viscosity & Glucose Estimation (MH‑ET Live)**

1. MH‑ET Live sensor measures viscosity‑related electrical parameter.
2. Arduino reads raw data.
3. Apply calibration curve to estimate glucose spike/drop:

   * **Higher viscosity → higher glucose levels.**
4. Generate glucose trend value.

---

### **Step 6: Additional Vitals (Optional)**

* Heart rate sensor can be added for integrated diabetic monitoring.
* Arduino reads signal and sends BPM values.

---

### **Step 7: Data Packaging & Bluetooth Transmission**

When conditions are met:

* Foot detected AND (hotspot detected)

Arduino sends structured packets:

```
<T1,T2,T3,..., HotspotFlag>
```

Mobile app receives:

* Foot presence flag
* Alerts for hotspot

---

## 8. MATLAB Dashboard Visualization

MATLAB receives serial data and displays:

* **Glucose trend graph**
* **Heart rate display**
* **Viscosity reading**

MATLAB updates in real time as long as data is received.

---

## 9. Arduino Serial Terminal Output

Shows:

* Temperature of each LM35 sensor
* FSR state
* Hotspot detection warning
* Packet sent confirmation

---

## 10. System Safety Logic

* If foot absent → no readings taken.
* Bluetooth sends only when meaningful changes occur.

---

## 11. Summary of Data Flow

**Sensors → Arduino → Processing → Bluetooth → Mobile App → MATLAB Dashboard**

---

