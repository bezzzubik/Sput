
void setupLoRa() {
// put your setup code here, to run once:
pinMode(8, OUTPUT);
pinMode(9, OUTPUT);
Serial1.begin(9600);
}

void PrintL(){

char c[32];
//a=97, z=122
  if(Serial1.available()!=0){
    for(int i=0; Serial1.available()!=0; i++)
       c[i]=Serial1.read();
    Serial1.println(c);
  }
  
}
