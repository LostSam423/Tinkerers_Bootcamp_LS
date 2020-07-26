#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h> 

const char* ssid     = "Aniket";
const char* password = "123456712";

int REDLED = 21; 
int GREENLED = 32;
long ptime,currtime;
float acases,deaths,pacases,pdeaths;

void setup() 
{

  pinMode(REDLED, OUTPUT);
  digitalWrite(REDLED, LOW);
  pinMode(GREENLED, OUTPUT);
  digitalWrite(GREENLED, LOW);
  Serial.begin(9600);

  WiFi.enableSTA(true);
  
  delay(2000);

  connectToWifi();
  currtime = 0;
  ptime = currtime;

  initializePcases();

}

void loop(){
  currtime = millis()/1000;
  if(WiFi.status() == WL_CONNECTED){
    if(currtime - ptime >= 86400){
      ptime = currtime;
      doTheTask();
    }
  }
  else{
     Serial.println("WiFi not connected. Please Turn it on.");
     delay(1000);
     connectToWifi();
  }
}

void connectToWifi()
{

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void initializePcases()
{
  // Here you have to initialize the pacases,pdeaths with the current value
  // you can fetch it just like that or can copy paste from the wikipedia as this is for one time only.
  HTTPClient http;
  http.begin("https://api.thingspeak.com/apps/thinghttp/send_request?api_key=9CFMUV97KCH5WJWQ");
  pacases = http.GET();
  HTTPClient http1;
  http1.begin("https://api.thingspeak.com/apps/thinghttp/send_request?api_key=WN725Z2CIRRR4LNJ");
  pdeaths = http1.GET();
}

void doTheTask()
{
  // Here you have to fetch data every time of Active Cases and Deaths for Corona in India(For whole country and not state-wise)
  // Store the data in the variable defined and check whether this data is above or below 1 percent increase from previous datas
  // If any of the data is above 1 percent increase glow REDLED or else glow GREENLED
  // Also remember to turn off other LED
  // Then change previous data to current so to check again from this next day
  HTTPClient active;
  active.begin("https://api.thingspeak.com/apps/thinghttp/send_request?api_key=9CFMUV97KCH5WJWQ");
  acases = active.GET();
  HTTPClient tdeath;
  tdeath.begin("https://api.thingspeak.com/apps/thinghttp/send_request?api_key=WN725Z2CIRRR4LNJ");
  deaths = tdeath.GET();
  
  float activechg, deathchg;
  activechg = (acases - pacases)/pacases;
  deathchg = (deaths - pdeaths)/pdeaths;

  if(activechg>=0.01 || deathchg>=0.01){
    digitalWrite(REDLED, HIGH);
    digitalWrite(GREENLED, LOW);
  }
  else{
    digitalWrite(REDLED, LOW);
    digitalWrite(GREENLED, HIGH);
  }
  pacases = acases;
  pdeaths = deaths;
}
