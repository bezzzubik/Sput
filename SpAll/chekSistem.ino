#define COUNTBLOK 10



int hstart=-10;

int asd=-10;

float gg;



bool startZ()
{

  if(gps.hdop.isValid())
  {
    asd=gps.hdop.hdop();
    if(hstart==-10)
       hstart=gps.hdop.hdop(); 
     else if((hstart+5) < asd)
       zap=true;
  }
    return zap;
}



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
      for(i=1;(i<COUNTBLOK) && (a==0); i++)
          if(eeprom_read_byte(i) == 0)
            a=i;
      eeprom_update_byte(a, 2);
      Serial.print("System");
      Serial.print(a);
      Serial.println(" is off");
      Serial1.print("System");
      Serial1.print(a);
      Serial1.println(" is off");

  }
}



void Rez()
{
  for(int i=1; i<COUNTBLOK; i++)
    if(eeprom_read_byte(i) == 0)
      eeprom_update_byte(i, 1);
}




void EndB(int i)
{ 
     eeprom_update_byte(i, 1); 
}



bool checkBlock(int i)
{
  if ( eeprom_read_byte(i) == 2)
     return false;
  else
  {
     eeprom_update_byte(i, 0); 
     return true; 
  }
}
