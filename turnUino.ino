/************************************************************************************
* turnUino - Modell railway turnout controller with ESP8266 and PCA9685 for Arduino *
* author: Weisz Robert                                                              *
* link: https://github.com/wrobi/turnUino                                           *
* version: v1.0 - Initial version                                 date: 2022.03.01  *
* version: v1.1 - Stored states                                   date: 2022.03.02  *
************************************************************************************/
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>        // https://github.com/me-no-dev/ESPAsyncTCP
#include <ESPAsyncWebServer.h>  // https://github.com/me-no-dev/ESPAsyncWebServer
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <FS.h>

#define MIN_PULSE_WIDTH 650
#define MAX_PULSE_WIDTH 2350
#define FREQUENCY 50

// WiFi settings
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

AsyncWebServer webServer(80);

Adafruit_PWMServoDriver pwmA = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwmB = Adafruit_PWMServoDriver(0x41);

char states[33] = "00000000000000000000000000000000";
/*------------------------------------------------------------------------------
  function setServo - set posotion of servo
  @param id     - id of servo (eg:a12
  @param value  - angle
*/
void setServo(String id, int value) {
  String board = id.substring(0,1);
  int port = id.substring(1,id.length()).toInt();
  
  if (board == "a") {
    pwmA.setPWM(port, 0, angleToPulse(value));
  } else {
    pwmB.setPWM(port, 0, angleToPulse(value));
  }
}
/*------------------------------------------------------------------------------
  function angleToPulse - convert angle to PWM pulses for servo
  @param angle  - angle in degree (0 - 180)
*/
int angleToPulse(int angle){
  int pulse_wide, pulse;
  pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  pulse = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  return pulse;
}
/*------------------------------------------------------------------------------
  function getValue - get Nth item from separated string
  @param data   - input string
*/
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
/*------------------------------------------------------------------------------
  function notFound - 404 error
*/
void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}
/*------------------------------------------------------------------------------
  function setup
*/
void setup(){
  Serial.begin(115200);
  
  pwmA.begin();
  pwmA.setPWMFreq(FREQUENCY);
  pwmB.begin();
  pwmB.setPWMFreq(FREQUENCY);

  SPIFFS.begin();

  loadData();                 //open /data.js and set all servo to default angle
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi.");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false);
  });
  webServer.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
  webServer.on("/data.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/data.js", "text/javascript");
  });
  webServer.on("/jquery-3.6.0.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/jquery-3.6.0.min.js", "text/javascript");
  });
  webServer.on("/bootstrap-input-spinner.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bootstrap-input-spinner.js", "text/javascript");
  });
  webServer.on("/background.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/background.png", "image/png");
  });
  webServer.on("/turn0.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/turn0.png", "image/png");
  });
  webServer.on("/turn1.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/turn1.png", "image/png");
  });
  webServer.on("/gear.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/gear.png", "image/png");
  });

  webServer.on("/set", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if (request->hasParam("id")) {
      String id = request->getParam("id")->value();
      if (request->hasParam("index")) {
        int index = request->getParam("index")->value().toInt();
        if (request->hasParam("state")) {
          int state = request->getParam("state")->value().toInt();
          states[index] = (state==0) ? '0' : '1';                //storing state
        }
      }
      if (request->hasParam("value")) {
        int value = request->getParam("value")->value().toInt();
        setServo(id,value);        
      }
    }
    request->send(200, "text/plain", "OK");
  });
  
  webServer.on("/getstate", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", states);
  });
  
  webServer.on("/save", HTTP_GET, [] (AsyncWebServerRequest *request) {
    int statusCode;
    if (request->hasParam("value")) {
      statusCode = saveData(request->getParam("value")->value());
    }
    request->send(statusCode, "text/plain", "OK");
  });

  webServer.onNotFound(notFound);

  webServer.begin();
}
/*------------------------------------------------------------------------------
  function loadData - load /data.js from SPIFFS and set servos to default angle
*/
void loadData() {
  File file = SPIFFS.open("/data.js", "r");
  if (!file) {
    Serial.println("Error opening file for reading");
    return;
  }
  int line = 0;
  while (file.available()) {
    String buffer = file.readStringUntil('\n');
    line++;
    if (line > 2) {                                   //first 2 lines are header
      int i = buffer.indexOf('"');
      if(i > 0) {
        String id = buffer.substring(i+1,buffer.indexOf('"',i+1));
        int value = getValue(buffer,',',5).toInt();
        setServo(id,value);           //set all servos to default angle (angle0)
      }
    }
  }
  file.close();
}
/*------------------------------------------------------------------------------
  function saveData - save datas to data.js on SPIFFS
  @param data - contains data array toString() (eg:a0,1016,220...)
  @return statusCode
*/
int saveData(String data) {
  int statusCode;
  String buffer = "var data = [\n";
  buffer += "//  id\t\t,Xpos\t,Ypos\t,Rotate\t,Mirror\t,Angle0 ,Angle1 ,Enable\n";
  for (int j=0; j<32; j++) {
    buffer += "  [";
    for (int i=0; i<8; i++) {
      if (i == 0) { 
        buffer += "'";
      }
      buffer += getValue(data,',',j*8+i);
      if (i == 0) { 
        buffer += "' ";
      }
      if (i < 7) { 
        buffer += "\t,";
      }
    }
    buffer += "\t]";
    if (j < 31) { 
      buffer += ",\n";
    } else {
      buffer += "\n";          
    }
  }
  buffer += "];";
  File file = SPIFFS.open("/data.js", "w");
  if (!file) {
//    Serial.println("Error opening file for writing");
    return 512;
  }
  int bytesWritten = file.print(buffer);
  if (bytesWritten > 0) {
//    Serial.println("data.js was written");
    statusCode = 200;
  } else {
//    Serial.println("File write failed");
    statusCode = 513;
  }
  file.close();
  return statusCode;
}

void loop() {
  
}
