#include <Servo.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <MINDSi.h>
#include <EEPROM.h>
#include <SparkFunLSM9DS1.h>
#include <LSM9DS1_Types.h>
#include <LSM9DS1_Registers.h>
#include <Wire.h>
#include <SPI.h>

/**Definition of Constants**/

#define PINGL  11
#define PINGR  9
#define PINGC  10

#define MOTORPIN  4
#define FRONTSTEERPIN  5
#define BACKSTEERPIN  6
#define CAMERASTEERPIN 8

#define CAMCHECKOUT 1 //Intiate Check Camera on Pi
#define CAMCHECKIN 2     //Confirm Bucket

#define INTERRUPTPIN 3

#define STOPDISTR 17000
#define STOPDISTL 17000
#define STOPDISTC 10200

#define LEFT 0
#define RIGHT 179

#define TOPVELOCITY 4

#define LSM9DS1_M 0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW

/**Definition of Custom Data Structures**/

struct Point
{
	float x;
	float y;
};

struct Vector
{
	float x;
	float y;
	float magnitude = sqrt(pow(this->x, 2) + pow(this->x, 2));
	float unitX = this->x / this->magnitude;
	float unitY = this->y / this->magnitude;
};

struct Karen
{
	float x;
	float y;
};


/**Declaration of Functions**/

void setupAccel();
static void gpsDelay(unsigned long ms);

float determineTurnAngle(float radiusOfTurn);
void initiateTurn();
void turnRight(float);
void uTurnRight(float);
void turnLeft(float);
void uTurnLeft(float);
void goAround();
void driveForward(int);

void accelerate(int);
float calcVelocity();

void cameraCheck(int);

void getKarenPosition();
float calculateY(Point, Point);
void calculateSoftGeo(Point, Point, Point, Point);

bool checkSoftGeo();
bool checkHardGeo();

float getAccel();
//void gpsDelay(unsigned long ms);

/**Declaration of Variables**/

byte value;
LSM9DS1 imu;

int address = 0;

Servo frontSteer;
Servo backSteer;
Servo drive;

Servo camera;

//Multiply by 12 to convert to inches
float squareLength = 20;
float radius = (squareLength / 2) * 12; 
float turnAngle = determineTurnAngle(radius);
float goAroundRadius = 6 * 12;
float goAroundAngle = determineTurnAngle(goAroundRadius);
bool turnR = true;

float velocity;

Point W = { -117.7900848388, 33.8722496032 };
Point S = { -117.7900543212, 33.8722724914 };
Point N = { -117.7900772094, 33.8722534179 };
Point E = { -117.7900466918, 33.8722801208 };

Karen karen;

//Vectors from S to E and from S to W
Vector softLineSE;
Vector softLineSW;
//Vectors from E to N and from E to S
Vector softLineEN;
Vector softLineES;
//Vectors from N to W and from N to E
Vector softLineNW;
Vector softLineNE;
//Vectors from W to S and from W to N
Vector softLineWS;
Vector softLineWN;

Point softN;
Point softE;
Point softS;
Point softW;

/**Definition of GPS Objects**/

SoftwareSerial ss(13, 12);
TinyGPS gps;


/**Definition of Functions**/

void setupAccel()
{
	imu.settings.device.commInterface = IMU_MODE_I2C;
	imu.settings.device.mAddress = LSM9DS1_M;
	imu.settings.device.agAddress = LSM9DS1_AG;
	if (!imu.begin())
	{
		Serial.println("Failed to communicate with LSM9DS1.");
		Serial.println("Double-check wiring.");
		Serial.println("Default settings in this sketch will " \
					   "work for an out of the box LSM9DS1 " \
					   "Breakout, but may need to be modified " \
					   "if the board jumpers are.");
		while (true)
		{
		}
	}
	Serial.println("It should be working now");
}

void initiateTurn()
{
	turnR ? turnRight(turnAngle) : turnLeft(turnAngle);
	//driveForward(squareLength);
	turnR ? turnRight(turnAngle) : turnLeft(turnAngle);
	frontSteer.write(90);
	backSteer.write(90);
	driveForward(squareLength / 4);
	//turnR = turnR ? false : true;
	turnR = !turnR;

	/*if(turnR)
	{
	turnRight(turnAngle);
	turnRight(turnAngle);
	driveForward(squareLength / 4);
	turnR = false;
	}
	else
	{
	turnLeft(turnAngle);
	turnLeft(turnAngle);
	driveForward(squareLength / 4);
	turnR = true;
	} */
}

void turnRight(float turnAngle)
{
	float calculationTime = 0;
	float remainingDistance = 0;

	frontSteer.write(90 + turnAngle);
	backSteer.write(90 - turnAngle);

	calculationTime = calcVelocity();
	remainingDistance = ((radius * PI) / 2 ) - (velocity * calculationTime);
	
	delay(remainingDistance / velocity);
}

void uTurnRight(float turnAngle)
{
	float calculationTime = 0;
	float remainingDistance = 0;

	frontSteer.write(90 + turnAngle);
	backSteer.write(90 - turnAngle);

	calculationTime = calcVelocity();
	remainingDistance = (radius * PI) - (velocity * calculationTime);

	delay(remainingDistance / velocity);
}

void turnLeft(float turnAngle)
{
	float calculationTime = 0;
	float remainingDistance = 0;

	frontSteer.write(90 - turnAngle);
	backSteer.write(90 + turnAngle);

}

