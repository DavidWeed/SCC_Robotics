#include <Servo.h>
#include <MINDSi.h>

#define PINGL  11
#define PINGR  9
#define PINGC  10

#define MOTORPIN  4
#define FRONTSTEERPIN  5
#define BACKSTEERPIN  6
#define CAMERASTEERPIN 8

#define GPOUTCAM 1 //info to camera to double check if bucket
#define GPOUTGPS 7 //send info to check gps and take picture when at bucket
#define GPIN 2     //info from pi confirming that it is indeed a bucket, if no info is received within a certain timeout then we continue on without moving towards a bucket
#define INTERRUPTPIN 3

#define STOPDISTR 17000
#define STOPDISTL 17000

#define COMMANDNUM 52

//declare an array for each command that must be executed on the field
char commandArray[COMMANDNUM] = {'s','s','s','s','s','s','s','s','l','s','l','s','s','s','s','s','s','s','s','r','s','r','s','s','s','s','s','s','s','s','l','s','l','s','s','s','s','s','s','s','s','r','s','r','s','s','s','s','s','s','s','s'};


// 's','s','s','s','r','r',
Servo frontSteer;
Servo backSteer;
Servo drive;
Servo camera;

double stopTime = 3000;

//function prototypes
//functions for movement
void goStraight();
void turnLeft90();
void turnRight90();
void turnLeft180();
void turnRight180();

//functions to check ping
void checkPingRight();
void checkPingLeft();

//prototype a function to go around something in front of Karen
void goAround();

//functions to operate when ping sensors are tripped
void rightBucketFound();
void leftBucketFound();

//misc functions
void panicButton();
void alertPi();

void setup()
{
  //setup stop button
  attachInterrupt(INTERRUPTPIN, panicButton, CHANGE);
  
  //attach pins
	frontSteer.attach(FRONTSTEERPIN);
	backSteer.attach(BACKSTEERPIN);
	drive.attach(MOTORPIN);
	frontSteer.write(90);
	backSteer.write(90);	
	
	turnLeft90();
	goForward();
	goForward();
	goForward();
	goForward();
	turnRight90();
	turnRight90();

	//create for loop to execute each command in the commandArray 
	for (int i = 0; i < COMMANDNUM; i++)
	{
		char currInst = commandArray[i];
		switch (currInst)
		{
			case 's':
				goForward();
				//checkPingRight();
				//checkPingLeft();
				break;
			case 'l':
				turnLeft90();
				//checkPingRight();
				//checkPingLeft();
				break;
			case 'r':
				turnRight90();
				//checkPingRight();
				//checkPingLeft();
				break;
		}
	}
}

void loop() 
{
  
}

void goStraight()
{
  //TODO uses compass to go straight 
}

void goForward()
{
	unsigned long timeForward = millis();

	while (millis() - timeForward < 4500)
	{
	  //checkPingFront();
		frontSteer.write(90);
		backSteer.write(90);
		drive.write(66);
		goStraight();
	}
}

void checkPingFront()
{
	if (getPing(PINGC) <= 10000)
	{
		goAround();
	}
}
 
void checkPingRight()
{
	if (getPing(PINGR) <= STOPDISTR)
	{
		rightBucketFound();
	}
}

void checkPingLeft()
{
	if (getPing(PINGL) <= STOPDISTL)
	{
		leftBucketFound();
	}
}

void turnRight180()
{
	unsigned long turnTime = millis();
	frontSteer.write(102); //82 for 20 foot circle
	backSteer.write(78);  //98

	while (millis() - turnTime < 4595)
	{
		drive.write(66);
	}
	drive.write(90);
	drive.write(0);
	frontSteer.write(90);
	backSteer.write(90);
}

void turnLeft180()
{
	int turnTime = millis();
	frontSteer.write(78);
	backSteer.write(102);

	while (millis() - turnTime < 4595)
	{
		drive.write(66);
	}
	drive.write(90);
	frontSteer.write(90);
	backSteer.write(90);
}

void turnRight90()
{
	//unsigned long turnTime = millis();
  int turnTime = millis();
	frontSteer.write(102); //82 for 20 foot circle
	backSteer.write(78);  //98

	while (millis() - turnTime < 2097.5) //35.5 for a 20 foot circle
	{
		drive.write(66);
	}
	drive.write(90);
	drive.write(0);
	frontSteer.write(90);
	backSteer.write(90);
}

void turnLeft90()
{
	int turnTime = millis();
	frontSteer.write(78);
	backSteer.write(102);
	while (millis() - turnTime < 2097.5)
	{
		drive.write(66);
	}
	drive.write(90);
	frontSteer.write(90);
	backSteer.write(90);
}

void rightBucketFound()
{//sees a bucket to the right now do a 180 turn and stop next to it. take its info and then move on
	turnRight180();
	digitalWrite(GPOUTGPS, HIGH);
	while (digitalRead(GPIN) == HIGH)
	{
		drive.write(90);
		//frontSteer.write(90);
		//backSteer.write(90);
	}
	turnRight180();
}

void leftBucketFound()
{//sees a bucket to the left now do a 180 turn and stop next to it. take its info and then move on
	turnLeft180();
	digitalWrite(GPOUTGPS, HIGH);
	while (digitalRead(GPIN) == HIGH)
	{
		drive.write(90);
	  //frontSteer.write(90);
	  //backSteer.write(90);
	}
	turnLeft180();
}

void goAround()
{
	avoidRight90();
	avoidLeft180();
	avoidRight90();
}

void avoidRight90()
{
	unsigned long turnTime = millis();
	frontSteer.write(114);
	backSteer.write(66);

	while (millis() - turnTime < 1500.5)
	{
		drive.write(66);
	}
	drive.write(90);
	drive.write(0);
	frontSteer.write(90);
	backSteer.write(90);
}

void avoidLeft180()
{
	int turnTime = millis();
	frontSteer.write(66);
	backSteer.write(114);

	while (millis() - turnTime < 3000)
	{
		drive.write(66);
	}
	drive.write(90);
	frontSteer.write(90);
	backSteer.write(90);
}

void panicButton()
{//panic button to cease all action and put the robot in a dormant state
  while (true)
  {
    drive.write(0);
    frontSteer.write(90);
    backSteer.write(90);
    drive.write(90);
    drive.attach(27);
  }  
}



//Jacobs Brain Expansion meme
//bool readPi()
//{
//  if(digitalRead(GPIN) == HIGH)
//  {
//  return true;  
//  }  
//  else 
//  {
//  return false;  
//  }
//}
