

#include <TFT_eSPI.h> // для дисплея
TFT_eSPI tft = TFT_eSPI();

//#define PB1       0  // button 1
//#define PB2       35 // button 2
#define TX        33 // tx pin for Lora
#define RX        32 // rx pin for Lora

#define menu 0    //Button 1 = menu
#define select 35  //Button 2 = select

int pagecounter=0;
int counterTX; // счётчик отправленных
int counterRX; // счётчик полученных
int counterRXe; // счётчик полученных с ошибками
int counterTX2; // принятый счётчик отправленных
int counterRX2; // принятый счётчик полученных
int counterRXe2; // принятый счётчик полученных с ошибками


int flag1; // состояние кнопки меню
int flag2; // состояние кнопки выбор
int val; // переключение экранов switch
int clean; // флаг на очистку экрана

int timer; // таймер отправки
int timer2; // таймер приёма

char sz[32]; // массив для даты
char sx[32]; // массив для времени

#include <SoftwareSerial.h> // GPS connected via software serial
static const int RXPin = 26;     // RX TX for GPS module
static const int TXPin = 27;
static const uint32_t GPSS = 9600;
SoftwareSerial ss(RXPin, TXPin);

//GPS Library
#include <TinyGPS++.h>
TinyGPSPlus gps;



//float declination = +0.2006;
 // Now add the 'Declination Angle' for you location. Declination is the variation in magnetic field at your location.
 // Find your declination here: http://www.magnetic-declination.com/
 // Make declination = 0 if you can't find your Declination value, the error is negible for nearly all locations
 // for Moscow +11.33 E, = +11.5 degrees, +11,5/180*PI = +0.2006. West =(+), E =(-)
//  heading = heading + declination;
// if(heading < 0)    heading += 2*PI;  // Correct for when signs are reversed.
//  if(heading > 2*PI) heading -= 2*PI;  // Correct for when heading exceeds 360-degree, especially when declination is included
//  angle = int(heading * 180/M_PI); // Convert radians to degrees for more a more usual result
//---
/* для компаса поправка:
  heading = heading + declination;
 if(heading < 0)    heading += 2*PI;  // Correct for when signs are reversed.
  if(heading > 2*PI) heading -= 2*PI;  // Correct for when heading exceeds 360-degree, especially when declination is included
  angle = int(heading * 180/M_PI); // Convert radians to degrees for more a more usual result

*/

//Program variables

double Home_LAT0 = 55.777777; //if you need to setup home position and check distance from home
double Home_LNG0 = 37.333333; //if you need to setup home position and check distance from home 
double XXX = 137650; // поправка для кодирования 1 умножение
double YYY = 8911057; // поправка для кодирования 2 сложение 

// переменные для чтения GPS

  int GPSCa; // число
  int GPSDa; // месяц
  int GPSEa; // год
  int GPSFa; // часы
  int GPSGa; // минуты
  int GPSHa; // секунды
  int GPSIa; // кол-во спутников 
  int GPSJa; // скорость 
  double GPSKa; // долгота
  double GPSLa; // широта
  long GPSMa; // высота 
  double GPSSa; // расстояние до цели 
  long GPSTa; // направление до цели (courceTo) bearing  
  String GPSUa; // направление до цели (cardinalTo) NWS+
  String GPSVa; // heading NWS
  long GPSWa; // bearing   
//double Target_LAT = 90.000000; //target NP
//double Target_LNG = 0.000000; //target NP 
//double Target_LAT = Home_LAT0; //target NP
//double Target_LNG = Home_LNG0; //target NP 
// перенесли ниже чтобы присвоить целевые координаты = позицию оппонента





//новые переменные, которые ДЛЯ ПРИЁМА (b) !!!!. Присваиваем значения через формулы или вручную ТУТ!!!!!

  int Ab ; // тип сообщений (влияет на команду)
  int Bb ; // команда (влияет на выбор настроек)
  int Cb ; // число
  int Db ; // месяц
  int Eb ; // год
  int Fb ; // часы
  int Gb ; // минуты
  int Hb ; // секунды
  int Ib ; // кол-во спутников
  int Jb ; // скорость
  double Kb ; // longitude
  double Lb ; // lattitude
  long Mb ; // высота нормально работает если long 
  long Nb ; // TX2 принятый
  long Ob ; // RX2 принятый
  long Pb ; // RXe2 принятый
  double Qb ; // координаты цели Longitude
  double Rb ; // координаты цели Latitude
  double Sb ; // расстояние до цели 
//---------------- вверху - попадают в контрольную сумму
  double crcb;

  long  Tb ; // направление до цели (courceTo) bearing
  String Ub ; // направление до цели (cardinalTo) NWS
  String Vb ; // heading NWS
  long Wb ; // bearing  
/*  char Xb = 54; // текстовое сообщение2
  char Yb = 85; // текстовое сообщение2

*/

