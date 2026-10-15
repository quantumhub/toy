/*
Pre-acquisition
Step1: Installation

    Add Board Managers: In File > Preferences, add these two URLs:
    text

    https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
    https://raw.githubusercontent.com/ricardoquesada/esp32-arduino-lib-builder/master/bluepad32_files/package_esp32_bluepad32_index.json

Step2: Install Board (Note that it is not a Library): In Boards Manager, install esp32 (version 2.0.6). Then in Tools > Board, select "ESP32 Dev Module" under the Bluepad32 section.
*/

#include <Bluepad32.h>

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// Called when a controller connects
void onConnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      myControllers[i] = ctl;
      Serial.printf("Controller connected at index %d\n", i);
      return;
    }
  }
}

// Called when a controller disconnects
void onDisconnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      myControllers[i] = nullptr;
      Serial.printf("Controller disconnected from index %d\n", i);
      return;
    }
  }
}

void setup() {
  Serial.begin(115200);
  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys(); // Clear old pairings (optional)
}

void loop() {
  BP32.update();
  for (auto ctl : myControllers) {
    if (ctl && ctl->isConnected()) {
      // Read joysticks (range: -512 to 511)
      int16_t axisX = ctl->axisX();
      int16_t axisY = ctl->axisY();
      int16_t axisRX = ctl->axisRX();
      int16_t axisRY = ctl->axisRY();

      // Read triggers
      int throttle = ctl->throttle(); // R2
      int brake = ctl->brake();       // L2

      // Check buttons
      if (ctl->a()) Serial.println("Cross pressed");
      if (ctl->b()) Serial.println("Circle pressed");

      Serial.printf("L-Stick: %d,%d | R-Stick: %d,%d | R2:%d L2:%d\n",
                    axisX, axisY, axisRX, axisRY, throttle, brake);
    }
  }
  delay(20);
}