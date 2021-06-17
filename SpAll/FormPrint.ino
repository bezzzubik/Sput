
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

  if( i == 0){
    Serial.print("off ");
    Serial1.print("off ");
  }else{
    Serial.print(" on ");
    Serial1.print(" on ");  
  }
}



void PrintForm()
{
  Serial.print("T1    T2   T3   T4   T5  HEA   I    V1    V2    V3  ");
  Serial.print(F("Sats HDOP  Latitude   Longitude   Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum"));
  Serial.println(F(" Ax   Ay   Az   Aa    Gx     Gy     Gz     Ga    Fx   Fy   Fz   Fa    P       abA     reA     RadD   RadS  NumP gaz   T     Hum "));

}