double Target_LAT = Lb; //target NP  // перенесли ниже чтобы присвоить целевые координаты = позицию оппонента
double Target_LNG = Kb; //target NP  // перенесли ниже чтобы присвоить целевые координаты = позицию оппонента

  

//---------------------------------
/*

// unsigned long int timer; // для таймера отправки
// String inmessage,outmessage,message;  // для вывода строкой всего полученного / отправленного
 int counter1send=0;  // счётчик отправленных пакетов
int counter1received=0; // счётчик полученных пакетов
int counter1errors=0; // счётчик полученных пакетов с ошибками
int counter2send=0;  // счётчик отправленных пакетов (данные от второго модуля)
int counter2received=0; // счётчик полученных пакетов (данные от второго модуля)
int counter2errors=0; // счётчик полученных пакетов с ошибками (данные от второго модуля)
*/
//----------------------------------







void setup()
{
  Serial.begin(115200); //serial usb uart for debugging
  Serial1.begin(9600,SERIAL_8N1,RX,TX); // serial for lora uart
  ss.begin(9600); //sowtware UART for GPS 
//  pinMode(menu,INPUT_PULLUP); // кнопка 1 режим
//  pinMode(select,INPUT_PULLUP); // кнопка 2 режим
pinMode(menu, INPUT);  //for button 0
pinMode(select, INPUT);//for button 35 
  
  tft.init();
  tft.invertDisplay(true);
  tft.setRotation(3);  // поворот экрана
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);

  
}

void loop()
{ 


  while (ss.available() > 0)
    if (gps.encode(ss.read()))
//      displayInfo();  // вывод полученных от GPS данных в serial
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));

      while(true);
  }


// присвоение значений переменным - данные, которые берутся из GPS !
  if (gps.date.isValid())     {

    
 //     day = gps.date.day();
GPSCa = gps.date.day(); // число
      
 //     month = gps.date.month();
GPSDa = gps.date.month(); // месяц
      
 //     year = gps.date.year();
GPSEa = gps.date.year(); // год

    }

  
  if (gps.time.isValid())
    {
  //    hour = gps.time.hour()+3;// + 3 часа по московскому времени
GPSFa = gps.time.hour()+3; // часы

  //    minute = gps.time.minute();
GPSGa = gps.time.minute(); // минуты

   //   second = gps.time.second();
GPSHa = gps.time.second(); // секунды

    }


  if (gps.location.isValid() == 1) {


GPSIa = gps.satellites.value(); // кол-во спутников в формате без запятой
    

GPSLa = gps.location.lat(); // lattitude
    

GPSKa = gps.location.lng(); // longitude
    

GPSMa = gps.altitude.meters(); // altitude в формате без запятой long

    

GPSJa = gps.speed.kmph(); // скорость

    
 //   heading = gps.cardinal(gps.course.value());  // as standard directions W/E/S/N
GPSVa = gps.cardinal(gps.course.value()); // heading
   

GPSWa = gps.course.deg(); // bearing нормально работает если long




// заданная точка (определение расстояния до цели, направления и угла в градусах)
long courseTo =  
  TinyGPSPlus::courseTo(
    gps.location.lat(),
    gps.location.lng(),
    Target_LAT,
    Target_LNG );
GPSTa = courseTo; // направление до цели (courceTo) long / double bearing




String cardinalTo = TinyGPSPlus::cardinal(courseTo);
GPSUa = cardinalTo; // направление до цели (cardinalTo) NWS heading



GPSSa =   // расстояние до цели distancem
  (double)TinyGPSPlus::distanceBetween( 
    gps.location.lat(),
    gps.location.lng(),
    Target_LAT,
    Target_LNG );



//delay(100);


 
 }

// проверка состояния нажатия кнопок и выставление флагов

if (digitalRead(menu) == HIGH && digitalRead(select) == HIGH)  // check if menu button pressed.
val = 3;

if (digitalRead(menu) == LOW && digitalRead(select) == LOW)  // check if menu button pressed.
val = 3;

if (digitalRead(menu) == LOW && digitalRead(select) == HIGH)  // check if menu button pressed.
val = 1;

if (digitalRead(menu) == HIGH && digitalRead(select) == LOW)  // check if menu button pressed.
val = 2;

 
if (clean == val)
{}
else{
tft.fillScreen(TFT_BLACK); 
}

clean=val;





