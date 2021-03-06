#include <OneWire.h>
#define KOF 10 //коэффициент пропорциональности регулятора
#define TEMPNORM 15 //необходимая температура

byte data1[12];

// OneWire DS18S20, DS18B20, DS1822 Temperature Example
//
// http://www.pjrc.com/teensy/td_libs_OneWire.html
//
// The DallasTemperature library can do all this work for you!
// https://github.com/milesburton/Arduino-Temperature-Control-Library

OneWire  ds(10);  // on pin 10 (a 4.7K resistor is necessary)
int celA;

void setup(void) {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  analogReference( EXTERNAL );
//  pinMode(8, OUTPUT);
//  pinMode(9, OUTPUT);
  pinMode(37, OUTPUT);
  pinMode(11, OUTPUT);

}

void loop(void) {

  Serial1.print('$');
  byte i;
  byte j = 1;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  int celsius;
  int celA = -70;

  while (ds.search(addr)) {

    if (OneWire::crc8(addr, 7) != addr[7]) {
      return celA;
    }

    // the first ROM byte indicates which chip
    switch (addr[0]) {
      case 0x10:
        type_s = 1;
        break;
      case 0x28:
        type_s = 0;
        break;
      case 0x22:
        type_s = 0;
        break;
      default:
        return celA;
    }

    ds.reset();
    ds.select(addr);
    ds.write(0x44, 1);        // start conversion, with parasite power on at the end

    delay(10);     // maybe 750ms is enough, maybe not
    // we might do a ds.depower() here, but the reset will take care of it.

    present = ds.reset();
    ds.select(addr);
    ds.write(0xBE);         // Read Scratchpad

    for ( i = 0; i < 9; i++)           // we need 9 bytes
      data[i] = ds.read();
    OneWire::crc8(data, 8);

    // Convert the data to actual temperature
    // because the result is a 16 bit signed integer, it should
    // be stored to an "int16_t" type, which is always 16 bits
    // even when compiled on a 32 bit processor.
    int16_t raw = (data[1] << 8) | data[0];
    if (type_s) {
      raw = raw << 3; // 9 bit resolution default
      if (data[7] == 0x10) {
        // "count remain" gives full 12 bit resolution
        raw = (raw & 0xFFF0) + 12 - data[6];
      }
    } else {
      byte cfg = (data[4] & 0x60);
      // at lower res, the low bits are undefined, so let's zero them
      if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
      else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
      else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
      //// default is 12 bit resolution, 750 ms conversion time
    }

    celsius = (int)raw / 16;

    if (j == 1)
   {
      Serial1.print("T1");
      Serial1.print(celsius);
      Serial.println(celsius);
   }
   if (j==3)
   {
      celA=celsius;
      Serial1.print(' ');
      Serial1.print(celA);
      Serial1.println(';');
    }
   delay(100);
    j++;
  }
  
  delay(500);

  heater(celA);
}

int dError=0; //переменная обработки ошибки


void heater(int temp)
{
  
  if (temp < -60) //если функция выявления ошибок возвращает истину
    dError++; //к переменной добавляется 1, иначе она обнуляется
  else
  dError=0;
  
  if (dError > 5) //проверка ошибок:если переменная ошибок выявила
  error(); //более 5 ошибочных значений, то начинает работать
  else //функция обработки ошибки, иначе идет нормальный
  normal(temp); //режим работы программы

}





void voltage(int out) //функция передачи сигнала на затвор транзистора
{
if (out == 0){
  digitalWrite(37, LOW);
  digitalWrite(11, LOW);
}else if(out > 1000){
  digitalWrite(11, HIGH);
  digitalWrite(37, HIGH);
}else{
  digitalWrite(11, out);
  digitalWrite(37, out);
}

return ;
}





void normal(int temp)
{

int output=KOF*(TEMPNORM - temp);

if( temp < TEMPNORM )

  voltage(output);
  
else

  voltage(0);

return ;
}






void error(){ //функция обработки ошибок

Serial.print("Attention, possibly disabled signal");


Serial.println("The heater runs at 30%\n"); //сообщение о том, что передается 30% от максимального значения сигнала

voltage(225); //передача 30% от максимального сигнала

return ;
}
