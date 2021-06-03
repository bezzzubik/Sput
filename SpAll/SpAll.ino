void setup() {

//  pinMode(11, OUTPUT);
  Serial.begin(9600);
  setupBort();
  Serial1.begin(9600);

}


void loop() {

   bort();

   Serial.println("\nend cycle\n\n");
}
