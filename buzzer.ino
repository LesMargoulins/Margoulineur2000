# include "margoulinade.h"

void beep(unsigned char delayms)
{
  if(silentMode == false && EEPROM.read(3) == 0)
  {
    tone(buzzer, 440, delayms);
    //analogWrite(buzzer, 100);
    delay(delayms);
    analogWrite(buzzer, 0);
    delay(delayms);
  }  
}
