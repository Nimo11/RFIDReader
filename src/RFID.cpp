#include "RFID.h"
#include "Global.h"

uint32 RFIDRead()
{
    uint32 uid=0;

    // Look for new cards
    if (!_rfid.PICC_IsNewCardPresent())
        return uid;

    // Verify if the NUID has been readed
    if (!_rfid.PICC_ReadCardSerial())
        return uid;

    MFRC522::PICC_Type piccType = _rfid.PICC_GetType(_rfid.uid.sak);

    // Check is the PICC of Classic MIFARE type
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
        piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
        piccType != MFRC522::PICC_TYPE_MIFARE_4K)
    {
        Serial.println(F("Your tag is not of type MIFARE Classic."));
        return uid;
    }

    if ((millis() - _lastRead) > 3000 && _rfidd.uid.uidByte != _nuidPICC ||
                                         _rfid.uid.uidByte[1] != _nuidPICC[1] ||
                                         _rfid.uid.uidByte[2] != _nuidPICC[2] ||
                                         _rfid.uid.uidByte[3] != _nuidPICC[3]))
    {
        Serial.println(F("A new card has been detected."));
        Serial.print(F("PICC type: "));
        Serial.println(_rfid.PICC_GetTypeName(piccType));

        // Store NUID into nuidPICC array
        for (byte i = 0; i < 4; i++)
        {
            nuidPICC[i] = _rfid.uid.uidByte[i];
        }

        Serial.print(F("The NUID tag is:"));
        uid = toInt(_rfid.uid.uidByte, _rfid.uid.size);
        Serial.println(uid);
        _lastRead = millis();
        
    }
    else
    {
        //Serial.println(F("Card read previously."));
    }

    // Halt PICC
    _rfid.PICC_HaltA();

    // Stop encryption on PCD
    _rfid.PCD_StopCrypto1();

    return uid;
}

/**
 * Helper routine to dump a byte array as integer values.
 */
uint32 toInt(byte *buffer, byte bufferSize)
{

    uint32 value = 0;
    for (byte i = 0; i < bufferSize; i++)
    {
        value |= buffer[i] << (bufferSize - i - 1) * 8;
    }
    return value;
}