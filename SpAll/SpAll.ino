#include <avr/wdt.h>

void setup() {

//  pinMode(11, OUTPUT);
  wdt_disable();
  Serial.begin(9600);
  PrintForm();
  setupBort();
  setupGeo();
  setupEnv();
  setupCam();
  delay(500);
  wdt_enable (WDTO_8S);
}


void loop() {

   bort();
   geo();
   env();
   cam();
   Serial.println();
   wdt_reset();
}
