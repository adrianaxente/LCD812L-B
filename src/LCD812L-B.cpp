#include <Arduino.h>
#include "LCD812L-B.h"

#ifdef DEBUG
#include <ArduinoInterpolator.h>
#endif

void LCD821LB::begin(int clk_p, int data_p, int hks_p)
{
    _clk_p = clk_p;
    _data_p = data_p;
    _hks_p = hks_p;
    pinMode(clk_p, OUTPUT);
    pinMode(data_p, OUTPUT);
    pinMode(hks_p, OUTPUT);
    if (digitalRead(_hks_p) == LOW)
    {
        digitalWrite(hks_p, HIGH);
#ifdef DEBUG
        Serial.println(F("HKS HIGH"));
#endif
    }
}

void LCD821LB::clear()
{
    shiftString("");
}

void LCD821LB::print(const char *str, bool leftPadded)
{
    shiftString(str, leftPadded);
}

void LCD821LB::print(signed long long value, bool leftPadded)
{
    char str[21];

#if defined(ARDUINO_ARCH_AVR)
    char *p = &str[0]; // Point to the first index

    // Handle negative/absolute value safely
    unsigned long long v;
    if (value < 0)
    {
        v = -value;
        *p++ = '-';
    }
    else
    {
        v = value;
    }

    // Convert digits from right to left
    do
    {
        *p++ = (v % 10) + '0';
        v /= 10;
    } while (v);

    // Reversing the string in place
    char *start = (value < 0) ? &str[1] : &str[0];
    char *end = p - 1;
    while (start < end)
    {
        char temp = *start;
        *start++ = *end;
        *end-- = temp;
    }

    *p = '\0'; // Null terminator

#else
    snprintf(str, sizeof(str), "%" PRId64, value);
#endif

    shiftString(str, leftPadded);
}

void LCD821LB::print(double value, int precision, bool leftPadded)
{
    char str[25];

#if defined(ARDUINO_ARCH_AVR)
    // This code only exists when compiling for AVR
    dtostrf(value, 0, precision, str);
#elif defined(ESP32) || defined(ESP8266) || defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_RP2040)
    // This code only exists when compiling for ESP
    snprintf(str, sizeof(str), "%.*f", precision, value);
#else
    // Fallback for other platforms (ARM, etc.)
    snprintf(str, sizeof(str), "%f", value);
#endif

    char *ptr = strchr(str, '.');
    if (ptr != NULL)
    {
        *ptr = ',';
    }

    shiftString(str, leftPadded);
}

void LCD821LB::showClock()
{
    digitalWrite(_hks_p, HIGH);
}

void LCD821LB::startTimer()
{
    digitalWrite(_hks_p, HIGH);
    delay(25);
    digitalWrite(_clk_p, HIGH);
    digitalWrite(_clk_p, LOW);
    delay(25);
    digitalWrite(_hks_p, LOW);
}

byte LCD821LB::getCharacterCode(char character)
{
    for (byte i = 0; i < SEGMENT_VALUES_COUNT; i++)
    {
        // Read the character key from Flash
        char currentCharacter = (char)pgm_read_byte(&SEGMENT_CHARS[i]);

        if (currentCharacter == character)
        {
            // Read the character bynary code from Flash
            return (int)pgm_read_word(&SEGMENT_CODES[i]);
        }
    }

    return NON_EXISTING_CHARACTER_CODE;
}

void LCD821LB::shiftCharacter(byte characterCode)
{
    for (int i = 3; i >= 0; i--)
    {
        digitalWrite(_data_p, (characterCode >> i) & 0x01);

        // Pulsing the clock
        digitalWrite(_clk_p, HIGH);
        delayMicroseconds(50);
        digitalWrite(_clk_p, LOW);
    }
}

void LCD821LB::shiftString(const char *str, bool leftPadded)
{
    if (digitalRead(_hks_p) == HIGH)
    {
        digitalWrite(_hks_p, LOW);
        delay(50);
#ifdef DEBUG
        Serial.println(F("HKS LOW"));
#endif
    }

    byte count = strlen(str);

    if (count > SEGMENT_COUNT)
    {
        for (byte i = 0; i < SEGMENT_COUNT; i++)
        {
            shiftCharacter(ERROR_CHARACTER_CODE);
        }

#ifdef DEBUG
        Serial.println(format(F("Value too long to display: '%s', Count: %d, Max: %d"), str, strlen(str), SEGMENT_COUNT));
#endif
        return;
    }

    if (leftPadded)
    {
        for (byte i = 0; i < SEGMENT_COUNT - count; i++)
        {
            shiftCharacter(SPACE_CHARACTER_CODE);
        }
    }

    for (int i = 0; i < count; i++)
    {
        byte characterCode = getCharacterCode(str[i]);
        if (characterCode == NON_EXISTING_CHARACTER_CODE)
        {

#ifdef DEBUG
            Serial.println(format(F("Invalid character detected: '%c', Code: %d"), str[i], characterCode));
#endif
            characterCode = ERROR_CHARACTER_CODE;
        }

        shiftCharacter(characterCode);
    }

    if (!leftPadded)
    {
        for (byte i = 0; i < SEGMENT_COUNT - count; i++)
        {
            shiftCharacter(SPACE_CHARACTER_CODE);
        }
    }
}