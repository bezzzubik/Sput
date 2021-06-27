#define KFP 111120;

void setup() {

Serial.begin(9600);

}



void loop(){

double h0, h1;

double la0, lo0, la, lo, dt;


la=la0+( ((la-la0)/dt) * ( h0/((h0-h1)/dt) ) );
lo=lo0+( ((lo-lo0)/dt) * ( h0/((h0-h1)/dt) ) );

}
