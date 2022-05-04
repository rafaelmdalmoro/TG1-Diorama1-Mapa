
#include <stdio.h>
#include <string.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 60

//BRILHO da luz (de 0 a 255)
#define BRIGHTNESS 10

//CORES
#define WHITE strip.Color(255,255,255)
#define PINK strip.Color(255,53,184)
#define RED strip.Color(255,0,0)
#define YELLOW strip.Color(204,103,0) // MELHORAR
#define GREEN strip.Color(0,255,0)
#define BLUE strip.Color(0,0,255)
#define CYAN strip.Color(0,255,255)
#define PURPLE strip.Color(128,0,128)

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

SoftwareSerial bt(2,3);
String index;
String msg;
char msg_c; // message color or code 
char msg_m; // message mode
uint32_t color;


void setup() {
  // put your setup code here, to run once:
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.
  //setup LED strip
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS); // Set BRIGHTNESS to about 1/5 (max = 255)

  //setupBT
  bt.begin(9600);
  Serial.begin(9600);
  randomSeed(analogRead(A6));
}

void loop() {
//  if(bt.available() > 0) {
//        msg = bt.readString();
//        Serial.println(msg);
//        splitMessage();
//        if(msg_m == '1'){
//          setColor();
//          setLocation();
//        }else if(msg_m == '2'){
         impulse();        
//        }
//        bt.flush();
//        delay(150);
//    }
    
}
void splitMessage(){
  String aux;
  aux = split(msg, '|',0);
  msg_c = aux[0];
  index = split(msg, '|',1);
  aux = split(msg, '|',2);
  msg_m = aux[0];
//  Serial.print("Cor: ");
//  Serial.println(msg_c);
//  Serial.println("Index: " + index);    
}

void setLocation(){  
  strip.clear();
  strip.setPixelColor((int)index.toInt(), color);
  strip.show();
}

void setColor(){  
  switch(msg_c){
    case 'W':
      color = WHITE;
    break;
    case 'P':
      color = PINK;
    break;
    case 'R':
      color = RED;
    break;
    case 'Y':
      color = YELLOW;
    break;
    case 'G':
      color = GREEN;
    break;
    case 'B':
      color = BLUE;
    break;
    case 'C':
      color = CYAN;
    break;
    case 'O':
      color = PURPLE;
    break;
    default:
      color = GREEN;
    break;
  }
}

void impulse(){
  color = CYAN;
  int index;
//  for(int i = 22; i<=26; i++){
//    if (i == 26) i = 47;
//    strip.setPixelColor(i, color);
//    strip.show();
//    delay(500);
//    strip.clear();
//  }
  for(int i = 0; i<15; i++){
    index = random(22, 27);
    if(index == 26) index = 47;
    strip.setPixelColor(index, color);
    strip.show();
    delay(80);
    strip.clear();
  }
  for(int i = 26; i<33; i++){
    strip.setPixelColor(i, color);
    strip.show();
    delay(80);   
    strip.clear(); 
  }
  delay (1000);
}

String split(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}


// Some functions of our own for creating animated effects -----------------

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    strip.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
