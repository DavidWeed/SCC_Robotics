#include <Servo.h>
//#include <Encoder.h>
#include <MINDSi.h>

const int motorPin = 4;
const int frontSteerPin = 5;
const int backSteerPin = 6;

Servo frontSteer;
Servo backSteer;
Servo drive;
void setup() {
  // put your setup code here, to run once:
 frontSteer.attach(frontSteerPin);
 backSteer.attach(backSteerPin);
 drive.attach(motorPin); 
 frontSteer.write(90);
 backSteer.write(90);

 drive.write(40);
 delay(5000);
 drive.write(90);
 
}

void loop() {
  // put your main code here, to run repeatedly:
   

}
