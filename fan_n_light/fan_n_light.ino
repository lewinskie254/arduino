#include <LiquidCrystal.h>

//Setting the pins for the LCD: 
LiquidCrystal lcd(12, 11, 5, 4, 3, 6);

//setting the analog pins for recoridng 
//the temparature and photoresistor  inputs 
int tempPin = A0;
int ldrPin = A1;

//determining the pins for the led and for the motor 
int ledPin = 8;
int motorPin = 9;

//these are the threshoholds for setting the temparature on or off 
// thresholds
float tempOn = 28.0;
float tempOff = 26.0;

int lightOn = 500;
int lightOff = 600;

//these are the variables for 
//determining of the motor should rotate or not
//depending on the temparature threshold crossing 

bool fanState = false;
bool lightState = false;

//function for reading the temparature, 
// Convert analog voltage to temperature (LM35: 10mV per °C)
//and displaying the temparature on screen. 
float readTemperature() {
  float sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(tempPin);
    delay(5);
  }
  float avg = sum / 10.0;
  float voltage = avg * (5.0 / 1023.0);
  return voltage * 100;
}

void setup() {
    
  pinMode(ledPin, OUTPUT);
  pinMode(motorPin, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("Initializing...");
  delay(1500);
  lcd.clear();
  
  //anticipating for various random startup states 
  digitalWrite(ledPin, LOW);
  digitalWrite(motorPin, LOW);
}

void loop() {
  float temp = readTemperature();
  int lightVal = analogRead(ldrPin);

  // --- FAN CONTROL (hysteresis) ---
  if (!fanState && temp > tempOn) {
    fanState = true;
  } else if (fanState && temp < tempOff) {
    fanState = false;
  }
  digitalWrite(motorPin, fanState);

  // --- LIGHT CONTROL (hysteresis) ---
  if (!lightState && lightVal < lightOn) {
    lightState = true;
  } else if (lightState && lightVal > lightOff) {
    lightState = false;
  }
  digitalWrite(ledPin, lightState);

  // --- LCD ---
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp, 1);
  lcd.print("C   ");

  lcd.setCursor(0, 1);
  lcd.print("F:");
  lcd.print(fanState ? "ON " : "OFF");

  lcd.print(" L:");
  lcd.print(lightState ? "ON " : "OFF");

  delay(500);
  
  //its still the same code as the one shown at the beginning of the the video, only with fewer comments. 
}