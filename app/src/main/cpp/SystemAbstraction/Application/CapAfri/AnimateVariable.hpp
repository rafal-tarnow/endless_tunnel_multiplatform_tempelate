#pragma once

#include <iostream>

using namespace std;

//void valueFloat_A(float & a)
//{
//    //cout << "valueFloat(float & a) = " << a << endl;
//}

//void valueFloat(float a)
//{
//    //cout << "valueFloat(float a) = " << a << endl;
//}




template <class T>
class Animated
{
    public:
    Animated(float a)
    {
        //cout << "Dziala Animated(float a)" << endl;
        start_value = a;
        current_value = a;
    }

    void print()
    {
        // cout << "***************************" << endl;
        // cout << "start_value = " << start_value << endl;
        // cout << "current_value = " << current_value << endl;
        // cout << "stop_value = " << stop_value << endl;
        // cout << "------------" << endl;
        // cout << "shoud_run = " << shoud_run << endl;
        // cout << "run = " << run << endl;
        // cout << "------------" << endl;
        // cout << "start_time = " << start_time << endl;
        // cout << "current_time = " << current_time << endl;
        // cout << "normalized_time = " << normalized_time << endl;

    }
    T start_value = 0;
    T current_value = 0;
    T stop_value = 0;

    T b = 0;
    T a = 0;

    bool shoud_run = false;
    bool run = false;

    float start_time = 0;
    float current_time = 0;
    float normalized_time = 0;

    void update(float t)
    {
        current_time = t;

        if(!run)
        {
            if(shoud_run)
            {
                run = true;
                start_time = current_time;
                normalized_time = 0;
                a = stop_value - start_value;
                b = start_value;
            }
        }
        else
        {
            normalized_time = 10.0f*(current_time - start_time);
            current_value = a*normalized_time + b;// linear function

            if(normalized_time > 1.0f)
            {
                run = false;
                shoud_run = false;
                normalized_time = 1.0f;
                current_value = stop_value;
                start_value = stop_value;
            }
        }
    }

    Animated<T>& operator = (const Animated<T> & graphe);
    Animated<T>& operator = (const float & value);
    operator float() const;
};

template <class T>
Animated<T>::operator float() const
{
    //cout << "Dziala Animated<T>::operator float() const" << endl;
    return this->current_value;
}



template <class T>
Animated<T>& Animated<T>::operator = (const Animated<T> & b)
{
    //cout << "Dziala Animated<T>& Animated<T>::operator = (const Animated<T> & b)" << endl;
    if(this != &b)
    {
        this->start_value =  b.start_value;
        this->current_value = b.current_value;
        this->stop_value = b.stop_value;
    }
    return *this;
}

template <class T>
Animated<T>& Animated<T>::operator = (const float & b)
{
    //cout << "Dziala Animated<T>& Animated<T>::operator = (const float & b)" << endl;

    if(run)
    {
        this->stop_value =  b;
        this->start_value = this->current_value;
        shoud_run = true;
        run = false;
    }
    else
    {
        this->stop_value = b;
        shoud_run  = true;
    }

    return *this;
}





//int main()
//{
//    //cout<<"Hello World" << endl;

//    Animated<float> a = 1.23f;
//    a.print();

//    a = 2.34f;

//    float T = 10.0f;
//    a.update(T);

//    //cout << "For start" << endl;
//    for(unsigned int i = 0; i < 12; i++)
//    {
//        T += 0.122f;
//        a.update(T);
//        a.print();
//    }

//    a = -2.0f;
//       // cout << "For second start" << endl;
//    for(unsigned int i = 0; i < 24; i++)
//    {
//        T += 0.122f;
//        a.update(T);
//        a.print();
//        if(i == 6)
//        {
//            a = 5;
//        }
//    }

//    cout << "EXIT OK" << endl;
//    cout << flush;

//    return 0;
//}
