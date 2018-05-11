#include "Sheldon.h"

#define RCPIN A3
#define SWITCH A5

int prevSig;

void setup() 
{
  lcdSetup();
  gpsSetup();
  lcd.clear();
  gpsDelay(500);
  lcd_show_lat_lon();  
  pinMode(RCPIN, INPUT);  
  pinMode(SWITCH, INPUT); 

  if(digitalRead(SWITCH) == LOW)
  {//RC control and storing data to the EEPROM
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("RC Mode Activate");
    while(1)
    {      
      prevSig = pulseIn(RCPIN, HIGH);
      gpsDelay(200);
      lcd_show_lat_lon();
      int diff = prevSig - pulseIn(RCPIN, HIGH);
      bool change = ((diff > 0)? diff : -diff) > 300;
      if(change)
      {
        eepromWrite(lat);
        eepromWrite(lon);
      } 
    }
  }
  else
  {//Read the EEPROM to the serial monitor
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Reading EEPROM");
    Serial.begin(9600);  
    while(!Serial)
    {;} 
    long tempData;
    //EEPROM.get(0, nextAddress);
    for (int i = 4; i < EEPROM.length() - 4; i += 4)
    {
      EEPROM.get(i, tempData);
      Serial.print(tempData);
      i += 4;
      Serial.print(", ");
      EEPROM.get(i, tempData);
      Serial.println(tempData);
    }    
  }
}
 
void loop() 
{  
}

