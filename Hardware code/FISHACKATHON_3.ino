#include "pin.h"
#include <Servo.h>
Servo servo;
#include <NewPing.h>
NewPing range1(trigger1, echo1, Max_Distance);
NewPing range2(trigger2, echo2, Max_Distance);

boolean detect;                     // DETEKSI IKAN
int potentioValue = 0;              // Potensiometer
int motorSpeed = 50;                // Kecepatan Konveyor
float previous = 0;
float next = 0;
float panjangIKAN = 0;              // PANJANG IKAN
float panjangIKANTemp = 0;          // UNTUK KALKULASI AVERAGE
float panjangIKANRata2 = 0;         // Panjang Rata-rata
float tebalIKAN = 0;                // TEBAL IKAN
float tebalIKANTemp = 0;            // UNTUK KALKULASI AVERAGE
float tebalIKANRata2 = 0;           // Tebal Rata-rata
float distanceHold = 100;           // Variable tebal IKAN
unsigned long jumlahIKAN = 0;       // JUMLAH IKAN
unsigned long timeS1 = 0;           // Ultrasonic 1
unsigned long timeS2 = 0;           // Ultrasonic 2
unsigned long distance1 = 0;        // Jarak deteksi 1
unsigned long distance2 = 0;        // Jarak deteksi 2
unsigned long second = 0;           // Waktu real time
unsigned long second1 = 0;          // Variable waktu 1
unsigned long second2 = 0;          // Variable waktu 2
float secondValue = 0;              // Waktu IKAN lewat

void setup(){
  Serial.begin(9600);
//  Serial.println("Team DOA IBU @FISHACKATHON 2016 (Tangkapan Ikan Tuna)");
  pinMode(DC, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(pushButton, INPUT_PULLUP);
  digitalWrite(13, LOW);
  servo.attach(servoPIN);
  servo.write(0);
  delay(500);
  servo.write(170);
  delay(500);
  servo.write(30);
//  potentioValue = analogRead(potentioPIN);
//  motorSpeed = potentioValue / 4;
  detect = 0;
  secondValue = 0;
  jumlahIKAN = 0;
  panjangIKAN = 0;
  tebalIKAN = 0;
}

void loop(){
  if(digitalRead(pushButton) == LOW && jumlahIKAN != 0){
    Serial.print(jumlahIKAN);
    Serial.print(";");
    Serial.print(panjangIKANRata2);
    Serial.print(";");
    Serial.println(tebalIKANRata2);
//RESET
    detect = 0;
    secondValue = 0;
    jumlahIKAN = 0;
    panjangIKAN = 0;
    tebalIKAN = 0;
    panjangIKANRata2 = 0;
    tebalIKANRata2 = 0;
  }
  unsigned long second = millis();
  analogWrite(DC, motorSpeed);
  scan();
  //tunggu sampai ada ikan terdeteksi
  if(distance1 < distance || distance2 < distance){
    second1 = 0;
    second1 = second - second1;
    detect = 1;
  }
  while(detect == 1){
    scan();
    digitalWrite(13, HIGH);
    if(distanceHold > distance1 || distanceHold > distance2){
      distanceHold = min(distance1, distance2);
    }
    if(distance1 > distance && distance2 > distance){
      second2 = millis();
      secondValue = second2 - second1;
      jumlahIKAN = jumlahIKAN + 1;
      previous = panjangIKAN;
      next = Speed * secondValue / 1000;
      panjangIKAN = next - previous;
      tebalIKAN = distance - distanceHold;
      digitalWrite(13, LOW);
      secondValue = 0;
      
      if(panjangIKAN > 5 && panjangIKAN != 0){
        servo.write(30);
//        Serial.print("1, ");
//        Serial.print(tebalIKAN);
//        Serial.print("cm, ");
//        Serial.print(panjangIKAN);
//        Serial.print("cm, ");
//        Serial.println(jumlahIKAN);
//        panjangIKAN = 0;
//        tebalIKAN = 0;
      }
      if(panjangIKAN <= 5 && panjangIKAN != 0){
        servo.write(150);
//        Serial.print("0, ");
//        Serial.print(tebalIKAN);
//        Serial.print("cm, ");
//        Serial.print(panjangIKAN);
//        Serial.print("cm, ");
//        Serial.println(jumlahIKAN);
//        panjangIKAN = 0;
//        tebalIKAN = 0;
      }
      detect = 0;
//Rata2
      if(panjangIKANTemp == 0){ panjangIKANTemp = panjangIKAN; }
      panjangIKANRata2 = (panjangIKAN + panjangIKANTemp)/2;
      if(tebalIKANTemp == 0){ tebalIKANTemp = tebalIKAN; }
      tebalIKANRata2 = (tebalIKAN + tebalIKANTemp)/2; 
    }
  }
}

void scan(){
  timeS1 = range1.ping();
  distance1 = timeS1 / US_ROUNDTRIP_CM;
  if(distance1 >= Max_Distance){ distance1 = Max_Distance; }
  delay(50);
  timeS2 = range2.ping();
  distance2 = timeS2 / US_ROUNDTRIP_CM;
  if(distance2 >= Max_Distance){ distance2 = Max_Distance; }
  delay(50);
}
