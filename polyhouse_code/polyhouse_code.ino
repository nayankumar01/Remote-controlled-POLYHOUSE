#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#define BLYNK_TEMPLATE_ID "TMPL3cu5EN-XP"
#define BLYNK_TEMPLATE_NAME "WATERPUMP"

//Initialize the LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = "SpX7L5K5CMd71f5yxfGv56qHLjRrXPve";
char ssid[] = "kamalnayan";
char pass[] = "kamalnayan";

BlynkTimer timer;
 bool s1 = 0;
 bool s2=0;

DHT dht(D4, DHT11);
#define sensor A0
const int mot = D6;
const int wat = D7;
// #define waterPump D3
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
    Blynk.virtualWrite(V2, t);
    Blynk.virtualWrite(V3, h);
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print("    Humidity : ");
    Serial.println(h);
    lcd.setCursor(4, 0);
    lcd.print("T:");
  lcd.print(t);
 lcd.setCursor(11, 0);
  //  lcd.print(" ");
  lcd.print("H:");
  lcd.print(h);
}
void setup() {
  Serial.begin(9600);
  pinMode(mot, OUTPUT);
  pinMode(wat, OUTPUT);
  // digitalWrite(waterPump, HIGH);
  lcd.init();
  lcd.backlight();

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  lcd.setCursor(1, 0);
  lcd.print("System Loading");
  for (int a = 0; a <= 15; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(500);
  }
  lcd.clear();

  //Call the function
  timer.setInterval(100L, soilMoistureSensor);
   dht.begin();
  timer.setInterval(100L, sendSensor);
}
// BLYNK_WRITE(V1){

// }
//Get the button value
// BLYNK_WRITE(V1) {
//   Relay = param.asInt();

//   if (Relay == 1) {
//     digitalWrite(waterPump, LOW);
//     lcd.setCursor(0, 1);
//     lcd.print("Motor is ON ");
//   } else {
//     digitalWrite(waterPump, HIGH);
//     lcd.setCursor(0, 1);
//     lcd.print("Motor is OFF");
//   }
// }
 BLYNK_WRITE(V1){
   s1=param.asInt();

   if(s1==1){
     digitalWrite(wat,HIGH);
     lcd.setCursor(0, 1);
     lcd.print("pump is ON ");
   }else{
     digitalWrite(wat, LOW);
    lcd.setCursor(0, 1);
    lcd.print("pump is OFF");
   }
  //  lcd.clear();


 };
  BLYNK_WRITE(V4){
   s2=param.asInt();
   if(s2==1){
     digitalWrite(mot,HIGH);
     lcd.setCursor(0, 1);
     lcd.print("motor is ON ");
   }else{
     digitalWrite(mot, LOW);
    lcd.setCursor(0, 1);
    lcd.print("motor is OFF");
   }
  };
  BLYNK_WRITE(V5){
    int pinValue = param.asInt();
    analogWrite(D8,pinValue);
    Blynk.virtualWrite(V6,pinValue);
    Serial.print("Led intensity is: ");
    Serial.println(pinValue);

  };
   BLYNK_WRITE(V7){
    int pinValue = param.asInt();
    analogWrite(D5,pinValue);
    Blynk.virtualWrite(V8,pinValue);
    Serial.print("Led intensity is: ");
    Serial.println(pinValue);

  };

//Get the soil moisture values
void soilMoistureSensor() {
  int value = analogRead(sensor);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  Blynk.virtualWrite(V0, value);
  lcd.setCursor(0, 0);
  lcd.print("M:");
  lcd.print(value);
  // lcd.print(" ");

}

void loop() {
  
  //   digitalWrite(mot,HIGH);
  // digitalWrite(wat,LOW);
  // delay(3000);
  
  // digitalWrite(mot,LOW);
  // digitalWrite(wat,HIGH);
  // delay(3000);
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}