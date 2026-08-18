#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebServer.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const char* ssid = "WIFI_NAME";
const char* password = "WIFI_PASSWORD"; 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); //starts the OLED up

#define TRIG_PIN 5
#define ECHO_PIN 18
#define SELECT_BUTTON 27
#define SERVO_PIN 13
#define MOVE_BUTTON 26
#define RESCAN_BUTTON 23

long duration;
float distance; 
float distancemem = 200;
int anglemem = 30;
int mode = 0;
int menuChoice = 1;
int angle = 30;
int centerX = 64;
int centerY = 60;
unsigned long lastScanTime = 0;
int scanDirection = 1;
String objectStatus;
bool scanComplete = false;
bool insideObject = false;
int objectStartAngle = 0;
int radarRadius = 55;
int maxRadarDistance = 50;
float scanDistances[121];
unsigned long scanTimes[121];
Servo scanServo;
WebServer webServer(80);

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
readDistance();

 display.clearDisplay();

display.setTextColor(WHITE);
  display.setTextSize(1);

  display.drawRect(12,2,100,62,WHITE);


  display.setCursor(30,7);
display.println("EVE STATUS");

display.drawLine(13,18,110,18,WHITE);

 display.setCursor(15,21);
 display.println("SYS: ONLINE");

 display.setCursor(15,29);
 display.print("SERVO: ");

 display.setCursor(55, 29);
 display.print(angle);

display.setCursor(75, 29);
display.println((char)247);

display.setCursor(15,38);
 display.println("SONAR: ");

 display.setCursor(55, 38);
 display.print(distance, 1);
 display.println("cm");

 display.setCursor(15, 46);
 display.print("TARGET: ");

 display.setCursor(60, 46);
 display.print(distancemem, 1);

 display.setCursor(83, 46);
 display.println("cm");

display.setCursor(15, 55);
display.println("ANGLE: ");

display.setCursor(55, 55);
display.println(anglemem);

display.setCursor(72, 55);
display.println((char)247);


 display.display();

}


void targetLockScreen() {

display.clearDisplay();

display.setTextColor(WHITE);
display.setTextSize(1);

  display.drawRect(12,2,100,62,WHITE);

display.setCursor(25, 8);
display.println("TARGET LOCKED");

display.drawLine(15,20,112,20, WHITE);

display.setCursor(20, 23);
display.print("DIST: ");
display.print(distancemem, 1);
display.println(" cm");

display.setCursor(20, 33);
display.print("ANGLE: ");
display.print(anglemem);
display.println((char)247);

display.setCursor(20, 43);
display.print("LOCK: ");
display.println("ACTIVE");

display.setCursor(40, 53);
display.print("[TARGET]");

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

duration = pulseIn(ECHO_PIN, HIGH, 30000);

if (duration > 0) {

distance = duration * 0.034 / 2;

// Serial.println(duration);
// Serial.println(distance);
}
else {

  distance = 0;
}

 }

void distanceScreen() {

  display.clearDisplay();
  display.drawRect(12,2,100,60,WHITE);
    display.setCursor(40,10);
  display.println("EVE SCAN");
  display.drawLine(13,20,110,20,WHITE);
display.setCursor(20, 24);
display.println("Angle: ");
display.setCursor(63, 24);
display.println(angle);

  display.setCursor(20,32);
    display.println("Distance: ");

  display.setCursor(73,32);   
  display.println(distance);

  display.setCursor(20, 40);
  display.println("Closest: ");

  display.setCursor(73, 40);
  display.println(distancemem);

display.setCursor(20, 50);
display.println("STATUS: ");


display.setCursor(73, 50);
display.println(objectStatus);


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
scanComplete = false;
distancemem = 200;
anglemem = 30;
angle = 30;
scanDirection = 1;
for (int i = 0; i < 121; i++) {
scanDistances[i] = 0;
scanTimes[i] = 0;

}

}


if (menuChoice == 2) {

mode = 2;
  
}





}


