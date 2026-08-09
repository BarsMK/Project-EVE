#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); //starts the OLED up

#define TRIG_PIN 5
#define ECHO_PIN 18
#define SELECT_BUTTON 27
#define SERVO_PIN 13
#define MOVE_BUTTON 26

long duration;
float distance; 
int mode = 0;
int menuChoice = 1;
int angle = 30;
unsigned long lastScanTime = 0;
int scanDirection = 1;

Servo scanServo;

// Shows EVE startup screen
// bootScreen is to simplify the code so it doesn't all jumble up and be a very large capacity, same for statusScreen and scanScreen
void bootScreen() {
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setTextSize(1);

  display.drawRect(12,2,100,60,WHITE);


  display.setCursor(30,15);
display.println("PROJECT EVE");

display.drawLine(13,27,110,27,WHITE);

 display.setCursor(30,35);
 display.println("BOOTING...");

 display.display();
  
}
void bootAnimation() {

  // Frame 1
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setTextSize(1);

  display.drawRect(12,2,100,60,WHITE);

  display.setCursor(30,15);
  display.println("PROJECT EVE");

  display.drawLine(13,27,110,27,WHITE);

  display.setCursor(30,35);
  display.println("BOOTING.");

  display.display();

  delay(500);


  // Frame 2
  display.clearDisplay();

  display.drawRect(12,2,100,60,WHITE);

  display.setCursor(30,15);
  display.println("PROJECT EVE");

  display.drawLine(13,27,110,27,WHITE);

  display.setCursor(30,35);
  display.println("BOOTING..");

  display.display();

  delay(500);


  // Frame 3
  display.clearDisplay();

  display.drawRect(12,2,100,60,WHITE);

  display.setCursor(30,15);
  display.println("PROJECT EVE");

  display.drawLine(13,27,110,27,WHITE);

  display.setCursor(30,35);
  display.println("BOOTING...");

  display.display();

  delay(500);

}


// Shows the status of EVE 
void statusScreen() {
 display.clearDisplay();

display.setTextColor(WHITE);
  display.setTextSize(1);

  display.drawRect(12,2,100,62,WHITE);


  display.setCursor(30,15);
display.println("EVE STATUS");

display.drawLine(13,27,110,27,WHITE);

 display.setCursor(20,32);
 display.println("SYS: ONLINE");

 display.setCursor(20,42);
 display.println("SERVO: READY");

display.setCursor(20,52);
 display.println("SONAR: READY");

 display.display();

}
// Shows that EVE is scanning
void scanScreen() {
 display.clearDisplay();

display.setTextColor(WHITE);
  display.setTextSize(1);

  display.drawRect(12,2,100,60,WHITE);


  display.setCursor(40,15);
display.println("EVE SCAN");

display.drawLine(13,27,110,27,WHITE);

 display.setCursor(35,35);
 display.println("SEARCHING...");

 display.display();

}

void scanAnimation() {

// Frame 1
 display.clearDisplay();

display.setTextColor(WHITE);
  display.setTextSize(1);

  display.drawRect(12,2,100,60,WHITE);


  display.setCursor(40,15);
display.println("EVE SCAN");

display.drawLine(13,27,110,27,WHITE);

 display.setCursor(35,35);
 display.println("SEARCHING.");

 display.display();

delay(500);


// Frame 2
 display.clearDisplay();

display.setTextColor(WHITE);
  display.setTextSize(1);

  display.drawRect(12,2,100,60,WHITE);


  display.setCursor(40,15);
display.println("EVE SCAN");

display.drawLine(13,27,110,27,WHITE);

 display.setCursor(35,35);
 display.println("SEARCHING..");

 display.display();

 delay(500);

 // Frame 3
  display.clearDisplay();

display.setTextColor(WHITE);
  display.setTextSize(1);

  display.drawRect(12,2,100,60,WHITE);


  display.setCursor(40,15);
display.println("EVE SCAN");

display.drawLine(13,27,110,27,WHITE);

 display.setCursor(35,35);
 display.println("SEARCHING...");

 display.display();

 delay(500);
 }


 void readDistance() {

digitalWrite(TRIG_PIN, LOW);
delayMicroseconds(2);

digitalWrite(TRIG_PIN, HIGH);
delayMicroseconds(10);
digitalWrite(TRIG_PIN, LOW);

duration = pulseIn(ECHO_PIN, HIGH);
distance = duration * 0.034 / 2;

Serial.println(duration);
Serial.println(distance);
 }

void distanceScreen() {

  display.clearDisplay();
  display.drawRect(12,2,100,60,WHITE);
    display.setCursor(40,15);
  display.println("EVE SCAN");
  display.drawLine(13,27,110,27,WHITE);
display.setCursor(20, 35);
display.println("Angle: ");
display.setCursor(73, 35);
display.println(angle);

  display.setCursor(20,45);
    display.println("Distance: ");

  display.setCursor(73,45);   
  display.println(distance);
  display.display();

}


void menuScreen() {

display.clearDisplay();

display.setTextColor(WHITE);
display.setTextSize(1);

display.drawRect(12,2,100,60,WHITE);
display.setCursor(35, 15);
display.println("EVE MENU");
display.drawLine(13,27,110,27,WHITE);

if (menuChoice == 1) {

display.setCursor(15, 35);
display.println(">");

}

if (menuChoice == 2) {

display.setCursor(15, 47);
display.println(">");

}


display.setCursor(25, 35);
display.println("SCAN");
display.setCursor(25, 47);
display.println("STATUS");

display.display();


}


void openSelect() {

if (menuChoice == 1) {

mode = 1;
  
}


if (menuChoice == 2) {

mode = 2;
  
}





}



void runMode() {

if (mode == 0) {

  menuScreen();
}

if (mode == 1) {

if (millis() - lastScanTime >= 500) {

scanServo.write(angle);

readDistance();
distanceScreen();

if (angle == 150) {

scanDirection = -1;

}

if (angle == 30) {

scanDirection = 1; 

}

angle += (10 * scanDirection);
  lastScanTime = millis();
}



}

if (mode == 2) {

statusScreen();


 }

}

void checkButtons() {
if (digitalRead(SELECT_BUTTON) == LOW) {

  if (mode == 0) {
  openSelect();

}

else if (mode == 1) {
  mode = 0;

}

else if (mode == 2) {
  mode = 0;

}


  delay(500);
}

if (digitalRead(MOVE_BUTTON) == LOW) {

if (menuChoice == 1) {

menuChoice = 2;


}

else if (menuChoice == 2) {

menuChoice = 1;


}
delay(1000);
}

}




// Starts EVE
void setup() {
  // put your setup code here, to run once:
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
pinMode(TRIG_PIN, OUTPUT);
pinMode(ECHO_PIN, INPUT);
pinMode(SELECT_BUTTON, INPUT_PULLUP);
pinMode(MOVE_BUTTON, INPUT_PULLUP);

Serial.begin(115200);

scanServo.attach(SERVO_PIN);
scanServo.write(30);


 bootScreen();
 delay(1000);
 bootAnimation();
 delay(500);

}

void loop() {
  // put your main code here, to run repeatedly:


checkButtons();
runMode();

}
 
