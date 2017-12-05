#include <Servo.h>
//#include <Encoder.h>
#include <MINDSi.h>

const int pingL = 11; 
const int pingR = 9;
const int pingC = 10;

const int motorPin = 4;
const int frontSteerPin = 5;
const int backSteerPin = 6;

const int stopDistC = 2000;
const int stopDistL = 300;//made these two much shorter because the center one should still pick up most everything.
const int stopDistR = 300;//         <--/

Servo frontSteer;
Servo backSteer;
Servo drive;

void setup() 
{
 frontSteer.attach(frontSteerPin);
 backSteer.attach(backSteerPin);
 drive.attach(motorPin); 
 frontSteer.write(90);
 backSteer.write(90);
 // put your setup code here, to run once:

}

void loop() 
{
  // put your main code here, to run repeatedly:
  drive.write(50);
  frontSteer.write(45);
  backSteer.write(135);
}
