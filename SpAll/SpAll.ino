#include <avr/wdt.h>
#include <avr/eeprom.h>

int numbl=0;

void setup() {

//  pinMode(11, OUTPUT);
  wdt_disable();
  Serial.begin(9600);

  //checkStart();
  delay(100);
  PrintForm();
  
  wdt_enable (WDTO_8S);

  setupBort();
  setupLoRa();
  setupGeo();
  setupEnv();
  setupCam();

  EndB(0);
  delay(100);
}

byte a=0;

void loop() {
   if(a){
      PrintL();
      a=0;
   }else
      a=1;
   timeP();
   wdt_reset();
   numbl=1;
   bort();
   geo();
   wdt_reset();
   
   env();
   wdt_reset();
   cam();
   Serial1.println();
   Serial.println();
   Zer();
   wdt_reset();
}
