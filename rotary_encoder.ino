# include "margoulinade.h"

void encoderMenu()
{
    int newPosition = myEnc.read();
    if ((newPosition != oldPosition) )
    {
        oldPosition = newPosition;
        if(oldPosition < 0)
        {
            myEnc.write(255);
        }
        if(oldPosition > 255)
        {
            myEnc.write(0);
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
    Serial.print("jeej");
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
                lcd.print("New balance :");
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
    Serial.println("\n\nDONE ! waiting 4 the button");
    digitalWrite(readLedPin, LOW);
    digitalWrite(writeLedPin, LOW);
    while(digitalRead(encButton))
    {
    }
    delay(500);
    Serial.print("end read = ");Serial.println(oldPosition);
    lcd.clear();

}