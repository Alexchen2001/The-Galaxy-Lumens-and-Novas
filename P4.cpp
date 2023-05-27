// Alexander Chen SUID: 4186272
// 5/19/2023
//Platform: Mac, Clion, C++17

// Assumptions of program:
// This programcreates a nova class that will be used to create a nova object.
// The nova object will injected a number of lumens. The program creates 3 nova objects 
// and will display the MIN/MAX GLOW of each nova object.

// Assumptions of driver:
// The random number generator is seeded using the current time, assuming that the
// program will not be surpass more than once per second. THere are some exception circumstances.
// The program assumes that the number of nova objects and the properties of the
// lumen objects will only be int (range).

//Description
//This program creates Nova ojects containing a collectio of Lumen objects.
//Generation of Nova objects, each with a random number of Lumen objects.
//Execution of operations on Nova objects, such as glowing, making all Lumen objects glow, replacing unstable Lumen objects, and displaying the minimum and maximum glow.
//Execution of operations with overloaded operators on Nova objects, including +, -, ++, --, and +=.

//User Interface
//The program runs in a console interface. There is no user input during the execution of the program.
//The output includes the results of operations on the Nova objects such as the number of Lumen objects in each Nova, the minimum and maximum glow, and the results of the overloaded operator operations.

//Use and Validity
//The program uses unique pointeres which handles memory management
//Error handeling has been implemented for various functions including operator overloading and class functionality.


//Assumptions
//The parameters for the Lumen constructor are valid. For example, they are within the alloted range
//The Nova class provides the necessary member functions like lumenAmtGetter, minGlowGetter, maxGlowGetter, glowAllActive,etc.
//The number of Lumen objects should not exceed a Nova object maximum limit.
//
//NOTE ASSUMPTION OF FUNCTIONS: 
// NOVA:
// The below are assumptions that what it supports and all clients should follow the rules (format)
// Certain vice versas of format are not allowed Ex: nova to int != int to nova
// * Supports copy and move semantics
// *Supports arithmetic operator: +(standard type by type),-(standard type by type) nova to nova types only
// * Supports comparison oerpator: ==, !=, <, >, <=, >= if and only if both are nova types
// * Supports shortcut assignment operator: +=, -= if and only if both are nova types.
// * COMPARISON Operator: it has to be in the same pattern (sequence of luments specs) to be considered the same
//     
//Lumens:
//lumen has a maximum reset limit, it will no longer be unlimited reset.
//upports arithmetic operator: +(standard type by type),-(standard type by type) lumen to lumen types only
//  * Supoorts arithmetic operator: +(mix-mode int type only in format of lumen + int), -(mix-mode int type only in format of lumen - int)
//  * Supports comparison oerpator: ==, !=, <, >, <=, >= if and only if both are lumen types
//  * Supports shortcut assignment operator: +=, -= if and only if both are lumen types.
//  *
//  *  DO NOT SUPPORT (INT TYPE operator LUMEN TYPE): we are not manipulating any ints, we only deal with lumens
//  *  DO NOT SUPPORT (LUMEN + NOVA type) lumen do not deal with nova
//  * Any newly reassignemnet (+=, -=)of lumens will be seen as eratic lumens containing higher or lower than usual glow activity.
//  
//Assume all inputs are valid.


#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "nova.h"

