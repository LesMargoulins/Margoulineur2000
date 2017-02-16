# include "margoulinade.h"

//EEPROM DATA
// SLOT 0: MEM CLEARED ? 1 -> yes 
// SLOT 1: PASSWORD SET ? 1 -> yes
// SLOT 3: BUZZER DISABLED ? 1 -> yes

void    checkMem()
{
  if(EEPROM.read(0) != 0)
  {
      lcd.print(F("RESET EEPROM"));
      for (int i = 0 ; i < 1023 ; i++)
      {
        EEPROM.write(i, 0);
      }
      lcd.setCursor(0, 1);
      lcd.print(F("OK"));
      wait4button();
  }
}

void    eepromDebug()
{
  int value;
  int address;
  
  while(wait4button)
  {
    value = EEPROM.read(address);
    lcd.print(address);
    lcd.print(F(":"));
    lcd.print(value);
    lcd.print(F("|"));
    lcd.print(EEPROM.length());
    lcd.print(F("."));
    address++;
    delay(500);
    lcd.clear();
  } 
  wait4button();
}

void    selectMenu()
{

}

void    checkPassword()
{
  int i = 0;
  char storedpasswd[PASSWDLENGHT + 2] = "";
  Serial.print(PASSWDLENGHT);
  if (true || EEPROM.read(1) == 1)
  {
    lcd.print(F("Enter passwd"));
    lcd.setCursor(0, 1);
    while(i < PASSWDLENGHT)
    {
      while(digitalRead(encButton))
      {
          int newPosition = myEnc.read();
          if ((newPosition != oldPosition) )
          {
            oldPosition = newPosition;
            if(oldPosition > (PASSWDELEMENTS * ENCODERSTEPS) - 1)
            {
               myEnc.write(0);
            } 
            else if(oldPosition < 0)
            {
               myEnc.write(PASSWDELEMENTS * ENCODERSTEPS - 1);
            }
            Serial.println(newPosition / ENCODERSTEPS);
            cycleMenu = (abs(newPosition) / ENCODERSTEPS) / MENUELEMENTS;
            lcd.clear();
          }
          lcd.setCursor(0, 1);
          lcd.print(storedpasswd);
          lcd.setCursor(i, 1);
          lcd.print(passwdChars[newPosition / ENCODERSTEPS]);
          storedpasswd[i] = passwdChars[newPosition / ENCODERSTEPS];
      }
          beep(50);
          delay(200);
        i++;
    }
    verifPasswd(storedpasswd);
  }
}

int                 verifPasswd(char storedpasswd[PASSWDLENGHT + 2])
{
  Serial.print(storedpasswd);
  lcd.clear();
  Serial.print(">");
  Serial.print(storedpasswd);
  Serial.print("<>");
  Serial.print(password);
  Serial.print("<");
  if (strcmp(storedpasswd, password) == 0)
  {
     Serial.println(F("\n\nDONE ! passwd is ok"));
     beep(50);
     beep(50);
     beep(50);
  }
  else
  {
    Serial.println(F("\n\Wrong passwd"));
     beep(200);
     beep(200);
     checkPassword();
  }
  delay(250);
}

void                about()
{   
    lcd.print(F("By guigur&oborotev"));
    lcd.setCursor(0, 1);
    lcd.print(F("2015-2016 china"));
  midi();
    wait4button();
}

void                buzzer_eeprom()
{

    lcd.print(F("Buzzer"));
    lcd.setCursor(0, 1);
    if (EEPROM.read(3) == 0)
    {
      lcd.print(F("DISABLE"));
    }
    else
    {
         lcd.print(F("ENABLE"));
    }
    wait4button();
    if (EEPROM.read(3) == 0)
    {
      EEPROM.put(3, 1);  
    }
    else
    {
      EEPROM.put(3, 0);
      silentMode = false;
    }
}

