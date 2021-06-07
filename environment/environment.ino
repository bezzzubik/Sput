#include <radSens1v2.h>


ClimateGuard_RadSens1v2 radSens(RS_DEFAULT_I2C_ADDRESS); /*Constructor of the class ClimateGuard_RadSens1v2,
                                                           sets the address parameter of I2C sensor.
                                                           Default address: 0x66.*/


void setup() 
{
  Serial.begin(115200);  

  radSens.radSens_init();
}

void loop(){
  Serial.print("Rad intensy dyanmic: ");

  Serial.println(radSens.getRadIntensyDyanmic());

  Serial.print("Rad intensy static: ");
  
  Serial.println(radSens.getRadIntensyStatic());

  Serial.print("Number of pulses: ");

  Serial.println(radSens.getNumberOfPulses());

  delay(2000);
}
