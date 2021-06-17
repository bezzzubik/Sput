#include <avr/wdt.h>
#include <avr/eeprom.h>

void setup() {

//  pinMode(11, OUTPUT);
  wdt_disable();
  Serial.begin(9600);

  checkStart();
  delay(100);
  
  wdt_enable (WDTO_8S);

  PrintForm();

  setupBort();
  setupLoRa();
  setupGeo();
  setupEnv();
  setupCam();

  EndB(0);
  delay(100);
}


void loop() {

   wdt_reset();
   bort();
   geo();
   
   wdt_reset();
   
   env();
   cam();
   Serial1.println();
   Serial.println();
   Zer();
}