switch (val) {  // выбор экранов для отображения
 case 1:

/*tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.setCursor(30,0);
tft.print(flag1);
tft.setCursor(30,30);
tft.print(flag2);
tft.setCursor(30,60);
tft.print("Case 1 val 1");
*/


  // ЭКРАН 2 (цели и таргеты)

//tft.fillScreen(TFT_BLACK);
tft.setCursor(0,0);
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.print("Tgt_lat: ");
tft.setCursor(120,0);
tft.setTextColor(28000, 0);
tft.printf("%6.6f", Target_LAT);

tft.setCursor(0,20);
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.print("Tgt_lng: ");
tft.setCursor(120,20);
tft.setTextColor(28000, 0);
tft.printf("%6.6f", Target_LNG);

tft.setCursor(0,40);
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.print("Distance:");
tft.setCursor(110,40);
tft.setTextColor(28000, 0);
tft.print(GPSSa/1000);
tft.setCursor(212,40);
tft.print("km");


tft.setCursor(0,60);
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.print("Speed: ");
tft.setCursor(80,60);
tft.setTextColor(28000, 0);
tft.print(GPSJa);

tft.setCursor(0,80);
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.print("CardinalTo: ");
tft.setCursor(150,80);
tft.setTextColor(64000, 0);
tft.print(GPSUa);


tft.setCursor(0,100);
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.print("target (`): ");
tft.setCursor(150,100);
tft.setTextColor(64000, 0);
tft.print(GPSTa);



tft.setCursor(0,120);
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.print("sth-1:");
tft.setCursor(80,120);
tft.setTextColor(2500000, 0);
tft.printf("GPSunit1");

   break;

  
  case 2:
/*
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.setCursor(30,0);
tft.print(flag1);
tft.setCursor(30,30);
tft.print(flag2);
tft.setCursor(30,60);
tft.print("Case 2 val 2");
*/


    
    // выполнить, если val == 2  нажата кнопка 2
  // ЭКРАН 3 (счётчик)


tft.setCursor(40,0);
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.print("-= Packcages =-");

tft.setCursor(0,30);
tft.print("units");

tft.setCursor(0,60);
tft.print("Sent");

tft.setCursor(0,90);
tft.print("Received");

tft.setCursor(0,120);

tft.print("Errors");


tft.setCursor(110,30);
tft.print("Me");

tft.setCursor(170,30);
tft.print("2nd");


tft.setCursor(120,60);
tft.setTextColor(555, TFT_BLACK);
tft.print(counterTX);

tft.setCursor(120,90);
tft.print(counterRX);

tft.setCursor(120,120);
tft.print(counterRXe);



tft.setCursor(180,60);
tft.print(counterTX2);

tft.setCursor(180,90);
tft.print(counterRX2);

tft.setCursor(180,120);
tft.print(counterRXe2);
    
    break;

  
 case 3: // обычный экран
/*
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.setCursor(30,0);
tft.print(flag1);
tft.setCursor(30,30);
tft.print(flag2);
tft.setCursor(30,60);
tft.print("Case 3 val 3");
*/



//   default: // обычный экран
    // выполнить, если val ни 1 ни 2  нажаты обе или ни одна не нажата
    // default опционален
   // выполнить, если val == 1  нажата кнопка 1
 // ЭКРАН 1 (основной)

// отображение на экране страницы 1 - 
  
tft.setCursor(0,0);
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.print("Lat.:");
tft.setCursor(80,0);
tft.setTextColor(28000, 0);//  текст / заливка)
tft.printf("%6.6f", GPSLa);



tft.setCursor(0,20);
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.print("Long.:");
tft.setCursor(80,20);
tft.setTextColor(28000, 0);
tft.printf("%6.6f", GPSKa);


tft.setCursor(0,40);
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.print("Sat: ");
tft.setCursor(80,40);
tft.setTextColor(64000, 0);
tft.print(GPSIa);// нормально показывает в формате int

tft.setCursor(120,40);
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.print("Alt: ");// нормально показывает в формате double
tft.setCursor(170,40);
 tft.setTextColor(65000, 0);
tft.print(GPSMa);
tft.setCursor(222,40);
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.print("m");


tft.setCursor(0,60);
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.print("Head: ");
tft.setCursor(80,60);
tft.setTextColor(65000, 0);
tft.print(GPSVa);
//tft.print(cardinalTo);

tft.setCursor(120,60);
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.print("Bear: ");
tft.setCursor(180,60);
tft.setTextColor(65000, 0);
tft.print(GPSWa);
//tft.print(courseTo);
tft.setCursor(222,60);
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.print("`");


tft.setCursor(0,80);
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.print("date: ");
tft.setCursor(80,80);
//char sz[32];
sprintf(sz, "%02d.%02d.%02d ", GPSCa,GPSDa,GPSEa);

tft.setTextColor(1000000, 0);
tft.print(sz);

tft.setCursor(0,100);
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.print("Time: ");
tft.setCursor(80,100);
//char sx[32];
sprintf(sx, "%02d:%02d:%02d ", GPSFa,GPSGa,GPSHa);
tft.setTextColor(1000000, 0);
tft.print(sx); 

    break;
    
}





//****************************************ОТПРАВКА!
//}
//****************************************ОТПРАВКА!


bailout:

//----------sending to lora ------- (a)  ------------------


