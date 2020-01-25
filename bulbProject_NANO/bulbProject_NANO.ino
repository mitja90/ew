#include <TimerOne.h>

#define SHORT_DELAY delayMicroseconds(5)
#define DELAY_TIME 100
#define BULB_COUNT 8
#define EFFECT_TIMER 1.5

unsigned char channel[] = {3, 4, 5, 6, 7, 8, 9, 10};  // OUTPUT PINS
unsigned int analogPin = A6;
unsigned char CH[BULB_COUNT];
int i = 0;
unsigned char clock_tick;
bool effectOn = false;
int timeCounter = 0; 

void setup() {
  for (i = 0; i < BULB_COUNT; i++) pinMode(channel[i], OUTPUT);
  attachInterrupt(0, zeroCrossInt, RISING);   
  Timer1.initialize(100);
  Timer1.attachInterrupt(timerIsr);
  randomSeed(analogRead(4));
}

/*
  controls PWM
*/
void timerIsr() {
  clock_tick++;
  for (int j = 0; j < BULB_COUNT; j++) {
    if (CH[j] == clock_tick) {
      digitalWrite(channel[j], HIGH);  // triac firing
      SHORT_DELAY;
      digitalWrite(channel[j], LOW);  // triac Off
    }
  }
}

/*
  sets random LightBulb to full brightness
*/
void randomBulbFullBrightness() {
  if (effectOn == true) {
    int tmp;
    for (i = 0; i < 3; i++) {
      tmp = (int)random(0, BULB_COUNT - 1);
      if (CH[tmp] == 70) {
        CH[tmp] = 5;
        break;
      }
    }
  }
}

void setAllLeds(int val) {
  effectOn = false;
  for (i = 0; i < BULB_COUNT; i++) CH[i] = val;
}

/*
  Every zerocrossing interrupt: For 50Hz (1/2 Cycle) => 10ms ; For 60Hz (1/2
  Cycle) => 8.33ms
  10ms=10000us , 8.33ms=8330us
*/
void zeroCrossInt() { clock_tick = 0; timeCounter++; }

void loop() {
  int val = analogRead(analogPin);
  if (val < 900)
    setAllLeds(val / 28 + 40);
  else {
    if (effectOn == false) setAllLeds(70);
    effectOn = true;
    for (i = 0; i < BULB_COUNT; i++) {
      if (CH[i] < 70) CH[i]++;
    }
  }
  if(timeCounter > EFFECT_TIMER * 100) {
    timeCounter = 0;
    randomBulbFullBrightness();
  }
  delay(DELAY_TIME);
}
