/*
 * ESP32-S3 + DRV8833 + Closed-loop position control for a DC motor with feedback (servo modification)
 * Feedback: 5kΩ potentiometer, wiper output 0–3.3V
 * Control: Proportional control (P control), can be further tuned to PID
 */

// ==================== Pin Definitions ====================
#define AIN1_PIN        7
#define AIN2_PIN        8
#define STBY_PIN        11
#define ERR_PIN 10
#define FEEDBACK_PIN    12   // ADC1_CH6

// ==================== PWM Parameters ====================
#define PWM_FREQ        20000   // 20 kHz, beyond human hearing range, suitable for motor drive
#define PWM_RESOLUTION  8       // 8-bit, duty cycle 0–255
#define PWM_MAX         255

// ==================== Feedback and Angle Parameters ====================
#define ADC_RESOLUTION  12      // Use 12-bit ADC (0–4095)
#define ADC_MAX         4095
#define VOLTAGE_MAX     3.3f    // ADC full-scale voltage (11dB attenuation is about 3.1V; approximated here as 3.3V)

// Mechanical angle range (modify according to actual servo travel)
#define ANGLE_MIN       0.0f
#define ANGLE_MAX       180.0f

// Calibration: measured ADC raw values of the potentiometer at ANGLE_MIN and ANGLE_MAX
// Run the calibration program first, and fill in the adcRaw printed by Serial here
#define ADC_AT_ANGLE_MIN    50
#define ADC_AT_ANGLE_MAX    4050

// ==================== Control Parameters ====================
float Kp = 2.5f;            // Proportional gain; start small then increase to avoid oscillation
float deadband = 4.0f;      // Angle deadband (degrees), reduces jitter

// ==================== Global Variables ====================
float targetAngle = 135.0f; // Target angle, can be modified
float currentAngle = 0.0f;
int   motorPWM = 0;


// Serial input buffer
char inputBuffer[16];
uint8_t inputIndex = 0;
// ==================== Motor Drive ====================
void motorDrive(int pwm) {
  // pwm > 0 forward, pwm < 0 reverse, pwm = 0 stop
  pwm = constrain(pwm, -PWM_MAX, PWM_MAX);

  if (pwm > 0) {
    ledcWrite(AIN1_PIN, pwm);
    ledcWrite(AIN2_PIN, 0);
  } else if (pwm < 0) {
    ledcWrite(AIN1_PIN, 0);
    ledcWrite(AIN2_PIN, -pwm);
  } else {
    ledcWrite(AIN1_PIN, 0);
    ledcWrite(AIN2_PIN, 0);
  }
}

// ==================== Read Feedback Angle ====================
float readAngle() {
  int adcRaw = analogRead(FEEDBACK_PIN);
  // Map ADC raw value to angle range
  float angle = map(adcRaw, ADC_AT_ANGLE_MIN, ADC_AT_ANGLE_MAX,
                    (int)ANGLE_MIN, (int)ANGLE_MAX);
  return constrain(angle, ANGLE_MIN, ANGLE_MAX);
}
// ==================== Check Serial Input ====================
void checkSerialInput() {
  while (Serial.available() > 0) {
    char c = Serial.read();

    // On newline or carriage return, parse the received string
    if (c == '\n' || c == '\r') {
      if (inputIndex > 0) {
        inputBuffer[inputIndex] = '\0';          // String terminator
        float newAngle = atof(inputBuffer);      // Convert to float

        if (newAngle >= ANGLE_MIN && newAngle <= ANGLE_MAX) {
          targetAngle = newAngle;
          Serial.print(">>> Target angle set to: ");
          Serial.print(targetAngle);
          Serial.println("°");
        } else {
          Serial.print(">>> Invalid angle! Range: ");
          Serial.print(ANGLE_MIN);
          Serial.print(" - ");
          Serial.println(ANGLE_MAX);
        }
        inputIndex = 0;                          // Clear buffer
      }
    } else {
      // Accumulate characters, prevent overflow
      if (inputIndex < sizeof(inputBuffer) - 1) {
        inputBuffer[inputIndex++] = c;
      }
    }
  }
}
// ==================== Initialization ====================
void setup() {
  Serial.begin(115200);

  // Configure STBY
  pinMode(STBY_PIN, OUTPUT);
  pinMode(ERR_PIN, INPUT);
  digitalWrite(STBY_PIN, HIGH);   // Enable DRV8833

  // Configure LEDC PWM
  ledcAttach(AIN1_PIN, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(AIN2_PIN, PWM_FREQ, PWM_RESOLUTION);

  // Configure ADC
  analogReadResolution(ADC_RESOLUTION);
  analogSetAttenuation(ADC_11db);   // 11dB attenuation, can measure about 0–3.1V

  delay(500);
  Serial.println("=== ESP32-S3 DRV8833 Closed-Loop Motor ===");
}

// ==================== Main Loop ====================
void loop() {
  // 1. Handle keyboard input
  checkSerialInput();
  // 1. Read current angle
  currentAngle = readAngle();
int adcRaw = analogRead(FEEDBACK_PIN);
  // 2. Calculate error
  float error = targetAngle - currentAngle;

  // 3. Proportional control (can add integral and derivative to form PID)
  if (abs(error) < deadband) {
    motorPWM = 0;                 // Stop within deadband to prevent jitter
  } else {
    //motorPWM = (int)(Kp * error);
    //motorPWM = constrain(motorPWM, -PWM_MAX, PWM_MAX);
    if (error>0){
      motorPWM=PWM_MAX;
    }else{
      motorPWM=-PWM_MAX;
    }

  }

  // 4. Drive motor
  motorDrive(motorPWM);

  // 5. Debug output (once every 100 ms)
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 100) {
    lastPrint = millis();
    Serial.print("Target: ");
    Serial.print(targetAngle);
    Serial.print("°  Current: ");
    Serial.print(currentAngle);
    Serial.print("°  Current: ");
    Serial.print(adcRaw);
    Serial.print("°  Error: ");
    Serial.print(error);
    Serial.print("°  PWM: ");
    Serial.println(motorPWM);
  }

  delay(10);   // Control period about 10 ms
}