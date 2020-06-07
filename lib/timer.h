#pragma once

#include "Windows.h"

namespace MyAscii {
    class Timer {
        public:
            Timer(void);

        public:
            void start(void);
            void stop(void);
            double get_elapsed_seconds(void);

        private:
            bool timerRunning = false;
            double startTime = 0.0;
            double stopTime = 0.0;
    };
}