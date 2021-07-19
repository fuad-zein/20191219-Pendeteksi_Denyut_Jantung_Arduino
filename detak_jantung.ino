#include <Keypad.h>            // memasukan library keypad
#include <Wire.h>              // memasukan library komunikasi I2C (komunikasi LCD)
#include <LiquidCrystal_I2C.h> //memasukan library LCD I2C
#define PulseWire A0           // deklarasi pin untuk sensor
#define LED13 13               // deklarasi pin LED13
#define merah 11               // deklarasi pin merah
#define hijau 10               // deklarasi pin hijau

LiquidCrystal_I2C lcd(0x27, 16, 2); // setting kolom dan baris lcd

const byte ROWS = 4; // jumlah baris keypad
const byte COLS = 3; // jumlah kolom keypad
char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

byte rowPins[ROWS] = {9, 8, 7, 6}; // pin untuk keypad
byte colPins[COLS] = {5, 4, 3};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup()
{
  Serial.begin(9600); // setting baudrate komunikasi serial
  lcd.backlight();
  lcd.begin();            // memulai lcd
  pinMode(LED13, OUTPUT); // setting pin sebagai output
  pinMode(hijau, OUTPUT); // setting pin sebagai output
  pinMode(merah, OUTPUT); // setting pin sebagai output
  lcd.setCursor(3, 0);    // print karakter
  lcd.print("Pendeteksi");
  lcd.setCursor(1, 1);
  lcd.print("Denyut Jantung");
  delay(5000);
  lcd.clear();
}

byte kolom = 0;  // variabel untuk menampung kolom posisi karakter di LCD
String ket = ""; // keterangan
int myBPM, x, Max, Min;
char customKey;
int time1, time2;
bool Stop1 = false;
char umur[8];  // buat variabel umur (char array)
int Signal;    // variabel untuk menampung signal / hasil pembacaan sensor
int Threshold; // variabel untuk menampung trenshold / batas signal

void loop()
{
  lcd.setCursor(0, 0);
  lcd.print("Masukan Umur");
  customKey = customKeypad.getKey(); // akses keypad

  if (customKey)
  {                            // jika keypad diakses
    Serial.println(customKey); // tampilkan karakter di serial monitor
    umur[kolom] = customKey;   // tampung hasil keypad di variabel kolom
    lcd.setCursor(kolom, 1);
    lcd.print(umur[kolom]); // tampilkan di LCD
    kolom++;                // geser baris di LCD

    if (customKey == '*')
    { // jika * ditekan
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("* = Mulai");
      lcd.setCursor(0, 1);
      lcd.print("# = Batal");
      Stop1 = false;

      while (!Stop1)
      {
        customKey = customKeypad.getKey();
        if (customKey == '#') // jika # ditekan
        {
          Stop1 = true;
          lcd.clear();
          kolom = 0;
        }
        else if (customKey == '*')
        { // jika * ditekan
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Wait...");
          digitalWrite(hijau, HIGH); // hidupkan led hijau
          digitalWrite(merah, LOW);  // matikan led merah
          long lastMillis = millis();
          Max = 0;    //Variabel untuk menampung range sensor
          Min = 1023; //Variabel untuk menampung range sensor
          Threshold = 0;
          while (millis() - lastMillis < 5000)
          {
            Signal = analogRead(PulseWire);
            Serial.println(Signal);

            if (Signal < Min)
            {
              Min = Signal;
            }

            if (Signal > Max)
            {
              Max = Signal;
            }
          }
          Threshold = Min + ((Max - Min) * 0.5);
          Serial.println(Threshold);
          lastMillis = millis();
          myBPM = 0;
          if (Max - Min > 20)
          {
            while (millis() - lastMillis < 60000)
            {
              Signal = analogRead(PulseWire);
              Serial.println(Signal);

              time1 = (millis() - lastMillis) / 1000;
              if (time1 != time2)
              {
                time2 = time1;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Time : ");
                lcd.setCursor(8, 0);
                lcd.print(time1);
                lcd.setCursor(0, 1);
                lcd.print("BPM  : ");
                lcd.setCursor(8, 1);
                lcd.print(myBPM);
              }
              if (Signal > Threshold)
              {
                digitalWrite(LED13, HIGH);
                if (x == 0)
                {
                  myBPM++;
                  x = 1;
                }
              }
              if (Signal < Threshold)
              {
                digitalWrite(LED13, LOW);
                x = 0;
              }
            }

            int Umur = String(umur).toInt();
            if (Umur > 0 && Umur <= 1)
            {
              if (myBPM >= 120 && myBPM <= 160)
              {
                ket = "Normal";
              }
              else
              {
                if (myBPM > 120)
                  ket = "Denyut Cepat";
                else
                  ket = "Denyut Lambat";
              }
            }
            else if (Umur > 1 && Umur <= 3)
            {
              if (myBPM >= 90 && myBPM <= 140)
              {
                ket = "Normal";
              }
              else
              {
                if (myBPM > 90)
                  ket = "Denyut Cepat";
                else
                  ket = "Denyut Lambat";
              }
            }
            else if (Umur > 3 && Umur <= 6)
            {
              if (myBPM >= 80 && myBPM <= 110)
              {
                ket = "Normal";
              }
              else
              {
                if (myBPM > 80)
                  ket = "Denyut Cepat";
                else
                  ket = "Denyut Lambat";
              }
            }
            else if (Umur > 6 && Umur <= 12)
            {
              if (myBPM >= 75 && myBPM <= 105)
              {
                ket = "Normal";
              }
              else
              {
                if (myBPM > 75)
                  ket = "Denyut Cepat";
                else
                  ket = "Denyut Lambat";
              }
            }
            else if (Umur > 12 && Umur <= 18)
            {
              if (myBPM >= 60 && myBPM <= 100)
              {
                ket = "Normal";
              }
              else
              {
                if (myBPM > 60)
                  ket = "Denyut Cepat";
                else
                  ket = "Denyut Lambat";
              }
            }
            else if (Umur > 18 && Umur <= 60)
            {
              if (myBPM >= 60 && myBPM <= 100)
              {
                ket = "Normal";
              }
              else
              {
                if (myBPM > 60)
                  ket = "Denyut Cepat";
                else
                  ket = "Denyut Lambat";
              }
            }
            else if (Umur > 60)
            {
              if (myBPM >= 67 && myBPM <= 80)
              {
                ket = "Normal";
              }
              else
              {
                if (myBPM > 67)
                  ket = "Denyut Cepat";
                else
                  ket = "Denyut Lambat";
              }
            }

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(ket);
            lcd.setCursor(0, 1);
            lcd.print("BPM : ");
            lcd.setCursor(7, 1);
            lcd.print(myBPM);
          }
          else
          {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Error");
            lcd.setCursor(0, 1);
            lcd.print("Try Again");
            delay(1000);
            lcd.clear();
            Stop1 = true;
            kolom = 0;
          }
          digitalWrite(hijau, LOW);  // matikan led merah
          digitalWrite(merah, HIGH); // hidupkan led hijau
        }
      }
    }
  }
}
