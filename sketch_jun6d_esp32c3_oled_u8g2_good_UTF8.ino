#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
 
#define BOARD_I2C_SCL   6
#define BOARD_I2C_SDA   5      
 
void displayWelcome();
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ BOARD_I2C_SCL, /* data=*/ BOARD_I2C_SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display
 
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Init u8g2 ....");
 
 
  u8g2.setFont( u8g2_font_wqy12_t_gb2312 );// Set to Chinese Font
  u8g2.begin();
  u8g2.enableUTF8Print();
  displayWelcome();               /* Display Welcome Message */
 
  delay(100);
}
 
void loop() {
    Serial.println("u8g2 showing chars ....");
    delay(1000);
}
 
 
 
void displayWelcome() {
  char *str = ">正在启动..";
  u8g2.clearBuffer();
  /* Center Alignment of String */
  u8g2.drawUTF8( u8g2.getDisplayWidth() / 2 - u8g2.getUTF8Width( str ) / 2, u8g2.getDisplayHeight() / 2 + u8g2.getMaxCharHeight() / 2, str );
  u8g2.sendBuffer();
}