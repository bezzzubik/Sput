#define CAM 44

unsigned long maxTime = (long) 1000*60*60*3;

unsigned long Time;

bool VT=false;

void setupCam()
{
 
    digitalWrite(CAM, HIGH);
    Time=millis();

}

void cam()
{
 
   if (VT == false)
   if((millis - Time >= maxTime) || (VolAk < 10) )
   {
      bool VT=true;
      digitalWrite(CAM, LOW);
   }

}
