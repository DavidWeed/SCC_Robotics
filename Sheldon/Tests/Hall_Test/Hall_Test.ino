int hallPin = A0;
double wheelRadius = 18.3783170235;

void setup() 
{
    pinMode(A0,INPUT);
    Serial.begin(9600);
    while(!Serial)
    {
      
    }
}

void loop() 
{
    Serial.println(hallSpeed());
}

double hallSpeed()
{
  double speeds;
  bool digitalVal = !digitalRead(hallPin);
  float startTime = micros();
  while(!digitalVal)
  {
  }
  speeds = (1000000/(micros() - startTime))/2;
  speeds = 1/speeds;
  return speeds *= wheelRadius;
}

