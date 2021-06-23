#define CAM 44
unsigned long maxTime = (long) 1000*60*60*3;

unsigned long Time;

bool VT;

void setupCam()
{
 
  pinMode(CAM, OUTPUT);
  pinMode(33, OUTPUT);
  digitalWrite(CAM, HIGH);
  digitalWrite(33, HIGH);
  Time=millis();
  VT=true;
}

int s=0;


void timeP()
{
  int sec = s%84600%60;
  int mi = (s%(24*60*60))/60%60;
  int ho = (s%86400)/60/60;
  Serial.print(ho);
  Serial.print(':');
  Serial.print(mi);
  Serial.print(':');
  Serial.print(sec);
}

void cam()
{
  s=(millis()- Time)/1000;
   if (VT)
     if( ( millis()- Time >= maxTime) || (VolAk < 10.0) )
        off_cam();
  
}

void off_cam()
{

   digitalWrite(CAM, LOW);
   digitalWrite(33, LOW);
   VT=false;

}
