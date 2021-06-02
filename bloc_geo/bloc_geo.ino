#include <TinyGPS++.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <MS5611.h>
#define ss Serial3 
#define fl Serial2

TinyGPSPlus gps;

Adafruit_MPU6050 mpu;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

MS5611 ms5611;

double referencePressure;



void setup() {

  Serial.begin(9600);
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
  
//  delay(500);
  
  display.setTextSize(1);
  
  display.setTextColor(WHITE);
  
  display.setRotation(0);


  // Initialize MS5611 sensor
  Serial.println("Initialize MS5611 Sensor");
  
  while(!ms5611.begin())
  {
  Serial.println("Could not find a valid MS5611 sensor, check wiring!");
  delay(500);
  }
  
  // Get reference pressure for relative altitude
  referencePressure = ms5611.readPressure();
  
  // Check settings
  checkSettings();
}

void checkSettings()
{
Serial.print("Oversampling: ");
Serial.println(ms5611.getOversampling());
}

void loop() {

  GPS();
  Axel();  
  Pres();

  Serial.println("\nend\n");

  delay(1000);
}




void Temp4()
{
  
  long realPressure = ms5611.readPressure();
  
  // Calculate altitude
  float absoluteAltitude = ms5611.getAltitude(realPressure);
  float relativeAltitude = ms5611.getAltitude(realPressure, referencePressure);
  
  Serial.print("Pressure = ");
  Serial.print(realPressure);
  Serial.println(" Pa");
  
  Serial.print(" absoluteAltitude = ");
  Serial.print(absoluteAltitude);
  Serial.print(" m\nrelativeAltitude = ");
  Serial.print(relativeAltitude);
  Serial.println(" m");


}



void Axel()
{

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  display.clearDisplay();
  display.setCursor(0, 0);
  
  Serial.print("Accelerometer ");
  Serial.print("X: ");
  Serial.print(a.acceleration.x, 1);
  Serial.print(" m/s^2, ");
  Serial.print("Y: ");
  Serial.print(a.acceleration.y, 1);
  Serial.print(" m/s^2, ");
  Serial.print("Z: ");
  Serial.print(a.acceleration.z, 1);
  Serial.println(" m/s^2");
  
  display.println("Accelerometer - m/s^2");
  display.print(a.acceleration.x, 1);
  display.print(", ");
  display.print(a.acceleration.y, 1);
  display.print(", ");
  display.print(a.acceleration.z, 1);
  display.println("");
  
  Serial.print("Gyroscope ");
  Serial.print("X: ");
  Serial.print(g.gyro.x, 1);
  Serial.print(" rps, ");
  Serial.print("Y: ");
  Serial.print(g.gyro.y, 1);
  Serial.print(" rps, ");
  Serial.print("Z: ");
  Serial.print(g.gyro.z, 1);
  Serial.println(" rps");
  
  display.println("Gyroscope - rps");
  display.print(g.gyro.x, 1);
  display.print(", ");
  display.print(g.gyro.y, 1);
  display.print(", ");
  display.print(g.gyro.z, 1);
  display.println("");
  
  display.display();
  
}




void GPS()
{

   Serial.println(F("Sats HDOP  Latitude   Longitude   Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum"));
   Serial.println(F("           (deg)      (deg)       Age                      Age  (m)    --- from GPS ----  ---- to London  ----  RX    RX        Fail"));
   Serial.println(F("----------------------------------------------------------------------------------------------------------------------------------------"));
  
  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;

  printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
  printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
  printInt(gps.location.age(), gps.location.isValid(), 5);
  printDateTime(gps.date, gps.time);
  printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
  printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);
  printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
  printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.deg()) : "*** ", 6);
 
  unsigned long distanceKmToLondon =
    (unsigned long)TinyGPSPlus::distanceBetween(
      gps.location.lat(),
      gps.location.lng(),
      LONDON_LAT, 
      LONDON_LON) / 1000;
  printInt(distanceKmToLondon, gps.location.isValid(), 9);
 
  double courseToLondon =
    TinyGPSPlus::courseTo(
      gps.location.lat(),
      gps.location.lng(),
      LONDON_LAT, 
      LONDON_LON);
 
  printFloat(courseToLondon, gps.location.isValid(), 7, 2);
 
  const char *cardinalToLondon = TinyGPSPlus::cardinal(courseToLondon);
 
  printStr(gps.location.isValid() ? cardinalToLondon : "*** ", 6);
 
  printInt(gps.charsProcessed(), true, 6);
  printInt(gps.sentencesWithFix(), true, 10);
  printInt(gps.failedChecksum(), true, 9);
  Serial.println();
  
  smartDelay(1000);
 
  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
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
 


static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
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
  }
 
  printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}
 


static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartDelay(0);
}