//новые переменные, которые ДЛЯ ОТПРАВКИ (a). Присваиваем значения через формулы или вручную ТУТ!!!!!

  int Aa = 11; // тип сообщений (влияет на команду)
  int Ba = 111111; // команда (влияет на выбор настроек)
  int Ca = GPSCa; // число
  int Da = GPSDa; // месяц
  int Ea = GPSEa; // год
  int Fa = GPSFa; // часы
  int Ga = GPSGa; // минуты
  int Ha = GPSHa; // секунды
  int Ia = GPSIa; // кол-во спутников
  int Ja = GPSJa; // скорость
  double Ka = GPSKa; // longitude
  double La = GPSLa; // lattitude
  long Ma = GPSMa; // высота нормально работает если long 
  long Na = counterTX; // TX свой счётчик
  long Oa = counterRX; // RX свой счётчик
  long Pa = counterRXe; //RXe свой счётчик
  double Qa = Target_LNG; // координаты цели Longitude
  double Ra = Target_LAT; // координаты цели Latitude
  double Sa = GPSSa; // расстояние до цели 
//---------------- вверху - попадают в контрольную сумму
  double crca = ((Aa+Ba+Ca+Da+Ea+Fa+Ga+Ha+Ia+Ja+Ka+La+Ma+Na+Oa+Pa+Qa+Ra+Sa));
  long  Ta = GPSTa; // направление до цели (courceTo) bearing
  String Ua = GPSUa; // направление до цели (cardinalTo) NWS
  String Va = GPSVa; // heading NWS
  long Wa = GPSWa; // bearing  
/*  char Xa = 24; // текстовое сообщение2
  char Ya = 25; // текстовое сообщение2

*/

//****************************************ОТПРАВКА!
 if(millis()-timer>=3000) // начало цикла отправки по таймеру mills-timer !!!!!!!!!!!!!!!!!!
  {
struct Str {  // перечень переменных буфера ОТПРАВКИ = задаём  -= ФОРМАТ =- переменных на отправку ТУТ!

  int Aa  ; // тип сообщений (влияет на команду)
  int Ba ; // команда (влияет на выбор настроек)
  int Ca ; // число
  int Da ; // месяц
  int Ea ; // год
  int Fa; // часы
  int Ga; // минуты
  int Ha; // секунды
  int Ia; // кол-во спутников
  int Ja; // скорость
  double Ka; // longitude
  double La; // lattitude
  long Ma; // высота нормально работает если long 
  long Na; // TX свой счётчик
  long Oa; // RX свой счётчик
  long Pa; // RXe свой счётчик
  double Qa; // координаты цели Longitude
  double Ra; // координаты цели Latitude
  double Sa; // расстояние до цели 
    double crca ;
//---------------- вверху - попадают в контрольную сумму
  long  Ta; // направление до цели (courceTo) bearing
  String Ua; // направление до цели (cardinalTo) NWS
  String Va; // heading NWS
  long Wa; // bearing  
/*  char Xa; // текстовое сообщение2
  char Ya; // текстовое сообщение2  

*/

//****************************************ОТПРАВКА!

 };

  Str buf;    // заполняем буфер значениями ТУТ! (формулы только в пределах переменных буфера отправки!)
    
 buf.Aa = Aa ; // тип сообщений (влияет на команду)
 buf.Ba = Ba; // команда (влияет на выбор настроек)
 buf.Ca = Ca; // число
 buf.Da = Da; // месяц
 buf.Ea = Ea; // год
 buf.Fa = Fa; // часы
  buf.Ga = Ga; // минуты
  buf.Ha = Ha; // секунды
  buf.Ia = Ia; // кол-во спутников
  buf.Ja = Ja; // скорость
  buf.Ka = Ka; // longitude
  buf.La = La; // lattitude
  buf.Ma = Ma; // высота нормально работает если long 
  buf.Na = Na; // TX свой счётчик
  buf.Oa = Oa; // RX свой счётчик
  buf.Pa = Pa; // RXe свой счётчик
  buf.Qa = Qa; // координаты цели Longitude
  buf.Ra = Ra; // координаты цели Latitude
  buf.Sa = Sa; // расстояние до цели 
//---------------- вверху - попадают в контрольную сумму
  buf.crca = crca ;  //контрольная сумма

  buf.Ta = Ta; // направление до цели (courceTo) bearing
  buf.Ua = Ua; // направление до цели (cardinalTo) NWS
  buf.Va = Va; // heading NWS
  buf.Wa = Wa; // bearing  
/*  buf.Xa = Xa; // текстовое сообщение2
  buf.Ya = Ya; // текстовое сообщение2

*/
//****************************************ОТПРАВКА!

  // отправляем родным write()
  // указываем ему буфер-структуру, но приводим тип к byte*
  // размер можно указать через sizeof()

Serial1.write((byte*)&buf, sizeof(buf));  // отправка на Лора модуль для передачи по радио
    timer=millis();
    counterTX++;  //добавляем к счётчику, влияющему на отправку Na


//-----------------------------------------------------------------------------------------------------

    Serial.println("-------------Sent following: ----------");  // комментирование что было отправлено
    Serial.println(Aa);
    Serial.println(Ba);
    Serial.println(Ca);
    Serial.println(Da);
    Serial.println(Ea);
    Serial.println(Fa);
    Serial.println(Ga);
    Serial.println(Ha);
    Serial.println(Ia);
    Serial.println(Ja);
//    Serial.println(buf.Ka);// надо вывести 6 знаков после запятой
  Serial.printf("%6.6f", Ka); // кодированные координаты
  Serial.print(" = ");
  Serial.printf("%6.6f", (Ka-1593118)/9715587); // раскодированные координаты
  Serial.println();

//    Serial.println(La); // надо вывести 6 знаков после запятой
  Serial.printf("%6.6f", La); 
    Serial.println();
  
    Serial.println(Ma);
    Serial.println(Na);
    Serial.println(Oa);
    Serial.println(Pa);
    
//    Serial.println(Qa); 
  Serial.printf("%6.6f", Qa); 
    Serial.println();
//    Serial.println(Ra); 
  Serial.printf("%6.6f", Ra); 
    Serial.println();

    Serial.println(Sa);
    
    Serial.println();
    Serial.printf("%6.6f", buf.crca);
    Serial.print(  "  CRC  .");
Serial.println();
  
    Serial.println(Ta);
    Serial.println(Ua);
    Serial.println(buf.Va);
    Serial.println(buf.Wa);
/*    Serial.println(buf.Xa);
    Serial.println(buf.Ya);
*/


    Serial.println("================= end of message sent =================");

Serial.println();
Serial.println("* * * * * * * *");
Serial.println();


  } // окончание цикла отправки по таймеру mills-timer !!!!!!!!!!!!!!!!!!