void uTurnLeft(float turnAngle)
{
	float calculationTime = 0;
	float remainingDistance = 0;

	frontSteer.write(90 - turnAngle);
	backSteer.write(90 + turnAngle);

}

float determineTurnAngle(float radius)
{
	float karenMultiplier = radius / 120;
	float addKaren = radius + (karenMultiplier * 13.5);
	float turnAngle = (asin(17.5 / addKaren) * 180) / PI;
	return turnAngle;
}

void driveForward(int distance)
{
	delay((distance / velocity) * 1000);
}

float calcVelocity()
{
	float startTime = 0;
	float endTime = 0;
	float calculationTime = 0;

	float accel;

	startTime = millis();

	delay(100);
	accel = abs(getAccel());
	velocity = sqrt(accel * radius);
	Serial.println(velocity);

	endTime = millis();
	calculationTime = endTime - startTime;

	return calculationTime;
}

void goAround()
{
	turnRight(goAroundAngle);
	turnLeft(goAroundAngle);
	cameraCheck(LEFT);
	turnLeft(goAroundAngle);
	turnRight(goAroundAngle);
}

void accelerate(int topVelocity)
{
	velocity = 0;
	int i = 0;
	
	while (velocity < topVelocity)
	{
		int time = 0;
		float totalTime = 0;
		float accel = 0;
		
		drive.write(66 - i);

		while (time <= 50)
		{
			accel = getAccel();
			delay(1);
			totalTime = 1 / 1000;
			velocity += accel * totalTime;

			time++;

			if (velocity > topVelocity)
			{
				break;
			}
		}
		i++;
	}

	EEPROM.put(address, velocity);
	/*value = EEPROM.read(address);
	Serial.println(value, DEC);
	delay(1000000);*/
}

float getAccel()
{
	if (imu.accelAvailable())
	{
		imu.readAccel();
	}
	
	while (true)
	{
		if (imu.gyroAvailable())
		{
			imu.readGyro();
		}
		delay(10);
		Serial.println(imu.calcGyro(imu.gy));
	}
	return imu.calcAccel(imu.ay) * 32.1522; //Converting from g -> ft/s^2
}

void cameraCheck(int angle)
{
	drive.write(0);
	velocity = 0;
	camera.write(angle);
	digitalWrite(CAMCHECKOUT, HIGH);
	gpsDelay(1000);
	digitalWrite(CAMCHECKOUT, LOW);

	while (!digitalRead(CAMCHECKIN))
	{
		drive.write(0);
		gpsDelay(50);
		drive.write(90);
	}
}

void getKarenPosition()
{
	gps.f_get_position(&karen.y, &karen.x);
	//get GPS coordinates
	//karen.x =   //Remember, x is longitude
	//karen.y =   //          y is latitude

}

bool checkSoftGeo()
{
	return karen.y < calculateY(softS, softE) ||
		karen.y < calculateY(softW, softS) ||
		karen.y > calculateY(softE, softN) ||
		karen.y > calculateY(softN, softW);
}

bool checkHardGeo()
{
	return karen.y < calculateY(S, E) ||
		karen.y < calculateY(W, S) ||
		karen.y > calculateY(E, N) ||
		karen.y > calculateY(N, W);
}

float calculateY(Point one, Point two)
{
	float slope = (two.y - one.y) / (two.x - one.x);
	return slope * (karen.x - one.x) + one.y;
}

void calculateSoftGeo(Point N, Point E, Point S, Point W)
{
	softLineNW.x = W.x - N.x;
	softLineNW.y = W.y - N.y;
	softLineNE.x = E.x - N.x;
	softLineNE.y = E.y - N.y;
	
	softLineEN.x = N.x - E.x;
	softLineEN.y = N.y - E.y;
	softLineES.x = S.x - E.x;
	softLineES.y = S.y - E.y;

	softLineSE.x = E.x - S.x;
	softLineSE.y = E.y - S.y;
	softLineSW.x = W.x - S.x;
	softLineSW.y = W.y - S.y;
	
	softLineWS.x = S.x - W.x;
	softLineWS.y = S.y - W.y;
	softLineWN.x = N.x - W.x;
	softLineWN.y = N.y - W.y;

	
	softN.x = N.x + ((softLineNW.unitX * squareLength) + (softLineNE.unitX * squareLength));
	softN.y = N.y + ((softLineNW.unitY * squareLength) + (softLineNE.unitY * squareLength));

	softE.x = E.x + ((softLineEN.unitX * squareLength) + (softLineES.unitX * squareLength));
	softE.y = E.y + ((softLineEN.unitY * squareLength) + (softLineES.unitY * squareLength));

	softS.x = S.x + ((softLineSE.unitX * squareLength) + (softLineSW.unitX * squareLength));
	softS.y = S.y + ((softLineSE.unitY * squareLength) + (softLineSW.unitY * squareLength));

	softW.x = W.x + ((softLineWS.unitX * squareLength) + (softLineWN.unitX * squareLength));
	softW.y = W.y + ((softLineWS.unitY * squareLength) + (softLineWN.unitY * squareLength));
}

void fullStop()
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

static void gpsDelay(unsigned long ms)
{
	unsigned long start = millis();
	do
	{
		while (ss.available())
		{
			gps.encode(ss.read());
		}
	} while (millis() - start < ms);
}