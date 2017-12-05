
#include <Servo.h>
#include <Encoder.h>
#include <MINDSi.h>

const int pingL = 11; 
const int pingR = 9;
const int pingC = 10;

const int motorPin = 4;
const int frontSteerPin = 5;
const int backSteerPin = 6;

Servo frontSteer;
Servo backSteer;
Servo drive;


void setup() 
{
 frontSteer.attach(frontSteerPin);
 backSteer.attach(backSteerPin);
 drive.attach(motorPin);

 drive.write(90);
 frontSteer.write(90);
 backSteer.write(90);

 delay(2000);
}

void loop() 
{   
    if (getPing(pingC) <= 2000) //is there a wall?
    {
    drive.write(90);
    //delay(500);
    drive.write(110); //reverse
    delay(1000);
    //drive.write(90);
    //drive.write(70); //forward
    }
    else
    {
    drive.write(90);
    drive.write(70); //forward
    //delay(10);
    }  
}


