#pragma once

#include <sys/time.h>
#include <stdlib.h>
#include <vector>

using namespace std;

template <class Type>
class Average
{
public:
    Average(int size)
    {
        mSize = size;
        elements.resize(size);
        for(unsigned int i = 0; i < elements.size(); i++)
        {
            elements.at(i) = 0;
        }
    }

    void putValue(Type curr_val)
    {
        Type prev_val  = elements.at(index);
        elements.at(index) = curr_val;
        sum = sum - prev_val + curr_val;
        if(++index == elements.size())
        {
            index = 0;
        }
    }

    Type getSrednia()
    {
        return sum/mSize;
    }
private:
    vector<Type> elements;
    Type sum = 0;
    Type mSize = 0;
    unsigned int index = 0;
};


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
