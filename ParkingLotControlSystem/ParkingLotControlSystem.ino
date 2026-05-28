//Libary to control the servo (Actuators)
#include <Servo.h>

//Libraries to control the 7-segment Display 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

//Initializing the Display 
Adafruit_7segment display = Adafruit_7segment();

// the entry and exit actuators to control the inflow and outflow of cars into the parking lot 
Servo entryGate;
Servo exitGate;

//buttons to control the entry and exit boom barriers 
const int entryBtn = 2;
const int exitBtn = 7; 

//Buzzer to make a sound indicating the entry of the opening and closing of the boom barriers 
const int buzzer = 6;

//Red LED to show a car leaving the parking lot, 
const int redLed = A0;   

//Green LED to show a car entering the parking lot, indicating more cars can be accomodated. 
const int greenLed = A1; 

//initialize the available spaces to 5 (Just for demo purposes) can be increased to 1000 cars if need be. 
int spaces = 0; 


//intialize all the compoennents 
void setup() {

  //initialize the serial monitor 
  Serial.begin(9600);
  
  // Initialize Display
  // the display is not found add a message to the serial monitor to highlight errors. 
  if (!display.begin(0x70)) {
    Serial.println("Display not found. Check I2C wiring.");
  }


  //attach the servos (actuators) to the respective pins. 
  entryGate.attach(9);
  exitGate.attach(10);

  //ensure that the servos (actuators) start while they are off. 
  entryGate.write(0);
  exitGate.write(0);
  
  //attach the entry and exit buttons to pins D2 and D7 respectively using a Pull Up network, 
  // meaning that current is always flowing, and clicking the buttons turns the current low
  pinMode(entryBtn, INPUT_PULLUP);
  pinMode(exitBtn, INPUT_PULLUP);

  //turn pin D6 into an output pin for the buzzer 
  pinMode(buzzer, OUTPUT);

  //turn pin A0 to an output pin for the red LED 
  pinMode(redLed, OUTPUT);

  //turn pin A1 into an output pin for the green LED 
  pinMode(greenLed, OUTPUT);
  
  //Update the display with the number of spaces currently occupied by the cars. 
  updateDisplay(spaces);
}

//the function that will be responsible for updating the display. 
void updateDisplay(int num) {
  //first clear everything 
  display.clear();

  //the print the number passed in as as this function's paramter to the display 
  display.print(num, DEC);

  //write the number on the 7-Segment display screen. 
  display.writeDisplay();
}

void loop() {
  // Logic for allowing the manual entry and exit of cars. 
  if (digitalRead(entryBtn) == LOW) {

    //if the entry button is clicked and pulls the pull up network to ground 
    if (spaces < 5) { 
      //Make the green LED light up 
      digitalWrite(greenLed, HIGH);

      //play a tone  
      tone(buzzer, 2000, 200); 

      //add some volatge to the entry gate and turn the servos (actuators)
      entryGate.write(90); 

      // add a 3 second delay to allow the car to pass 
      delay(3000);

      //close the boom barrier 
      entryGate.write(0); 

      //increase the number of spaces currently occupied 
      spaces++; 

      //change the display to update and show the current number of spaces currently occupied 
      updateDisplay(spaces);

      //switch off the green LED 
      digitalWrite(greenLed, LOW);
    } else {
      tone(buzzer, 500, 800); 
    }
    delay(500); 
  }

  // --- Manual Exit ---
  if (digitalRead(exitBtn) == LOW) {
    //if the exit button is pressed 
    if (spaces > 0) { 
      //if the number of spaces is greater than zero 

      //first switch on the red LED light 
      digitalWrite(redLed, HIGH);

      //play a tone to indicate cars leaving the lot 
      tone(buzzer, 1000, 200); 

      //turn on the exit boom barrier (servo/actuator)
      exitGate.write(90); 

      //add a 3 second delay to allow the car to pass 
      delay(3000);

      //close the boom barrier 
      exitGate.write(0); 

      //decrease the number of spaces 
      spaces--; 
      
      //update the display with the right value 
      updateDisplay(spaces);

      //turn the led light low. 
      digitalWrite(redLed, LOW);
    }

    //add a small 0.5 second delay during each loop to propagate changes 
    delay(500); 
  }
}