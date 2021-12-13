#include <Wire.h>

#define ADC_DOD  0x48

//int KciukD = A3;
//int KciukG = A4;
//int KciukN = A5;
//int WskazujacyD = A0;
//int SrodkowyD = A2;
//int SerdecznyD = A9;
//int SerdecznyG = A6;
//int SerdecznyN = A7;
//int MalyD = A11;
//int MalyG = A10;
//int MalyN = A8;
//int Dodatkowy = A1;

//uint8_t WskazujacyG;  // I2c
//uint8_t WskazujacyN; // I2c
//uint8_t SrodkowyG; // I2c
//uint8_t SrodkowyN; // I2c
uint16_t val[16];  // wartosc zczytana
float res[12]; //rezystancja czujnika ugiecia
float Uwe = 5;
float R = 1000; // rezystor
float Uwy[12];
char *A[12] = {"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "A10", "A11"};

void setup() {
  Wire.begin();
  Serial.begin(9600);           //  setup serial

  Wire.beginTransmission(ADC_DOD);          //rozpocznij transmisję do urządzenia z adresem 0x48
  Wire.write(ADC_DOD + 1);                         //wyślij adres 0x6B, odpowiadający za uruchomienie modułu
  Wire.write(0);                              //obudź układ
  Wire.endTransmission();
}

void loop() {

  Wire.beginTransmission(ADC_DOD);         //rozpocznij transmisję do urządzenia z adresem 0x68
  Wire.write(0x04);
  Wire.endTransmission();                    //zakończ transmisję
  Wire.requestFrom(ADC_DOD, 4);            //zażądaj od urządzenia o adresie 0x68 zwrócenia 8 bajtów
//
//
  val[12] = Wire.read(); //WN
  val[13] = Wire.read(); //WG
  val[14] = Wire.read(); //ŚG
  val[15] = Wire.read(); //ŚN

  //  Serial.print("WskazujacyG= ");
  //  Serial.print(WskazujacyG);
  //  Serial.print("  WskazujacyN= ");
  //  Serial.print(WskazujacyN);
  //  Serial.print("  SrodkowyG= ");
  //  Serial.print(SrodkowyG);
  //  Serial.print("  SrodkowyN= ");
  //  Serial.println(SrodkowyN);

  val[0] = analogRead(A0); delay(1); val[1] = analogRead(A1); delay(1);
  val[2] = analogRead(A2); delay(1);  val[3] = analogRead(A3); delay(1);
  val[4] = analogRead(A4); delay(1); val[5] = analogRead(A5); delay(1);
  val[6] = analogRead(A6); delay(1); val[7] = analogRead(A7); delay(1);
  val[8] = analogRead(A8); delay(1); val[9] = analogRead(A9); delay(1);
  val[10] = analogRead(A10); delay(1);  val[11] = analogRead(A11); delay(1);

  //  for (int i = 0; i < 12; i++) {
  //    //    Serial.println(A[i]);
  //    //    val[i] = analogRead(A[i]); // read the input pin
  //    //  Serial.print(val[i]);  Serial.print("          ");
  //    Uwy[i] = (val[i] / 1024) * Uwe;
  //    //  Serial.println(Uwy[i]);
  //    res[i] = (Uwy[i] * R) / (Uwe - Uwy[i]);
  //    Serial.print(res[i]); Serial.print("       ");
  //    //Serial.println(val[6]);
  //    // delay(10);
  //  }

  //while (Serial.available()) {
    Serial.print("P");
    for (int i = 0; i < 16; i++)
    {
      Serial.print(val[i]);Serial.print("-");
    }
    Serial.println("K");
 // }
delay(50);
}
