#include <ThingSpeak.h>  //To connect our thingspeak channel with the esp8266 through this code.
#include <ESP8266WiFi.h>  // To connect the esp with internet
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>             

const char* ssid     = "saicharan";    //Name of your wifi network
const char* password = "123456789";  // Wifi password

const long channelID = 2462879; // Channel ID
const char *writeAPIKey = "WASOYXTZBDOJOPBO"; // Your write API Key

WiFiClient client;

unsigned long delayStart = millis();   // start delay

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() 
{
  lcd.init();
  lcd.backlight();
   pinMode(14, OUTPUT);
   pinMode(12, OUTPUT);

   Serial.begin(115200); 
   Serial.println();
   Serial.print("Connecting to ");
   Serial.println(ssid);

   lcd.setCursor(0,0);
   lcd.print("IOT BASED LIGHT" );
   lcd.setCursor(0,1);
   lcd.print("INTENSITY SYSTEM" ); 
   delay(5000);
   lcd.clear();

   WiFi.begin(ssid, password);  // Connecting esp to wifi
   while (WiFi.status() != WL_CONNECTED)  //This loop will work untill esp connects to the wifi
   {   
   delay(500);
   Serial.print(".");
   }
   ThingSpeak.begin(client);       //The client here is esp8266 
   delay(1000);
}

void loop() {
    int intensity = analogRead(A0);

    intensity = map(intensity, 0, 1024, 100, 0);

    lcd.setCursor(0, 0);
    lcd.print("INTENSITY : ");
    lcd.print(intensity);

    if (intensity < 50) {
        digitalWrite(12, HIGH);
        digitalWrite(14, LOW);
    } 
    else  {
        digitalWrite(14, HIGH);
        digitalWrite(12, LOW);
    }

    if ((millis() - delayStart) >= 15000) {
        ThingSpeak.writeField(channelID, 1, intensity, writeAPIKey);
        delayStart = millis(); // Reset the delay start time
    }

    delay(500);                // Take a reading every  half a second for testing
    Serial.println();          // Normally you should take reading perhaps once or twice a day
}
