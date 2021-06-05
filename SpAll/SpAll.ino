void setup() {

//  pinMode(11, OUTPUT);
  Serial.begin(9600);
  setupBort();
  setupGeo();
  Serial1.begin(9600);

}


void loop() {

   Serial.println("Bort:\n");
   bort();
   Serial.println("Geo:\n");
   geo();
   Serial.println("\n\nend cycle\n\n");
}
