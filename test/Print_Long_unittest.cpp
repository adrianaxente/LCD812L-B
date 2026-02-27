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

class PrintLongTest : public ParamTest<LongTestCase>
{
protected:
    void SetUp() override
    {
        ParamTest<LongTestCase>::SetUp();
        lcd.begin(CLK_PIN, DATA_PIN, HKS_PIN);
    }
};

TEST_P(PrintLongTest, TEST_PRINT_LONG)
{
    // 1. Setup
    LongTestCase testCase = GetParam();
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
    lcd.print(testCase.input, testCase.leftPadded);

    // 4. CLEANUP
}

INSTANTIATE_TEST_SUITE_P(
    BASIC_NUMBERS,
    PrintLongTest,
    testing::Values(
        LongTestCase{0, STR_TO_SEQ("0         "), false},
        LongTestCase{0, STR_TO_SEQ("         0"), true},
        LongTestCase{123, STR_TO_SEQ("123       "), false},
        LongTestCase{123, STR_TO_SEQ("       123"), true},
        LongTestCase{67563221, STR_TO_SEQ("67563221  "), false},
        LongTestCase{67563221, STR_TO_SEQ("  67563221"), true},
        LongTestCase{9876543210, STR_TO_SEQ("9876543210"), false},
        LongTestCase{9876543210, STR_TO_SEQ("9876543210"), true}));

INSTANTIATE_TEST_SUITE_P(
    NEGATIVE_NUMBERS,
    PrintLongTest,
    testing::Values(
        LongTestCase{-0, STR_TO_SEQ("0         "), false},
        LongTestCase{-0, STR_TO_SEQ("         0"), true},
        LongTestCase{-1, STR_TO_SEQ("-1        "), false},
        LongTestCase{-1, STR_TO_SEQ("        -1"), true},
        LongTestCase{-123456, STR_TO_SEQ("-123456   "), false},
        LongTestCase{-123456, STR_TO_SEQ("   -123456"), true},
         LongTestCase{-876543210, STR_TO_SEQ("-876543210"), false},
        LongTestCase{-876543210, STR_TO_SEQ("-876543210"), true},
        LongTestCase{-999999999, STR_TO_SEQ("-999999999"), false},
        LongTestCase{-999999999, STR_TO_SEQ("-999999999"), true}
    ));

INSTANTIATE_TEST_SUITE_P(
    OVERFLOW_NUMBERS,
    PrintLongTest,
    testing::Values(
        LongTestCase{12345678901, STR_TO_SEQ("FFFFFFFFFF"), false},
        LongTestCase{12345678901, STR_TO_SEQ("FFFFFFFFFF"), true},
        LongTestCase{-123456789012, STR_TO_SEQ("FFFFFFFFFF"), false},
        LongTestCase{-123456789012, STR_TO_SEQ("FFFFFFFFFF"), true},
        LongTestCase{99999999999999, STR_TO_SEQ("FFFFFFFFFF"), false},
        LongTestCase{99999999999999, STR_TO_SEQ("FFFFFFFFFF"), true},
        LongTestCase{-99999999999999, STR_TO_SEQ("FFFFFFFFFF"), false},
        LongTestCase{-99999999999999, STR_TO_SEQ("FFFFFFFFFF"), true}
    ));