//----------------------------------------------------------------------------------------------
// Теперь примем аналогичное сообщение от второго девайса.


//*******************************receiving from Lora **************************************


//****************************************ПРИЁМ!

struct Str2 {  // структура буфера ПРИЁМА  = -= ФОРМАТ =- переменных на приём задаётся ТУТ!

  int Ab; // тип сообщений (влияет на команду)
  int Bb; // команда (влияет на выбор настроек)
  int Cb; // число
  int Db; // месяц
  int Eb; // год
  int Fb; // часы
  int Gb; // минуты
  int Hb; // секунды
  int Ib; // кол-во спутников
  int Jb; // скорость
  double Kb; // longitude
  double Lb; // lattitude
  long Mb; // высота нормально работает если long 
  long Nb; // TX2 (просто получение значения по радио)
  long Ob; // RX2 (просто получение значения по радио)
  long Pb; // RXe2 (просто получение значения по радио)
  double Qb; // координаты цели Longitude //полученные координаты
  double Rb; // координаты цели Latitude
  double Sb; // расстояние до цели 
//---------------- вверху - попадают в контрольную сумму
  double crcb;
  
  long  Tb; // направление до цели (courceTo) bearing  
  String Ub; // направление до цели (cardinalTo) NWS
  String Vb; // heading NWS
  long Wb; // bearing  
/*  char Xb; // текстовое сообщение2
  char Yb; // текстовое сообщение2

*/

//****************************************ПРИЁМ!

  };


  Str2 buf2; // заполняем буфер значениями ТУТ! (формулы только в пределах переменных буфера приёма!)
 
  buf2.Ab; // тип сообщений (влияет на команду)
  buf2.Bb; // команда (влияет на выбор настроек)
  buf2.Cb; // число
  buf2.Db; // месяц
  buf2.Eb; // год
  buf2.Fb; // часы
  buf2.Gb; // минуты
  buf2.Hb; // секунды
  buf2.Ib; // кол-во спутников
  buf2.Jb; // скорость
  buf2.Kb; // longitude
  buf2.Lb; // lattitude
  buf2.Mb; // высота нормально работает если long 
  buf2.Nb; // TX2 кол-во переданных пакетов (получение по радио)
  buf2.Ob; // RX2 кол-во принятых пакетов (получение по радио)
  buf2.Pb; // RXe2 кол-во принятых пакетов с ошибками (получение по радио)
  buf2.Qb; // координаты цели Longitude
  buf2.Rb; // координаты цели Latitude
  buf2.Sb; // расстояние до цели 
//---------------- вверху - попадают в контрольную сумму
  buf2.crcb;  //контрольная сумма

  buf2.Tb; // направление до цели (courceTo) bearing
  buf2.Ub; // направление до цели (cardinalTo) NWS
  buf2.Vb; // heading NWS
  buf2.Wb; // bearing  
/*  buf2.Xb; // текстовое сообщение2
  buf2.Yb; // текстовое сообщение2

*/


