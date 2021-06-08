#define CAMERA_ON_OFF 44
#define CAMERA_PLAY_STOP 45
unsigned long maxTime = (long) 1000*60*60*3;

unsigned long Time;

bool VT=false;

void setupCam()
{
 
    digitalWrite(CAMERA_ON_OFF, HIGH); //камера будет включаться и выключаться через реле, поэтому нужно сделать имитацию кнопки. кнопку питания нажали
    delay(1000);//держим
    digitalWrite(CAMERA_ON_OFF, LOW);//отпустили
    digitalWrite(CAMERA_PLAY_STOP, HIGH);//кнопку записи нажали
    delay(1000);//держим
    digitalWrite(CAMERA_PLAY_STOP, LOW);//отпустили
    Time=millis();
    digitalWrite(CAMERA_ON_OFF, HIGH);// включаем подпитку камеры

}

void cam()
{
 
   if (VT == false)
   if((millis - Time >= maxTime) || (VolAk < 10) )
   {
      bool VT=true;
      digitalWrite(CAMERA_ON_OFF, LOW);
   }

}
