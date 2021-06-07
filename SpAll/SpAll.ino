void setup() {

//  pinMode(11, OUTPUT);
  Serial.begin(9600);
  setupBort();
  setupGeo();
  setupEnv();
  setupCam();
  Serial1.begin(9600);

}


void loop() {

   Serial.println("Bort:\n");
   bort();
   Serial.println("Geo:\n");
   geo();
   Serial.println("Inv:\n");
   env();
   Serial.println("Camera:\n");
   cam();
   Serial.println("\n\nend cycle\n\n");
}
