# include "margoulinade.h"

void encoderMenu()
{
  int newPosition = myEnc.read();
  if ((newPosition != oldPosition) )
  {
    oldPosition = newPosition;
    if(oldPosition > (MENUELEMENTS * ENCODERSTEPS) - 1)
    {
       myEnc.write(0);
    } 
    else if(oldPosition < 0)
    {
       myEnc.write(MENUELEMENTS * ENCODERSTEPS - 1);
    }
    Serial.println(newPosition / ENCODERSTEPS);
    cycleMenu = (abs(newPosition) / ENCODERSTEPS) / MENUELEMENTS;
    lcd.clear();
  }
}

int encoderWrite()
{
    int newPosition;
    myEnc.write(0);
    Serial.print(F("jeej"));
    lcd.print(F("Loading..."));
    delay(500);
    oldPosition = -1;
    while(digitalRead(encButton))
    {
        newPosition = myEnc.read();
        if ((newPosition != oldPosition) )
        {
            if (newPosition < VALMIN * ENCODERSTEPS)
            {
                myEnc.write(VALMIN * ENCODERSTEPS);
                newPosition = 0;
            }
            else if (newPosition > VALMAX * ENCODERSTEPS)
            {
                myEnc.write(VALMAX * ENCODERSTEPS);
                newPosition = 40;
            }
            else
            {
                lcd.clear();
                lcd.print(F("New balance :"));
                lcd.setCursor(0,1);
                lcd.print(newPosition / ENCODERSTEPS);

                oldPosition = newPosition;
                Serial.println(newPosition / ENCODERSTEPS);
            }
        }
    }
    myEnc.write(0);
    oldPosition = -1;
    return (newPosition / ENCODERSTEPS);
}

void wait4button()
{
    Serial.println(F("\n\nDONE ! waiting 4 the button"));
    digitalWrite(readLedPin, LOW);
    digitalWrite(writeLedPin, LOW);
    if (OPTIMIZATION_MODE)
        delay(500);
    while(digitalRead(encButton))
    {
    }
    delay(500);
    Serial.print(F("end read = "));Serial.println(oldPosition);
    lcd.clear();
}
