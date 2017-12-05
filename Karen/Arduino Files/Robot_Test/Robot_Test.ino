#include <Servo.h>
//#include <Encoder.h>
//#include <MINDSi.h>

int pingL = 11; 
int pingR = 9;
int pingC = 10;

int motorPin = 4;
int frontSteerPin = 5;
int backSteerPin = 6;

Servo frontSteer;
Servo backSteer;
Servo drive;
int increment = 0;

void setup() 
{
 frontSteer.attach(frontSteerPin);
 backSteer.attach(backSteerPin);
 drive.attach(motorPin); 

 frontSteer.write(90);
 backSteer.write(90); 
  
}

void loop() 
{
  /*if(increment < 4)
  {
  frontSteer.write(90);
  backSteer.write(90);

  drive.write(70);
  delay(2000);
  drive.write(0);
  delay(500);

  increment++; */

  int time = millis();
  int driveTime = 2000;
  if(time <= driveTime)
  {
    drive.write(70);
  }
  else
    drive.write(90);
}
