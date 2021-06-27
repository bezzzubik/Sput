#include <TinyGPS++.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <MS5611.h>

#define KX 9.5
#define KY 9.81
#define KZ 9.0
#define adr 0x1E
#define ss Serial3 
#define fl Serial2


TinyGPSPlus gps;

Adafruit_MPU6050 mpu;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

MS5611 ms5611;

double referencePressure;

double la0, lo0, h0;


void setupGeo() {

  ss.begin(9600);
  fl.begin(9600);

  
  if (!mpu.begin())
    while (1)
      yield();
  
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
      for (;;)
        ;
  }
  
  display.display();
  
//  delay(10);
  
  display.setTextSize(1);
  
  display.setTextColor(WHITE);
  
  display.setRotation(0);


  // Initialize MS5611 sensor
//  Serial.println("Initialize MS5611 Sensor");
  
  while(!ms5611.begin())
  {
  Serial.println("Could not find a valid MS5611 sensor, check wiring!");
  delay(10);
  }
 
  // Get reference pressure for relative altitude
  referencePressure = ms5611.readPressure();

  StartComp();

  // Check settings
}


void StartComp()
{

    Wire.begin(); // инициализация I2C
  
    // Задаём режим работы датчика HMC5883:
    Wire.beginTransmission(adr);
    Wire.write(0x00); // выбираем регистр управления CRA (00)
    Wire.write(0x70); // записываем в него 0x70 [усреднение по 8 точкам, 15 Гц, нормальные измерения]
    Wire.write(0xA0); // записываем в регистр CRB (01) 0xA0 [чувствительность = 5]
    Wire.write(0x00); // записываем в регистр Mode (02) 0x00 [бесконечный режим измерения]
    Wire.endTransmission();
  
}




void geo() {

  if(checkBlock(5))
     Axel();
  if(checkBlock(6))
    Compas();
  if(checkBlock(7))
    Pres();
  delay(10);
}





void Compas()
{
  numbl=23;  
  Wire.beginTransmission(adr);
  Wire.write(0x03);
  Wire.endTransmission();
  
  Wire.requestFrom(adr, 6);
  while( Wire.available() )  
  { 
    int h = Wire.read();
    int l = Wire.read();
    int x = word(h, l);

    int y = Wire.read();
    y = y << 8;
    y = y | Wire.read();
    
    int z = Wire.read() << 8;
    z |= Wire.read();

    int Abs=(int)sqrt( pow(x,2) + pow(y,2) + pow(z,2) );

    PrintIn(x, 4);
    if(printLoRa())
       printIntL("Com",x);
    numbl++;
    PrintIn(y, 4);

    if(printLoRa())
       printIntL("Com",y);
    numbl++;
    
    PrintIn(z, 4);

    if(printLoRa())
       printIntL("Com",z);
    numbl++;
    
    PrintIn(Abs, 4);

    if(printLoRa())
       printIntL("Com", Abs);
  }
  
  EndB(6);
}





void Pres()
{
  numbl=27;
  long realPressure = ms5611.readPressure();
  
  // Calculate altitude
  float absoluteAltitude = ms5611.getAltitude(realPressure);
  float relativeAltitude = ms5611.getAltitude(realPressure, referencePressure);

  PrintIn(realPressure, 6);
    
   if(printLoRa())
      printIntL("P", realPressure);
    numbl++;
  PrintFl(absoluteAltitude, 8, 2);
  PrintFl(relativeAltitude, 8, 2);
  if(printLoRa())
    printFL2L("AbsH", absoluteAltitude);
  numbl++;
  if(printLoRa())
    printFL2L("relH", relativeAltitude);

  EndB(7);
  
}



