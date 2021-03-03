//This Video Link https://youtu.be/0dbws1i2GoE
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SPI.h>
#include <MFRC522.h>
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YOUR BLYNK AUTH CODE";
 
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "WIFI NAME";
char pass[] = "WIFI PASSWORD";
 char server[] = "SERVER IP ADDRESS"; 
#define DHTPIN 0          // D3
int flag = 0 ; 
WiFiClient client;
// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301
 
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
 
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
  int flamesensor = digitalRead(D0);
  if(flamesensor ==0){
    Blynk.notify("FIRE");
    Blynk.virtualWrite(V7, flamesensor);
    delay(750);
    flamesensor = 0;
    digitalWrite(D6,HIGH);
    digitalWrite(D5,LOW); // green d5 red d6
  }
  else if(flamesensor==1){
   digitalWrite(D5,HIGH);
    digitalWrite(D6,LOW); 
  }
  Blynk.virtualWrite(V7, flamesensor);
 

//  void Sending_To_phpmyadmindatabase()   //CONNECTING WITH MYSQL
if(h>85||V7==0){
   if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    delay(5000);
    Serial.print("GET /testcode/dht11.php?humidity=");
    client.print("GET /testcode/dht11.php?humidity=");     //YOUR URL
    Serial.println(h);
    client.print(h);
    client.print("&temperature=");
    Serial.println("&temperature=");
    client.print(t);
    Serial.println(t);
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: "+server);
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
 }

}

 
void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);
 
  dht.begin();
 pinMode(D0,INPUT);
 pinMode(D5,OUTPUT);
 pinMode(D6,OUTPUT);
  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
  
}
// .
void loop()
{
  Blynk.run();
  timer.run();
}


