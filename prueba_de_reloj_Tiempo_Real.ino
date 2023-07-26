#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>



#define SS_pin 10
int sensor = 2;
int ledDatos = 3;
int temperatura;
int humedad;
double cont;

File report;

LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 RTC;
DHT dht(sensor, DHT11);

byte wifiFull[8] = {B11111, B00000, B01110, B00000, B01110, B00000, B00100, B00100};

//Definining GeneriXOs Icon
byte genchar1[8] = {B00000, B00000, B00000, B00000, B00000, B00001, B00011, B00100};
byte genchar2[8] = {B10000, B10000, B10001, B01110, B00000, B00000, B00000, B00000};
byte genchar3[8] = {B00000, B00000, B00110, B01001, B10001, B00001, B00001, B10010};
byte genchar4[8] = {B01001, B10001, B10001, B01110, B00000, B00000, B00000, B00000};


//Definining Termometer Icon
byte tempchar1[8] = {B00000, B00001, B00010, B00100, B00100, B00100, B00100, B00111};
byte tempchar2[8] = {B00111, B00111, B00111, B00111, B00111, B00111, B00111, B00011};
byte tempchar3[8] = {B00000, B10000, B01011, B00100, B00111, B00100, B00111, B11100};
byte tempchar4[8] = {B11111, B11100, B11111, B11100, B11111, B11100, B11111, B11000};

//Defining Humidity Icon
byte humchar1[8] = {B00000, B00001, B00011, B00011, B00111, B01111, B01111, B11111};
byte humchar2[8] = {B11111, B11111, B11111, B01111, B00011, B00000, B00000, B00000};
byte humchar3[8] = {B00000, B10000, B11000, B11000, B11100, B11110, B11110, B11111};
byte humchar4[8] = {B11111, B11111, B11111, B11110, B11100, B00000, B00000, B00000};

//Defining Time Icon
byte timechar1[8] = {B00000, B00000, B00000, B00011, B00100, B01000, B10010, B10001};
byte timechar2[8] = {B10000, B10000, B01000, B00100, B00011, B00000, B00000, B00000};
byte timechar3[8] = {B00000, B00000, B00000, B11000, B00100, B00010, B01001, B10001};
byte timechar4[8] = {B00001, B00001, B00010, B00100, B11000, B00000, B00000, B00000};

//Defining WiFi Icon
byte wifichar1[8] = {B00000, B00111, B00111, B11000, B11000, B00000, B00111, B01111};
byte wifichar2[8] = {B01100, B00000, B00011, B00111, B00100, B00000, B00001, B00001};
byte wifichar3[8] = {B00000, B11100, B11100, B00011, B00011, B00000, B11100, B11110};
byte wifichar4[8] = {B00110, B00000, B11000, B11100, B00100, B00000, B10000, B10000};



