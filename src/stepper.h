#pragma once

#include <pico/time.h>

enum StepDirection
{
    STEP_LEFT,
    STEP_RIGHT
};

class Stepper
{
private:
    int ul, ur, com;
    bool busy = false;
    int currentStep = 0;
    int currentSpeed = 0;
    int remainingSteps = 0;
    void step(StepDirection dir);

public:
    Stepper(int ul, int ur, int com);
    void init();
    bool isBusy();
    bool stepTo(float degrees);
    void busyStepTo(float degrees);

    friend long long stepperStepAlarm(alarm_id_t id, void *user_data);
};