void completedScanScreen() {
display.clearDisplay();

display.setTextColor(WHITE);
display.setTextSize(1);

display.drawRect(12,2,100,60,WHITE);

display.setCursor(20, 10);
display.println("SCAN COMPLETE");

  display.drawLine(13,20,110,20,WHITE);

  display.setCursor(20, 30);
  display.println("Closest: ");

    display.setCursor(70, 30);
  display.println(distancemem);

  
  display.setCursor(20, 40);
  display.println("Angle: ");

    display.setCursor(70, 40);
  display.println(anglemem);

    display.setCursor(25, 50);
  display.println("TARGET FOUND");

  display.display();
}


void runMode() {

if (mode == 0) {

  menuScreen();
}

if (mode == 1) {

if (millis() - lastScanTime >= 20) {

if (!scanComplete) {

scanServo.write(angle);

readDistance();

int scanIndex = angle - 30;

if (duration > 0) {
scanDistances[scanIndex] = distance;
scanTimes[scanIndex] = millis();
}

if (distance > 100) {
objectStatus = "CLEAR";

}

else if (distance > 40) {
objectStatus = "FAR";

}

else if (distance > 20) {
objectStatus = "NEAR";

}

else {
objectStatus = "CLOSE";

}

if (duration > 0) {
if (distance < distancemem) {

distancemem = distance;
anglemem = angle;

}
}

if (angle == 150) {
scanDirection = -1;
scanComplete = true;
scanServo.write(anglemem);
completedScanScreen();
delay(2000);
}

else {
radarScreen();
}


if (angle == 30) {

scanDirection = 1; 

}


angle += (1 * scanDirection);
  lastScanTime = millis();
}

else {
targetLockScreen();

}

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
delay(500);
}

if (digitalRead(RESCAN_BUTTON) == LOW) {

scanComplete = false;
distancemem = 200;
anglemem = 30;
angle = 30;
scanDirection = 1;
insideObject = false;

for (int i = 0; i < 121; i++) {
  scanDistances[i] = 0;
  scanTimes[i] = 0;


}

scanServo.write(angle);

}

}

void radarScreen() {
display.clearDisplay();

display.setTextSize(1);
display.setTextColor(WHITE);

for (int a = 0; a <= 150; a += 2) {
float r = a * PI / 180.0;

int arcX = centerX + radarRadius * cos(r);
int arcY = centerY - radarRadius * sin(r);

display.drawPixel(arcX, arcY, WHITE);
}
display.drawLine(0, centerY, 127, centerY, WHITE);
display.drawLine(centerX, 0, centerX, 63, WHITE);

float radarDistance = (distance / maxRadarDistance) * radarRadius;

float radians = angle * PI / 180.0;
int objectX;
int objectY;

int lineX;
int lineY;

lineX = centerX + radarRadius * cos(radians);
lineY = centerY - radarRadius * sin(radians);

display.drawLine(centerX, centerY, lineX, lineY, WHITE);

if (duration > 0) {

  objectX = centerX + radarDistance * cos(radians);
objectY = centerY - radarDistance * sin(radians);

display.fillCircle(objectX, objectY, 1, WHITE);
}

for (int i = 0; i < 121; i++) {

int storedAngle = i + 30;
float storedDistance = scanDistances[i];
unsigned long age = millis() - scanTimes[i];

float storedRadians = storedAngle * PI / 180.0;
float storedRadarDistance = (storedDistance / maxRadarDistance) * (radarRadius - 3);
int storedX;
int storedY;

storedX = centerX + storedRadarDistance * cos(storedRadians);
storedY = centerY - storedRadarDistance * sin(storedRadians);

if (storedDistance > 5 && storedDistance <= 50 && age < 2500) {
 int dotSize = 1;

display.fillCircle(storedX, storedY, dotSize, WHITE);

}

for (int i = 0; i < 120; i++) {
float currentDistance = scanDistances[i];
float nextDistance = scanDistances[i + 1];
if (currentDistance > 0 && nextDistance > 0) {

float distanceDifference =
  abs(currentDistance - nextDistance);

if (distanceDifference >= 10) {

  if (nextDistance < currentDistance && !insideObject) {

insideObject = true;

objectStartAngle = i + 30;
}
}

if (nextDistance > currentDistance && insideObject) {
insideObject = false;

int objectEndAngle = i + 30;

int objectCenterAngle = 
  (objectStartAngle + objectEndAngle) / 2;

  float objectDistance = scanDistances[objectCenterAngle - 30];

  float objectRadians = objectCenterAngle * PI / 180.0;

  float objectRadarDistance = 
    (objectDistance / maxRadarDistance) * radarRadius;

  int targetX = 
    centerX + objectRadarDistance * cos(objectRadians);

  int targetY = 
    centerY - objectRadarDistance * sin(objectRadians);


display.fillCircle(targetX, targetY, 3, WHITE);



}

}
}
}

if (distancemem < 200) {

float targetRadians = anglemem * PI / 180.0;

float targetRadarDistance =
 (distancemem / maxRadarDistance) * radarRadius;

int savedTargetX = 
  centerX + targetRadarDistance * cos(targetRadians);

int savedTargetY =
  centerY - targetRadarDistance * sin(targetRadians);

  display.fillCircle(savedTargetX, savedTargetY, 3, WHITE);

}

display.display();
}

void handleData() {

  String data = "{";

  data += "\"distance\":";
  data += String(distance, 1);

  data += ",\"angle\":";
  data += String(angle);

  data += ",\"targetDistance\":";
  data += String(distancemem, 1);

  data += ",\"targetAngle\":";
  data += String(anglemem);

  data += "}";

webServer.send(200, "application/json", data);

}

void connectWiFi() {
WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
  Serial.print(".");
  delay(500);

}
Serial.println("WiFi connected");

Serial.println(WiFi.localIP());

webServer.on("/", handleRoot);
webServer.on("/data", handleData);
webServer.begin();

}

