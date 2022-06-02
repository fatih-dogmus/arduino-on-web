#include "EEPROM.h"
#include "color.h"
#include "math.h"
// math.h isnan fonksiyonu için

int analog;
float volt;
Color color;

typedef struct Watt{
  bool init = false;
  float val;
}watt;

watt _watt;

void setup() {

  color = Color();
  /*
  _watt.val = 0.0f;
  _watt.init = false;
  EEPROM.put(0, _watt);
  */
  
  Serial.begin(9600); // 9600 baud olarak serial portu ayarla
  cli(); // tüm kesmeler devre dışı
  
  TCCR1A = 0; // TCCR1A kaydedicisini 0'la
  // TCCR1A bitleri -> COM1A1 COM1A0 COM1B1 COM1B0 - - WGM11 WGM10
  TCCR1B = 0; // TCCR1B kaydedicisini 0'la
  // TCCR1B bitleri -> ICNC1 ICES1 - WGM13 WGM12 CS12 CS11 CS10
  TCNT1 = 0; // kullanımına göre ICR1 veya OCR1A değerine eşitlendiğinde sıfırlanır
  OCR1A = 15624; // (16*10^6 / 1024)-1  16 bit kaydedici -> OCR1AH OCR1AL
  TCCR1B |= (1 << WGM12); // CTC modunu aktif eder.
  // CTC -> daha önce set edilen sayaca eşit olunca aktif olan kesme
  TCCR1B |= (1 << CS12) | (1 << CS10); // adımlar arası geçen osilatör hızının 1024'e ayarlandı
  TIMSK1 |= (1 << OCIE1A); // timer1 kesmesi aktif hale getirildi

  sei(); // tüm kesmeler aktif
}

void loop() {
  analog = analogRead(A0);
  volt = 5*analog/1023.0; // analog değer 0-1023 arasında yer alır
  Serial.println(color.toHex(volt)); // #222222 ile #ffffff arasında değer üret
  delay(100);
}



ISR(TIMER1_COMPA_vect){ // Timer fonksiyonu
  
  EEPROM.get(0, _watt); // EEPROM üzerinden değeri oku
  if(isnan(_watt.val) || !_watt.init || 4294967040.0 < _watt.val || _watt.val < -4294967040.0){
    // baslatilmadiysa veya overflow olduysa veya sayi olmadiysa sifirla
    _watt.val = 0.04f*volt; // başlangıç olarak 40mA*okunan voltu ata
    _watt.init = true; // daha önce başlatılıp başlatılmadığını kontrol etmek için
    EEPROM.put(0, _watt); // atanan değeri EEPROM'a yaz
  }else{
    _watt.val += 0.04f*volt; // okunan değere 40mA*okunan volt'u ekle
    EEPROM.put(0, _watt); // EEPROM'a eklenmiş değeri yaz
  }
  Serial.println(_watt.val); // Seri port üzerinden eklenmiş değeri ilet
}
