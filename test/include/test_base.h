#include <gtest/gtest.h>
#include <Arduino.h>
#include <array>
#include <type_traits>

enum HighLow
{
    LOW_V = 0x0,
    HIGH_V = 0x1
};

using DataDigitalSequence = std::array<HighLow, 4 * 10>;

constexpr std::array<const HighLow, 4> charToSequence(char c)
{
    switch (c)
    {
    case '0': // 10
        return {HIGH_V, LOW_V, HIGH_V, LOW_V};
    case '1': // 1
        return {LOW_V, LOW_V, LOW_V, HIGH_V};
    case '2': // 2
        return {LOW_V, LOW_V, HIGH_V, LOW_V};
    case '3': // 3
        return {LOW_V, LOW_V, HIGH_V, HIGH_V};
    case '4': // 4
        return {LOW_V, HIGH_V, LOW_V, LOW_V};
    case '5': // 5
        return {LOW_V, HIGH_V, LOW_V, HIGH_V};
    case '6': // 6
        return {LOW_V, HIGH_V, HIGH_V, LOW_V};
    case '7': // 7
        return {LOW_V, HIGH_V, HIGH_V, HIGH_V};
    case '8': // 8
        return {HIGH_V, LOW_V, LOW_V, LOW_V};
    case '9': // 9
        return {HIGH_V, LOW_V, LOW_V, HIGH_V};
    case 'F': // 11
        return {HIGH_V, LOW_V, HIGH_V, HIGH_V};
    case ',': // 12
        return {HIGH_V, HIGH_V, LOW_V, LOW_V};
    case 'l': // 13
        return {HIGH_V, HIGH_V, LOW_V, HIGH_V};
    case 'P': // 14
        return {HIGH_V, HIGH_V, HIGH_V, LOW_V};
    case '-': // 15
        return {HIGH_V, HIGH_V, HIGH_V, HIGH_V};
    case ' ': // 0
        return {LOW_V, LOW_V, LOW_V, LOW_V};
    default:
        // ERROR F <=> 11
        return {HIGH_V, LOW_V, HIGH_V, HIGH_V};
    }
}

constexpr DataDigitalSequence stringToSequence(const char (&str)[11])
{
    std::array<HighLow, 40> result{};

    for (int i = 0; i < 10; i++)
    {
        auto sequence = charToSequence(str[i]);
        byte offset = i * 4;
        for (int j = 0; j < 4; j++)
        {
            result[offset + j] = sequence[j];
        }
    }

    return DataDigitalSequence{result};
}

#define STR_TO_SEQ(str) stringToSequence(str)

template <typename T>
struct PrintTestCase
{
    const T input;
    DataDigitalSequence expectedDigitalSequence;
    bool leftPadded = false;
};

using StringTestCase = PrintTestCase<const char *>;
using LongTestCase = PrintTestCase<signed long long>;
struct DoubleTestCase : public PrintTestCase<double> {
    byte precision;
};

class TestBase
{
protected:
    ArduinoMock *arduinoMock;

    LCD821LB lcd;

    void _SetUp()
    {
        // arduinoMock = arduinoMockInstance();
        arduinoMock = arduinoMockInstanceNice();
        // arduinoMock = arduinoMockInstanceStrict();
    }

    void _TearDown()
    {
        releaseArduinoMock();
    }
};

class SimpleTest : public ::testing::Test, public TestBase
{
protected:
    void SetUp() override { _SetUp(); }
    void TearDown() override { _TearDown(); }
};

template <typename T>
class ParamTest : public ::testing::TestWithParam<T>, public TestBase
{
    static_assert(
        std::is_same<T, PrintTestCase<char *>>::value ||
            std::is_convertible<T, PrintTestCase<const char *>>::value ||
            std::is_convertible<T, PrintTestCase<long long>>::value ||
            std::is_convertible<T, PrintTestCase<double>>::value,
        "ERROR: DataWrapper only supports char*, long long, or double!");

protected:
    void SetUp() override { _SetUp(); }
    void TearDown() override { _TearDown(); }
};