using namespace std;

        int randomInt() {
            return rand() % 60 + 11;
        }

        int randomIntB(int max){
            return rand()% max + 7;
        }

        //helper fucntion for printing the Glow Value
        void printGlow(vector<unique_ptr<nova>>& novaVec) {
            int count = 1;
            for (auto& n : novaVec) {
                cout << "Nova " << count << " ";
                cout << "\tMin glow: " << n->MinGlowGetter() << endl;
                cout << "\tMax glow: " << n->MaxGlowGetter() << endl;
                cout << endl;
                count++;
            }
        }

        // As long as every cout lines are printed it means the overload operator
        // is working in the designated functionality
        void testOperatorOverloads(vector<unique_ptr<nova>>& novaVec) {
            cout << " -- Testing operator overloads... --\n" << endl;
            int brightness = randomInt();
            int size = randomInt();
            int power = randomInt();
            const lumen *lumenExt = new lumen(brightness, size, power);

            // standard addition and subtraction
            nova result = *novaVec[0] + *novaVec[1];
            cout << " Standard addition of nova objects: Success.\n";

            result = *novaVec[0] - *novaVec[1];
            cout << " Standard subtraction of two nova objects: Success.\n";

            // mixed-mode addition and subtraction
            result = *novaVec[0] + *lumenExt;
            cout << " Mixed-mode addition of Nova and lumen:Success.\n";


            result = *novaVec[0] - *lumenExt;
            cout << " Mixed-mode subtraction of a lumen from a nova object: Success.\n";

            // shortcut assignment
            *novaVec[0] += *novaVec[1];
            cout << " Shortcut assignment (addition) between two nova objects: Success.\n";

            *novaVec[0] -= *novaVec[1];
            cout << " Shortcut assignment (subtraction) between nova objects: Success.\n";
            
            // prefix increment and decrement
            ++(*novaVec[0]);
            cout << "increment of a nova object: Success.\n";

            --(*novaVec[0]);
            cout << "decrement of a nova object: Success.\n";

            // comparison operators
            bool equal = (*novaVec[0] == *novaVec[1]);
            cout << " Equality check between two nova objects successful.\n";

            bool notEqual = (*novaVec[0] != *novaVec[1]);
            cout << " Inequality check between two nova objects successful.\n";

            bool lessThan = (*novaVec[0] < *novaVec[1]);
            cout << " Less than check between two nova objects successful.\n";

            bool greaterThan = (*novaVec[0] > *novaVec[1]);
            cout << " Greater than check between two nova objects successful.\n";

            bool lessEqual = (*novaVec[0] <= *novaVec[1]);
            cout << " Less than or equal to check between two nova objects successful.\n";

            bool greaterEqual = (*novaVec[0] >= *novaVec[1]);
            cout << " Greateror equal to check between nova objects: Success .\n";

            cout << "\n Operator overloads test completed.\n" << endl;
        }

        void testGlow(vector<unique_ptr<nova>>& novaVec) {
            cout << "###Testing glow###\n" << endl;

            cout << " Before glow: " << endl;
            printGlow(novaVec);

            for (auto& n : novaVec) {
                  n->glow(1);
            }
           
            cout << " After glow: " << endl;
            printGlow(novaVec);

            cout << " -----Glow test complete ----\n" << endl;
        }

        
        void testCopy(vector<unique_ptr<nova>>& novaVec) {
            cout << " ##Testing copy## --\n" << endl;

            nova novaCopy = *novaVec[0];

            cout << "nova Copy glow values: " << endl;
            cout << "\tMin glow: " << novaCopy.MinGlowGetter() << endl;
            cout << "\tMax glow: " << novaCopy.MaxGlowGetter() << endl;

            cout << "nova Original glow values: " << endl;
            cout << "\tMin glow: " << novaVec[0]->MinGlowGetter() << endl;
            cout << "\tMax glow: " << novaVec[0]->MaxGlowGetter() << endl;

            cout << "\n ----Copy test completed------n" << endl;
        }

        

        void testAddRemove(vector<unique_ptr<nova>>& novaVec,lumen** lumenFactory,int lumenQty) {
            cout << "###Testing add and remove heap-allocated nova objects### \n" << endl;

            novaVec.pop_back();
            novaVec.pop_back();

            cout << " Glow value of current objects in the container after removing 2 objects:" << endl;
            printGlow(novaVec);

            novaVec.push_back(make_unique<nova>(lumenFactory,lumenQty ));

            cout << " Glow value of current objects in the container after adding an object:" << endl;
            printGlow(novaVec);

            cout << " Add and remove test completed\n" << endl;
        }

        void testSharedPtr(vector<unique_ptr<nova>>& novaVec) {
            cout << " ### Testing shared_ptr ###" << endl;
            cout <<endl;

            //last nova in the vector are used for share pointer
            shared_ptr<nova> sharedNova(novaVec.back().release());
            shared_ptr<nova> sharedNovaCopy1 = sharedNova;
            shared_ptr<nova> sharedNovaCopy2 = sharedNova;

            // checks shared pointer use count to show multiple references to the object.
            cout << " sharedNova      use count: " << sharedNova.use_count() << "\n";
            cout << " sharedNovaCopy1 use count: " << sharedNovaCopy1.use_count() << "\n";
            cout << " sharedNovaCopy2 use count: " << sharedNovaCopy2.use_count() << "\n";

            // Resetting share pointer
            sharedNova.reset();
            cout << "\nAfter reset: " << endl;
            cout << " sharedNova      use count: " << sharedNova.use_count() << "\n";
            cout << " sharedNovaCopy1 use count: " << sharedNovaCopy1.use_count() << "\n";
            cout << " sharedNovaCopy2 use count: " << sharedNovaCopy2.use_count() << "\n";

            cout << "\n -----shared_ptr test complete ----\n" << endl;
        }

        

        int main() {

            srand(time(NULL));



            vector<unique_ptr<nova>> novaVec0;
            vector<unique_ptr<nova>> novaVec1;
            int num_nova = 3;

            int brightnessA = randomInt();
            int sizeA = randomInt();
            int powerA = randomInt();
            int numLumenA = randomIntB(10);
            lumen** lumenFac = new lumen*[numLumenA];
            for(int i = 0; i < numLumenA; i++){
                lumenFac[i] = new lumen(brightnessA,sizeA, powerA);
                
            }

            for(int i = 0; i < num_nova; ++i) {
                int brightness = randomInt();
                int size = randomInt();
                int power = randomInt();
                int numLumen = randomIntB(117);
                lumen** lumenList = new lumen*[numLumen];
                for(int j = 0; j < numLumen; j++){
                    lumenList[j] = new lumen(brightness,size,power);
                }
                novaVec0.push_back(make_unique<nova>(lumenList, numLumen));
            }
            
            for(int i = 0; i < num_nova; ++i) {
                int brightness = randomInt();
                int size = randomInt();
                int power = randomInt();
                int numLumen = randomIntB(117);
                lumen** lumenList = new lumen*[numLumen];
                for(int j = 0; j < numLumen; j++){
                    lumenList[j] = new lumen(brightness,size,power);
                }
                novaVec1.push_back(make_unique<nova>(lumenList, numLumen));
            }
            

            // Demonstrate operator overloads
            testOperatorOverloads(novaVec0);

            // Demonstrate glow
            testGlow(novaVec1);

            // Demonstrate copying of nova objects via call by value.
            testCopy(novaVec1);

            // Demonstrate add and remove heap-allocated nova objects
            testAddRemove(novaVec1,lumenFac, numLumenA);

            // Demonstrate glow after object in the container added and removed
            testGlow(novaVec1);

            // Demonstrate the usage of shared_ptr
            testSharedPtr(novaVec1);

            return 0;
        }

/*
 * Revision history
 * lumen: recharge
 * lumen: constructor
 * lumen: data fields
 * pre/post conditions, design, comments, expalanation for all
 * lumen reset
 * lumen overload operators
 * 
 * nova overload operators
 * 
 *
 */