void Axel()
{

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  display.clearDisplay();
  display.setCursor(0, 0);
  float x, y, z;
  x=a.acceleration.x*9.81/KX;
  z=a.acceleration.z*9.81/KZ;
  y=a.acceleration.y*9.81/KY;  
  double all=sqrt( pow(x, 2) + pow(y, 2) + pow(z, 2) ); 

  numbl=15;
  PrintFl(x, 4, 1);  
  if(printLoRa())
    printAGL(x);
  numbl++;

  PrintFl(y, 4, 1);
  if(printLoRa())
    printAGL(y);
  numbl++;
    
  PrintFl(z, 4, 1);
  
  if(printLoRa())
    printAGL(z);
  numbl++;
    
  PrintFl(all, 4, 1);
  
  if(printLoRa())
    printAGL(all);
    

  
  numbl++;
    
  all=sqrt( pow(g.gyro.x, 2) + pow(g.gyro.y, 2) + pow(g.gyro.z, 2) );

  PrintFl(g.gyro.x, 6, 1);

  if(printLoRa())
    printAGL(g.gyro.x);
  numbl++;

  PrintFl(g.gyro.y, 6, 1);
  if(printLoRa())
    printAGL(g.gyro.y);
  numbl++;
  
  PrintFl(g.gyro.z, 6, 1);
  if(printLoRa())
    printAGL(g.gyro.z);
  numbl++;
  
  PrintFl(all, 6, 1);
  if(printLoRa())
    printAGL(all);
  numbl++;

  if(all > 0.6 || z > 0.6)
   if(boD)
    Serial1.println("\nI down'_'");
  
  EndB(5);
}

double dism;

  static double LONDON_LAT, LONDON_LON;


void GPS()
{
  numbl=12;
  
  k=5;
 
  if(LONDON_LAT == 0)
    if(gps.location.isValid() && ((millis()-Time)/(1000*60))>1)
      {
         LONDON_LAT=gps.location.lat();
         LONDON_LON=gps.location.lng();
      }

 if( (dism/h0>2) && ((millis()-Time)/(1000*60*60) < 1) && (k > 4) )
 {
    Serial1.print("prLat ");
    Serial1.print(la0+((gps.location.lat()-la0)*h0/(h0-gps.hdop.hdop())));
    Serial1.print(" prLng ");
    Serial1.print(lo0+((gps.location.lat()-lo0)*(h0/(h0-gps.hdop.hdop()))));
    k=0;
    Serial1.print(' ');
  }
  printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
  printFloat((h0=gps.hdop.hdop()), gps.hdop.isValid(), 6, 1);
  printFloat1(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
  printFloat((la0=gps.location.lat()), gps.location.isValid(), 11, 6);
  printFloat((lo0=gps.location.lng()), gps.location.isValid(), 12, 6);

    Serial1.print("lat ");
    printFloat1(gps.location.lat(), gps.location.isValid(), 11, 6);
  k++;
  numbl++;

    Serial1.print("lng ");
    printFloat1(gps.location.lng(), gps.location.isValid(), 12, 6);

  numbl++;
  
  printInt(gps.location.age(), gps.location.isValid(), 5);
  
  TinyGPSTime t=gps.time;
  TinyGPSDate d=gps.date;
//{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
    {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
  
    Serial.print(sz);
  }
  
  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);

    Serial1.print(sz);
  }
 
  printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
  //}
  printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
  printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);
  printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
  printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.deg()) : "*** ", 6);
  unsigned long distanceKmToLondon =
    TinyGPSPlus::distanceBetween(
      gps.location.lat(),
      gps.location.lng(),
      LONDON_LAT,
      LONDON_LON);
  printInt(distanceKmToLondon, gps.location.isValid(), 9);

  double courseToLondon =
    TinyGPSPlus::courseTo(
      gps.location.lat(),
      gps.location.lng(),
      LONDON_LAT, 
      LONDON_LON);
 
  printFloat(courseToLondon, gps.location.isValid(), 7, 2);
 
  const char *cardinalToLondon = TinyGPSPlus::cardinal(courseToLondon);


  if(gps.location.isValid())
  {

  Serial1.print(cardinalToLondon);
  Serial1.print(' ');
  Serial1.print( (dism=distanceKmToLondon) );
  Serial1.print(' ');
  
  }

  if(gps.hdop.isValid() && gps.location.isValid())
  {
    Serial1.print("az ");
    Serial1.print(atan(distanceKmToLondon/gps.hdop.hdop())*180/PI);
    Serial1.print(' ');
  }

  
  printStr(gps.location.isValid() ? cardinalToLondon : "*** ", 6);
 
  printInt(gps.charsProcessed(), true, 6);
  printInt(gps.sentencesWithFix(), true, 10);
  printInt(gps.failedChecksum(), true, 9);
  
  smartDelay(1000);
 
  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
  EndB(4);
}
 


static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
 


static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}
 

static void printFloat1(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
    Serial1.print('*');
    Serial1.print(' ');
  }
  else
  {
    Serial1.print(val, prec);
    Serial1.print(' ');

  }
  smartDelay(0);
}

static void printInt1(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial1.print(sz);
  smartDelay(0);
}


static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartDelay(0);
}
 
 


static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartDelay(0);
}
