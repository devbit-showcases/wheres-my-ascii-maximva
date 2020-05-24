#include "timer.h"

namespace MyAscii {

    Timer::Timer(void) {
        
    }

    void Timer::start(void) {
        timerRunning = true;
        startTime = GetTickCount();
    }

    void Timer::stop(void) {
        stopTime = GetTickCount();
        timerRunning = false;
    }

    double Timer::get_elapsed_seconds(void) {
        stopTime = (timerRunning ? GetTickCount() : stopTime);
        return (stopTime - startTime) / 1000;
    }

}