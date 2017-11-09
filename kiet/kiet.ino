

//Sensor inputs 
double alpha=0.75;
int period=20;
double refresh=0.0;
int c=0;

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
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  digitalWrite(13,HIGH);
  pinMode(DHpin, OUTPUT);
  pinMode(A0,INPUT);// pulse semsor
}
void loop() {

  
  // ye part panic button ka h 
  int x =  analogRead(5);
  Serial.println(x);
  delay(1000);
  if(x>1000)
  {c++;}
  if(c==3)
  {Serial.println ("help needed");}

  
   // ye part pulse ka hai

   static double oldValue=0;
   static double oldrefresh=0;
 
   int beat=analogRead(A0);
  
   double value=alpha*oldValue+(0-alpha)*beat;
   refresh=value-oldValue;
   Serial.print(" Heart Monitor "); 
   Serial.print("          ");
   Serial.print(beat/10);
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
  }
