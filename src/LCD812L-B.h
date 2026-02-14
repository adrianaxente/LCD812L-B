#ifndef LCD821LB_H_
#define LCD821LB_H_

#include <Arduino.h>

#define SEGMENT_COUNT 10

static const char SEGMENT_CHARS[] PROGMEM = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'F', ',', 'l', 'P', '-', ' '};
static const byte SEGMENT_CODES[] PROGMEM = {10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 0};
static const byte SEGMENT_VALUES_COUNT = 16;

#define NON_EXISTING_CHARACTER_CODE 255
#define ERROR_CHARACTER_CODE 11
#define SPACE_CHARACTER_CODE 0

class LCD821LB
{
public:
    void begin(int clk_p, int data_p, int hks_p);
    void clear();
    void print(const char *str, bool leftPadded = false);
    void print(signed long long value, bool leftPadded = false);
    void print(double value, int precision = 3, bool leftPadded = false);
    void showClock();
    void startTimer();

private:
    int _clk_p;
    int _data_p;
    int _hks_p;
    byte getCharacterCode(char character);
    void shiftCharacter(byte characterCode);
    void shiftString(const char *str, bool leftPadded = false);
};
#endif