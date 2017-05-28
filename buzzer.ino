/*
 * GUIGUR 2015 - 2017
 */
 
 # include "margoulinade.h"

void  beep(unsigned char delayms)
{
  if(silentMode == false && EEPROM.read(3) == 0)
  {
    //tone(buzzer, 440, delayms);
    analogWrite(buzzer, EEPROM.read(4));
    delay(delayms);
    analogWrite(buzzer, 0);
    delay(delayms);
  }  
}

void  buzzer_level_eeprom()
{
  int level= 0;
  int menuPos = myEnc.read();
        Serial.print("menu pos");
      Serial.println(menuPos);
  myEnc.write(42); //ne marche pas ?
  delay(200);
  lcd.clear();
  lcd.print(F("Buzzer level"));
          
      while(digitalRead(encButton))
      {
          int newPosition = myEnc.read();
          if ((newPosition != oldPosition) )
          {
            oldPosition = newPosition;
            if(oldPosition > (256 * ENCODERSTEPS) - 1)
            {
               myEnc.write(0);
            } 
            else if(oldPosition < 0)
            {
               myEnc.write(256 * ENCODERSTEPS - 1);
            }
            Serial.println(newPosition / ENCODERSTEPS);
            lcd.clear();
            lcd.print(F("Buzzer level"));
          }
          lcd.setCursor(0, 1);
          level = newPosition / ENCODERSTEPS;
          lcd.print(level);
      }
      lcd.clear();
      EEPROM.put(4, level);
      if(oldPosition > (256 * ENCODERSTEPS) - 1)
      {
        myEnc.write(0);
      }
      Serial.print("menu pos");
      Serial.println(menuPos);
}

