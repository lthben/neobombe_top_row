/*
Author: Benjamin Low (Lthben@gmail.com)
Date: May 2015
Description: Code for controlling 36 stepper motors for project "Neobombe",
presented at Singapore Maker Faire 2015 by Art Makes Us. "Neobombe" stimulates
the Alan Turing machine seen in the 2014 film "The Imitation Game".

This code runs the top row of 12 motors. Each Arduino has 1 motor shield and 2
motors only.
*/

/*
Notes:
        - call release() whenever the motors do not need to turn, to dissipate heat
        - avoid address 0x70 since this addresses ALL the motors
        - http://www.dunkels.com/adam/pt/ Protothreads for simultaneous control
        - use SINGLE stepping mode to reduce the current loading and heat buildup
*/
#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS_1 = Adafruit_MotorShield(0x60);
Adafruit_StepperMotor *myStepper_1 = AFMS_1.getStepper(200, 1);
Adafruit_StepperMotor *myStepper_2 = AFMS_1.getStepper(200, 2);

void forwardstep_1() {
  myStepper_1->onestep(FORWARD, SINGLE);
}
void backwardstep_1() {
  myStepper_1->onestep(BACKWARD, SINGLE);
}

void forwardstep_2() {
  myStepper_2->onestep(FORWARD, SINGLE);
}
void backwardstep_2() {
  myStepper_2->onestep(BACKWARD, SINGLE);
}

AccelStepper stepper_1(forwardstep_1, backwardstep_1);
AccelStepper stepper_2(forwardstep_2, backwardstep_2);

int ledPin = 5;
int switchPin = 7;

void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT);

  AFMS_1.begin();

  TWBR = ((F_CPU / 400000l) - 16) / 2; // Change the i2c clock to 400KHz

  //NEMA 200-step stepper. Simulate top row
  stepper_1.setSpeed(300.0);
  stepper_2.setSpeed(300.0);
  
  Serial.flush();
}


boolean is_processing;
byte incoming_byte;

void loop() {

  if (Serial.available() > 0) {

    incoming_byte = Serial.read();

    if (incoming_byte == 65) {

      digitalWrite(ledPin, HIGH);
      is_processing = true;

    }
    else if (incoming_byte == 66) {
      digitalWrite(ledPin, LOW);
      is_processing = false;
      myStepper_1->release();
      myStepper_2->release();

    }
  }

  if (is_processing) {
    stepper_1.runSpeed();
    stepper_2.runSpeed();
  }
}


