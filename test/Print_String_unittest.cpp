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

class PrintStringTest : public ParamTest<StringTestCase>
{
protected:
    void SetUp() override
    {
        ParamTest<StringTestCase>::SetUp();
        lcd.begin(CLK_PIN, DATA_PIN, HKS_PIN);
    }
};

TEST_P(PrintStringTest, TEST_PRINT_STRING)
{
    // 1. Setup
    StringTestCase testCase = GetParam();
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
    EMPTY_AND_NULL,
    PrintStringTest,
    Values(
        StringTestCase{
            "",
            STR_TO_SEQ("          "),
            false},
        StringTestCase{
            "",
            STR_TO_SEQ("          "),
            true},
        StringTestCase{
            nullptr,
            STR_TO_SEQ("          "),
            false},
        StringTestCase{
            nullptr,
            STR_TO_SEQ("          "),
            true}));

INSTANTIATE_TEST_SUITE_P(
    FULL_SEGMENTS,
    PrintStringTest,
    Values(
        StringTestCase{
            "0123456789",
            STR_TO_SEQ("0123456789"),
            false},
        StringTestCase{
            "0123456789",
            STR_TO_SEQ("0123456789"),
            true},
        StringTestCase{
            "51 3,-FPl7",
            STR_TO_SEQ("51 3,-FPl7"),
            false},
        StringTestCase{
            "51 3,-FPl7",
            STR_TO_SEQ("51 3,-FPl7"),
            true}));

INSTANTIATE_TEST_SUITE_P(
    LENGTH_EXCEEDS_SEGMENT_COUNT,
    PrintStringTest,
    Values(
        StringTestCase{
            "1234567890A",
            STR_TO_SEQ("FFFFFFFFFF"),
            false},
        StringTestCase{
            "1234567890A",
            STR_TO_SEQ("FFFFFFFFFF"),
            true},
        StringTestCase{
            "000AAA1234567890A",
            STR_TO_SEQ("FFFFFFFFFF"),
            false},
        StringTestCase{
            "000AAA1234567890A",
            STR_TO_SEQ("FFFFFFFFFF"),
            true}));

INSTANTIATE_TEST_SUITE_P(
    LEFT_PADDING,
    PrintStringTest,
    Values(
        StringTestCase{
            "1234",
            STR_TO_SEQ("      1234"),
            true},
        StringTestCase{
            "567890",
            STR_TO_SEQ("    567890"),
            true},
        StringTestCase{
            "09 7F-,lP",
            STR_TO_SEQ(" 09 7F-,lP"),
            true}));

INSTANTIATE_TEST_SUITE_P(
    RIGHT_PADDING,
    PrintStringTest,
    Values(
        StringTestCase{
            "4567",
            STR_TO_SEQ("4567      "),
            false},
        StringTestCase{
            "098765",
            STR_TO_SEQ("098765    "),
            false},
        StringTestCase{
            "23-0P, lP",
            STR_TO_SEQ("23-0P, lP "),
            false}));

INSTANTIATE_TEST_SUITE_P(
    UNKNOWN_CHARACTERS,
    PrintStringTest,
    Values(
        StringTestCase{
            "78BNU10",
            STR_TO_SEQ("78FFF10   "),
            false},
        StringTestCase{
            "78BNU10",
            STR_TO_SEQ("   78FFF10"),
            true},
        StringTestCase{
            "ABCDE$12#",
            STR_TO_SEQ("FFFFFF12F "),
            false},
        StringTestCase{
            "ABCDE$12#",
            STR_TO_SEQ(" FFFFFF12F"),
            true}));