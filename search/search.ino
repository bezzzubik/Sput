#include "stdio.h"
//#define F "map.txt"


   FILE *f;
void setup() {
  Serial.begin(9600);
  f=fopen("map.txt", 'w');
}


void loop() {
    char c[64];
    if (Serial.available() > 0 ) {
         for(int i=0; Serial.available() > 0; i++ ) 
           c[i] = Serial.read();
         fprintf(f, c);
         
    }
}
