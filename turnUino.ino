#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>        // https://github.com/me-no-dev/ESPAsyncTCP
#include <ESPAsyncWebServer.h>  // https://github.com/me-no-dev/ESPAsyncWebServer
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <FS.h>

#define MIN_PULSE_WIDTH 650
#define MAX_PULSE_WIDTH 2350
#define DEFAULT_PULSE_WIDTH 1500
#define FREQUENCY 50

// WiFi settings
const char* ssid = "fym";
const char* password = "WeiszPatrik21";

 const char* PARAM_MESSAGE = "value";

AsyncWebServer webServer(80);

Adafruit_PWMServoDriver pwmA = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwmB = Adafruit_PWMServoDriver(0x41);

int angleToPulse(int angle){
  int pulse_wide, pulse;
  pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  pulse = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  return pulse;
}

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

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup(){
  Serial.begin(115200);
  
  pwmA.begin();
  pwmA.setPWMFreq(FREQUENCY);
  pwmB.begin();
  pwmB.setPWMFreq(FREQUENCY);

  SPIFFS.begin();
  
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
    String message;
    if (request->hasParam(PARAM_MESSAGE)) {
      message = request->getParam(PARAM_MESSAGE)->value();
      int sep = message.indexOf("_");
      String port = message.substring(0,1);
      int id = message.substring(1,sep).toInt();
      int value = message.substring(sep+1, message.length()).toInt();
      if (port == "a") {
        pwmA.setPWM(id, 0, angleToPulse(value));
      } else {
        pwmB.setPWM(id, 0, angleToPulse(value));
      }
      Serial.println("Set port:"+port+" id:"+String(id)+" value:"+String(value));
    } else {
      Serial.println("No message sent");
    }
    request->send(200, "text/plain", "OK");
  });

  webServer.on("/save", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String message;
    if (request->hasParam(PARAM_MESSAGE)) {
      message = request->getParam(PARAM_MESSAGE)->value();
      String buffer = "var data = [\n";
      buffer += "//  id\t\t,Xpos\t,Ypos\t,Rotate\t,Mirror\t,Angle0 ,Angle1 ,Enable\n";
      for (int j=0; j<32; j++) {
        buffer += "  [";
        for (int i=0; i<8; i++) {
          if (i == 0) { 
            buffer += "'";
          }
          buffer += getValue(message,',',j*8+i);
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
      Serial.println(buffer);
      File file = SPIFFS.open("/data.js", "w");
      if (!file) {
        Serial.println("Error opening file for writing");
        return;
      }
      int bytesWritten = file.print(buffer);
      if (bytesWritten > 0) {
        Serial.println("data.js was written");
        Serial.println(bytesWritten);
      } else {
        Serial.println("File write failed");
      }
      file.close();      
    }
    request->send(200, "text/plain", "OK");
  });
  
  webServer.onNotFound(notFound);
      
  webServer.begin();
}
  
void loop() {
  
}
