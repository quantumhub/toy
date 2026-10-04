const int ledPin = 8; // Onboard LED

void setup() {
Serial.begin(115200);
pinMode(ledPin, OUTPUT);
}

void loop() {
digitalWrite(ledPin, HIGH); // LED OFF (inverted)
Serial.println("LED OFF");
delay(1000);
digitalWrite(ledPin, LOW); // LED ON
Serial.println("LED ON");
delay(1000);
}