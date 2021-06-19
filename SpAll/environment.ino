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

  if(checkBlock(8))
    setupDos();
  if(checkBlock(9))
    setupDHT_Gas();

}


void env()
{

  if(checkBlock(8))
    Dos();
  delay(50);
  if(checkBlock(9))
    DHT_Gas();
  delay(50);

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

  numbl=30;
  PrintFl(radSens.getRadIntensyDyanmic(), 6, 2);
  Serial1.print("RI=");
  Serial1.print(radSens.getRadIntensyDyanmic());
  Serial1.print(' ');

  if(printLoRa())
    printFL2L("RadDy", radSens.getRadIntensyDyanmic());
  numbl++;

  if(printLoRa())
    printFL2L("RadSt", radSens.getRadIntensyStatic());
  numbl++;

  PrintFl(radSens.getRadIntensyStatic(), 6, 2);

  
  PrintIn(radSens.getNumberOfPulses(), 4);
  
  if(printLoRa())
    printFL2L("NumP", radSens.getNumberOfPulses());
  EndB(8);
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

  numbl=34;
  delay(delayMS);
  int gaz;
  float sensorValueGaz = analogRead(A3);
  gaz=(int)(100*(sensorValueGaz/1024*5.00));
  PrintIn(gaz, 3);
  if(printLoRa())
     printIntL("gaz", gaz);
  numbl++;

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  
  if (isnan(event.temperature))
   Serial.print(F("Err    "));
  else 
  {
    PrintFl(event.temperature, 6, 2);
    if(printLoRa())
      printFL2L("T", event.temperature);   
  }
  numbl++;
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
    Serial.print(F("Err   "));
  else{
    PrintFl(event.relative_humidity, 5, 2);
    if(printLoRa())
      printFL2L("Hum", event.relative_humidity);
  }
  EndB(9);
}
