#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
 
#define BOARD_I2C_SCL   6
#define BOARD_I2C_SDA   5      
 
void displayWelcome(float distanceCm);
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ BOARD_I2C_SCL, /* data=*/ BOARD_I2C_SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display
 


const int trigPin = 8; // Trigger pin
const int echoPin = 9; // Echo pin

#define SOUND_SPEED 0.034 // cm/µs
//#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
//float distanceInch;

void setup() {
Serial.begin(115200);
// init oled
Serial.println("Init u8g2 ....");
u8g2.setFont( u8g2_font_wqy12_t_gb2312 );//Set Chinese Font
u8g2.begin();
u8g2.enableUTF8Print();
displayWelcome(0.0);               /* Display Welcome Message */

delay(100);
// init sound ranging
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
}

void loop() {
// Clear trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Send a 10µs HIGH pulse
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Read echo time
duration = pulseIn(echoPin, HIGH);

// Calculate distance
distanceCm = duration * SOUND_SPEED / 2;
//distanceInch = distanceCm * CM_TO_INCH;

// Output results
Serial.print("Distance (cm): ");
Serial.println(distanceCm);
displayWelcome(distanceCm);
//Serial.print("Distance (inch): ");
//Serial.println(distanceInch);

delay(500);
}

 
void displayWelcome(float distanceCm) {
  //char str[10];// = ">123450000.";
  char diststr[10];
  //diststr[8]='\r';
  //char *str1;
  dtostrf(distanceCm, 6, 2, diststr);
  //for (int ii=0;ii<6;ii++)
  //  str[ii]=diststr[ii];
  u8g2.clearBuffer();
  /* Center Alignment */
  u8g2.drawUTF8( u8g2.getDisplayWidth() / 2 - u8g2.getUTF8Width( diststr ) / 2, u8g2.getDisplayHeight() / 2 + u8g2.getMaxCharHeight() / 2, diststr );
  //u8g2.setCursor(1, 0);
  //u8g2.print(diststr);
  u8g2.sendBuffer();
  //u8g2.display();
}