//****************************************ПРИЁМ!
// if(millis()-timer2>=1000) // начало цикла приёма по таймеру mills-timer !!!!!!!!!!!!!!!!!!
//  {

  if(Serial1.available())  {
   
    delay(1);

 if (Serial1.readBytes((byte*)&buf2, sizeof(buf2)))  {

if(buf2.crcb != (buf2.Ab+buf2.Bb+buf2.Cb+buf2.Db+buf2.Eb+buf2.Fb+buf2.Gb+buf2.Hb+buf2.Ib+buf2.Jb+buf2.Kb+buf2.Lb+buf2.Mb+buf2.Nb+buf2.Ob+buf2.Pb+buf2.Qb+buf2.Rb+buf2.Sb))
{
    Serial.println("CRC is NOT correct");
    counterRX++; //добавляем к счётчику, влияющему на отправку Oa
    counterRXe++; //добавляем к счётчику, влияющему на отправку Pa
  }
 else{
    counterRX++; // добавляем к счётчику полученных доставленных
  
    Serial.println("CRC is CORRECT");
    
    Serial.println("*****************RECEIVED following: **********************");  // комментирование что было отправлено
    Serial.println(buf2.Ab);
    Serial.println(buf2.Bb);
    Serial.println(buf2.Cb);
    Serial.println(buf2.Db);
    Serial.println(buf2.Eb);
    Serial.println(buf2.Fb);
    Serial.println(buf2.Gb);
    Serial.println(buf2.Hb);
    Serial.println(buf2.Ib);
    Serial.println(buf2.Jb);
//    Serial.println(buf2.Kb);// надо вывести 6 знаков после запятой
  Serial.printf("%6.6f", buf2.Kb); // кодированные координаты
//  Serial.print(" = ");
//  Serial.printf("%6.6f", buf2.Kb); // раскодированные координаты
  Serial.println();

  Serial.printf("%6.6f", buf2.Lb); 
  Serial.println();
  
    Serial.println(buf2.Mb);
    Serial.println(buf2.Nb);
    Serial.println(buf2.Ob);
    Serial.println(buf2.Pb);
    
//    Serial.println(buf2.Qb); 
  Serial.printf("%6.6f", buf2.Qb); 
    Serial.println();
//    Serial.println(buf2.Rb); 
  Serial.printf("%6.6f", buf2.Rb); 
    Serial.println();

    Serial.println(buf2.Sb);

    
    Serial.println();
    Serial.printf("%6.6f", buf2.crcb);
    Serial.print(  "  CRC  .");
Serial.println();
    
    Serial.println(buf2.Tb);
    Serial.println(buf2.Ub);
    Serial.println(buf2.Vb);
    Serial.println(buf2.Wb);
/*    Serial.println(buf2.Xb);
    Serial.println(buf2.Yb);

*/


Serial.println();
Serial.println("*******************end of message received******************");
Serial.println("+ + + + +");


//-------------------

// перенос полученных значений в (b) массив для вывода на экран и для обработки
Ab=(buf2.Ab);
Bb=(buf2.Bb);
Cb=(buf2.Cb);
Db=(buf2.Db);
Eb=(buf2.Eb);
Fb=(buf2.Fb);
Gb=(buf2.Gb);
Hb=(buf2.Hb);
Ib=(buf2.Ib);
Jb=(buf2.Jb);
Kb=(buf2.Kb); 
Target_LNG=(buf2.Kb);// присвоение целевой долготе полученного значения (нахождение оппонента) для расчёта расстояния по GPS
Lb=(buf2.Lb); 
Target_LAT=(buf2.Lb);// присвоение целевой широте полученного значения (нахождение оппонента) для расчёта расстояния по GPS

Mb=(buf2.Mb);

//Nb=(buf2.Nb); // принимаем число сколько пакетов отправил собеседник = для него это TX, а для нас(RX)
counterTX2=(buf2.Nb); // присваиваем число пакетов, которое нам отправляли

//Ob=(buf2.Ob); // принимаем число сколько пакетов принял собеседник (RX)= для нас это TX, а для него (TXd)
counterRX2=(buf2.Ob); // присваиваем число пакетов, которые были получены абонентом от нас

//Pb=(buf2.Pb);// принимаем число сколько пакетов принял собеседник с ошибками = для него это RXe, а для нас (TXe)
counterRXe2=(buf2.Pb);// присваиваем сколько пакетов от нас было получено с ошибками


Qb=(buf2.Qb); 
Rb=(buf2.Rb); 
Sb=(buf2.Sb);
crcb=(buf2.crcb);
Tb=(buf2.Tb);
Ub=(buf2.Ub);
Vb=(buf2.Vb);
Wb=(buf2.Wb);
/*Xb=(buf2.Xb);
Yb=(buf2.Yb);

*/
 } // окончание действия если контрольная сумма верна !

 } 
    } // окончание действия если не получены никакие данные 

   
   
//   else {
//Serial.println();
//Serial.println("- - - - - -");
//Serial.println("no data in serial");
//Serial.println("- - - - - -");
//Serial.println();



//  } // окончание цикла ПРИЁМА по таймеру mills-timer !!!!!!!!!!!!!!!!!!
    
     
} // окончание общего цикла loop