void                batStatus()
{   
    delay(250);
    
    while (digitalRead(encButton))
    {
      lcd.setCursor(0, 0);

      if (digitalRead(BatFull) == 0 && digitalRead(BatCharg) == 0)
        {
               Serial.println(F("Error charging circuit"));
               lcd.print(F("chrg circ. fail"));
        }
      else if (digitalRead(BatFull) == 1 && digitalRead(BatCharg) == 0)
        {
               Serial.println(F("Charging"));
               lcd.print(F("    Charging    "));
        }
      else if (digitalRead(BatFull) == 0 && digitalRead(BatCharg) == 1)
        {
               Serial.println(F("Battery full"));
               lcd.print(F("  Battery full  "));
        }
      else
        {
               Serial.println(F("On battery"));
               lcd.print(F("   On battery   "));
        }
      lcd.setCursor(0, 1);
      lcd.print(F("V : "));
      int v = map(analogRead(A1), 0, 1023, 0, 859);
      lcd.print(v/100);
      lcd.print(F("."));
      lcd.print(v%100);
      lcd.setCursor(9 ,1);
      lcd.print(F("% : "));
      lcd.print(map(map(analogRead(A1), 0, 1023, 0, 859), 0,470, 0, 100));
  
      delay(200);
    }
    lcd.clear();
    beep(50);
    delay(250);
}

int tonePin = 5;

void midi() {

    tone(tonePin, 184, 416.66625);
    delay(462.9625);
    delay(92.5925);
    tone(tonePin, 277, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 416.66625);
    delay(462.9625);
    delay(92.5925);
    tone(tonePin, 207, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 246, 494.791171875);
    delay(549.76796875);
    delay(5.78703125);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 207, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 207, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(283.56453125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 277, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 207, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 207, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 155, 494.791171875);
    delay(549.76796875);
    delay(5.78703125);
    tone(tonePin, 138, 666.666);
    delay(740.74);
    delay(648.1475);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 277, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 207, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 207, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 246, 494.791171875);
    delay(549.76796875);
    delay(5.78703125);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 207, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 207, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 277, 494.791171875);
    delay(549.76796875);
    delay(5.78703125);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 207, 494.791171875);
    delay(549.76796875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 207, 494.791171875);
    delay(549.76796875);
    delay(5.78703125);
    tone(tonePin, 155, 666.666);
    delay(740.74);
    delay(648.1475);
    tone(tonePin, 184, 369.791296875);
    delay(410.87921875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(283.56453125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(283.56453125);
    tone(tonePin, 233, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 277, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 277, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 277, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 246, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 246, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(144.67578125);
    tone(tonePin, 138, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 155, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 155, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 138, 119.791546875);
    delay(133.10171875);
    delay(283.56453125);
    tone(tonePin, 233, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 277, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 311, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 277, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 311, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 277, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 246, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 207, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 207, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 207, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(422.45328125);
    tone(tonePin, 138, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(700.23078125);
    tone(tonePin, 138, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 233, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 155, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 155, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 155, 416.66625);
    delay(462.9625);
    delay(648.1475);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 494.791171875);
    delay(549.76796875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 155, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(283.56453125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 155, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(283.56453125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(283.56453125);
    tone(tonePin, 233, 666.666);
    delay(740.74);
    delay(92.5925);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 494.791171875);
    delay(549.76796875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 155, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(283.56453125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 155, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(283.56453125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(283.56453125);
    tone(tonePin, 233, 666.666);
    delay(740.74);
    delay(92.5925);
    tone(tonePin, 233, 416.66625);
    delay(462.9625);
    delay(92.5925);
    tone(tonePin, 277, 369.791296875);
    delay(410.87921875);
    delay(5.78703125);
    tone(tonePin, 277, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 246, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 207, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 207, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 869.790796875);
    delay(966.43421875);
    delay(144.67578125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 207, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 207, 666.666);
    delay(740.74);
    delay(92.5925);
    tone(tonePin, 207, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 494.791171875);
    delay(549.76796875);
    delay(5.78703125);
    tone(tonePin, 207, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 155, 1494.79017187);
    delay(1660.87796875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(283.56453125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 138, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 138, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 138, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 233, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 277, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 277, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 246, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 233, 244.791421875);
    delay(271.99046875);
    delay(5.78703125);
    tone(tonePin, 207, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 207, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 244.791421875);
    delay(271.99046875);
    delay(144.67578125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
    tone(tonePin, 184, 119.791546875);
    delay(133.10171875);
    delay(5.78703125);
}
