
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

String inString;   //  Make global
long bitt = 0;   //  Make global
long rssi =0;
int counter = 0;
/*
  U8g2lib Example Overview:
    Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
    Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
    U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.
  This is a page buffer example for oLED.
  Some fonts:
    // u8g2.setFont(u8g2_font_ncenB14_tr);
    // 11 Pixel height font
    //u8g2.setFont(u8g2_font_ncenB10_tr);
    // 14 Pixel font
    //u8g2.setFont(u8g2_font_profont22_mf);
    // 16 Pixel font
    //u8g2.setFont(u8g2_font_crox3hb_tn);
    // 49 Pixel Height
    //u8g2.setFont(u8g2_font_fur49_tn);
    //u8g2.drawStr(20, 55, b);
    // 62 Pixel Height
    u8g2.setFont(u8g2_font_logisoso62_tn);
*/

// Please UNCOMMENT one of the contructor lines below
// U8g2 Contructor List (Picture Loop Page Buffer)
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
//U8G2_NULL u8g2(U8G2_R0);  // null device, a 8x8 pixel display which does nothing
//U8G2_SSD1306_128X64_NONAME_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 12, /* dc=*/ 4, /* reset=*/ 6); // Arduboy (Production, Kickstarter Edition)
//U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_SSD1306_128X64_NONAME_1_3W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* reset=*/ 8);
//U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SSD1306_128X64_ALT0_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // same as the NONAME variant, but may solve the "every 2nd line skipped" problem
//U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* reset=*/ 8);
U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

// End of constructor list.RDF removed a lot of these.

void setup(void) {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial1.begin(9600);

  /* while (!Serial) {
    //   ; // wait for serial port to connect. Needed for native USB port only
    // }

    while (!Serial1) {
     ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.print("Wake-up USB!");
    delay(1000); // We wait 1 second
  */

  //Test oled
  u8g2.begin();
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0, 15, "DATA RX");
  } while ( u8g2.nextPage() );
  delay(1000);

}
