#define camera_on_off 44
#define camera_play_stop 45
unsigned long maxTime = (long) 1000*60*60*3;

unsigned long Time;

bool VT=false;

void setupCam()
{
 
    digitalWrite(camera_on_off, HIGH); //камера будет включаться и выключаться через реле, поэтому нужно сделать имитацию кнопки. кнопку питания нажали
    delay(2000);//держим
    digitalWrite(camera_on_off, LOW);//отпустили
    digitalWrite(camera_play_stop, HIGH);//кнопку записи нажали
    delay(1000);//держим
    digitalWrite(camera_play_stop, LOW);//отпустили
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
