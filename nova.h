

#ifndef P2_NOVA_H
#define P2_NOVA_H

#include "lumen.h"
#include <stdexcept>
#include <cstdlib>


class nova {

private:
    lumen** lumens;
    int MinGlow;
    int MaxGlow;
    int lumenAmt;

public:
    nova(const lumen* const* lumenObjects, int count);
    void glow(int numOfGlow);
    int MinGlowGetter();
    int MaxGlowGetter();
    int lumenAmtGetter();
    nova& operator=(const nova& other);
    nova(nova&& other);
    nova& operator=(nova&& other);
    nova(const nova& other);
    ~nova();

private:
   void MinGlowCalculator(int givenGlowVal);
   void MaxGlowCalculator(int givenGlowVal);
   void internRecharge();

public:
    nova operator+(const nova& other);
    nova operator+(const lumen& other);
    nova operator-(const nova& other);
    nova operator-(const lumen& other);
    nova& operator+=(const nova& other);
    nova& operator-=(const nova& other);
    nova& operator++();
    nova operator++(int);
    nova& operator--();
    nova operator--(int);

    bool operator==(const nova& other);
    bool operator!=(const nova& other);
    bool operator<(const nova& other);
    bool operator>(const nova& other);
    bool operator>=(const nova& other);
    bool operator<=(const nova& other);
};


#endif //P2_NOVA_H
