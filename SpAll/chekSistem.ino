#define COUNTBLOK 10


void checkStart() 
{
  int i=0;
  int a=0;
  int k;
  for(;(i<COUNTBLOK) && (a==0); i++)
    if ( eeprom_read_byte(i)!=0)
        a=1;
  if(a!=0)
  {
      a=0;
      for(i=0;(i<COUNTBLOK) && (a==0); i++)
          if(eeprom_read_byte(i) == 0)
            a=i+1;
      eeprom_update_byte(a-1, 2);
  }
}



void Zer()
{
  for(int i=1; i<COUNTBLOK; i++)
    if(eeprom_read_byte(i))
      eeprom_update_byte(i, 0);
}




void EndB(int i)
{ 
     eeprom_update_byte(i, 1); 
}



bool checkBlock(int i)
{
  if ( eeprom_read_byte(i))
     return false;
  else
     return true; 
}
