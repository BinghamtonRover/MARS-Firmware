#ifndef compass_h
#define compass_h
#include <Arduino.h>
#define X_MAX 2015
#include <QMC5883LCompass.h>

class Compass {
    private:
        int16_t startx = 0;
        int16_t starty = 0;
        int16_t startz = 0;
        QMC5883LCompass compass;
    public:
        int16_t getX();
        int16_t getY();
        int16_t getZ();
        void init();

};

#endif
