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

void cam()
{
 
   if (VT)
     if((millis()- Time >= maxTime) || (VolAk < 10.0) )
        off_cam();

}

void off_cam()
{

   digitalWrite(CAM, LOW);
   digitalWrite(33, LOW);
   VT=false;

}
