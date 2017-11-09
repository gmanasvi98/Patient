
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <SoftwareSerial.h>
#define RX_PIN 2
#define TX_PIN 3
#define ESP_BRATE 115200

SoftwareSerial esp8266(RX_PIN, TX_PIN);


boolean connect = true;

//Setting server with id of patient 13
String server = "192.168.43.125";
String uri = "/api/patients/patient/09";
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Sensor inputs 
double alpha=0.75;
int period=20;
double refresh=0.0;
int c=0;
String status = "help";
String heart_beat = "68";
String temprature = "35";

//Digital pins
int DHpin = 8; // input/output pin
byte dat[5];   
//--------------------------------------------------------------------------------------------------------------------------------------------------------
byte read_data()
{
  byte data;
  for(int i = 0; i < 8; i++)
  {
    if(digitalRead(DHpin) == LOW)
    {
      while(digitalRead(DHpin) == LOW); // wait 50us;
      delayMicroseconds(30); //The duration of the high level is judged to determine whether the data is '0' or '1';
      if(digitalRead(DHpin) == HIGH)
        data |= (1<<(7 - i)); //High in the former, low in the post;
      while(digitalRead(DHpin) == HIGH); //Data '1', waiting for the next bit of reception;
    }
  }
  return data;
}

void start_test()
{
  digitalWrite(DHpin, LOW); //Pull down the bus to send the start signal;
  delay(30); //The delay is greater than 18 ms so that DHT 11 can detect the start signal;
  digitalWrite(DHpin, HIGH);
  delayMicroseconds(40); //Wait for DHT11 to respond;
  pinMode(DHpin, INPUT);
  while(digitalRead(DHpin) == HIGH);
  delayMicroseconds(80); //The DHT11 responds by pulling the bus low for 80us;
  if(digitalRead(DHpin) == LOW);
    delayMicroseconds(80); //DHT11 pulled up after the bus 80us to start sending data;
  for(int i = 0; i < 4; i++) //Receiving temperature and humidity data, check bits are not considered;
    dat[i] = read_data();
  pinMode(DHpin, OUTPUT);
  digitalWrite(DHpin, HIGH); //After the completion of a release of data bus, waiting for the host to start the next signal
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
  // put your setup code here, to run once:
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Serial.begin(9600);
  esp8266.begin(ESP_BRATE); // I changed this
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  pinMode(13, OUTPUT);
  digitalWrite(13,HIGH);
  pinMode(DHpin, OUTPUT);
  pinMode(A0,INPUT);// pulse semsor
  connectWifi();
}
void loop() {
  
  if(!connect){
    connect = true;
    //---------------------------------------------------------------------------------------------------------
    // ye part panic button ka h 
    int x =  analogRead(5);
    Serial.println(x);
    delay(1000);
    if(x>1000)
    {c++;}
    if(c>0)
    {
      Serial.println ("help needed");
      status = "help";
      }
  
    
     // ye part pulse ka hai
  
     static double oldValue=0;
     static double oldrefresh=0;
   
     int beat=analogRead(A0);
    
     double value=alpha*oldValue+(0-alpha)*beat;
     refresh=value-oldValue;
     Serial.print(" Heart Monitor "); 
     Serial.print("          ");
     Serial.print(beat/10);
     heart_beat= beat /10 + " ";
     oldValue=value;
     oldrefresh=refresh;
     delay(period*10);

      
    //ye part temp sensor ka h 
      start_test();
      Serial.print("Current humdity = ");
      Serial.print(dat[0], DEC); //Displays the integer bits of humidity;
      Serial.print('.');
      Serial.print(dat[1], DEC); //Displays the decimal places of the humidity;
      Serial.println('%');
      Serial.print("Current temperature = ");
      Serial.print(dat[2], DEC); //Displays the integer bits of temperature;
      Serial.print('.');
      Serial.print(dat[3], DEC); //Displays the decimal places of the temperature;
      Serial.println('C');
      delay(700);

      temprature = "221";
      
  }
    //---------------------------------------------------------------------------------------------------------
    httppost();
    delay(1000);
  }

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
  String data="{\"patient_status\": \"" + status+ "\",\"patient_heartbeat\": \""+ heart_beat +"\",\"patient_temp\": \""+ temprature +"\"}";
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

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
