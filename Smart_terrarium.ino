#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>


const byte ROWS = 2;
const byte COLS = 2; 
char keys[ROWS][COLS] = {
  {'a','b'},
  {'c','d'},
};
byte rowPins[ROWS] = {4, 5,}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7,}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define PHOTO_SENSOR_PIN A1
#define SCREEN_WIDTH 124
#define SCREEN_HEIGHT 32
#define WATER_LEVEL_PIN A6
#define SOIL_MOISTURE A0
#define PUMP 13
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


int SOIL_MOISTURE_LIMIT = 20;
int SOIL_MOISTURE_VALUE;
int WATER_LEVEL_VALUE;
long duration;
int distance;
int lt_lvl;
char key;

void setup() {
  Serial.begin(9600);
  pinMode(PUMP, OUTPUT);
  digitalWrite(PUMP, LOW);
  // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  delay(2000);         
  oled.clearDisplay();
  oled.setTextSize(1);          // text size
  oled.setTextColor(WHITE);
  oled.setCursor(0,10);
  oled.print("welcome");
  oled.display();
}

void loop() {  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; 
  SOIL_MOISTURE_VALUE=analogRead(SOIL_MOISTURE);
  SOIL_MOISTURE_VALUE=map(SOIL_MOISTURE_VALUE,550,10,0,100);
  if (SOIL_MOISTURE_VALUE<SOIL_MOISTURE_LIMIT) {
  digitalWrite(PUMP, HIGH); 
  }
  else{
  digitalWrite(PUMP, LOW); 
  }
  key = keypad.getKey();
  oled.setTextSize(1);          // text size
  oled.setTextColor(WHITE);
  WATER_LEVEL_VALUE = analogRead(WATER_LEVEL_PIN); 
  if (WATER_LEVEL_VALUE<=100){
    oled.clearDisplay();
    oled.setCursor(0,10);
    oled.print("WATER LEVEL LOW");
    oled.display();
    }
  else{
  oled.clearDisplay();
  Serial.println(key);
  if (key=='d'){
  oled.clearDisplay(); // clear display
  oled.setCursor(0, 10);        // position to display
  SOIL_MOISTURE_VALUE=analogRead(SOIL_MOISTURE);
  oled.println("Soil moisture: "); // text to display
  oled.print(SOIL_MOISTURE_VALUE);
  oled.print(" bars");
  oled.display(); 
  }
  if (key == 'b'){
  oled.clearDisplay();
  oled.setCursor(0,10);
  WATER_LEVEL_VALUE = analogRead(WATER_LEVEL_PIN);
  oled.println("pump water level: ");
  oled.print(WATER_LEVEL_VALUE);
  oled.print(" cm");
  oled.display();  
  }
  if (key == 'c'){
  Serial.println("heloooo");
  lt_lvl = analogRead(PHOTO_SENSOR_PIN);
  oled.clearDisplay();
  oled.setCursor(0,10);
  oled.println("Light level: ");
  oled.print(lt_lvl);
  oled.print(" lux");
  oled.display();  
  }
  if (key == 'a'){
     if (distance<=0.5){
      oled.clearDisplay();
     oled.setCursor(0,10);
     oled.println("Ready for snipping");
     oled.display();
      }
      else{
        oled.clearDisplay();
     oled.setCursor(0,10);
     oled.println("Not ready for snipping");
     oled.display();
        }
    } 
  }
}
