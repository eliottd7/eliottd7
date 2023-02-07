#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

//Initialize accelerometer
Adafruit_MMA8451 mma = Adafruit_MMA8451();

//Globals
int LED = 11; //change to pin 11
int now = 0;
int Average = 0;
int LEDpin[ ] = {0 ,0 ,0};

void setup(void) {
  
  //accelerometer ping console
  Serial.begin(9600);
  Serial.println("Adafruit MMA8451 test!");
  pinMode(LED, OUTPUT);
  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("MMA8451 found!");
  
  //Expected acceleration range based on presets
  mma.setRange(MMA8451_RANGE_2_G);
  
  Serial.print("Range = "); Serial.print(2 << mma.getRange());  

  //LED default state is ON
  analogWrite(LED,75);
  
}

void loop() {
  // Read the 'raw' data in 14-bit counts
  mma.read();
  Serial.print("X:\t"); Serial.print(mma.x); 
  Serial.println();

  sensors_event_t event; 
  mma.getEvent(&event);

  //Read and display acceleration
  Serial.print("X: \t"); Serial.print(event.acceleration.x); Serial.print("\t");
  Serial.println("m/s^2 ");

  LEDpin[2] = event.acceleration.x;
  Average = (LEDpin[0]+LEDpin[1]+LEDpin[2])/3; //mean prevents hypersensitivity

  if(Average<-0.4){    
    for(int i = 0; i < 5; i++){ //blink sequence
      analogWrite(LED, 255);
      delay(100);
      analogWrite(LED, 25);
      delay(50);
    }
    analogWrite(LED, 255);
    delay(1400);
       
    now = 6;
  }
  else {
    if(now>0){
    now = now-1;
    if(now ==0) analogWrite(LED,75);
    }
  }

  LEDpin[1] = LEDpin[2];
  Serial.println();
  delay(100);
  
 }
