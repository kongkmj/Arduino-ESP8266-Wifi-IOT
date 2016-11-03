#include <SoftwareSerial.h>
#define SSID "[YOUR_SSID]"
#define PASS "[YOUR_PASSWORD]"
#define IP "184.106.153.149" // Put your IP
#define POST "POST /receiver.php HTTP/1.1\r\n"
#define HOST "Host: esp8266.allaboutee.com\r\n"
#define CONTENT_TYPE "Content-Type: application/x-www-form-urlencoded\r\n"
#define CONTENT_LEN  "Content-Length:"

#define HTTP_WAIT    6000
SoftwareSerial monitor(10, 11); // RX, TX

char ok[] = "OK";
char error[] = "ERROR";
char greaterThan[] = ">";

void setup() {
  // put your setup code here, to run once:
  monitor.begin(9600);
  Serial.begin(9600);
  Serial.setTimeout(HTTP_WAIT);  
  sendDebug("AT");
  delay(5000);
  if(Serial.find(ok)){
    monitor.println("RECEIVED: OK");
    bool rc = connectWiFi();
    if (rc == false)
       monitor.print("RECEIVED: Wifi Connect Error");
  }  
}

void loop() {
  // put your main code here, to run repeatedly:

  String apiData  = "Build your api data here"; 
  HTTPRequest(apiData);
  delay(600);
  HTTPResponse();
  delay(600);  
}


void HTTPRequest (String data){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  sendDebug(cmd);
  delay(2000);
  if(Serial.find(error)){
    monitor.print("RECEIVED: Error");
    return;
  }
  cmd  = POST;
  cmd += HOST;
  cmd += CONTENT_TYPE;
  cmd += CONTENT_LEN;
  cmd += data.length();
  cmd += "\r\n\r\n";
  cmd += data;
  cmd += "\r\n\r\n";  
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  delay (500);
  if(Serial.find(greaterThan)){
    monitor.print(">");
    monitor.print(cmd);
    Serial.print(cmd);
  }else{
    sendDebug("AT+CIPCLOSE");
  }
  if(Serial.find(ok)){
    monitor.println("RECEIVED: OK");
  }else{
    monitor.println("RECEIVED: Error");
  }
  delay(1000);
}

// Listen to the HTTP response now
void HTTPResponse(){
  String response = "";  
  while (Serial.available()){
     char c = Serial.read();
     monitor.write(c);
     if ( c == '\n'){
        responseHandling(response);
     }else
        response += c; 
  }
}

void sendDebug(String cmd){
  monitor.print("SEND: ");
  monitor.println(cmd);
  Serial.println(cmd);
} 

 
bool connectWiFi(){
  Serial.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  sendDebug(cmd);
  delay(5000);
  if(Serial.find(ok)){
    monitor.println("RECEIVED: OK");
    return true;
  }else{
    monitor.println("RECEIVED: Error");
    return false;
  }
}

void responseHandling (String response){

  
}

