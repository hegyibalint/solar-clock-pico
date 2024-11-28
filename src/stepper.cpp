#include <Wire.h>
#include <math.h>

#include "stepper.h"

#define MAX_SPEED 500

long long stepperStepAlarm(alarm_id_t id, void *user_data)
{
    Stepper *stepper = (Stepper *)user_data;

    if (stepper->remainingSteps == 0)
    {
        stepper->busy = false;
        return 0;
    }

    StepDirection dir = stepper->remainingSteps > 0 ? STEP_RIGHT : STEP_LEFT;
    stepper->step(dir);
    stepper->remainingSteps -= (dir == STEP_RIGHT) ? 1 : -1;

    stepper->currentSpeed = min(
        stepper->currentSpeed + (MAX_SPEED - stepper->currentSpeed) / 8 + 5,
        MAX_SPEED);
    int waitTimeUs = (1000 / stepper->currentSpeed) * 1000;
    return -waitTimeUs;
}

Stepper::Stepper(int ul, int ur, int com) : ul(ul), ur(ur), com(com)
{
    pinMode(ul, OUTPUT);
    pinMode(ur, OUTPUT);
    pinMode(com, OUTPUT);
}

void Stepper::init()
{
    this->stepTo(-360);
}

bool Stepper::isBusy()
{
    return this->busy;
}

bool Stepper::stepTo(float degrees)
{
    this->busy = true;
    this->currentSpeed = 0;
    this->remainingSteps = degrees * 3;
    add_alarm_in_ms(2, stepperStepAlarm, this, true);
    return true;
}

void Stepper::busyStepTo(float degrees)
{
    this->stepTo(degrees);
    while (this->isBusy())
    {
        sleep_ms(1);
    }
}

void Stepper::step(StepDirection direction)
{
    // Increment or decrement the current step based on the direction
    if (direction == STEP_LEFT)
    {
        this->currentStep--;
    }
    else
    {
        this->currentStep++;
    }

    int phase;
    if (this->currentStep >= 0)
    {
        phase = this->currentStep % 6;
    }
    else
    {
        phase = 5 - (abs(this->currentStep) % 6);
    }

    // Serial1.printf("Current step: %d\r\n", this->currentStep);
    // Serial1.printf("Phase: %d\r\n", phase);

    // The following is the 6-phase pulse sequence for the VID29 stepper motor
    switch (phase)
    {
    case 0:
        digitalWrite(this->ul, HIGH);
        digitalWrite(this->ur, HIGH);
        digitalWrite(this->com, LOW);
        break;
    case 1:
        digitalWrite(this->ul, LOW);
        digitalWrite(this->ur, HIGH);
        digitalWrite(this->com, LOW);
        break;
    case 2:
        digitalWrite(this->ul, LOW);
        digitalWrite(this->ur, HIGH);
        digitalWrite(this->com, HIGH);
        break;
    case 3:
        digitalWrite(this->ul, LOW);
        digitalWrite(this->ur, LOW);
        digitalWrite(this->com, HIGH);
        break;
    case 4:
        digitalWrite(this->ul, HIGH);
        digitalWrite(this->ur, LOW);
        digitalWrite(this->com, HIGH);
        break;
    case 5:
        digitalWrite(this->ul, HIGH);
        digitalWrite(this->ur, LOW);
        digitalWrite(this->com, LOW);
        break;
    default:
        Serial1.println("Invalid phase");
        break;
    }
}