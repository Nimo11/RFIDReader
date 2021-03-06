#include "LogObject.h"
#include "time.h"
//#include "FS.h"
#include "LittleFS.h"

const char *TIME_PATERN PROGMEM = "%02d-%02d-%02d %02d:%02d:%02d";

/// Log a string
void LogObject::print(DebugLevels l, const __FlashStringHelper *s)
{
    LogObject::print(l, (const char *)s);
}
void LogObject::print(DebugLevels l, String s)
{
    if (Debug && !(l > level))
        switch (debugType)
        {
        case LogObject::DebugType::SerialType:
            Serial.print(s);
            break;
        case LogObject::DebugType::FileType:
            File file = LittleFS.open(fileName, "a");
            file.print(getTimeString(0));
            file.print(" ");
            file.print(s);
            file.close();
            break;
        }
}
void LogObject::print(DebugLevels l, byte &b, int i)
{
    if (Debug && !(l > level))
        switch (debugType)
        {
        case LogObject::DebugType::SerialType:
            Serial.print(b, i);
            break;
        case LogObject::DebugType::FileType:
            File file = LittleFS.open(fileName, "a");
            file.print(getTimeString(0));
            file.print(" ");
            file.print(b, i);
            file.close();
            break;
        }
}

void LogObject::println(DebugLevels l, String s)
{
    if (Debug && !(l > level))
        switch (debugType)
        {
        case LogObject::DebugType::SerialType:
            Serial.println(s);
            break;
        case LogObject::DebugType::FileType:
            File file = LittleFS.open(fileName, "a");
            file.print(getTimeString(0));
            file.print(" ");
            file.print(s);
            file.print('\n');
            file.close();
            break;
        }
}
void LogObject::println(DebugLevels l, double s)
{
    if (Debug && !(l > level))
        switch (debugType)
        {
        case LogObject::DebugType::SerialType:
            Serial.println(s);
            break;
        case LogObject::DebugType::FileType:
            File file = LittleFS.open(fileName, "a");
            file.print(getTimeString(0));
            file.print(" ");
            file.print(s);
            file.print('\n');
            file.close();
            break;
        }
}
void LogObject::println(DebugLevels l, byte &b, int i)
{
    if (Debug && !(l > level))
        switch (debugType)
        {
        case LogObject::DebugType::SerialType:
            Serial.println(b, i);
            break;
        case LogObject::DebugType::FileType:
            File file = LittleFS.open(fileName, "a");
            file.print(getTimeString(0));
            file.print(" ");
            file.print(b, i);
            file.print('\n');
            file.close();
            break;
        }
}
void LogObject::printf(DebugLevels l, const __FlashStringHelper *f, ...)
{
    const char *s = (const char *)f;
    char buff[256];
    //build result
    va_list argptr;
    va_start(argptr, s);
    vsnprintf(buff, sizeof(buff), s, argptr);
    va_end(argptr);
    buff[sizeof(buff) / sizeof(buff[0]) - 1] = '\0';
    //send it
    if (Debug && !(l > level))
        switch (debugType)
        {
        case LogObject::DebugType::SerialType:
            Serial.print(buff);
            break;
        case LogObject::DebugType::FileType:
            File file = LittleFS.open(fileName, "a");
            file.print(getTimeString(0));
            file.print(" ");
            file.print(buff);
            file.close();
            break;
        }
}
void LogObject::printf(DebugLevels l, const char *s, ...)
{
    char buff[256];
    //build result
    va_list argptr;
    va_start(argptr, s);
    vsnprintf(buff, sizeof(buff), s, argptr);
    va_end(argptr);
    buff[sizeof(buff) / sizeof(buff[0]) - 1] = '\0';
    //send it
    if (Debug && !(l > level))
        switch (debugType)
        {
        case LogObject::DebugType::SerialType:
            Serial.print(buff);
            break;
        case LogObject::DebugType::FileType:
            File file = LittleFS.open(fileName, "a");
            file.print(getTimeString(0));
            file.print(" ");
            file.print(buff);
            file.close();
            break;
        }
}

String LogObject::getTimeString(int delta)
{
    return "...";
    time_t now;
    struct tm * timeinfo;
    time(&now);
    timeinfo = localtime(&now);  

    char txt_time[20];
    //sprintf(txt_time, (const char *)FPSTR(TIME_PATERN), month(), day(), year(), hour(), minute(), second() - delta);
    sprintf(txt_time, (const char *)FPSTR(TIME_PATERN),timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_year +1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec - delta);
    return txt_time;
}


void LogObject::initLogFile()
{
    LittleFS.remove(fileName);
    File initFile = LittleFS.open(fileName, "w");
    initFile.print("Debug file created ");
    initFile.println(getTimeString(0));
    initFile.close();
}