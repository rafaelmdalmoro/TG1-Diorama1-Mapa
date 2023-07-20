
#include <stdio.h>
#include <string.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// LED strip pin 
#define LED_PIN    5

// How many NeoPixels are attached to the Arduino
#define LED_COUNT 60

//LED brightness (de 0 a 255)
#define BRIGHTNESS 10

//Colors
#define WHITE strip.Color(255,255,255)
#define PINK strip.Color(255,53,184)
#define RED strip.Color(255,0,0)
#define YELLOW strip.Color(204,103,0)
#define GREEN strip.Color(0,255,0)
#define BLUE strip.Color(0,0,255)
#define CYAN strip.Color(0,255,255)
#define PURPLE strip.Color(128,0,128)

// LED strip object
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = strip type flags


// Project Variables
String index; //LED position 
int goal = 60;
int pos = 60;
int x;
int y;
//LED set in map
int set[5][12] = {{36,37,38,39,40,41,42,43,44,45,46,47},
                  {35,34,33,32,31,30,29,28,27,26,25,24},
                  {12,13,14,15,16,17,18,19,20,21,22,23},
                  {11,10,9,8,7,6,5,4,3,2,1,0}
                 };
String msg; //message received from the bt
char msg_c; // message color or code 
char msg_m; // message mode
uint32_t color; //color definition

//Project Functions
void splitMessage();
void setLocation();
void setColor();
void brain();
void orientation();
void accentuation();
void updatePosition();
void split();


void setup() {
 // Code to support the Adafruit Trinket 5V 16 MHz.
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif
 //setup LED strip
  strip.begin();           // INITIALIZE NeoPixel strip object
  strip.show();            // Turn OFF all pixels
  strip.setBrightness(BRIGHTNESS); // Set BRIGHTNESS

  //setupBT
  Serial.begin(9600);
  //randomSeed generator
  randomSeed(analogRead(A6));
}

void loop() {
  //main function
  if(Serial.available() > 0){
        //read bt data
        msg = Serial.readString();
        Serial.println("Message:" + msg); //DEBUG
        // Separate data in message blocks
        splitMessage();
        Serial.println(msg_c); //DEBUG
        Serial.println(index); //DEBUG
        Serial.println(msg_m); //DEBUG
        Serial.flush(); //Limpa buffer do BT
        if(msg_m == '1'){
          Serial.println("Mode: Coordenates"); //DEBUG
          setColor(); //set color based on bt data
          setLocation(); //set LED position based on bt data
        }else if(msg_m == '2'){
          Serial.println("Mode: Nervous System"); //DEBUG
          nervousSystem();  //Select Nervous system function      
        }else if(msg_m == '3'){
          Serial.println("Mode: Orientation"); //DEBUG
          orientation();      //set for orientation mode  
        }else if(msg_m == '4'){
          Serial.println("Mode: Accentuation"); //DEBUG
          accentuation();     //set for accentuation mode   
        }
        delay(150); //Small delay to simplify bt reading
        while(Serial.available() > 0) Serial.read(); // flushes buffer
    }    
}

