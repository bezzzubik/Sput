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

void setupEnv()
{
  setupDos();
  setupDHT_Gas();

}


void env()
{

  Dos();
  delay(100);
  DHT_Gas();
  delay(100);

}




void setupDos() 
{
  
  radSens.radSens_init(); /*Initialization function and sensor connection. 
                            Returns false if the sensor is not connected to the I2C bus.*/
  
  uint8_t sensorChipId = radSens.getChipId(); /*Returns chip id, default value: 0x7D.*/

  uint8_t firmWareVer = radSens.getFirmwareVersion(); /*Returns firmware version.*/

  uint8_t sensitivity = radSens.getSensitivity(); /*Rerutns the value coefficient used for calculating
                                                    the radiation intensity or 0 if sensor isn't connected.*/

  radSens.setSensitivity(55); /*Sets the value coefficient used for calculating
                                the radiation intensity*/

  sensitivity = radSens.getSensitivity();

  radSens.setSensitivity(105);

  radSens.getSensitivity();

  bool hvGeneratorState = radSens.getHVGeneratorState(); /*Returns state of high-voltage voltage Converter.
                                                           If return true -> on
                                                           If return false -> off or sensor isn't conneted*/


  radSens.setHVGeneratorState(false); /*Set state of high-voltage voltage Converter.
                                        if setHVGeneratorState(true) -> turn on HV generator
                                        if setHVGeneratorState(false) -> turn off HV generator*/
  
  hvGeneratorState = radSens.getHVGeneratorState();

  radSens.setHVGeneratorState(true);

  hvGeneratorState = radSens.getHVGeneratorState();
  
}




void Dos(){
  Serial.print("Rad intensy dyanmic: ");

  Serial.println(radSens.getRadIntensyDyanmic());

  Serial.print("Rad intensy static: ");
  
  Serial.println(radSens.getRadIntensyStatic());

  Serial.print("Number of pulses: ");

  Serial.println(radSens.getNumberOfPulses());

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
    
    // Initialize device.
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
