#include <TimerOne.h>
#include "kernel.h"

int distanceThreshold = 100;
int dangerThreshold = 40;
float cm = 0;
int ledState = 0;

unsigned long lastBlinkTime = 0;
unsigned long blinkInterval = 200;  
unsigned long lastDistanceReadTime = 0; 
unsigned long distanceReadInterval = 500; 

const int triggerPin = 3;
const int echoPin = 2;
const int buzzer = 5;
const int redPin = 9;
const int greenPin = 8;

float getDistance() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  long duracao = pulseIn(echoPin, HIGH, 30000);

  return duracao > 0 ? duracao * 0.034 / 2.0 : 0;
}

char showDistance(void) {
  Serial.print(cm);
  Serial.println(" cm");
  return REPEAT;
}

char manageLeds(void) {
  if (cm == 0 || cm > distanceThreshold) {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
  } else if (cm <= distanceThreshold && cm > dangerThreshold) {
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, HIGH);
  } else if (cm <= dangerThreshold) {
    startBlinking();
  }
  return REPEAT;
}

void startBlinking() {
  unsigned long currentTime = millis();
  if (currentTime - lastBlinkTime >= blinkInterval) {
    lastBlinkTime = currentTime;
    ledState = !ledState; 

    digitalWrite(redPin, ledState);
    digitalWrite(greenPin, !ledState);
    tone(buzzer, 2500, 100); 
  }
}

char readDistance(void) {
  unsigned long currentTime = millis();
  if (currentTime - lastDistanceReadTime >= distanceReadInterval) {
    lastDistanceReadTime = currentTime;
    cm = getDistance();
    Serial.print("Distancia: ");
    Serial.println(cm);
  }
  return REPEAT;
}

void setup() {
  Serial.begin(9600);
  
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  digitalWrite(buzzer, LOW);
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);

  kernelInit();

  kernelAddProc(readDistance, 1, 1);
  kernelAddProc(manageLeds, 1, 1);
  kernelAddProc(showDistance, 1, 5);

  Timer1.initialize(1000000);
  Timer1.attachInterrupt(kernelTick);

  kernelLoop();
}

void loop() { }
