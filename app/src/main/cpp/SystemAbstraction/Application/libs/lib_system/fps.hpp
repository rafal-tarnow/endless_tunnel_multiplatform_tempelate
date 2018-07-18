#pragma once

#include <sys/time.h>
#include <stdlib.h>

class DeltaTimer{
public:
    void start();
    double getTimeFromStart_ms();
    double getTimeFromStart_us();

    void printTimeFromStart_ms();
    void printTimeFromStart_us();
private:
    struct timeval start_time_val;
    struct timeval te;
    double start_time_us = 0;
    double measure_time_us = 0;
};

class FPS{
public:
    double getFPS();
private:
    struct timeval te;
    double current_time_ms = 0;
    double previous_time_ms = 0;
    double delta_time_ms = 0;
};
