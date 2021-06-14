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

  PrintIn(759, 3); //сила тока

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


  PrintIn(4, 3);

  PrintIn(13, 2);

  PrintFl(27.40, 6, 2);

  PrintFl(27.40, 5, 2);

  Serial.println();
  
  delay(1000);
  
}

void PrintIn(int t, int i)
{

  char buf[7];
  sprintf(buf, "%*d ", i, t);
  Serial.print(buf);
  
}


void PrintFl(float f, int i, int p)
{
  
  char buf[7];  
  sprintf(buf, "%*.*f ", i, p, f);
  Serial.print(buf);
  
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
  Serial.print(F(" Ax   Ay   Az   Aa    Gx     Gy     Gz     Ga    Fx   Fy   Fz   Fa    P       abA     reA     RadD   RadS  NumP gaz   T     Hum "));

}
