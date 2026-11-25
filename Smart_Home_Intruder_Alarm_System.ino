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
  while (true) {
    digitalWrite(redLedPin, LED_ON);
    tone(buzzerPin, 2500);
    if (waitAndCheckButton(150)) return;

    digitalWrite(redLedPin, LED_OFF);
    tone(buzzerPin, 1800);
    if (waitAndCheckButton(150)) return;
  }
}