// Separate data in message blocks
void splitMessage(){
  String aux;
  aux = split(msg, '|',0);
  msg_c = aux[0]; // color
  index = split(msg, '|',1); // position
  aux = split(msg, '|',2);
  msg_m = aux[0];  //mode
}
// Sets Strip Color
void setColor(){ 
  Serial.println(msg_c);
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

// Set location of index on Map Mode
void setLocation(){  
  strip.clear();
  strip.setPixelColor((int)index.toInt(), color);
  strip.show();
}
// Set nervous system function to run
void nervousSystem(){
  Serial.println(msg_c);
  switch(msg_c){
    case 'B':
      brain();
    break;
    case 'T':
      thought();
    break;
    case 'R':
      armR();
    break;
    case 'L':
      armL();
    break;
    case 'D':
      dorsal();
    break;
    case 'C':
      cervical();
    break;
    case 'K':
     // torso();
    break;
    case 'I':
      //impulse();
    break;
  }
  
}

// brain function on nervous system
void brain(){
  color = CYAN;
  int index;
  for(int i = 0; i<15; i++){
    index = random(22, 27);
    if(index == 26) index = 47;
    strip.setPixelColor(index, color);
    strip.show();
    delay(80);
    strip.clear();
  }
  for(int i = 26; i<34; i++){
    strip.setPixelColor(i, color);
    if(i > 24){
      strip.setPixelColor(44, color);
      strip.setPixelColor(20, color);
      strip.show();
      delay(80);
      strip.setPixelColor(i+25, color);
      strip.setPixelColor(i-23, color);
    }
    strip.show();
    delay(80);   
  }    
  for(int i = 14; i>11; i--){
    strip.setPixelColor(i, color);
    strip.setPixelColor(i+24, color);
    strip.show();
    delay(80);   
  }
  delay(3000);
  strip.clear();
  strip.show();
}
// Right Arm function on nervous system
void armR(){
  color = PINK;
  int index = random(8, 12);
  strip.clear(); 
  for(int i = 0; i<8; i++){
    strip.setPixelColor(index, color);
    strip.show();
    delay(100);
    strip.clear();
    strip.show();
    delay(100);
  }
  for(int i = index; i>2; i--){
    strip.setPixelColor(i, color);
    strip.show();
    delay(80);   
  }
  strip.setPixelColor(20, color);
  for(int i = 27; i>23; i--){
    strip.setPixelColor(i, color);
    strip.show();
    delay(80);   
  }
  delay(3000);
  strip.clear();
  strip.show();
}
// Left Arm function on nervous system
void armL(){
  color = PINK;
  int index = random(55, 59);
  strip.clear(); 
  for(int i = 0; i<8; i++){
    strip.setPixelColor(index, color);
    strip.show();
    delay(100);
    strip.clear();
    strip.show();
    delay(100);
  }
  for(int i = index; i>50; i--){
    strip.setPixelColor(i, color);
    strip.show();
    delay(80);   
  }
  strip.setPixelColor(44, color);
  for(int i = 27; i>23; i--){
    strip.setPixelColor(i, color);
    strip.show();
    delay(80);   
  }
  delay(3000);
  strip.clear();
  strip.show();
}
//thought function on nervous system
void thought(){
  color = GREEN;
  int index;
  for(int i = 0; i<25; i++){
    index = random(22, 27);
    if(index == 26) index = 47;
    strip.setPixelColor(index, color);
    strip.show();
    delay(80);
    strip.clear();
  }
  delay (1000);
  strip.clear();
  strip.show();
}
// cervical damage function on nervous system
void cervical(){
  color = CYAN;
  int index;
  for(int i = 0; i<15; i++){
    index = random(22, 27);
    if(index == 26) index = 47;
    strip.setPixelColor(index, color);
    strip.show();
    delay(80);
    strip.clear();
  }
  for(int i = 26; i<28; i++){
    strip.setPixelColor(i, color);
    strip.show();
    delay(80);   
  } 
  strip.setPixelColor(27, RED);
  strip.show();   
  delay (3000);
  strip.clear();
  strip.show();
}
// dorsal damage function on nervous system
void dorsal(){
  color = CYAN;
  int index;
  for(int i = 0; i<15; i++){
    index = random(22, 27);
    if(index == 26) index = 47;
    strip.setPixelColor(index, color);
    strip.show();
    delay(80);
    strip.clear();
  }
  for(int i = 26; i<29; i++){
    strip.setPixelColor(i, color);
     if(i > 24){
      strip.setPixelColor(44, color);
      strip.setPixelColor(20, color);
      strip.show();
      delay(80);
      strip.setPixelColor(i+25, color);
      strip.setPixelColor(i-23, color);
    }
    for(int i = 28; i<35; i++){
      strip.setPixelColor(i+25, color);
      strip.setPixelColor(i-23, color);
    } 
    strip.show();
    delay(80);  
  } 
  strip.setPixelColor(29, RED);
  strip.show();   
  delay (3000);
  strip.clear();
  strip.show();
}



//orientation mode
void orientation(){
 if(goal == 60){
    goal = random(0, 59);
    x = random(0, 11);
    y = random(0,4);
    pos = set[y][x];
    if(pos == goal)  goal = random(0, 59);
    strip.clear();
    strip.setPixelColor(goal, YELLOW);
    strip.setPixelColor(pos, BLUE);
    strip.show();
 }else{
   switch(msg_c){
    case 'N':
      pos = set[--y][x];
      updatePosition();
    break;
    case 'S':
      pos = set[++y][x];
      updatePosition();
    break;
    case 'L':
      pos = set[y][++x];
      updatePosition();
    break;
    case 'O':
      pos = set[y][--x];
      updatePosition();
    break;
    case 'T':
      goal = 60;
      resetOrientation();
    break;
   }
 }  
}
// accentuation mode
void accentuation(){
  int letters[] = {36,37,38,39,40,41,42,43,44,45,46, 47};
  switch(msg_c){
    case 'A':
      color = PINK;
    break;
    case 'C':
      color = PURPLE;
    break;
    case 'T':
      color = YELLOW;
    break;
    case 'S':
      color = WHITE;
      accentuationResults();
    break;
  }
  strip.clear();
  strip.setPixelColor(letters[((int)index.toInt())-1], color);
  strip.show();
  delay(80);  
}

// AUXILIARY FUNCTIONS

void updatePosition(){ // for orientation mode
    strip.clear();
    strip.setPixelColor(goal, YELLOW);
    strip.setPixelColor(pos, BLUE);
    strip.show();
    if(pos==goal){
      strip.clear();
      strip.setPixelColor(goal, GREEN);
      strip.show();
    }  
  
}

void resetOrientation(){ // for orienation mode
  int north[] = {5,17,18,19,29,42}; //Norh arrow 
  int east[] = {16,29,30,31,32,40}; //East arrow
  int south[] = {53,41,42,43,29,18}; //South arrow
  int west[] = {20,29,28,27,26,44}; //West arrow
  int *p; // arrow
  theaterChase(YELLOW, 50); // LED transition
  int directions = random(0, 4);
  switch(directions){
    case 0:
      p = north;
    break;
    case 3:
      p = south;
    break;
    case 1:
      p = east;
    break;
    case 2:
      p = west;
    break;      
  }
  strip.clear(); // turn off all LEDs
  //print arrow in LEDs
  for(int i = 0; i < 6; i++){
    strip.setPixelColor(p[i], GREEN); 
  }
  strip.show(); //  Update strip to match 
  delay(5000); // time for direction guessing
  orientation();
}


String split(String data, char separator, int index) //for string splitting
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

void accentuationResults(){ // for accentuation functions
  int letters[] = {36,37,38,39,40,41,42,43,44,45,46,47};
  Serial.println(index);
  for(int i = 0; i< 12; i++){
    if( i != (int)index.toInt()-1){
      strip.setPixelColor(letters[i], RED);
      strip.show(); //  Update strip to match 
      delay(600);
    }else{
      strip.setPixelColor(letters[i], GREEN);
      strip.show(); //  Update strip to match 
      delay(1000);
      for(int j = 0; j< 3; j++){
        strip.clear();
        strip.show(); //  Update strip to match 
        delay(400);
        strip.setPixelColor(letters[i], GREEN);
        strip.show(); //  Update strip to match 
        delay(400);
      }
      break;
    }  
  }
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
