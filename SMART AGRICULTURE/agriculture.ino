//This Video Link https://youtu.be/0dbws1i2GoE

#define BLYNK_PRINT Serial 
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).

char auth[] = "BLYNK AUTH CODE";


char ssid[] = "WIFI NAME";
char pass[] = "WIFI PASSWORD";

int moisturePercentage;
const int moisturePin = A0;
const int motorPin = D0;  

#define DHTPIN 0          // D3
 
// !...Uncomment whatever type you're using...!

#define DHTTYPE DHT11     // DHT 11

// #define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
// #define DHTTYPE DHT21   // DHT 21, AM2301
 
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
 
// In the app, Widget's reading frequency should be set to PUSH. This means that you define how often to send data to Blynk App.

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
 
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
   Serial.print("Humidity : ");
   Serial.print(h);
   Serial.println("g/m3"); 
 
   Serial.print("Temperature : ");
   Serial.print(t);
   Serial.println("degree Celcius");
   
   Blynk.virtualWrite(V5, t);
   Blynk.virtualWrite(V6, h);
}
int flag=0;

void notifySoil()
{

   float  output_value= (100-((analogRead(A0)/1023)*100));
   Serial.print("Mositure : ");
   Serial.print(output_value);
   Serial.println("%");
   Blynk.virtualWrite(V7,output_value);
 
   
   moisturePercentage =  ( 100- (analogRead(moisturePin) / 1023.00) * 100.00 ) ;
    
    Serial.print("Soil Moisture is  = ");
    Serial.print(moisturePercentage);
    Serial.println("%");
  
}
 
void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  // Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  // Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);
 
  dht.begin();
  
  timer.setInterval(1000L, sendSensor);
  pinMode(D1,INPUT_PULLUP);
timer.setInterval(1000L,notifySoil);
}


void loop()
{

  Blynk.run();
  timer.run();

}

