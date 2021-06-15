#include "stdio.h"

void setup() {

  Serial.begin(9600);
  PrintForm();

}

void loop() {

  PrintIn(35, 4); //Температуры
  PrintIn(0, 4);
  PrintIn(-35, 4);
  PrintIn(2, 4);
  PrintIn(-1, 4);  //Температуры

  PrintHeat(2);  //вкл\выкл

  PrintIn(759, 4); //сила тока

  PrintFl(3.05, 5, 2); //Напряжение
  PrintFl(12.52, 5, 2); //Напряжение
  PrintFl(0.51, 5, 2); //Напряжение


  PrintFl(9.33, 4, 1); //акслерерометр
  PrintFl(0.33, 4, 1); //акслерерометр
  PrintFl(0.33, 4, 1); //акслерерометр
  PrintFl(10.22, 4, 1); //акселерометр

  PrintFl(-4.2, 6, 1); //гироскоп
  PrintFl(-0.2, 6, 1); //гироскоп
  PrintFl(-23.2, 6, 1); //гироскоп
  PrintFl(223.2, 6, 1); //гироскоп

  PrintIn(-100, 4);// компас
  PrintIn(-100, 4);// компас
  PrintIn(-100, 4);// компас
  PrintIn(100, 4);// компас

  PrintIn(1000000, 6); //Давление

  PrintFl(12.55, 8, 2);
  PrintFl(12.55, 8, 2);

  PrintFl(3.00, 6, 2);
  PrintFl(3.00, 6, 2);


  PrintIn(4, 4);

  PrintIn(13, 3);

  PrintFl(27.40, 6, 2);

  PrintFl(27.40, 5, 2);

  Serial.println();
  
  delay(1000);
  
}

void PrintIn(int a, int k)
{ 
  int len, j;
  char c[32];
  itoa(a, c, 10);
  len=strlen(c);
  j=0;
  if(a<0)
  {
    j++;
    c[0]='-';
  }
  a=abs(a);
  len--;
  c[k]='\0';
  k--;
  for(; len < k; k--)
    c[k]=' ';
  for(; len>=j; len--)
  {
     c[len]=(a%10)+48;
    a=a/10;
  }
  Serial.print(c);
  Serial.print(' ');
}


void PrintFl(float f, int k, int l)
{ 
  int len, j, a;
  char c[32];
  a=(int)f;
  itoa(a, c, 10);
  len=strlen(c)+l;
  j=0;
  if(a<0)
  {
    j++;
    c[0]='-';
  }
  c[k]='\0';
  k--;
  for(; len < k; k--)
    c[k]=' ';
  for(int n=l; n != 0; n--)
    f=f*10;
  a=(int)f;
  a=abs(a);
  for(; l!=0; l--, len--)
  {
    c[len]=a%10+48;
    a=a/10;
  }
  c[len]='.';
  len--;
  for(; len>=j; len--)
  {
     c[len]=(a%10)+48;
    a=a/10;
  }
  Serial.print(c);
  Serial.print(' ');
}


void PrintHeat(int i)
{

  if( i == 0)
    Serial.print("off ");
  else
    Serial.print(" on ");  
}



void PrintForm()
{

  Serial.print(F(" T1   T2   T3   T4   T5  HEA   I    V1    V2    V3  "));
//  Serial.print(F("Sats HDOP  Latitude   Longitude   Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum"));
  Serial.println(F(" Ax   Ay   Az   Aa    Gx     Gy     Gz     Ga    Fx   Fy   Fz   Fa    P       abA     reA     RadD   RadS  NumP gaz   T     Hum "));

}
