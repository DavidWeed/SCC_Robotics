#include <TinyGPS.h>

TinyGPS gps;

//corner X lat and longitudes
long XcornerLat = 33795405;
long XcornerLon = -117766195;
//corner Y lat and longitudes
long YcornerLat = 33795293;
long YcornerLon = -117766005;
//corner W lat and longitudes
long WcornerLat = 33795303;
long WcornerLon = -117766277;
//corner Z lat and longitudes
long ZcornerLat = 33795183;
long ZcornerLon = -117766183;



void setup()
{
  pinMode(8, OUTPUT);
}

void loop()
{
  Geofence();
  
}

void Geofence()
{
  long lat, lon;
  gps.get_position(&lat, &lon);
  long currentX = lat;
  long currentY = lon;

  //for line XY
  if(((YcornerLon - XcornerLon)*(currentX - XcornerLat) + (YcornerLat - XcornerLat)*(currentY - XcornerLon)) >= 0)
  {
     digitalWrite(8,HIGH);
  }
  //for line XW
  else if(((WcornerLon - XcornerLon)*(currentX - XcornerLat) + (WcornerLat - XcornerLat)*(currentY - XcornerLon)) <= 0)
  {
      digitalWrite(8,HIGH);
  }
  //for line YZ
  else if(((ZcornerLon - YcornerLon)*(currentX - YcornerLat) + (ZcornerLat - YcornerLat)*(currentY - YcornerLon)) >= 0)
  {
      digitalWrite(8,HIGH);
  }
  //for line WZ
  else if(((ZcornerLon - WcornerLon)*(currentX - WcornerLat) + (ZcornerLat - WcornerLat)*(currentY - WcornerLon)) <= 0)
  {
      digitalWrite(8,HIGH);
  }
  else
  {
      digitalWrite(8, LOW);
  
  }

  
}
