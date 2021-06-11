#define CAM 44
unsigned long maxTime = (long) 1000*60*60*3;

unsigned long Time;

bool VT=false;

void setupCam()
{
 
  pinMode(CAM, OUTPUT);
  pinMode(33, OUTPUT);
  digitalWrite(CAM, HIGH);
  digitalWrite(33, HIGH);
  Time=millis();


}

void cam()
{
 
   if (VT == false)
     if((millis()-Time >= maxTime) || (VolAk < 10) )
        off_cam();

}

void off_cam()
{

   digitalWrite(CAM, LOW);
   digitalWrite(33, LOW);
   VT=true;

}
