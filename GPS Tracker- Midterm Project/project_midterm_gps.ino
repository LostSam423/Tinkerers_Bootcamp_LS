#include <TinyGPS.h>
#include <SoftwareSerial.h>

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "eQzZeJZ6N4-vLbJHy0EkZ8uBR0hBpX0O";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Redmi1";
char pass[] = "123456789";

SoftwareSerial gpsSerial(2,3);
TinyGPS gps;

WidgetMap myMap(V1);
unsigned int move_index = 1; 
float spd;
void setup()
{
  // Debug console
  Serial.begin(9600);
  gpsSerial.begin(4800);
  Blynk.begin(auth, ssid, pass);
  
}

void loop()
{
  while(gpsSerial.available() > 0){
    if(gps.encode(gpsSerial.read())){
      long latitude;
      long longitude;
      gps.get_position( &latitude, &longitude);
      spd = gps.f_speed_kmph();
      char comma = ',';
      String Stringall = (String(latitude,6) + comma + String(longitude,6) + comma + String(" Speed: ") + spd + String("kmph"));
      Blynk.virtualWrite(V2, Stringall);
      myMap.location(move_index, latitude, longitude, "GPS_Location");
    }
  }
  Blynk.run();
}
