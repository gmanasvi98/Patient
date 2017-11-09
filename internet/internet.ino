#include <SoftwareSerial.h>
#define RX_PIN 2
#define TX_PIN 3
#define ESP_BRATE 115200

SoftwareSerial esp8266(RX_PIN, TX_PIN);


//Setting server with id of patient 13
String server = "192.168.43.125";
String uri = "/api/patients/patient/09";


void setup()
{
  Serial.begin(9600);
  esp8266.begin(ESP_BRATE); // I changed this
  connectWifi();
}

void loop()
{
 httppost();
 delay(1000);
}
void connectWifi() {
  String cmd2 = "AT+CWJAP=\"GiONEE F103\",\"12111997\"";
  String cmd1 = "AT+RST";
  Serial.println("Resetting ESP Module..");
  esp8266.println(cmd1);
  delay(2000);
  Serial.println("Connecting to wifi..");
  esp8266.println(cmd2);
  delay(2000);
}

void httppost(){
  Serial.println("Connected to wifi..");
  String data="{\"patient_status\": \"help\"}";
  esp8266.println("AT+CIPSTART=\"TCP\",\"" + server + "\",3000");
  delay(2000);
  if( esp8266.find("OK")) {
    Serial.println("TCP connection ready");
    }
  delay(1000);
  String postRequest ="POST " + uri + " HTTP/1.0\r\n" +"Host: " + server + "\r\n" +"Accept: *" + "/" + "*\r\n" +"Content-Length: " + data.length() + "\r\n" 
  +"Content-Type: application/json; charset=utf-8\r\n" +"\r\n" + data;
  String sendCmd = "AT+CIPSEND=";
  Serial.println("Sending AT+CIPSEND....");
  esp8266.print(sendCmd);
  delay(2000);
  esp8266.println(postRequest.length() );
  delay(7000);
  if(esp8266.find(">")) { 
    delay(1000);
    Serial.println("Sending..");
    esp8266.print(postRequest);
    delay(5000);
    if( esp8266.find("SEND OK")) 
    { 
        Serial.println("Packet sent");
        delay(1000);
        while (esp8266.available()) 
        {
          String tmpResp = esp8266.readString();
          Serial.println(tmpResp);
          delay(500);
          }
        // close the connection
    esp8266.println("AT+CIPCLOSE");
    delay(60000);
    }
  }
}


