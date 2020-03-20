#ifndef LogObject_h
#define LogObject_h

#include <Arduino.h>

class LogObject
{
private:
public:
    enum DebugLevels
    {
        ErrorOnly,
        Normal,
        Verbose
    };
    enum DebugType
    {
        SerialType,
        FileType
    };

    bool Debug = true;
    DebugLevels level = Normal;
    DebugType debugType=SerialType;
    String fileName;

    void print(DebugLevels l, String s);
    void print(DebugLevels l, const __FlashStringHelper *s);
    void print(DebugLevels l, byte &b, int i);
    void println(DebugLevels l, String s);
    void println(DebugLevels l, double s);
    void println(DebugLevels l, byte &b, int i);
    void printf(DebugLevels l,const char *s, ...);
    void printf(DebugLevels l,const __FlashStringHelper *s, ...);

    void initLogFile();

    static String getTimeString(int delta);
};
#endif