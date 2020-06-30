/**
 * Buzz.h
 * 
 * Buzz, a library for the ESP8266
 * 
 * @author J DAPOT
 * @version 0.0.0
 * @license MIT
 */

#include "Buzz.h"

#if defined(ESP8266) || defined(ESP32)

Buzz::Buzz(int buzzPin)
{
    Buzz::BuzzPin = buzzPin;
}

void Buzz::buzz(int buzzCount)
{
    for (int i = 0; i < buzzCount; i++)
    {
        digitalWrite(Buzz::BuzzPin, HIGH); // Faire du bruit
        delay(100);                        // Attendre 10ms
        digitalWrite(Buzz::BuzzPin, LOW);  // Silence
        delay(100);
    }
}

void Buzz::continuousBuzz(int buzzCount)
{
    for (int i = 0; i < buzzCount; i++)
    {
        digitalWrite(Buzz::BuzzPin, HIGH); // Faire du bruit
        delay(1000);
    }                                 // Attendre 10ms
    digitalWrite(Buzz::BuzzPin, LOW); // Silence
}

#endif