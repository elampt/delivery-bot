#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

#define RXD1 16
#define TXD1 17

const char* ssid = "elampt";
const char* password = "elam2002";

unsigned long last_time = 0;
unsigned long timer_delay = 10000;

String json_array;
String robotmessage = "";
String posturl = "D";

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600,SERIAL_8N1, RXD1, TXD1);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WIFI...");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("First set of readings will appear after 10 seconds");
  if(WiFi.status()== WL_CONNECTED){
  String server = "http://192.168.207.161:4000/route";
  json_array = GET_Request(server.c_str());
  Serial.println(json_array);
  Serial1.print(json_array);}
}

void loop() {
    while (!Serial1.available()){}
    while(Serial1.available()){
      robotmessage = Serial1.readString();
      if (robotmessage=="Obastacle Detected"){
      Serial.println("Obstacle Detected in ESP32");  
    } 

      else if(robotmessage=="A" or robotmessage=="B" or robotmessage=="C" or robotmessage=="D" or robotmessage=="E" or robotmessage=="F" or robotmessage=="G" or robotmessage=="H" or robotmessage=="I")
      {
        Serial.println("Recieved Location: "+robotmessage);
        sendPostRequest(robotmessage);
      }
    }
    
}   
//  String readfromarduino = Serial1.readString();
//  if readfromarduino!= ""{
//    ESP.reset()
//  }
//}


String GET_Request(const char* server) {
  HTTPClient http;    
  http.begin(server);
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  return payload;
}

void sendPostRequest(String posturl) {
  HTTPClient http;

  // Set the target URL of your Node.js server
  http.begin("http://192.168.207.161:4000/map");

  // Set the content type header
  http.addHeader("Content-Type", "application/json");

  // Prepare the payload for the POST request
  String sendval = "{\"location\": \""+posturl+"\"}";
  //String sendval = "{\"location\": \"B\"}";
  Serial.print(sendval);

  // Send the POST request and obtain the response
  int httpResponseCode = http.POST(sendval);

  // Check if the POST request was successful
//  if (httpResponseCode > 0) {
//    String response = http.getString();
//    Serial.println("Response: " + response);
//  } else {
//    Serial.println("Error on sending POST request: " + String(httpResponseCode));
//  }

  // Close the connection
  http.end();
}
