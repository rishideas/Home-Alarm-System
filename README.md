# 🏠 Smart Home Intruder Alarm System (Arduino)

A low-cost, easy-to-use home security alarm system built with an **Arduino Uno**, a **PIR motion sensor**, an **RGB LED**, and a **piezo buzzer**. Designed for students and families who want reliable protection against break-ins without expensive commercial systems or frustrating false alarms.

---

## 🎯 Problem Statement & Design Goals

* **Target Users:** A student who is often home alone in the evening and their family members who need affordable peace of mind.
* **Key Pain Points Solved:**
  * **Immediate Intruder Alert:** Detects unauthorized entry instantly using infrared body-heat sensing.
  * **Zero False Alarms for Family:** Provides a 10-second two-stage grace period so family members returning home can easily disarm the system.
  * **Simple One-Button Control:** No complicated keypads or apps—just a single push button to reset the system.
  * **Affordable & Accessible:** Built entirely with standard beginner Arduino starter-kit components.

---

## ⚙️ How It Works (3-Stage Alert Sequence)

1. **Safe / Armed Mode (Solid Green):** The system actively monitors the doorway while glowing a steady **Green** light.
2. **Stage 1 — Friendly Reminder (Blinking Green for 5 Seconds):** When motion is detected, the **Green LED blinks** for 5 seconds to remind family members to press the disarm button.
3. **Stage 2 — Urgent Warning (Solid Red for 5 Seconds):** If the button is not pressed, the LED turns **Solid Red** for 5 seconds as a final warning.
4. **Stage 3 — Intruder Alarm (Blinking Red + High-Pitched Buzzer):** If still not disarmed after 10 seconds total, a loud alternating siren sounds and the **Red LED flashes** rapidly to draw attention and deter intruders.
5. **Instant Reset:** Pressing the push button at any stage immediately stops the alarm and returns the system to **Solid Green**.

---

## 🛠️ Components Required

| Component | Quantity | Purpose |
| :--- | :---: | :--- |
| **Arduino Uno** | 1 | Main microcontroller |
| **PIR Motion Sensor (HC-SR501)** | 1 | Detects human movement near the entrance |
| **RGB LED (Common Cathode)** | 1 | Displays system status (Green / Red alerts) |
| **220Ω Resistors** | 2 | Limits current to the Red and Green LED pins |
| **Piezo Buzzer** | 1 | Emits high-pitched alarm siren |
| **Tactile Push Button** | 1 | Disarms/resets the alarm |
| **Breadboard & Jumper Wires** | 1 set | Circuit connections |

---

## 🔌 Circuit Wiring Guide

| Component Pin | Connects To (Arduino Pin) | Notes |
| :--- | :--- | :--- |
| **PIR Sensor `VCC`** | `5V` | Power supply |
| **PIR Sensor `GND`** | `GND` | Ground |
| **PIR Sensor `OUT`** | `Digital Pin 2` | Motion signal input |
| **Push Button (Leg 1)** | `Digital Pin 3` | Uses internal `INPUT_PULLUP` |
| **Push Button (Diagonal Leg)** | `GND` | Completes circuit when pressed |
| **RGB LED Red (`R`)** | `Digital Pin 8` | Connect via **220Ω resistor** |
| **RGB LED Common (`-`)** | `GND` | Longest leg (Common Cathode) |
| **RGB LED Green (`G`)** | `Digital Pin 9` | Connect via **220Ω resistor** |
| **Piezo Buzzer (`+`)** | `Digital Pin 10` | Longer leg / marked `+` |
| **Piezo Buzzer (`-`)** | `GND` | Shorter leg |

---

## 💻 Arduino Code

Upload the code below using the [Arduino IDE](https://arduino.cc):

```cpp
const int pirPin = 2;      // PIR sensor OUT
const int buttonPin = 3;   // Push button (connected to Pin 3 and GND)
const int redLedPin = 8;   // RGB LED Red pin
const int greenLedPin = 9; // RGB LED Green pin
const int buzzerPin = 10;  // Buzzer (+) pin

// Set to true for Common Cathode RGB LED (HIGH = ON, LOW = OFF)
const bool isCommonCathode = true; 

const int LED_ON  = isCommonCathode ? HIGH : LOW;
const int LED_OFF = isCommonCathode ? LOW : HIGH;

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Start in Safe Mode: Solid Green ON, Red OFF
  setGreenMode();
}

void loop() {
  // Check if someone enters (PIR detects motion)
  if (digitalRead(pirPin) == HIGH) {
    
    // STAGE 1: Blink Green LED for 5 seconds
    unsigned long stage1Start = millis();
    while (millis() - stage1Start < 5000) {
      digitalWrite(greenLedPin, LED_OFF);
      if (waitAndCheckButton(250)) return;
      
      digitalWrite(greenLedPin, LED_ON);
      if (waitAndCheckButton(250)) return;
    }

    // STAGE 2: Solid Red LED for 5 seconds
    digitalWrite(greenLedPin, LED_OFF);
    digitalWrite(redLedPin, LED_ON);
    
    unsigned long stage2Start = millis();
    while (millis() - stage2Start < 5000) {
      if (waitAndCheckButton(50)) return;
    }

    // STAGE 3: Alarm! Blinking Red + Buzzer sound
    triggerAlarm();
  }
}

// Waits for 'ms' milliseconds while constantly checking the button
bool waitAndCheckButton(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    if (digitalRead(buttonPin) == LOW) {
      setGreenMode();
      
      // Wait for PIR sensor to settle before watching again
      while (digitalRead(pirPin) == HIGH) {
        delay(100);
      }
      delay(1000);
      return true;
    }
  }
  return false;
}

// Returns system to solid glowing Green
void setGreenMode() {
  noTone(buzzerPin);
  digitalWrite(redLedPin, LED_OFF);
  digitalWrite(greenLedPin, LED_ON);
}

// Blinking Red + Buzzing until button is pressed

void triggerAlarm() {
  unsigned long alarmStart = millis();

  while (millis() - alarmStart < 300000UL) {  // 5 minutes
    digitalWrite(redLedPin, LED_ON);
    tone(buzzerPin, 2500);

    if (waitAndCheckButton(150)) return;

    digitalWrite(redLedPin, LED_OFF);
    tone(buzzerPin, 1800);

    if (waitAndCheckButton(150)) return;
  }

  // Automatically stop alarm after 5 minutes
  setGreenMode();
}

```

---

## 🚀 Setup & Calibration Tips

1. **PIR Sensor Warm-Up:** Allow 30–60 seconds after powering on for the PIR sensor to stabilize to room temperature.
2. **Adjust PIR Time Delay:** Turn the orange **Time (`Tx`) potentiometer** on the back of the PIR sensor all the way counter-clockwise so the sensor resets quickly after motion stops.
3. **Placement:** Mount the PIR sensor facing the main doorway and hide the reset push button slightly out of sight (e.g., behind a door frame or shoe rack) so family members can easily press it upon entering.
