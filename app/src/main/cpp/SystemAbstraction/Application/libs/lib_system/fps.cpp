#include "fps.hpp"
#include <iostream>

using namespace std;


void DeltaTimer::start()
{
    gettimeofday(&start_time_val, NULL);
}

double DeltaTimer::getTimeFromStart_ms()
{
    gettimeofday(&te, NULL);
    start_time_us = ((double)(start_time_val.tv_sec))*1000000.0 + ((double)(start_time_val.tv_usec));
    measure_time_us = ((double)(te.tv_sec))*1000000.0 + ((double)(te.tv_usec));
    return (measure_time_us - start_time_us)/1000.0;
}


double DeltaTimer::getTimeFromStart_us()
{
    gettimeofday(&te, NULL);
    start_time_us = ((double)(start_time_val.tv_sec))*1000000.0 + ((double)(start_time_val.tv_usec));
    measure_time_us = ((double)(te.tv_sec))*1000000.0 + ((double)(te.tv_usec));
    return measure_time_us - start_time_us;
}

void DeltaTimer::printTimeFromStart_ms()
{
    cout << "DeltaTimer: timeFromStart = " << getTimeFromStart_ms() << " ms" << endl;
}

void DeltaTimer::printTimeFromStart_us()
{
    cout << "DeltaTimer: timeFromStart = " << getTimeFromStart_us() << " us" << endl;
}


double FPS::getFPS()
{
    gettimeofday(&te, NULL);
    current_time_ms = ((double)(te.tv_sec))*1000.0 + ((double)(te.tv_usec))/1000.0;

    delta_time_ms = current_time_ms - previous_time_ms;
    previous_time_ms = current_time_ms;

    return 1000.0/delta_time_ms;
}
