#include <Encoder.h>
#include <MINDSi.h>
#include <Servo.h>

int pingL = 11; 
int pingR = 9;
int pingC = 10;

void setup() 
{  
  Serial.begin(9600);
}

void loop() 
{
  Serial.println("Left time " + getPing(pingL));
  Serial.println("Center time " + getPing(pingC));
  Serial.println("Right time " + getPing(pingR)); 

  delay(100);
}
