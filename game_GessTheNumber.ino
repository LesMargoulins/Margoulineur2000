void    menu_select()
{
  lcd.clear();
  int menu_elements;
  int menu_selected;
  String choices[2] = {{"enable"},{"disable"}};
  menu_elements = 2;
  menu_selected = 0;
  
  lcd.print(F("ligne 1"));
  while(!digitalRead(encButton))
  {
    
  }
  lcd.print(choices[menu_selected]);
  wait4button();
}

void    gess_the_number()
{
  int max_nbr = 50;
  int min_nmr = 0;

  
  while(1)
  {
    lcd.print(F("Gess the number !"));
    
  }
}