/* ВСЯКИЙ ХЛАМ НИЖЕ
 * 
 * 
/*



//message = "Unit 1 sent A ";
  if(millis()-timer>=1000)
  {
    timer=millis();
    counterTX++;
//U=Home_LAT0;
//U=Lat;
//    Serial1.print(message+counterTX); // sent to lora
//    Serial1.printf("%6.6f", message); // sent to lora
//--  Serial1.printf("%6.6f", U);
//--  Serial.printf("%6.6f", U);
//outmessage=message+"___"+counterTX;
//    Serial.println(outmessage); // uart text
//message = "";   
//outmessage = "";
//--U=Long;

//    Serial1.print(message+counterTX); // sent to lora
//    Serial1.printf("%6.6f", message); // sent to lora
//--Serial1.println();
//--  Serial1.printf("%6.6f", U);
//--Serial.println();
//--  Serial.printf("%6.6f", U);

//-----sending array--
//Serial1.print(&data, sizeof(data)); // sent to lora
//Serial.print(&data, sizeof(data)); // uart text for control

/*
        Serial1.print(sizeof(data));
        Serial1.print(data[0]);
        Serial1.print(data[1]);
        Serial1.print(data[2]);
        Serial1.print(data[3]); 
        Serial1.print(data[4]);
        Serial1.print(data[5]);



        Serial.print("SENT to B : ");
        Serial.print(sizeof(data));
        Serial.print("): ");
        Serial.print(data[0]);
        Serial.print(" , ");
        Serial.print(data[1]);
        Serial.print(" , ");
        Serial.print(data[2]);
        Serial.print(" , ");
        Serial.print(data[3]); 
        Serial.print(" , ");
        Serial.print(data[4]);
        Serial.print(" = ");
        Serial.print(data[5]);
        Serial.print(" (");
        Serial.print("CRC");
        Serial.println(")");

delay(10000);

*/

/*    
 tft.setCursor(0,0);
  tft.println("       Unit 1     ");
  tft.println("    transferred:    ");
  tft.println(outmessage);
outmessage="";      
tft.fillScreen(TFT_BLACK);
*/
/*
//------------------receiving from Lora
  if(Serial1.available())
  {
   
    delay(1);
// читаем родным методом readBytes()
  // указываем ему буфер-структуру, но приводим тип к byte*
  // размер можно указать через sizeof()
  if (Serial1.readBytes((byte*)&buf, sizeof(buf))) {
    Serial.println("Received: ");
     Serial.println(A);
    Serial.println(B);
    Serial.println(C);
    Serial.println(D);

       
/*       while(Serial1.available()) inmessage+=(char)Serial1.read();
    Serial.println("received from 2 unit  ");
    Serial.println(inmessage);
 /*tft.setCursor(0,60);
 tft.print("    received:    ");
 tft.setCursor(0,80);
  tft.print(inmessage);
  tft.fillScreen(TFT_BLACK);
    inmessage="";

  }
else {
  Serial.println("no input message from 2 unit");
/* tft.setCursor(0,60);
 tft.print("    received:    ");
tft.setCursor(0,80);
 tft.print("                  ");
     inmessage="";
*/
     

/*



//message = "Unit 1 sent A ";
  if(millis()-timer>=1000)
  {
    timer=millis();
    counterTX++;
//U=Home_LAT0;
//U=Lat;
//    Serial1.print(message+counterTX); // sent to lora
//    Serial1.printf("%6.6f", message); // sent to lora
//--  Serial1.printf("%6.6f", U);
//--  Serial.printf("%6.6f", U);
//outmessage=message+"___"+counterTX;
//    Serial.println(outmessage); // uart text
//message = "";   
//outmessage = "";
//--U=Long;

//    Serial1.print(message+counterTX); // sent to lora
//    Serial1.printf("%6.6f", message); // sent to lora
//--Serial1.println();
//--  Serial1.printf("%6.6f", U);
//--Serial.println();
//--  Serial.printf("%6.6f", U);

//-----sending array--
//Serial1.print(&data, sizeof(data)); // sent to lora
//Serial.print(&data, sizeof(data)); // uart text for control

/*
        Serial1.print(sizeof(data));
        Serial1.print(data[0]);
        Serial1.print(data[1]);
        Serial1.print(data[2]);
        Serial1.print(data[3]); 
        Serial1.print(data[4]);
        Serial1.print(data[5]);



        Serial.print("SENT to B : ");
        Serial.print(sizeof(data));
        Serial.print("): ");
        Serial.print(data[0]);
        Serial.print(" , ");
        Serial.print(data[1]);
        Serial.print(" , ");
        Serial.print(data[2]);
        Serial.print(" , ");
        Serial.print(data[3]); 
        Serial.print(" , ");
        Serial.print(data[4]);
        Serial.print(" = ");
        Serial.print(data[5]);
        Serial.print(" (");
        Serial.print("CRC");
        Serial.println(")");

delay(10000);

   
 tft.setCursor(0,0);
  tft.println("       Unit 1     ");
  tft.println("    transferred:    ");
  tft.println(outmessage);
outmessage="";      
tft.fillScreen(TFT_BLACK);
*/
/*
//------------------receiving from Lora
  if(Serial1.available())
  {
   
    delay(1);
// читаем родным методом readBytes()
  // указываем ему буфер-структуру, но приводим тип к byte*
  // размер можно указать через sizeof()
  if (Serial1.readBytes((byte*)&buf, sizeof(buf))) {
    Serial.println("Received: ");
     Serial.println(A);
    Serial.println(B);
    Serial.println(C);
    Serial.println(D);

       
       while(Serial1.available()) inmessage+=(char)Serial1.read();
    Serial.println("received from 2 unit  ");
    Serial.println(inmessage);
 /*tft.setCursor(0,60);
 tft.print("    received:    ");
 tft.setCursor(0,80);
  tft.print(inmessage);
  tft.fillScreen(TFT_BLACK);
    inmessage="";

  }
else {
  Serial.println("no input message from 2 unit");
/* tft.setCursor(0,60);
 tft.print("    received:    ");
tft.setCursor(0,80);
 tft.print("                  ");
     inmessage="";
*/
     


