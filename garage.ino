
#if defined(__AVR_ATtiny85__)
#include <LowPowerAttiny.h>
#else
#include <LowPower.h>
#endif

#if defined(__AVR_ATtiny85__)
#define PIN_MOTION   PB0
#define PIN_TRIG     PB1
#define PIN_ECHO     PB2
#define PIN_PWR      PB3
#define PIN_LED      PB4
#else
#define PIN_MOTION   PD2
#define PIN_TRIG     PD4
#define PIN_ECHO     PD5
#define PIN_PWR      PD6
#define PIN_LED      PB5
#endif

#define CAR_MISSING_DISTANCE 100
#define CAR_MISSING_TIMEOUT    5
#define CAR_PRESENT_TIMEOUT   20

#if defined(__AVR_ATtiny85__)
#define digitalPinToInterrupt(p)  ( (p) == 2 ? 0 : ((p) == 3 ? 1 : -1) )
#endif

#if !defined(__AVR_ATtiny85__)
//#define SERIAL_DEBUG
#endif

volatile static bool motionSensorState = LOW;
volatile static signed char lightOnTimeout = -1;

void motionSensorISR() {
  motionSensorState = digitalRead(PIN_MOTION);
}

void lightOn() {
#if defined(SERIAL_DEBUG)
  Serial.println("lightOn");
  Serial.flush();
#endif
  digitalWrite(PIN_LED, HIGH);
  LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
}

void lightOff() {
#if defined(SERIAL_DEBUG)
  Serial.println("lightOff");
  Serial.flush();
#endif
  digitalWrite(PIN_LED, LOW);
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
}

unsigned char microsecondsToCentimeters(unsigned long microseconds) {
  return microseconds / 29 / 2;
}

long measureDistance() {
  digitalWrite(PIN_PWR, HIGH);
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  digitalWrite(PIN_PWR, LOW);

  return microsecondsToCentimeters(pulseIn(PIN_ECHO, HIGH));
}

bool isCarInside() {
  return measureDistance() > CAR_MISSING_DISTANCE;
}

bool movementDetected() {
  return motionSensorState == HIGH;
}

bool noMovementDetected() {
  return !movementDetected();
}


void setup() {

#if defined(SERIAL_DEBUG)
  Serial.begin(115200);
#endif

  pinMode(PIN_MOTION, INPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_PWR, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(PIN_MOTION), motionSensorISR, CHANGE);

#if defined(SERIAL_DEBUG)
  Serial.println("Init complete");
  Serial.flush();
#endif

  digitalWrite(PIN_PWR, LOW);
  lightOff();
}

void loop() {
#if defined(SERIAL_DEBUG)
  Serial.println();
#endif

  if (movementDetected()) {
    lightOnTimeout = -1;
    lightOn();
  }

  if (noMovementDetected()) {
    if (lightOnTimeout < 0) {
      lightOnTimeout =
        isCarInside() ? CAR_PRESENT_TIMEOUT : CAR_MISSING_TIMEOUT;
#if defined(SERIAL_DEBUG)
      Serial.print("Setting timeout ");
      Serial.println(lightOnTimeout);
#endif
      lightOn();
    } else if (lightOnTimeout == 0) {
      lightOnTimeout--;
      lightOff();
      return;
    } else {
      lightOnTimeout--;
#if defined(SERIAL_DEBUG)
      Serial.print("Current timeout ");
      Serial.println(lightOnTimeout);
#endif
      lightOn();
    }
  }

}
