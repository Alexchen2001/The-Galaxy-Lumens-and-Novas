//Alexander Chen SUID: 4186272

/*
 *Design:The lumen class has constructs the lumen with various fields with only constants and mutatahle fields
 * When the glow fires, it takes reduces power and performs other functions, checks status and returns the value
 * designated to the condition, the status of lumen (isActive and isStable) will be checked in glow to adjust status.
 * Reset will only fire if conditions ar emet, otherwise brightness decreases. Recharge will only happen ifi the lumen
 * is stable.
 *
 */
#include "lumen.h"
#include <stdexcept>

/*
 *Class Invariants
 *size, brightness, power has to be above positive
 *glowRequests must be greater than or equal to 0
 *isStable can only be true(stable) or false(instable)
 *isActive can only be true(stable) or false(instable)
 * POWER_THRESHOLD is 0, no power, cannot go below 0
 * At least one glow request needed : MIN_GLOW_REQUEST
 * unStableCount must start from 0 and do not have negative value\\
 *
 * lumen has a maximum reset limit based on its size
 *
 * Supports arithmetic operator: +(standard type by type),-(standard type by type) lumen to lumen types only
 * Supoorts arithmetic operator: +(mix-mode int type only in format of lumen + int), -(mix-mode int type only in format of lumen - int)
 * Supports comparison oerpator: ==, !=, <, >, <=, >= if and only if both are lumen types
 * Supports shortcut assignment operator: +=, -= if and only if both are lumen types.
 *
 *  DO NOT SUPPORT (INT TYPE operator LUMEN TYPE): we are not manipulating any ints, we only deal with lumens
 *  DO NOT SUPPORT (LUMEN + NOVA type) lumen do not deal with nova
 * Any newly reassignemnet (+=, -=)of lumens will be seen as eratic lumens containing higher or lower than usual glow activity.
 */


    // constructor
    // Preconditions: inputBrightness, inputSize,inputPower has to be positive.
    // Postonditions: lumen has been created with given variable and set
    // data fields(STABLE_THRESHHOLD, MAX_BRIGHTNESS, etc.) through given variable.
    lumen::lumen(int inputBrightness,int inputSize, int inputPower)
        :brightness(inputBrightness),size(inputSize), power(inputPower),
        brightCopy(inputBrightness), powerCopy(inputPower),
        isActive(true), isStable(true),unStableCount(0)
    {
    if (inputBrightness <= 0 || inputSize <= 0 || inputPower <= 0) {
            throw std::invalid_argument("Negative Values and 0s are not allowed ");
        }

            STABLE_THRESHHOLD = (int)(power * 0.2);
            MAX_BRIGHTNESS = maxBrightnessCalc();
            dimness = dimCalc();
            MAX_RESET = resetMaxCalc();
    }

    // standarrd addition type + type
    // precondition: the type has to be nova to nova
    // Postcondition: returns a new lumen objects with combination specs of the 2
    //                lumen objects
    lumen lumen::operator+(const lumen& other) {
        int brightnessSum = this->brightness + other.brightness;
        int sizeSum = this->size + other.size;  
        int powerSum = this->power + other.power;
        
        // Return the new lumen object
        lumen result = lumen(brightnessSum, sizeSum, powerSum);
        return result;
    }

    //addition mix-mode (int type allowed only)
    //Precondition: only allows lumen + int types
    //Post condition: returns a new lumen object with the addition value
    lumen lumen::operator +(const int value){
        
        int brightnessSum = this->brightness + value;
        int sizeSum = this->size + value; 
        int powerSum = this->power + value;
        // Return the new lumen object
        lumen result = lumen(brightnessSum, sizeSum, powerSum);
        return result;
    }

    //standard subtraction type - type (lumen to lumen only)
    //Precondition: only can be lumen + lumen
    //Postcondition: return a lumen objects difference of lumen1 - lumen2, 
    // NOTE: the constructor will have exception to handle
    //      if the lumen reults negative or 0 value
    lumen lumen::operator-(const lumen& other){
        int brightnessDiff = this->brightness - other.brightness;
        int sizeDiff = this->size - other.size;
        int powerDiff = this->power - other.power;
        // Return the new lumen object
        lumen result = lumen(brightnessDiff, sizeDiff, powerDiff);
        return result;
    }


    //subtraction mix-mode (int type allowed only)
    //Precondition: only allows subtraction of lumen - int
    //Post condition: returns a new lumen object with the subtraction value
    lumen lumen::operator-(const int value){
        int brightnessDiff = this->brightness - value;
        int sizeDiff = this->size - value;
        int powerDiff = this->power - value;

        if (brightnessDiff|| sizeDiff || powerDiff <=0){
            throw std::invalid_argument("values exceeded allowed lumen spec range ");
        }

        // Return the new lumen object
        lumen result = lumen(brightnessDiff, sizeDiff, powerDiff);
        return result;
    }


    // shortcut assignment type += type addition
    //Precond: only allows lumen += lumen
    //Postcond: reassigns the lumen1(left) with the new specs.
    lumen& lumen::operator+=(const lumen& other) {
        int brightnessSum = this->brightness + other.brightness;
        int powerSum = this->power + other.power;

        this->brightness = brightnessSum;
        this->power = powerSum;

        return *this;
    }

    // shortcut assignment type -= type subtraction
    //Precond:only allows lumen -= lumen
    //Postcond: reassigns the lumen1(left) with the new specs.
    lumen& lumen::operator-=(const lumen& other){
        int brightnessSum = this->brightness - other.brightness;
        int powerSum = this->power - other.power;

        this->brightness = brightnessSum;
        this->power = powerSum;

        return *this;
    }

    // pre addition (increment)
    //Precond: none
    //Postcond: the lumen specs (except size) increments
    lumen& lumen::operator++() {
        brightness = brightness + 1;
        power = power + 1;
        return *this;
    }

    // post addition (increment)
    //Precond:none
    //Postcond: the lumen specs increments, but retruns the oldState
    lumen lumen::operator++(int){
        lumen oldState = *this;
        ++this->brightness;
        ++this->power ;
        return oldState;
    }


    // pre subtraction (decrement)
     //Precond: none
    //Postcond: the lumen specs (except size) decrements
    lumen& lumen::operator--() {
        brightness = brightness - 1;
        power = power - 1;
        return *this;
    }

    //post subtraction (decrement)
    //Precond:none
    //Postcond: the lumen specs increments, but retruns the oldState
    lumen lumen::operator--(int){
        lumen oldState = *this;
        --this->brightness;
        --this->power ;
        return oldState;
    }

    //Equality operator
    //Precond: only between lumen objects
    //Postcond: returns true if it is copletetly same spect otherwise false
    bool lumen::operator==(const lumen& other){
        bool sameBright = (this->brightness == other.brightness);
        bool sameSize = (this->size == other.size);
        bool samePower = (this->power == other.power);
        if(sameBright && sameSize && samePower){
            return true;
        }
        return false;
    }

    
    //Not Equal operator
    //Precond: only between lumen objects
    //Postcond: returns false if it is copletetly same spect otherwise true
    bool lumen::operator!=(const lumen& other){
        bool sameBright = (this->brightness == other.brightness);
        bool sameSize = (this->size == other.size);
        bool samePower = (this->power == other.power);
        if(sameBright && sameSize && samePower){
            return false;
        }
        return true;
    }


    //Greater than operator
    //Precond: only between lumen objects
    //Postcond: returns true if it one spec is smallerthan the other otherwise false
    bool lumen::operator<(const lumen& other){
        bool smallerBright = (this->brightness < other.brightness);
        bool smallerSize = (this->size < other.size);
        bool smallerPower = (this->power < other.power);
        if(smallerBright || smallerSize || smallerPower){
            return true;
        }
        return false;
    }

    //Greater than operator
    //Precond: only between lumen objects
    //Postcond: returns true if it is copletetly bigger spect than the other otherwise false
    bool lumen::operator>(const lumen& other){
        bool smallerBright = (this->brightness < other.brightness);
        bool smallerSize = (this->size < other.size);
        bool smallerPower = (this->power < other.power);
        if(smallerBright && smallerSize && smallerPower){
            return false;
        }
        return true;
    }


    //Greater than operator
    //Precond: only between lumen objects
    //Postcond: returns true if it is greater than or equal to  otherwise false
    bool lumen::operator>=(const lumen& other){
        bool result = (*this > other) || (*this == other);
        return result;
    }

    
    //lesser than operator
    //Precond: only between lumen objects
    //Postcond: returns true if it is both lesser than or equal to, otherwise false
    bool lumen::operator<=(const lumen& other){
        bool result = (*this < other) || (*this == other);
        return result;
    }



    //Precondition: None
    //Postcondition: returns the state of active(true) or inactive(false)
    bool lumen::isActiveGetter(){
        return isActive;
    }

    //Precondition: None
    //Postcondition: returns the state of stable(true) or instable(false)
    bool lumen::isStableGetter() {
        return isStable;
    }

    // Helper function to reinitiate field with original values
    //Precondition: None
    //Postcondition: sets the data fields with 'original' data values
    void lumen::restoreInitVal() {
        brightness = brightCopy;
        power = powerCopy;
        isActive = true;
        isStable = true;
        glowRequestCount = 0;
        dimness = MAX_BRIGHTNESS - brightness;
        unStableCount = 0;
    }

    //PreCondition:whether if the size is the minimal size (1)
    //PostCondition:MAX_RESET value has been initialized
    int lumen::resetMaxCalc(){

        int resetLimit = -1;
        if (size == 1){
            resetLimit = 1;
            return resetLimit;
        }else{
             resetLimit = size / 2;
             return resetLimit;
        }
        return resetLimit;
    }

    //Precondition: None
    //Postcondition: returns the eratic value with a random formula(unexpected)
    int lumen::eraticNum() {
        return (power * brightness) * (size + power);
    }

    //Precondition: power is 0 or lower
    //Postcondition: sets the isActive to false.
    void lumen::isActiveStateCheck() {
        if (power <= POWER_THRESHHOLD) {
            isActive = false;
        }
    }

    //Precondition: power has to be greater than 0 and power has to be greater than 20% of the initial power
    //Postcondition: returns true if above condition satisfy otherwise its false
    void lumen::isStableStateCheck() {
        if ((power > POWER_THRESHHOLD) && (power > STABLE_THRESHHOLD)) {
            isStable = true;
        }
        else {
            isStable = false;
            unStableCount++;
        }
    }

    //Precondition:None
    //Postondition:returns the number of instable encounters detected in a lumen object
    int lumen::unStableGetter()  {
        return unStableCount;
    }

    //Precondition: None
    //Postcondition: returns the max brightness through calculation (self-created formula)
    int lumen::maxBrightnessCalc() {
        return size * brightness;;
    }

    //Precondition: None
    //Postcondition: returns the dimness(anti-bightness)
    int lumen::dimCalc() {
        return MAX_BRIGHTNESS - brightness;;
    }

    //Precondition: None
    //Postcondition: sets the dimness value when brightness is manipulated
    void lumen::dimnessRefresh(){
        dimness = dimCalc();
    }

    //Precondition: None
    //Postcondition: returns the value based on the isActive and isStable states.
    int lumen::glow() {
        power--;
        glowRequestCount++;
        isActiveStateCheck();
        isStableStateCheck();
        dimnessRefresh();

        if (isActive == false) {
            return dimness;
        }

        if ((isActive == true) && (isStable == false)) {
            return eraticNum();
        }

        return brightness * size;
    }


    //Precondition: echkes whether if it has reached max limit of reset.
    //Postcondition: restores the lumen object to original values if condition are met, otherwise the brightness decreases
    void lumen::reset() {
        if(resetCount >= MAX_RESET){
            throw std::invalid_argument("Reached Max Reset limit, NO MORE RESET ALLOWED!");
        }
        if (glowRequestCount >= MIN_GLOW_REQUEST && power >= POWER_THRESHHOLD) {
            restoreInitVal();
        }

        brightness--;
        dimnessRefresh();

        resetCount++;
    }

    //Precondition: the lumen object is Stable (isStable = true)
    //Postcondition: the power of the lumen object recharges 10%,
    void lumen::recharge(){
        if (isStable == true){
            power = power + (powerCopy/10);
        }
    }

/*
 * Implementation Invariant
 *Glow() always returns a value depending on the state
 *Reset() always has to reset the lumen object to original state or decrease brightness
 *Recharge() recharges the lumen object by 10% everytime if and only if the object is stable
 *dimCalc() calculates the dimness according to Max_brightness and current brightness
 *isActiveStateCheck() changes to inActive if the power hits 0 (no power)
 *isStableStateCheck() changes to inStable if that the power is below 0 and below 20% of the original power.
 * reStoreInitial() sets all fields needed back to original value
 *eracticNum() returns a value from a random formula that uses brightness, size and power
 *there is a reset limit for every lumen objects
 */

