// RGB pixel WS2812 on ESP32S3 board

#include <Adafruit_NeoPixel.h>

#define LED_PIN     48      // Change this! Common values: 48, 38, 21, 2, 8...
#define NUM_LEDS    1
#define BRIGHTNESS  40

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Hello World");

  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();
}

void loop() {
  static uint16_t hue = 0;

  strip.setPixelColor(0, strip.gamma32(strip.ColorHSV(hue * 182))); // 65536/360 ≈ 182
  strip.show();

  hue += 3;
  if (hue >= 360) hue = 0;

  delay(20);
}