void setup () {
  pinMode(ledDatos, OUTPUT);
  Wire.begin(); // Inicia el puerto I2C
  RTC.begin(); // Inicia la comunicaci¢n con el RTC
  //RTC.adjust(DateTime(__DATE__, __TIME__)); // Establece la fecha y hora (Comentar una vez establecida la hora)
  //el anterior se usa solo en la configuracion inicial
  Serial.begin(9600); // Establece la velocidad de datos del puerto serie
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, genchar1);
  lcd.createChar(2, genchar2);
  lcd.createChar(3, genchar3);
  lcd.createChar(4, genchar4);
  lcd.setCursor(0, 0);
  lcd.write(1);
  lcd.setCursor(0, 1);
  lcd.write(2);
  lcd.setCursor(1, 0);
  lcd.write(3);
  lcd.setCursor(1, 1);
  lcd.write(4);
  lcd.setCursor(3, 0);
  lcd.print(" GeneriXOs");
  lcd.setCursor(2, 1);
  lcd.print("Sensors System");
  delay(5000);
  lcd.clear();

  lcd.createChar(1, wifichar1);
  lcd.createChar(2, wifichar2);
  lcd.createChar(3, wifichar3);
  lcd.createChar(4, wifichar4);
  lcd.setCursor(0, 0);
  lcd.write(1);
  lcd.setCursor(0, 1);
  lcd.write(2);
  lcd.setCursor(1, 0);
  lcd.write(3);
  lcd.setCursor(1, 1);
  lcd.write(4);
  lcd.setCursor(3, 0);
  lcd.print(" Conectar red");
  lcd.setCursor(2, 1);
  lcd.print("");
  delay(5000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("  Inicializando ");
  lcd.setCursor(0, 1);
  lcd.print("Almacenamiento..");
  //delay(2000);
  for (int i = 0; i <= 10; i++) {
    digitalWrite(ledDatos, HIGH);
    delay(100);
    digitalWrite(ledDatos, LOW);
    delay(100);
  }

  if (!SD.begin(SS_pin)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ha fallado al ");
    lcd.setCursor(0, 1);
    lcd.print("Inicializar! ");
    for (int i = 0; i <= 4; i++) {
      digitalWrite(ledDatos, HIGH);
      delay(500);
      digitalWrite(ledDatos, LOW);
      delay(500);
    }
    lcd.clear();
    delay(1000);
    return;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Inicializado  ");
  digitalWrite(ledDatos, HIGH);
  delay(2000);
  lcd.clear();
  delay(1000);

}
void loop() {

  report = SD.open("report.txt", FILE_WRITE);
  DateTime now = RTC.now(); // Obtiene la fecha y hora del RTC
  temperatura = dht.readTemperature();
  humedad = dht.readHumidity();

  // para la fecha

  //logo de conexxion a red
  lcd.createChar(5, wifiFull);
  lcd.setCursor(3, 1);
  lcd.write(5);

  lcd.createChar(1, timechar1);
  lcd.createChar(2, timechar2);
  lcd.createChar(3, timechar3);
  lcd.createChar(4, timechar4);
  lcd.setCursor(0, 0);
  lcd.write(1);
  lcd.setCursor(0, 1);
  lcd.write(2);
  lcd.setCursor(1, 0);
  lcd.write(3);
  lcd.setCursor(1, 1);
  lcd.write(4);

  lcd.setCursor(6, 1);
  lcd.print(now.year(), DEC);
  lcd.print("/");
  if ((now.month()) <= 9) {
    lcd.print("0");
  }
  lcd.print(now.month(), DEC);
  lcd.print("/");
  if ((now.day()) <= 9) {
    lcd.print("0");
  }
  lcd.print(now.day(), DEC);

  lcd.setCursor(8, 0);
  if ((now.hour()) <= 9) {
    lcd.print("0");
  }
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  if (now.minute() <= 9) {
    lcd.print("0");
  }
  lcd.print(now.minute(), DEC);


  for (int i = 0; i <= 7; i++) {
    lcd.setCursor(10, 0);
    lcd.print(":");
    delay(500);
    lcd.setCursor(10, 0);
    lcd.print(" ");
    delay(500);
  }

  lcd.clear();
  delay(1000);



  // mostrando icono temperatura

  //logo de conexxion a red
  lcd.createChar(5, wifiFull);
  lcd.setCursor(3, 1);
  lcd.write(5);
  //temperatura
  lcd.createChar(1, tempchar1);
  lcd.createChar(2, tempchar2);
  lcd.createChar(3, tempchar3);
  lcd.createChar(4, tempchar4);
  lcd.setCursor(0, 0);
  lcd.write(1);
  lcd.setCursor(0, 1);
  lcd.write(2);
  lcd.setCursor(1, 0);
  lcd.write(3);
  lcd.setCursor(1, 1);
  lcd.write(4);
  lcd.setCursor(3, 0);
  lcd.print("Temperatura A");
  lcd.setCursor(8, 1);
  lcd.print(temperatura);
  lcd.print((char)223);
  lcd.print("C");

  delay(3000);
  lcd.clear();
  delay(1000);


  // mostrando icono humedad

  //logo de conexxion a red
  lcd.createChar(5, wifiFull);
  lcd.setCursor(3, 1);
  lcd.write(5);
  //humedad
  lcd.createChar(1, humchar1);
  lcd.createChar(2, humchar2);
  lcd.createChar(3, humchar3);
  lcd.createChar(4, humchar4);
  lcd.setCursor(0, 0);
  lcd.write(1);
  lcd.setCursor(0, 1);
  lcd.write(2);
  lcd.setCursor(1, 0);
  lcd.write(3);
  lcd.setCursor(1, 1);
  lcd.write(4);
  lcd.setCursor(3, 0);
  lcd.print("Humedad Relat");
  lcd.setCursor(8, 1);
  lcd.print(humedad);
  lcd.print("%");
  delay(3000);
  lcd.clear();
  delay(1000);

  cont = cont + 0.34;

  if (SD.begin(SS_pin)) {
    if (report) {
      digitalWrite(ledDatos, HIGH);
      if (cont >= 15) {
        
        crearRegistro();
        
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Proximo Registro");
        lcd.setCursor(0, 1);
        lcd.print("   ");
        lcd.print(15 - (int)cont);
        lcd.print(" ");
        lcd.print("minutos  ");
        digitalWrite(ledDatos, LOW);
        delay(100);
        digitalWrite(ledDatos, HIGH);
        delay(2800);
        lcd.clear();
        delay(1000);
        report.close();
        
      }
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error en");
      lcd.setCursor(0, 1);
      lcd.print("Almacenamiento");
      for (int i = 0; i <= 4; i++) {
      digitalWrite(ledDatos, HIGH);
      delay(500);
      digitalWrite(ledDatos, LOW);
      delay(500);
    }
      lcd.clear();
      delay(1000);
    }
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sin");
    lcd.setCursor(0, 1);
    lcd.print("Almacenamiento");
    for (int i = 0; i <= 4; i++) {
      digitalWrite(ledDatos, HIGH);
      delay(500);
      digitalWrite(ledDatos, LOW);
      delay(500);
    }
    lcd.clear();
    delay(1000);
    report.close();

  }

}

void crearRegistro(){
  report = SD.open("report.txt", FILE_WRITE);
  report.print(now.year(), DEC);
        report.print("/");
        if ((now.month()) <= 9) {
          report.print("0");
        }
        report.print(now.month(), DEC);
        report.print("/");
        if ((now.day()) <= 9) {
          report.print("0");
        }
        report.print(now.day(), DEC);
        report.print(", ");

        if (now.hour() <= 9) {
          report.print("0");
        }
        report.print(now.hour(), DEC);
        report.print(":");
        if (now.minute() <= 9) {
          report.print("0");
        }
        report.print(now.minute(), DEC);
        report.print(":");
        if (now.second() <= 9) {
          report.print("0");
        }
        report.print(now.second(), DEC);
        report.print(", ");
        report.print(temperatura);
        report.print(", ");
        report.print(humedad);
        report.println("");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("    Creando     ");
        lcd.setCursor(0, 1);
        lcd.print("    Registro    ");
        for (int i = 0; i <= 10; i++) {
          digitalWrite(ledDatos, HIGH);
          delay(100);
          digitalWrite(ledDatos, LOW);
          delay(100);
          digitalWrite(ledDatos, HIGH);
        }
        lcd.clear();
        delay(1000);
        cont = 0;
        report.close();
  
  }
