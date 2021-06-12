#include <avr/wdt.h>

void setup() {

//  pinMode(11, OUTPUT);
  wdt_disable();
  Serial.begin(9600);
  setupBort();
  setupGeo();
  setupEnv();
  setupCam();
  delay(500);
  wdt_enable (WDTO_8S);
}


void loop() {
   Serial.println("Bort:");
   bort();
   Serial.println("\nGeo:");
   geo();
   Serial.println("Inv:");
   env();
   cam();
   Serial.println("\n\nend cycle\n");
   wdt_reset();
}
