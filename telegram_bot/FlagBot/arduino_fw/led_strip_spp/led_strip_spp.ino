#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

/*
#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI 
extern uint8_t I2C_SLA;
*/
#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(51, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

Servo servo_flag;
int rainbow_delay = 20;
bool rainbow_mode = false;
int strip_Color[3];

String command;

/*
char *lcdText;

#define message_width 20
#define message_height 20
static unsigned char message_bits[] U8G_PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x0f,
   0x01, 0x00, 0x08, 0x03, 0x00, 0x0c, 0x05, 0x00, 0x0a, 0x09, 0x00, 0x09,
   0x11, 0x80, 0x08, 0x21, 0x40, 0x08, 0xc1, 0x30, 0x08, 0xb1, 0xd9, 0x08,
   0x09, 0x06, 0x09, 0x05, 0x00, 0x0a, 0x03, 0x00, 0x0c, 0x01, 0x00, 0x08,
   0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  

  u8g.setFont(u8g_font_unifont);
  u8g.setScale2x2();
  //u8g.setFont(u8g_font_osb21);
  if (I2C_SLA == 0x078){
    lcdText = "Tienes 0 mensajes";
    //u8g.drawStr(0,10,lcdText);
    for (int i=0; i<20; i++){
      u8g.drawXBMP( i, 0, message_width, message_height, message_bits);
      u8g.setHiColorByRGB(0,0,0);
      /*
      for (u8g_uint_t h=0; h<128; h++){
        for (u8g_uint_t j=0; j<64; j++){
          u8g_DrawPixel(u8g, h, j);
        }
      }
      */
      /*
      delay(100);
    }
  }
  else if (I2C_SLA == 0x07a){
    u8g.drawStr( 0, 22, "Julius");
  }

}

*/


void setup()
{
  servo_flag.attach(5);
  servo_flag.write(90);
 
  
  // This is for Trinket 5V 16MHz, you can remove these three lines ifeyou are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
  strip.begin();

  strip.show(); // Initialize all pixels to 'off'

  Serial.begin(19200);

  Serial.println("---- Wellcome to control board ----");
  Serial.println("Available commands:");
  Serial.println("STRIP_COLOR:255,255,255;");
  Serial.println("RAINBOW:ms_delay;");
  Serial.println("SERVO:180;");
  Serial.println("SERVO_WAVE:4");
  Serial.println("RANDOM:ms_delay;");
  Serial.println("STRIP_BAR_COLOR:0,15,255,240,189;");
  Serial.println("---- No more for now! ----");



}

void loop()
{

    if(Serial.available()){
        char c = Serial.read();

        if(c == '\n'){
          parseCommand(command);
          command = "";
        }
        else command += c;
    }

    if(rainbow_mode){
      servo_flag.detach();
      rainbow(rainbow_delay);
      servo_flag.attach(5);
    }

    //fire();

  //imprimeEn(0x07a);
  //imprimeEn(0x078);


}

void parseCommand(String com)
{
  String part1;
  String part2;

  part1 = com.substring(0,com.indexOf(":"));
  part2 = com.substring(com.indexOf(":")+1,com.indexOf(";"));

  if(part1.equalsIgnoreCase("STRIP_COLOR"))
  {
    //-- STRIP_COLOR:255,255,255;

    rainbow_mode = false;

    servo_flag.detach();     
    for(int i=0; i<3; i++){

      strip_Color[i] = getValuesFromCommand(part2);
    }

    //Print colorwipe:
    colorWipe(strip.Color(strip_Color[0], strip_Color[1] , strip_Color[2] ), 0, true);
    servo_flag.attach(5);

  }

  else if(part1.equalsIgnoreCase("RAINBOW"))
  {
    //-- RAINBOW:ms_delay;

    rainbow_delay = getValuesFromCommand(part2);
    rainbow_mode = true;

  }
  else if(part1.equalsIgnoreCase("RANDOM"))
  {
    //-- RANDOM:ms_delay;

    rainbow_mode = false;
    random_pixels(getValuesFromCommand(part2));


  }
  else if(part1.equalsIgnoreCase("SERVO"))
  {
    //-- SERVO:180;

    servo_flag.write(getValuesFromCommand(part2));

  }
  else if(part1.equalsIgnoreCase("SERVO_WAVE"))
  {
    //-- SERVO_WAVE:4;

    rainbow_mode = false;

    int waves = getValuesFromCommand(part2);
    strip.Color(255, 255, 255);
    colorWipe(strip.Color(255, 255, 255), 0, true); // White

    for(int i=0; i< waves ; i++){

      servo_flag.write(0);
      colorWipe(strip.Color(0, 255, 0), 12, false); // Green
      //delay(80);
      servo_flag.write(180);
      colorWipe(strip.Color(255, 0, 0), 12, true); // Red
      //delay(80);

    }

    colorWipe(strip.Color(255, 255, 255), 0, true); // White
    servo_flag.write(90);

  }
  else if(part1.equalsIgnoreCase("STRIP_BAR_COLOR"))
  {
    //-- STRIP_BAR_COLOR:0,15,255,240,189;
    //-- STRIP_BAR_COLOR:index_first_led,index_last_led,R,G,B;

    rainbow_mode = false;

    servo_flag.detach();     

    int index_first_led = getValuesFromCommand(part2);
    int index_last_led = getValuesFromCommand(part2);

    for(int i=0; i<3; i++){
      strip_Color[i] = getValuesFromCommand(part2);
    }
    //Print bar & color:
    show_bar(strip.Color(strip_Color[0], strip_Color[1] , strip_Color[2] ), index_first_led, index_last_led, true);
    servo_flag.attach(5);

  }
  else
  {
    Serial.print("Command not recognized: ");
    Serial.println(com);

  }  

}

int getValuesFromCommand (String& command_values){
  int value = command_values.substring(0,command_values.indexOf(",")).toInt();
  command_values = command_values.substring(command_values.indexOf(",")+1);
  return value;
}

/*
//------------------------ LCD functions:
void imprimeEn(uint8_t address){

I2C_SLA = address;
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(50);
}
*/

//------------------------  Strip functions:

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait, bool dir) {
  int n_pixels = strip.numPixels();
  for(uint16_t i=0; i<n_pixels; i++) {
     if(dir == true)  strip.setPixelColor(i, c);
     else strip.setPixelColor(n_pixels-(i+1), c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
  rainbow_mode = false;
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


void random_pixels(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, random(0, 128), random(0, 128), random(0, 128));
    }
    strip.show();
    delay(wait);
  }
}

void fire (void){

  int r = 255;
  int g = r-100;
  int b = 40;

  for(int x = 8; x <99; x++)
  {
    int flicker = random(0,150);
    int r1 = r-flicker;
    int g1 = g-flicker;
    int b1 = b-flicker;
    if(g1<0) g1=0;
    if(r1<0) r1=0;
    if(b1<0) b1=0;
    strip.setPixelColor(x,r1,g1, b1);
  }
  strip.show();
  delay(random(50,150));

}

void show_bar(uint32_t c, uint8_t index_first, uint8_t index_last, bool dir) {
  for(uint16_t i=index_first; i<=index_last; i++) {
     if(dir == true)  strip.setPixelColor(i, c);
     else strip.setPixelColor(index_last-(i+1), c);
    strip.show();
  }
}