//----------------------- вывод данных, полученных от GPS -------------------


/*

 void displayInfo() // отображение на мониторе ПК
{
 
Serial.println("---------current GPS data----------");
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.println(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.println(gps.location.lng(), 6);
  }
  else
  {
    Serial.println(F("no GPS data"));
  }
  Serial.print(F("Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("no GPS data"));
  }
  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour()+3);
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("no GPS data"));
  }
Serial.println();
Serial.println(num_sat);
Serial.println(gps_speed);
Serial.println(Alt);
Serial.println(heading);
Serial.println(bearing);
Serial.println(distancem);
Serial.println(courseTo);
Serial.println(cardinalTo);

Serial.println("=================end of GPS data================");

  
}
// buf.crc = crc8_bytes((byte*)&buf, sizeof(buf) - 1);
*/





//---------------------------------------------ниже - второй алгоритм со стрингом (не используется)

/*
//--- string  
String L = String(gps.location.lat());
L.toCharArray(X, 6);
String Lg = String(gps.location.lng());
Lg.toCharArray(Lng, 6);
String M = String(gps.date.month());
M.toCharArray(Month, 6);
 String D = String(gps.date.day());
D.toCharArray(Day, 6);
 String Y = String(gps.date.year());
Y.toCharArray(Year, 6);
 String Hh = String(gps.time.hour()+3);
Hh.toCharArray(Hour, 6);
 String Mm = String(gps.time.minute());
Mm.toCharArray(Minute, 6);
 String Ss = String(gps.time.second());
Ss.toCharArray(Second, 6);

String spd = String(gps.speed.kmph());
spd.toCharArray(Speed, 6);
String sat = String(gps.satellites.value());
sat.toCharArray(Sattelites, 6);
String head = String(gps.cardinal(gps.course.value()));
head.toCharArray(Heading, 6);



tft.setCursor(0,0);
tft.print("Latt: ");
tft.setCursor(80,0);
tft.print(X);

tft.setCursor(0,20);
tft.print("Long: ");
tft.setCursor(80,20);
tft.print(Lng);

tft.setCursor(0,40);
tft.print("Sat: ");
tft.setCursor(80,40);
tft.print(Sattelites);

tft.setCursor(0,60);
tft.print("Head: ");
tft.setCursor(80,60);
tft.print(Heading);


tft.setCursor(0,80);
tft.print("H.lat: ");
tft.setCursor(80,80);
tft.print(Home_LAT0);

tft.setCursor(0,100);
tft.print("H.lng: ");
tft.setCursor(80,100);
tft.print(Home_LNG0);
//-----------------
*/


/*
//  screen.noStroke();  // выключаем линии и контуры:
//tft.background(0,0,0); // делаем фон черным:
//tft.stroke(255,255,255);  // делаем цвет линий белым:
//tft.fill(0,0,0);  // делаем цвет заливки серым:
 tft.setTextSize(1); //++  // set the font size
tft.text("Latt: ", 0, 0); //++
tft.text("Long: ", 0, 10); //++
tft.text("Date: ", 0, 20); //++
tft.text("Time: ", 0, 30); //++
tft.text("Sats: ", 0, 40); //++
tft.text("Speed: ", 0, 50); //++
tft.text("Heading: ", 0, 60); //++

 tft.setTextSize(1); //++  // set the font size
tft.text(X, 70, 0);
tft.text(Lng, 70, 10);
tft.text(Day, 70, 20);
tft.text(Month, 85, 20);
tft.text(Year, 100, 20);
tft.text(Hour, 70, 30);
tft.text(Minute, 85, 30);
tft.text(Second, 100, 30);
tft.text(Sattelites, 70, 40);
tft.text(Speed, 70, 50);
tft.text(Heading, 70, 60);
*/
