// 1. Install the library
//   Arduino IDE → Library Manager → Search ESP32Encoder (by madhephaestus) → Install

#include <ESP32Encoder.h>

// === Pin configuration (change to your pins) ===
#define ENC_A_PIN   4     // Optical sensor Channel A
#define ENC_B_PIN   5     // Optical sensor Channel B

ESP32Encoder encoder;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("ESP32-S3 Quadrature Encoder (PCNT) started");

  // Enable internal pull-ups (most optical sensors are open-collector)
  ESP32Encoder::useInternalWeakPullResistors = puType::up;

  // Full quadrature mode (counts on all 4 edges → highest resolution)
  encoder.attachFullQuad(ENC_A_PIN, ENC_B_PIN);

  // Optional: half quadrature or single edge
  // encoder.attachHalfQuad(ENC_A_PIN, ENC_B_PIN);
  // encoder.attachSingleEdge(ENC_A_PIN, ENC_B_PIN);

  encoder.clearCount();   // start from zero
}

void loop() {
  int64_t position = encoder.getCount();   // signed 64-bit position

  Serial.printf("Position: %lld\n", position);

  delay(50);   // adjust as needed
}