void setup() {

//  pinMode(11, OUTPUT);
  Serial.begin(9600);
  setupBort();
  setupGeo();
  setupEnv();
  //setupCam();
  Serial1.begin(9600);

}


void loop() {
   Serial.println("Bort:");
   bort();
   Serial.println("\nGeo:");
   geo();
   Serial.println("Inv:");
   env();
//   cam();
   Serial.println("\n\nend cycle\n");
}
