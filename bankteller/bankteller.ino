#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment display = Adafruit_7segment();

int latestTicket = 0;
int currentlyServing = 0;
int queueCount = 0; 

int ledPins[] = {5, 6, 7, 8, 9}; 

void setup() {
  display.begin(0x70); 
  
  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  
  pinMode(2, INPUT);  
  pinMode(3, INPUT);  
  pinMode(4, INPUT);  
  pinMode(11, OUTPUT); 
  
  updateDisplay(0);
  updateQueueBar(); 
}

void loop() {
  // 1. Customer takes a ticket
  if (digitalRead(2) == HIGH) {
    latestTicket++;
    queueCount++;    
    
    // NEW: Short "chirp" for the customer
    tone(11, 1500, 50); 
    
    updateQueueBar();
    delay(300); 
  }

  // 2. Teller calls next
  if (digitalRead(3) == HIGH) {
    if (currentlyServing < latestTicket) {
      currentlyServing++;
      
      // Prevent queueCount from going below 0
      if(queueCount > 0) queueCount--; 
      
      // Standard "Ding-Dong" or loud beep for Teller
      tone(11, 1000, 200); 
      
      updateDisplay(currentlyServing);
      updateQueueBar();
    } else {
      // Very low "error" tone if no one is waiting
      tone(11, 150, 100); 
    }
    delay(300); 
  }

  // 3. Reset System
  if (digitalRead(4) == HIGH) {
    latestTicket = 0;
    currentlyServing = 0;
    queueCount = 0; 
    
    // Triple beep for reset
    for(int i=0; i<3; i++) {
      tone(11, 2000, 100);
      delay(150);
    }
    
    updateDisplay(0);
    updateQueueBar(); 
    delay(300);
  }
}

void updateDisplay(int val) {
  display.print(val, DEC);
  display.writeDisplay();
}

void updateQueueBar() {
  for (int i = 0; i < 5; i++) {
    // If the person waiting is at least the index of the LED, turn it on
    if (queueCount > i) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }
}