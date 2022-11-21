#include "WiFi.h"
#include "WebServer.h"

// SSID & Password
const char* ssid = "inxuan";
const char* password = "77777777";
int led1 =16;
int led2 =17;

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

void setup() {
  Serial.begin(115200);
  Serial.println("Try Connecting to ");
  Serial.println(ssid);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);
    ledcSetup(0,2000,8);
    ledcAttachPin(led2,0);
  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  //建立server的路徑
  createWebServer();
  
  //server啟動
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();  //handleClient要寫在loop
}


//----設定HTML資料----
String htmlContent = "\
<html>\
    <head>\
      <style>\
        body{ background: #EFF5F5; display: flex; flex-direction: column; padding-top: 30px;}\
         a { background: #497174; color: #EFF5F5; width: 400px; height: 70px; margin: 15px auto; border-radius:20px; font-family:Arial; font-size: 30pt; text-align: center;line-height: 70px;text-decoration: none;}\
      </style>\
    </head>\
    <body>\
      <a href='/led/on'>LED ON</a>\
      <a href='/led/off'>LED OFF</a>\
      <form method='get' action='/led/setLightBrightness' style='margin: 15px auto; font-family:Arial;'>\
            <label >Led Brightness</label>\
        <input type='number' name='lightBrightness'><input type='submit' value='set' style='margin: 15px auto;'>\
      </form>\
    </body>\
  </html>";

String LEDOpenPage = "\
  <!DOCTYPE html>\
  <html>\
      <head>\
        <style>\
          body{ background-color: #EFF5F5; display: flex; flex-direction: column;}\
          h1 { color: #EB6440; font-family: Arial, Helvetica, sans-serif; margin: 20px auto; font-size: 50pt;}\
          a { background-color: #497174; color: #EFF5F5; width: auto; height: 50px; margin: 20px auto; padding: 0 20px; border-radius:20px; font-family: Arial, Helvetica, sans-serif; font-size: 15pt; line-height: 50px; text-decoration: none;}\
        </style>\
      </head>\
      <body>\
        <h1>LED ON</h1>\
        <a href='/'>GO BACK</a>\
      </body>\
    </html>";

String LEDCLOSEPage = "\
  <!DOCTYPE html>\
   <html>\
    <head>\
        <style>\
          body{ background-color: #EFF5F5; display: flex; flex-direction: column;}\
          h1 { color: #EB6440; font-family: Arial, Helvetica, sans-serif; margin: 20px auto; font-size: 50pt;}\
          a { background-color: #497174; color: #EFF5F5; width: auto; height: 50px; margin: 20px auto; padding: 0 20px; border-radius:20px; font-family: Arial, Helvetica, sans-serif; font-size: 15pt; line-height: 50px; text-decoration: none;}\
        </style>\
    </head>\
    <body>\
      <h1>LED OFF</h1>\
      <a href='/'>GO Back</a>\
    </body>\
  </html>";
  
//------設定路徑------
void createWebServer()
{
  //初始頁面
  server.on("/", []() {
      server.send(200, "text/html", htmlContent);
  });
  server.on("/led/on", []() {
    Serial.println("LED On");
    digitalWrite(led1,HIGH);
    delay(1000);
    //add your code here
    
    String response = "{\"LED On Success\"}";
    server.send(200, "text/html", LEDOpenPage);
  });
  server.on("/led/off", []() {
    Serial.println("LED Off");
    digitalWrite(led1,LOW);
    delay(1000);
    //add your code here
    
    String response = "{\"LED Off Success\"}";
    server.send(200, "text/html", LEDCLOSEPage);
  });
  server.on("/led/setLightBrightness", []() {
    //以server.arg&引數名稱，取得form傳來的資料
    int lightBrightness = server.arg("lightBrightness").toInt();
    //判斷條件  
      
    if(lightBrightness>=0 && lightBrightness<=1024){
      Serial.print("Set Brightness: ");
      Serial.println(lightBrightness);
      ledcWriteTone(0,lightBrightness);
        /*void loop(){
        digitalWrite(led2,HIGH);
      }*/


      //add your code here
      
      String response = "Set Brightness: " + (String)lightBrightness;
      server.send(200, "application/json", response);
    } 
    else {
      String response = "{\"Error\":\"over range 0-1024\"}";
      server.send(404, "application/json", response);
    }
  });
}
