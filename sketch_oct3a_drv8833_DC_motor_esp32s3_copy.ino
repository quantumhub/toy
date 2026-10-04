/*
  ESP32-S3 + DRV8833 – Single DC motor
  Forward / Backward / Stop (coast) / Brake
*/
#include <Arduino.h>

// ---------- Pin definitions ----------
const int PIN_IN1 = 7;   // DRV8833 IN1
const int PIN_IN2 = 8;   // DRV8833 IN2

const int nFault_INPUT_PIN  = 10;   // button / sensor input
const int nSleep_OUTPUT_PIN = 11;   // LED / relay output

// ---------- PWM settings ----------
const int PWM_FREQ     = 20000;   // 20 kHz – quiet for most motors
const int PWM_RES      = 8;       // 8-bit → 0-255
const int CH_IN1       = 0;       // LEDC channel for IN1
const int CH_IN2       = 1;       // LEDC channel for IN2

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("ESP32-S3 + DRV8833 motor control ready");


  pinMode(nFault_INPUT_PIN, INPUT_PULLUP);
  pinMode(nSleep_OUTPUT_PIN, OUTPUT);
  digitalWrite(nSleep_OUTPUT_PIN, HIGH);


  // Attach pins to LEDC channels
  ledcAttach(PIN_IN1, PWM_FREQ, PWM_RES);
  ledcAttach(PIN_IN2, PWM_FREQ, PWM_RES);

  // Start stopped
  motorStop();
}

void loop() {
  int nFault = digitalRead(nFault_INPUT_PIN);

  // Demo sequence
  Serial.printf("Forward 70%, nFault=%d\n",nFault);
  motorForward(180);          // 0-255
  delay(3000);

  Serial.printf("Stop (coast), nFault=%d\n",nFault);
  motorStop();
  delay(1500);

  Serial.printf("Backward 70%, nFault=%d\n",nFault);
  motorBackward(180);
  delay(3000);

  Serial.printf("Brake, nFault=%d\n",nFault);
  motorBrake();
  delay(1500);
}

// ---------- Motor control functions ----------

// Forward: IN1 = PWM, IN2 = 0
void motorForward(uint8_t speed) {
  speed = constrain(speed, 0, 255);
  ledcWrite(PIN_IN1, speed);
  ledcWrite(PIN_IN2, 0);
}

// Backward: IN1 = 0, IN2 = PWM
void motorBackward(uint8_t speed) {
  speed = constrain(speed, 0, 255);
  ledcWrite(PIN_IN1, 0);
  ledcWrite(PIN_IN2, speed);
}

// Coast (free-wheel)
void motorStop() {
  ledcWrite(PIN_IN1, 0);
  ledcWrite(PIN_IN2, 0);
}

// Active brake (both high)
void motorBrake() {
  ledcWrite(PIN_IN1, 255);
  ledcWrite(PIN_IN2, 255);
}