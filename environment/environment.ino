#include <radSens1v2.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2
#define DHTTYPE DHT11


DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

ClimateGuard_RadSens1v2 radSens(RS_DEFAULT_I2C_ADDRESS); /*Constructor of the class ClimateGuard_RadSens1v2,
                                                           sets the address parameter of I2C sensor.
                                                           Default address: 0x66.*/

void setup()
{

  Serial.begin(115200);  
  setupDos();
  setupDHT_Gas();

}


void loop()
{

  Dos();
  DHT_Gas();

  Serial.println("\n\nend cicle\n");
}




void setupDos() 
{

  radSens.radSens_init();

}




void Dos(){
  Serial.print("Rad intensy dyanmic: ");

  Serial.println(radSens.getRadIntensyDyanmic());

  Serial.print("Rad intensy static: ");
  
  Serial.println(radSens.getRadIntensyStatic());

  Serial.print("Number of pulses: ");

  Serial.println(radSens.getNumberOfPulses());

  delay(2000);
}




void setupDHT_Gas() {
    
    pinMode(42, OUTPUT);
    pinMode(43, OUTPUT);
    pinMode(31, OUTPUT);
    pinMode(32, OUTPUT);
    digitalWrite(42, HIGH);
    digitalWrite(43, HIGH);
    digitalWrite(31, HIGH);
    digitalWrite(32, HIGH);
    
    dht.begin();
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    dht.humidity().getSensor(&sensor);
    delayMS = sensor.min_delay / 1000;
}


void DHT_Gas() {

  delay(delayMS);
  float gaz;
  int sensorValueGaz = analogRead(A3);
  gaz=(float)sensorValueGaz/1024*5.0;
  Serial.print("газ=");
  Serial.println(gaz*100,0);
  

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  
  if (isnan(event.temperature)) {
   Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
}
