#include <avr/wdt.h>
#include <avr/eeprom.h>

bool zap=false;

int Nap=255;

int numbl=0;

bool NapR=false;

bool graf=false;

bool boD=true;

unsigned long Time=millis();


int k;


void setup() {

//  pinMode(11, OUTPUT);
  wdt_disable();
  Serial.begin(9600);
  checkStart();
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

   Rez();
   if(startZ())
   {
   timeP();
   if(checkBlock(4))
    GPS();
    
   if(graf)
      Serial1.print('$');
      
   wdt_reset();
   numbl=1;
   bort();
   geo();
   wdt_reset();
   
   env();
   wdt_reset();
   cam();

   if(graf)
       Serial1.print(';');

   Serial1.println();
   Serial.println();
   }
   delay(800);
   wdt_reset();
}
