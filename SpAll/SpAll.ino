#include <avr/wdt.h>
#include <avr/eeprom.h>

void setup() {

//  pinMode(11, OUTPUT);
  wdt_disable();
  Serial.begin(9600);

  checkStart();
  
  PrintForm();

  setupBort();
  setupLoRa();
  setupGeo();
  setupEnv();
  setupCam();

  EndB(0);
  delay(100);
  wdt_enable (WDTO_8S);

}


void loop() {

   bort();
   geo();
   env();
   cam();
   Serial.println();
   wdt_reset();
   Zer();
}
