#include <OneWire.h>

#define KOF 10 //коэффициент пропорциональности регулятора
#define TEMPNORM 15 //необходимая температура
#define acs712_pin A11

int zero;


OneWire ds(10);


void bort()
{

  heater( Temp() );
  Amper();
  Voltage();
  return;
}




int Temp()
{

  byte i;
  byte j=1;
  byte present = 0;
  byte type_s=0;
  byte data[12];
  byte addr[8];
  int celsius;
  int celA=-70;
  
  while ( ds.search(addr)) {
    
      if (OneWire::crc8(addr, 7) != addr[7]) {
        Serial.println("CRC is not valid!");
      return celA;
    }
  
  
    ds.reset();
    ds.select(addr);
    ds.write(0x44, 1);
    
    delay(1000);
    
    present = ds.reset();
    ds.select(addr);
    ds.write(0xBE);
    
    for ( i = 0; i < 9; i++)
      data[i] = ds.read();
    
    OneWire::crc8(data, 8);
    
    int16_t raw = (data[1] << 8) | data[0];
    
    
    byte cfg = (data[4] & 0x60);
    
    if (cfg == 0x00) raw = raw & ~7;
    else if (cfg == 0x20) raw = raw & ~3;
    else if (cfg == 0x40) raw = raw & ~1;
    
    
    celsius = (int)raw / 16;

    if (j==1)
      celA=celsius;
    
    Serial.print('T');
    
    Serial.print(j);
    
    Serial.print('=');
    
    Serial.print(celsius);
    
    Serial.println('C');
    
    j++;
  
  }
  
  ds.reset_search();
  
  delay(250);
  
  return celA;

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
    digitalWrite(11, out);

    if (out != 0)
        digitalWrite(37, HIGH);  
    else
        digitalWrite(37, LOW);
    
    return ;
}





void normal(int temp)
{
  
    int outputValue = 0; //необходимое значение сигнала для транзистора
    outputValue = KOF*(TEMPNORM - temp); //расчет нужного значения сигнала для транзистора
    
    if(temp >= TEMPNORM ) //нужно ли передавать сигнал на транзистор
      voltage(0); //передастся 0
    else
      voltage(outputValue); //иначе передается нужное значение
    
    
/*    Serial.print("output = "); //значения нужного для подачи сигнала
    Serial.println(outputValue);
*/  
    return ;
}






void error(){ //функция обработки ошибок

  Serial.print("Attention, possibly disabled signal");
  
  
  Serial.println("The heater runs at 30%\n"); //сообщение о том, что передается 30% от максимального значения сигнала
  
  voltage(225); //передача 30% от максимального сигнала
  
  return ;
}



// получает сглаженное значение с АЦП Arduino

int getSmoothedValue(){

    int value;
    int repeats = 10;
    
    for (int i=0; i<repeats; i++){ // измеряем значение несколько раз
      value += analogRead(acs712_pin); // суммируем измеренные значения
      delay(1);
    }
    
    value /= repeats; // и берём среднее арифметическое
    return value;
}




void Amper() {

  int sensorValue = getSmoothedValue(); // читаем значение с АЦП и выводим в монитор
  Serial.print('I');
  Serial.print(" = ");
  int c = getCurrent(sensorValue); // преобразуем в значение тока и выводим в монитор
  Serial.print(c);
  Serial.println(" mA");
  
  
  delay(100);
  //Serial1.println("Всем привет! ");
}



// рассчитывает ток в мА по значению с АЦП
int getCurrent(int adc) {

  int delta = zero - adc; // отклонение от нуля шкалы
  float scale = 37.888; // сколько единиц АЦП приходится на 1 ампер
  int current = (int)delta*1000/scale; // считаем ток в мА
  return current;

}



void Voltage()
{
  int analogInput, value;
  float vout, vin, R1, R2;

  for(analogInput=12; analogInput !=15; analogInput++ )
  {
    switch (analogInput)
    {
    
     case 12: R1 = 6800.0;
              R2 = 4700.0;
            break;
     case 13: R1 = 4700.0;
              R2 = 6800.0;
            break;
     case 14: R1 = 6800.0;
              R2 = 1100.0;
    }
    
    value = analogRead(analogInput);
    vout = (value * 5.0) / 1024.0;
    vin = vout / (2*R2/(R1+R2));

    if (vin<0.09) {
      vin=0.0;// обнуляем нежелательное значение
    }
    
    Serial.print("INPUT V");
    Serial.print(analogInput);
    Serial.print('=');
    Serial.println(vin);
    delay(500);
  }

  return;
}


void setupBort()
{

  zero = getSmoothedValue(); // определим нуль шкалы (до включения нагрузки)
  analogReference( EXTERNAL );
//  pinMode(8, OUTPUT);
//  pinMode(9, OUTPUT);
  pinMode(37, OUTPUT);

  
}
