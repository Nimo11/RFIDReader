/**
 * Buzz.h
 * 
 * Buzz, a library for the ESP8266
 * 
 * @author J DAPOT
 * @version 0.0.0
 * @license MIT
 */

#ifndef Buzz_h
#define Buzz_h

#if defined(ESP8266)

#include <Arduino.h>

class Buzz
{

public:
    /*
            Create a new Buzzer
        */
    Buzz(int buzzPin,int ledPin);

    /*
            Buzzer pin
        */
    int BuzzPin;
    int ledPin;
    
    /*
            Generate x beep
            Where x is define by param buzzCount
        */
    void buzz(int buzzCount);

    /*
            Generate x beep
            Where x is define by param buzzCount
        */
    void continuousBuzz(int buzzCount);

private:
};

#endif
#endif