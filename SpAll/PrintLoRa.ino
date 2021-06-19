int pflag1=0;
int pflag2=0;



void setupLoRa() 
{
// put your setup code here, to run once:

pinMode(8, OUTPUT);
pinMode(9, OUTPUT);
Serial1.begin(9600);

}


void PrintL(){

char c[10];
//a=97, z=122
  if(Serial1.available()!=0){
    for(int i=0; Serial1.available()!=0; i++)
       c[i]=Serial1.read();
    Serial1.println(c);
  }
  switch (c[0])
  {
    
  case 't': on_the_block(c[1]);
              break;
  case 'f': off_the_block(c[1]);
              break;
  case 'p': print_zn(c[1]);
              break;
  case 'd': //off_print(c[1]);
              break;
  default :
            Serial1.print("Com is not ...");
    
  }
}


void print_zn(char d)
{

    

}




void off_the_block(char d)
{
  
  int a=(int)d-48;
  eeprom_update_byte(a, 2);
  Serial1.print("Block ");
  Serial1.print(a);
  Serial1.println("is off");
  return;

}




void on_the_block(char d)
{
  
  int a=(int)d-48;
  eeprom_update_byte(a, 0);
  Serial1.print("Block ");
  Serial1.print(a);
  Serial1.println("is on");
  return;

}




bool printLoRa()
{
  if(numbl<19)
    if(!((int)pow(2,numbl)&pflag1))
      return true;
    else
      return false;
  else
    if(!((int)pow(2,numbl-19)&pflag2))
      return true;
    else
      return false;
}


void printTL(int t, int j)
{

  Serial1.print('T');
  Serial1.print(j);
  Serial1.print('=');
  Serial1.print(t);
  Serial1.print(' ');  

}

void printCountL(int j)
{
  
  Serial1.print("CoT=");
  Serial1.print(j);
  Serial1.print(' ');  
  
}


void printHeL(int o)
{
  Serial1.print("Heat ");
  if(o)
      Serial1.print("on ");
  else
      Serial1.print("off ");
  return;
}



void printFL2L(char* n, float v)
{

  if(n[0] == 'V')
  {
    Serial1.print('V');
    Serial1.print(numbl+3);
  }  
  else
    Serial1.print(n);

  Serial1.print('=');
  Serial1.print(v,2);
  Serial1.print(' ');
 
}



void printAGL(float A)
{
  int i=numbl;
  if(i<19)
     Serial1.print('g');
  else{
     Serial1.print("gir");
     i=i-4;
  }
  XYZprL(i-15);

  Serial1.print('=');
  Serial1.print(A, 1);
  Serial1.print(' ');
}



void printIntL(char* n, long A)
{

  Serial1.print(n);
  if( numbl>=23 && numbl<=26)
      XYZprL(numbl-23);
      
  Serial1.print('=');
  Serial1.print(A);
  Serial1.print(' ');
}

void XYZprL(int i)
{

  switch(i)
  {
    case 0: Serial1.print('x');
              break;
    case 1: Serial1.print('y');
              break;  
    case 2: Serial1.print('z');
              break;
  }
}


void PrintP(long p)
{
  Serial1.print("P=");
  Serial1.print(p);
  Serial1.print(' ');  
}

void prH(double a)
{
  Serial1.print(a, 2);
  Serial1.print(' ');
  
}


/*
   T1 - 1
   T2 - 2
   T3 - 3
   T4 - 4
   T5 - 5
   Количество температурных датчиков - 6
   Состояние нагревателя - 7
   I - 8a
   V12 - 9
   V13 - 10
   V14 - 11
   Lat - 12
   Long - 13
   Time - 14
   accelerometer
   X - 15
   Y - 16
   Z - 17
   ALL - 18
   
   gyro
   X - 19
   Y - 20
   Z - 21
   ALL - 22
   compas
   X - 23
   Y - 24
   Z - 25
   ALL - 26
   Давление - 27
   AbsoluteAltitude - 28
   relativeAltitude - 29
   Rad dyn - 30
   Rad stat - 31
   Num of puls - 32
   gaz - 33
   temp - 34
   Hum - 35
 */


 
