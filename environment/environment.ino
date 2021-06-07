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
  delay(1000);
  DHT_Gas();
  delay(1000);
  Serial.println("\n\nend cicle\n");
}




void setupDos() 
{
  
  radSens.radSens_init(); /*Initialization function and sensor connection. 
                            Returns false if the sensor is not connected to the I2C bus.*/
  
  uint8_t sensorChipId = radSens.getChipId(); /*Returns chip id, default value: 0x7D.*/

  Serial.print("Chip id: 0x");
  Serial.println(sensorChipId, HEX);

  uint8_t firmWareVer = radSens.getFirmwareVersion(); /*Returns firmware version.*/

  Serial.print("Firmware version: ");
  Serial.println(firmWareVer);

  Serial.println("-------------------------------------");
  Serial.println("Set Sensitivity example:\n");

  uint8_t sensitivity = radSens.getSensitivity(); /*Rerutns the value coefficient used for calculating
                                                    the radiation intensity or 0 if sensor isn't connected.*/

  Serial.print("\t getSensitivity(): "); Serial.println(sensitivity);
  Serial.println("\t setSensitivity(55)... ");

  radSens.setSensitivity(55); /*Sets the value coefficient used for calculating
                                the radiation intensity*/

  sensitivity = radSens.getSensitivity();
  Serial.print("\t getSensitivity(): "); Serial.println(sensitivity);
  Serial.println("\t setSensitivity(105)... ");

  radSens.setSensitivity(105);

  Serial.print("\t getSensitivity(): "); Serial.println(radSens.getSensitivity());

  bool hvGeneratorState = radSens.getHVGeneratorState(); /*Returns state of high-voltage voltage Converter.
                                                           If return true -> on
                                                           If return false -> off or sensor isn't conneted*/

  Serial.print("\n\t HV generator state: "); Serial.println(hvGeneratorState);
  Serial.println("\t setHVGeneratorState(false)... ");

  radSens.setHVGeneratorState(false); /*Set state of high-voltage voltage Converter.
                                        if setHVGeneratorState(true) -> turn on HV generator
                                        if setHVGeneratorState(false) -> turn off HV generator*/
  
  hvGeneratorState = radSens.getHVGeneratorState();
  Serial.print("\t HV generator state: "); Serial.println(hvGeneratorState);
  Serial.println("\t setHVGeneratorState(true)... ");

  radSens.setHVGeneratorState(true);

  hvGeneratorState = radSens.getHVGeneratorState();
  Serial.print("\t HV generator state: "); Serial.print(hvGeneratorState);
  Serial.println("\n-------------------------------------");
  
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
    Serial.println(F("DHTxx Unified Sensor Example"));
    // Print temperature sensor details.
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    Serial.println(F("------------------------------------"));
    Serial.println(F("Temperature Sensor"));
    Serial.print (F("Sensor Type: ")); Serial.println(sensor.name);
    Serial.print (F("Driver Ver: ")); Serial.println(sensor.version);
    Serial.print (F("Unique ID: ")); Serial.println(sensor.sensor_id);
    Serial.print (F("Max Value: ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
    Serial.print (F("Min Value: ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
    Serial.print (F("Resolution: ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
    Serial.println(F("------------------------------------"));
    // Print humidity sensor details.
    dht.humidity().getSensor(&sensor);
    Serial.println(F("Humidity Sensor"));
    Serial.print (F("Sensor Type: ")); Serial.println(sensor.name);
    Serial.print (F("Driver Ver: ")); Serial.println(sensor.version);
    Serial.print (F("Unique ID: ")); Serial.println(sensor.sensor_id);
    Serial.print (F("Max Value: ")); Serial.print(sensor.max_value); Serial.println(F("%"));
    Serial.print (F("Min Value: ")); Serial.print(sensor.min_value); Serial.println(F("%"));
    Serial.print (F("Resolution: ")); Serial.print(sensor.resolution); Serial.println(F("%"));
    Serial.println(F("------------------------------------"));
    // Set delay between sensor readings based on sensor details.
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