void handleRoot() {
String webpage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Project EVE</title>
  
  <meta name="viewport" content="width=device-width, initial-scale=1>

  <style>
    body {
      font-family: Arial;
      text-align: center;
      background: #111;
      color: white
    }

    .box {
      background: #222;
      padding: 20px;
      margin: 15px auto;
      max-width: 400px;
      border-radius: 15px;
    }

    h1 {
      margin-bottom: 25px;
    }

    .value {
      font-size: 24px;
      margin: 10px;
    }
  </style>
  </head>

  <body>

    <h1>PROJECT EVE</h1>

    <div class= "box">

    <h2>SYSTEM STATUS</h2>
    <div>ONLINE</div>

    </div>

    <div class="box">

      <h2>SCAN</h2>

      <div class="value">
      Distance: <span id="distance">--</span> cm
      </div>

      <div class="value">
      Angle: <span id="angle">--</span>&deg;
    </div>

  </div>

  <div class="box">

    <h2>TARGET</h2>

    <div class="value">
     Distance: <span id="targetDistance">--</span> cm
    </div>

    <div class="value">
      Angle: <span id="targetAngle">--</span>&deg;
    </div>

  </div>

<script>

function updateData() {

  fetch('/data')
    .then(response => response.json())
    .then(data => {

      document.getElementById("distance").innerHTML = data.distance;

      document.getElementById("angle").innerHTML = data.angle;

      document.getElementById("targetDistance").innerHTML =
        data.targetDistance;

      document.getElementById("targetAngle").innerHTML =
        data.targetAngle;

    });

}

updateData();

setInterval(updateData, 500);

</script>

    </body>
    </html>
  )rawliteral";

webServer.send(200, "text/html", webpage);

}



// Starts EVE
void setup() {
  // put your setup code here, to run once:
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
pinMode(TRIG_PIN, OUTPUT);
pinMode(ECHO_PIN, INPUT);
pinMode(SELECT_BUTTON, INPUT_PULLUP);
pinMode(MOVE_BUTTON, INPUT_PULLUP);
pinMode(RESCAN_BUTTON, INPUT_PULLUP);

Serial.begin(115200);
connectWiFi();

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
webServer.handleClient();

}
 
