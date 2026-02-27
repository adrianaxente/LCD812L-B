#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <Arduino.h>
#include "LCD812L-B.h"
#include <test_pins.h>
#include <test_base.h>

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::Values;

class PrintDoubleTest : public ParamTest<DoubleTestCase>
{
protected:
    void SetUp() override
    {
        ParamTest<DoubleTestCase>::SetUp();
        lcd.begin(CLK_PIN, DATA_PIN, HKS_PIN);
    }
};

TEST_P(PrintDoubleTest, TEST_PRINT_LONG)
{
    // 1. Setup
    DoubleTestCase testCase = GetParam();
    ON_CALL(*arduinoMock, digitalRead(HKS_PIN)).WillByDefault(Return(LOW));

    // 2. EXPECTATIONS (Must come BEFORE the action)
    //  These wildcards catch EVERYTHING
    // TODO: FIND A BETTER GENRIC WAY TO MATCH THE EXPECTED DIGITAL SEQUENCE WITHOUT HAVING TO WRITE EXPECT_CALL FOR EACH CALL
    EXPECT_CALL(*arduinoMock, digitalWrite(_, _)).Times(AnyNumber());
    EXPECT_CALL(*arduinoMock, delay(_)).Times(AnyNumber());
    EXPECT_CALL(*arduinoMock, delayMicroseconds(_)).Times(AnyNumber());

    {
        InSequence s;
        for (byte i = 0; i < testCase.expectedDigitalSequence.size(); i++)
        {
            EXPECT_CALL(*arduinoMock, digitalWrite(DATA_PIN, testCase.expectedDigitalSequence[i]));
            EXPECT_CALL(*arduinoMock, digitalWrite(CLK_PIN, HIGH));
            EXPECT_CALL(*arduinoMock, delayMicroseconds(50));
            EXPECT_CALL(*arduinoMock, digitalWrite(CLK_PIN, LOW));
        }
    }

    // 3. ACTION (The code that triggers the calls)
    lcd.print(testCase.input, testCase.precision, testCase.leftPadded);

    // 4. CLEANUP
}

INSTANTIATE_TEST_SUITE_P(
    BASIC_NUMBERS,
    PrintDoubleTest,
    testing::Values(
        DoubleTestCase{3.14, STR_TO_SEQ("3,14      "), false, 2},
        DoubleTestCase{3.14, STR_TO_SEQ("      3,14"), true, 2},
        DoubleTestCase{3.14, STR_TO_SEQ("3,1400    "), false, 4},
        DoubleTestCase{3.14, STR_TO_SEQ("    3,1400"), true, 4},
        DoubleTestCase{983453.142, STR_TO_SEQ("983453,142"), false, 3},
        DoubleTestCase{983453.142, STR_TO_SEQ("983453,142"), true, 3},
        DoubleTestCase{123456, STR_TO_SEQ("123456    "), false, 0},
        DoubleTestCase{123456, STR_TO_SEQ("    123456"), true, 0},
        DoubleTestCase{897654.123, STR_TO_SEQ("897654,1  "), false, 1},
        DoubleTestCase{897654.123, STR_TO_SEQ("  897654,1"), true, 1}));

INSTANTIATE_TEST_SUITE_P(
    NEGATIVE_NUMBERS,
    PrintDoubleTest,
    testing::Values(
        DoubleTestCase{-3.14, STR_TO_SEQ("-3,14     "), false, 2},
        DoubleTestCase{-3.14, STR_TO_SEQ("     -3,14"), true, 2},
        DoubleTestCase{-3.14, STR_TO_SEQ("-3,1400   "), false, 4},
        DoubleTestCase{-3.14, STR_TO_SEQ("   -3,1400"), true, 4},
        DoubleTestCase{-98345.142, STR_TO_SEQ("-98345,142"), false, 3},
        DoubleTestCase{-98345.142, STR_TO_SEQ("-98345,142"), true, 3},
        DoubleTestCase{-123456, STR_TO_SEQ("-123456   "), false, 0},
        DoubleTestCase{-123456, STR_TO_SEQ("   -123456"), true, 0},
        DoubleTestCase{-897654.123, STR_TO_SEQ("-897654,1 "), false, 1},
        DoubleTestCase{-897654.123, STR_TO_SEQ(" -897654,1"), true, 1}));

INSTANTIATE_TEST_SUITE_P(
    OVERFLOW_NUMBERS,
    PrintDoubleTest,
    testing::Values(
        DoubleTestCase{-3.14, STR_TO_SEQ("FFFFFFFFFF"), false, 10},
        DoubleTestCase{-3.14, STR_TO_SEQ("FFFFFFFFFF"), true, 10},
        DoubleTestCase{1234567890123, STR_TO_SEQ("FFFFFFFFFF"), false, 0},
        DoubleTestCase{1234567890123, STR_TO_SEQ("FFFFFFFFFF"), true, 0},
        DoubleTestCase{12345678.012, STR_TO_SEQ("FFFFFFFFFF"), false, 2},
        DoubleTestCase{12345678.012, STR_TO_SEQ("FFFFFFFFFF"), true, 2},
        DoubleTestCase{-12345678.1, STR_TO_SEQ("FFFFFFFFFF"), false, 1},
        DoubleTestCase{-12345678.1, STR_TO_SEQ("FFFFFFFFFF"), true, 1},
        DoubleTestCase{0.00001, STR_TO_SEQ("FFFFFFFFFF"), false, 9},
        DoubleTestCase{0.00001, STR_TO_SEQ("FFFFFFFFFF"), true, 9},
        DoubleTestCase{-0.00011, STR_TO_SEQ("FFFFFFFFFF"), false, 8},
        DoubleTestCase{-0.00011, STR_TO_SEQ("FFFFFFFFFF"), true, 8}));

INSTANTIATE_TEST_SUITE_P(
    ROUNDING_NUMBERS,
    PrintDoubleTest,
    testing::Values(
        DoubleTestCase{1.256, STR_TO_SEQ("1,26      "), false, 2},
        DoubleTestCase{1.256, STR_TO_SEQ("      1,26"), true, 2},
        DoubleTestCase{1.254, STR_TO_SEQ("1,25      "), false, 2},
        DoubleTestCase{1.254, STR_TO_SEQ("      1,25"), true, 2},
        DoubleTestCase{0.999, STR_TO_SEQ("1,00      "), false, 2},
        DoubleTestCase{0.999, STR_TO_SEQ("      1,00"), true, 2},
        DoubleTestCase{9.99, STR_TO_SEQ("10,0      "), false, 1},
        DoubleTestCase{9.99, STR_TO_SEQ("      10,0"), true, 1},
        DoubleTestCase{10.51, STR_TO_SEQ("11        "), false, 0},
        DoubleTestCase{10.51, STR_TO_SEQ("        11"), true, 0},
        DoubleTestCase{10.49, STR_TO_SEQ("10        "), false, 0},
        DoubleTestCase{10.49, STR_TO_SEQ("        10"), true, 0},
        DoubleTestCase{-1.256, STR_TO_SEQ("-1,26     "), false, 2},
        DoubleTestCase{-1.256, STR_TO_SEQ("     -1,26"), true, 2},
        DoubleTestCase{-0.999, STR_TO_SEQ("-1,00     "), false, 2},
        DoubleTestCase{-0.999, STR_TO_SEQ("     -1,00"), true, 2},
        DoubleTestCase{9999999.996, STR_TO_SEQ("10000000,0"), false, 1},
        DoubleTestCase{9999999.996, STR_TO_SEQ("10000000,0"), true, 1}));
