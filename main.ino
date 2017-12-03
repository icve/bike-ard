#include "LedControl.h"

#define HALL_EFFECT_SENSOR 6
#define HE_VPP 12 // hall effect sensor V+
#define HE_GND 11 //hall effect sensor ground
#define OUTPUT_EVERY 200 //output interval in milisecond
#define DOT_POS 2 // position of fixed doc
#define BUTTON_PIN 6
#define MODE_CHANGE_DELAY 2000
#define BRIGHTNESS_ADJ_WAIT 5000


int low_cutoff = 527;
int high_cutoff = 533;

LedControl lc = LedControl(2, 3, 4, 1);
unsigned long startt = 0;
unsigned long stopt = 0;
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(HE_VPP, OUTPUT);
  pinMode(HE_GND, OUTPUT);
  digitalWrite(HE_VPP, HIGH);
  digitalWrite(HE_GND, LOW);
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 0); // sets brightness (0~15 possible values)
  lc.clearDisplay(0); // clear screen
  Serial.begin(9600);

  if (digitalRead(BUTTON_PIN) == LOW) {
    debugMode();
  }

}

void debugMode() {
  showi(1);
  delay(MODE_CHANGE_DELAY);
  while (1) {
    showi(analogRead(HALL_EFFECT_SENSOR));
    delay(OUTPUT_EVERY);
    if (digitalRead(BUTTON_PIN) == LOW) {
      brightnessAdj();
      break;
    }
  }
}

void brightnessAdj(){
  unsigned long t = millis() + BRIGHTNESS_ADJ_WAIT;
  int bri = 0;
  while(millis() < t){
    if(digitalRead(BUTTON_PIN)  == LOW){
    lc.setIntensity(0, bri);
    showd(bri);
    bri++;
    delay(OUTPUT_EVERY);
    }
  }
  }


void loop() {
  //check low time() high low high time()
  while(analogRead(HALL_EFFECT_SENSOR) > low_cutoff){}
  while(analogRead(HALL_EFFECT_SENSOR) < high_cutoff){}
  startt = micros();
  while(analogRead(HALL_EFFECT_SENSOR) > low_cutoff){}
  while(analogRead(HALL_EFFECT_SENSOR) < high_cutoff){}
  stopt = micros();
  double x = (1 / ((stopt - startt) * 1E-6));
  x = x * 32 E-4;
  showd(x);
  delay(OUTPUT_EVERY);

}

/* display a double */
void showd(double d) {
  int h = (int)d;
  int h1 = h / 10 % 10;
  int h2 = h % 10;
  int t = d * 100;
  int t1 = t / 10 % 10;
  int t2 = t % 10;

  displayDg(1, h1);
  displayDg(2, h2);
  displayDg(3, t1);
  displayDg(4, t2);
  Serial.println(d);
}
/* display a integer*/
void showi(int i) {
  int d1 = i / 1000 % 10;
  int d2 = i / 100 % 10;
  int d3 = i / 10 % 10;
  int d4 = i % 10;

  displayDg(1, d1);
  displayDg(2, d2);
  displayDg(3, d3);
  displayDg(4, d4);



}

void displayDg(int pos, int dg) {
  int dgSeq;
  switch (dg) {
    case 0:
      //    dgSeq = B11111100;
      dgSeq = B0000000;
      break;
    case 1:
      dgSeq = B01100000;
      break;
    case 2:
      dgSeq = B11011010;
      break;
    case 3:
      dgSeq = B11110010;
      break;
    case 4:
      dgSeq = B01100110;
      break;
    case 5:
      dgSeq = B10110110;
      break;
    case 6:
      dgSeq = B10111110;
      break;
    case 7:
      dgSeq = B11100000;
      break;
    case 8:
      dgSeq = B11111110;
      break;
    case 9:
      dgSeq = B11110110;
      break;
    default:
      dgSeq = B11111110;
  }
  if (pos == DOT_POS) {
    dgSeq += 1;
  }
  lc.setColumn(0, pos, dgSeq);

}
