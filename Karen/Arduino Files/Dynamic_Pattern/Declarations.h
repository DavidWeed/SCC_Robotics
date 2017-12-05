
/**Definition of Constants**/
#define PINGL  11
#define PINGR  9
#define PINGC  10

#define MOTORPIN  4
#define FRONTSTEERPIN  5
#define BACKSTEERPIN  6
#define CAMERASTEERPIN 8

#define CAMCHECKOUT 1 //Intiate Check Camera on Pi
#define GPSGETOUT 7 //Intiate Get GPS on Pi
#define CAMCHECKIN 2     //Confirm Bucket

#define INTERRUPTPIN 3

#define STOPDISTR 17000
#define STOPDISTL 17000


/**Declaration of Functions**/

float determineTurnAngle(float);
void initiateTurn();
void turnRight(float);
void turnLeft(float);

void getKarenPosition();
//float calcLatitude(Point, Point, Karen);

bool checkSoftGeo();
bool checkHardGeo();
bool checkCamera();
/**Declaration and intilization of Globals**/

float squareLength = 30;
float radius = (squareLength / 2) * 12; //convert to inches
float turnAngle = determineTurnAngle(radius);
bool turnR = true;


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


/**Definition of Functions**/

float determineTurnAngle(float radius)
{
	float karenMultiplier = radius / 120;
	float addKaren = radius + (karenMultiplier * 13.5);
	float turnAngle = (asin(17.5 / addKaren) * 180) / PI;
	return turnAngle;
}


void initiateTurn()
{
	turnR ? turnRight(turnAngle) : turnLeft(turnAngle);
	driveForward(squareLength);
	turnR ? turnRight(turnAngle) : turnLeft(turnAngle);
	driveForward(squareLength / 4);
	turnR = turnR ? false : true;

	/*if(turnR)
	{
	turnRight(turnAngle);
	driveForward(squareLength);
	turnRight(turnAngle);
	driveForward(squareLength / 4);
	turnR = false;
	}
	else
	{
	turnLeft(turnAngle);
	driveForward(squareLength);
	turnLeft(turnAngle);
	driveForward(squareLength / 4);
	turnR = true;
	} */
}

void turnRight(float turnAngle)
{
	//readAccel()
	//determineTurnTime()
	//Turn
}

void turnLeft(float turnAngle)
{
	//readAccel()
	//determineTurnTime()
	//Turn
}

void driveForward(int squareLength)
{

}

void getKarenPosition()
{

	//get GPS coordinates
	//karen.x =   //Remember, x is longitude
	//karen.y =   //          y is latitude
}

bool checkSoftGeo()
{
	//TODO calculate the inner lines
}

bool checkHardGeo()
{
	return karen.y < calculateY(S, E) &&
		karen.y < calculateY(W, S) &&
		karen.y > calculateY(E, N) &&
		karen.y > calculateY(N, W);
}

float calculateY(Point one, Point two)
{
	float slope = (two.y - one.y) / (two.x - one.x);
	return slope * (karen.x - one.x) + one.y;
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

bool checkCamera()
{

}