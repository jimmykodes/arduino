#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define CLK 2
#define DT 3
#define ENC_BTN 4

#define LED_PIN 5
#define NUM_PIXELS 50

Adafruit_NeoPixel strip(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

enum Mode: int {
  HUE = 0,
  SAT = 1,
  VAL = 2,
  ANIM = 3
};

int mode = HUE;
int btnState;
int currentStateCLK;
int lastStateCLK;
uint16_t hue ;
int sat = 255;
int val = 255;
int animNum;

void setup() {
  Serial.begin(9600);

  strip.begin();
  strip.setBrightness(64);
  strip.show();

  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);
  pinMode(ENC_BTN, INPUT);

  lastStateCLK = digitalRead(CLK);
}

void loop() {
  strip.clear();
  processBtn();
  processEnc();
  if (mode == ANIM) {
    playAnim();
  }
  for(int i=0; i<NUM_PIXELS; i++) {
    strip.setPixelColor(i, strip.ColorHSV(hue, sat, val));
  }
  strip.show();
}

void processBtn() {
  int btn = digitalRead(ENC_BTN);
  if (btn == LOW && btnState != LOW) {
    // encoder button normally closed
    mode = (mode + 1) % 4;
  }
  btnState = btn;
}

void processEnc() {
  currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK  && currentStateCLK == HIGH){
    updateSettings(digitalRead(DT) == currentStateCLK);
  }
  lastStateCLK = currentStateCLK;
}

void updateSettings(bool cw) {
  switch (mode) {
    case HUE:
    updateHue(cw);
    break;
    case SAT:
    updateSat(cw);
    break;
    case VAL:
    updateVal(cw);
    break;
    case ANIM:
    updateAnim(cw);
    break;
  }
}

void updateHue(bool cw) {
  if (cw) {
    if (hue == 65535) {
      hue = 0;
    }
    hue += 65535 / 200;
  } else {
    if (hue == 0) {
      hue = 65535;
    }
    hue -= 65535 / 200;
  }
  Serial.print("updated hue");
  Serial.println(hue);
}

void updateSat(bool cw) {
  if (cw) {
    if (sat < 255) {
      sat += 5;
    }
  } else {
    if (sat > 0) {
      sat -= 5;
    }
  }
  Serial.print("updated sat");
  Serial.println(sat);
}

void updateVal(bool cw) {
  if (cw) {
    if (val < 255) {
      val += 5;
    }
  } else {
    if (val> 0) {
      val -= 5;
    }
  }
  Serial.print("updated val");
  Serial.println(val);
}

void updateAnim(bool cw) {
  if (cw) {
    animNum++;
  } else {
    animNum--;
  }
}

void playAnim() {
  Serial.println("playing anim");
}

