//Alexander Chen SUID:4186272
/*
 * Design: Nova class creates a list of lumen objects, with restrictions on the specs.
 * the glow of nova class triggers the glow of lumens in it, and chckes on the max/min glow,
 * along with whether if it needs to internal charge. where internal charge will check if half
 * of the lumens are inActive to recharge, where the internal charge will also look if replacement of
 * lumen is needed because that its persistenly unstable. The max/min glows will look for overall max and min glow
 * from all the possible glow of lumen.
 *
 */

#include "nova.h"



    /*
     * Class invariants
     * the given brightVal, sizeVal, and powerVal, and count annot be 0 or negative
     * the power has to be greater than the amount of lumen objects
     * the number of glow wanted cannot be a negative value neither can it exceed
     * the amount of lumen objects in the nova object
     *
     * Supports copy and move semantics
     * Supports arithmetic operator: +(standard type by type),-(standard type by type) nova to nova types only
     * Supports comparison oerpator: ==, !=, <, >, <=, >= if and only if both are nova types
     * Supports shortcut assignment operator: +=, -= if and only if both are nova types.
     * Supports mix-mode operator: +(nova + lumens), -(nova -lumens) it will only be in this format
     *
     * COMPARISON Operator: it has to be in the same pattern (sequence of luments specs) to be considered the same
     */

    //Precondition:amount of lumens cannot be 0 or below.
    //Postcondition:nova object created, assigned an array of count lumen subobjects,
    // taken in through dependency injection.
    //Construtor (constructor dependency injection).
    nova::nova(const lumen* const* lumenObjects, int count){

        if (count <=0) {
            throw std::invalid_argument("Empty array of lumens is not allowed ");
        }

        lumens = new lumen*[count];
        MaxGlow = -1;
        MinGlow = -1;
        lumenAmt = count;
        for (int i = 0; i < lumenAmt; i++) {
            lumens[i] = const_cast<lumen*>(lumenObjects[i]);
        }

    }


    //standarrd addition type + type NOVA to NOVA ONLY
    //Pre condition:both are nova type
    //Post condition: returns a new nova obejct (combination of the 2 nova object)
    nova nova::operator+(const nova& other) {
        int lumenAmtSum = this ->lumenAmt + other.lumenAmt;
        int limiter = this->lumenAmt;
        lumen** newlumens = new lumen*[lumenAmtSum];
        for (int i = 0; i < lumenAmtSum; i++) {
            if(i < limiter){
                newlumens[i] = this->lumens[i];
            }else{
                newlumens[i] = other.lumens[i - limiter];
            }
        }
        nova result = nova(newlumens, lumenAmtSum);
        return result;
    }

    // mix-mode addition (only supports nova to lumen ONLY)
    nova nova::operator+(const lumen& other){
        int lumenAmtSum = this ->lumenAmt + 1;
        int limiter = this->lumenAmt;
        lumen** newlumens = new lumen*[lumenAmtSum];
        for (int i = 0; i < lumenAmtSum; i++) {
            if(i <= limiter){
                newlumens[i] = this->lumens[i];
            }else{
                newlumens[i] = new lumen(other);
            }
        }
        nova result = nova(newlumens, lumenAmtSum);
        return result;
    }

    //standard subtraction type - type nova with nova ONLY
    // Pre condition: checks whether if nova1 is larger than nova2 (nova1 has to be larger than nova 2)
    // Post condition: if diff is > 0 then provide new decreased amount nova, if not it
    //                 would return an empty nova which would trigger constructor error
    nova nova::operator-(const nova& other){
        int lumenAmtDiff = this ->lumenAmt - other.lumenAmt;
        if(lumenAmtDiff <= 0){
            lumenAmtDiff = 1;
        }

        lumen** newlumens = new lumen*[lumenAmtDiff];
        for (int i = 0; i < lumenAmtDiff; i++) {
            newlumens[i] = this->lumens[i];
        }
        nova result = nova(newlumens, lumenAmtDiff);
        return result;
    }

    //mix mode subtraction nova - lumen ONLY
    // Pre condition: checks whether if nova1 is larger than nova2 (nova1 has to be larger than nova 2)
    // Post condition: if diff is > 0 then provide new decreased amount nova, if not it
    //                 would return an empty nova which would trigger constructor error
    nova nova::operator-(const lumen& other){
        int lumenAmtDiff =  this ->lumenAmt - 1;
        bool found = false;
        int  foundPos;
        lumen** newlumens = new lumen*[lumenAmtDiff];

        for (int i = 0; i < lumenAmtDiff; i++) {
            found = (this->lumens[i] == new lumen(other));
            if(found){
                foundPos = i;
                break;
            }
        }
        if (found){
            for (int i = 0; i < lumenAmtDiff; i++) {
                if (i < foundPos){
                    newlumens[i] = this->lumens[i];
                }else{
                    newlumens[i] = this->lumens[i+1];
                }
            }
            nova result = nova(this->lumens, this->lumenAmt);
            return result;
        }
        nova result = nova(this->lumens, this->lumenAmt);
        return result;
    }

    // shortcut assignment addition type += type
    //precond: it has to be both nova objects
    //postond:returns the nova objects that had incremented from the other nova
    nova& nova::operator+=(const nova& other) {
        int lumenAmtSum = this ->lumenAmt + other.lumenAmt;
        int limiter = this->lumenAmt;
        lumen** newlumens = new lumen*[lumenAmtSum];
        for (int i = 0; i < lumenAmtSum; i++) {
            if(i == limiter){
                newlumens[i] = this->lumens[i];
            }else{
                newlumens[i] = other.lumens[i - limiter];
            }
        }
        this->lumens = newlumens;
        this->lumenAmt = lumenAmtSum;

        return *this;
    }

    // shortcut assignment subtraction type- type
    // Pre condition: checks whether if nova1 is larger than nova2 (nova1 has to be larger than nova 2)
    // Post condition: if diff is > 0 then provide assign new decreased amount nova to nova1(left), if not it
    //                 would return an empty nova which would trigger constructor error
    nova& nova::operator-=(const nova& other){
        int lumenAmtDiff = this ->lumenAmt - other.lumenAmt;

        if(lumenAmtDiff <= 0){
            throw std::invalid_argument("Invalid nova subtraction: Nova1 needs to be larger than Nova2 ");
        }

        lumen** newlumens = new lumen*[lumenAmtDiff];
        for (int i = 0; i < lumenAmtDiff; i++) {
            newlumens[i] = this->lumens[i];
        }

        this->lumens = newlumens;
        this->lumenAmt = lumenAmtDiff;
        return *this;
    }

    
    //pre increment
    //Precond:N/A
    //Postcond: this nova object will have an extra lumen
    nova& nova::operator++(){
        // In this case, we will just add one default Lumen to the current Nova.
        int newLumenAmt = this->lumenAmt + 1;
        lumen** newLumens = new lumen*[newLumenAmt];
        
        // Copy contents of the current Nova.
        for (int i = 0; i < this->lumenAmt; i++) {
            newLumens[i] = this->lumens[i];
        }
        
        // Add new Lumen.
        newLumens[this->lumenAmt] = new lumen(1, this->lumenAmt, 1); 
        
        // Clean up and update properties.
        delete[] this->lumens;
        this->lumens = newLumens;
        this->lumenAmt = newLumenAmt;
        
        return *this;
    } 

    // post increment
    // Precondition: none
    // Postcondition: returns a copy of the original 'this' instance and modifies 'this' instance to include one new default lumen
    nova nova::operator++(int){
        nova oldState = *this;
        ++*this;
        return oldState;
    } 

    //pre-decrement
    // Precondition: the nova object has at minimal one lumen
    // Postcondition: one lumens will be removed from the nova object
    nova& nova::operator--(){
        if (lumenAmt <= 0) {
            throw std::runtime_error("no lumens in the Nova object.");
        }
        
        lumen** newLumens = new lumen*[lumenAmt - 1];

        // Copy all but the last Lumen.
        for (int i = 0; i < lumenAmt - 1; i++) {
            newLumens[i] = lumens[i];
        }

        // Delete the last Lumen and free its memory.
        delete lumens[lumenAmt - 1];
        
        // Free the old array and update the lumens pointer.
        delete[] lumens;
        lumens = newLumens;

        // Decrement the lumen count.
        --lumenAmt;

        return *this;

    }

    //post decrement
    // pre: none
    // post: decrements the nova object and returns the oldState of nova
    nova nova::operator--(int)
    {
        nova old(*this);
        --*this;
        return old;
    }


    //Equality operator
    //precond:has to be 2 nova object
    //Postcond: returns true if all the lumens(sequence matter) are the same and has the same amount of lumens
    //          otherwise false
    bool nova::operator==(const nova& other){
        bool isSame = true;
        if(this->lumenAmt != other.lumenAmt){
            return false;
        }
        for(int i = 0;i < this->lumenAmt;i++){
            if(!isSame){
                return false;
            }
            isSame = (this->lumens[i] == other.lumens[i]);
        }
        return true;

    }


    //Not Equality operator
    //precond:has to be 2 nova object
    //Postcond: returns false if all the lumens(sequence matter) are the same and has the same amount of lumens
    //          otherwise true
    bool nova::operator!=(const nova& other){
        bool isSame = false;
        int  matchCounter = 0;
        if(this->lumenAmt != other.lumenAmt){
            return true;
        }
        for(int i = 0;i < this->lumenAmt;i++){
            if(isSame){
                matchCounter++;
            }
            isSame = (this->lumens[i] == other.lumens[i]);
        }
        if(matchCounter == this->lumenAmt){
            return false;
        }
        return true;
    }


    //Lesser than operator
    //precond:has to be 2 nova object
    //Postcond: returns true if the lumen amount is smaller or half of the lumens are smaller
    //          otherwise false
    bool nova::operator<(const nova& other){
        bool isSmaller = false;
        int smallerCount = 0;

        if(this->lumenAmt < other.lumenAmt){
            return true;
        }

        for(int i = 0;i < this->lumenAmt;i++){
            if(isSmaller){
                smallerCount++;
            }
            isSmaller = (this->lumens[i] < other.lumens[i]);
        }

        if(smallerCount > this->lumenAmt / 2){
            return true;
        }
        return false;

    }


    //Greater than operator
    //precond:has to be 2 nova object
    //Postcond: returns false if the lumen amount is smaller or half of the lumens are smaller
    //          otherwise true
    bool nova::operator>(const nova& other){
        bool isSmaller = false;
        int smallerCount = 0;

        if(this->lumenAmt < other.lumenAmt){
            return false;
        }

        for(int i = 0;i < this->lumenAmt;i++){
            if(isSmaller){
                smallerCount++;
            }
            isSmaller = (this->lumens[i] < other.lumens[i]);
        }

        if(smallerCount > this->lumenAmt / 2){
            return false;
        }
        return true;

    }

    // greaterthan equal to operator
    // precond: has to be two nova objects
    // postond: returns true if it is greater than or equal to the other nova
    bool nova::operator>=(const nova& other){
        bool result = (*this > other) || (*this == other);
        return result;
    }

    // lesser than equal to operator
    // precond: has to be two nova objects
    // postond: returns true if it is less than or equal to the  othernova
    bool nova::operator<=(const nova& other){
        bool result = (*this < other) || (*this == other);
        return result;
    }


    // Pre-condition: the passed in object has to be valid
    // Post-condition: the returned objects has the same specs as the source object
    // Deep Copy, pass by value
    nova::nova(const nova& other)
    {
        // Allocate memory for lumens array
        this->lumenAmt = other.lumenAmt;
        lumens = new lumen*[lumenAmt];

        // copies objects from other to desired object
        for (int i = 0; i < lumenAmt; i++)
        {
            lumens[i] = new lumen(*other.lumens[i]);
        }
    }



    // Pre-condition: the other is a valid move object
    // Post-condition:lumen objects and amount of lumen objects
    // from other to the desired object.
    // Overloaded assignment operator, Pass by assignment Deep copy
    nova& nova::operator=(const nova& other)
    {
        // checks for self-assigning
        if (this == &other){
            return *this;
        }

        //Deallocates memory
        for (int i = 0; i < lumenAmt; i++){
            delete lumens[i];
        }
        delete[] lumens;

        // Copies from the other objects
        this->lumenAmt = other.lumenAmt;
        lumens = new lumen*[lumenAmt];
        for (int i = 0; i < lumenAmt; i++)
        {
            lumens[i] = new lumen(*other.lumens[i]);
        }

        return *this;
    }


    // Pre-condition: the other has to be valid (object)
    // Post-condition: creates a new object that has the same specs as
    //the other object.
    //Move, pass by value
    nova::nova(nova&& other){
        this->lumenAmt = other.lumenAmt;
        this->lumens = other.lumens;
        other.lumenAmt = 0;
        other.lumens = nullptr;
    }

    // Move pass by assignment
    // Pre-condition: parameter objects that is passed in is valid
    // Post-condition: moves the other object to desired object
    nova& nova::operator=(nova&& other){

        // checking for self-assigning
        if (this == &other){
            return *this;
        }
        // Move from other object
        this->lumenAmt = other.lumenAmt;
        this->lumens = other.lumens;
        other.lumenAmt = 0;
        other.lumens = nullptr;

        return *this;
    }


    //Precondition: the number of objects cannot be negative or
    //exceed the contained lumen objects in nova
    //Postcondition: glows the number of lumen objects in nova
    void nova::glow(int numOfGlow){
            if(numOfGlow< 0 || numOfGlow > lumenAmt){
                throw std::invalid_argument("Invalid Amount entered");
            }

            int glowVal;
            for(int j = 0; j < numOfGlow; j++){
                glowVal = lumens[j]->glow();
                MinGlowCalculator(glowVal);
                MaxGlowCalculator(glowVal);
            }
            internRecharge();
        }

    //Precondition:None
    //Postondition: assigns the min value,min is not intialized (-1) then initiate current value,
    // if the current min is bigger, assign the smaller value to min.
    void nova::MinGlowCalculator(int givenGlowVal) {
        if(MinGlowGetter() < 0) {
            MinGlow = givenGlowVal;
        }
        if(MinGlowGetter() > givenGlowVal){
            MinGlow = givenGlowVal;
        }
    }

    //Precondition:None
    //Postondition:assigns the max value, if is not initiated(-1), it
    // assigns current value. If it is smaller than the given value,
    // assigns that value as max.
    void nova::MaxGlowCalculator(int givenGlowVal){
        if(MaxGlowGetter() < 0) {
            MaxGlow = givenGlowVal;
        }
        if(MaxGlowGetter() < givenGlowVal){
            MaxGlow = givenGlowVal;
        }

    }

    //Precondition:None
    //Postcondition:returns min glow of all glow across all lumen in a nova
    int nova::MinGlowGetter() {
        return MinGlow;
    }

    //Precondition:None
    //Postcondition:returns max glow of all glow across all lumen.
    int nova::MaxGlowGetter(){
        return MaxGlow;
    }

    int nova::lumenAmtGetter() {
        return lumenAmt;
    }

    //Precondition: If half is inactive or if it is persistenly unstable
    //Postcondition:may recharge the objects, may replace the objects
    void nova::internRecharge(){
        int inactiveLumen = 0;
        for(int k = 0; k < lumenAmt;  k++){
            bool isActiveStatus = lumens[k]->isActiveGetter();
            bool replaceReq = lumens[k]->unStableGetter() > 2;
            if(isActiveStatus == false){
                inactiveLumen++;
            }
            if(replaceReq){
                lumens[k]->reset();
            }
        }
        if (inactiveLumen > (lumenAmt/2)){
            for(int i = 0; i < lumenAmt; i ++){
                lumens[i]->recharge();
            }
        }
    }


    //Precondition: None
    //Postcondition: deallocates memory
    // Destructor
    nova::~nova() {
        for (int i = 0; i < lumenAmt; i++) {
            delete lumens[i];
        }
        delete[] lumens;
    }


    /*
     * Implementation invariants
     *internCharge() recharges the lumens if half of it is inactive,
     * also it replaces the lumens if it is persistenly unstable
     *
     *MinGlowCalculator() constantly finds the value MinGlow depending on the condition for the whole nova glow.
     *MinGlowCalculator() constantly finds the value MaxGlow depending on the condition for the whole nova glow.
     * glow() it triggers glow for the needed amount of element to glow from the array.Cannot exceed the lumen amount.
     * Deep copy constructors: copies the object, needs the object to exist
     * Move constructors: moves the references (swap), needs the object to exist.
     */