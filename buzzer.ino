# include "margoulinade.h"

void beep(unsigned char delayms)
{
  analogWrite(buzzer, 100);
  delay(delayms);
  analogWrite(buzzer, 0);
  delay(delayms); 
}  
