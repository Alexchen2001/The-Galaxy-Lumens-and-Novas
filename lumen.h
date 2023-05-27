
#ifndef P2_LUMEN_H
#define P2_LUMEN_H


class lumen {

private:
    // constants and private class variables
    const int MIN_GLOW_REQUEST = 1;
    const int POWER_THRESHHOLD = 0;
    int MAX_RESET;
    int MAX_BRIGHTNESS;
    int STABLE_THRESHHOLD;
    const int brightCopy;
    const int powerCopy;
    int brightness;
    const int size;
    int power;
    int dimness;
    bool isActive;
    bool isStable;
    int glowRequestCount;
    int unStableCount;
    int resetCount;

public:
    lumen(int inputBrightness, int inputSize, int inputPower);
    bool isActiveGetter();
    bool isStableGetter();
    int maxBrightnessCalc();
    int dimCalc();
    int glow();
    void reset();
    void recharge();
    int unStableGetter();

private:
    void restoreInitVal();
    int eraticNum();
    void isActiveStateCheck();
    void isStableStateCheck();
    void dimnessRefresh();
    int resetMaxCalc();

public:
    lumen operator+(const lumen& other);
    lumen operator+(const int value);
    lumen operator-(const lumen& other);
    lumen operator-(const int value);
    lumen& operator+=(const lumen& other);
    lumen& operator-=(const lumen& other);
    lumen& operator++();
    lumen operator++(int);
    lumen& operator--();
    lumen operator--(int);

    bool operator==(const lumen& other);
    bool operator!=(const lumen& other);
    bool operator<(const lumen& other);
    bool operator>(const lumen& other);
    bool operator>=(const lumen& other);
    bool operator<=(const lumen& other);
};



#endif //P2_LUMEN_H
