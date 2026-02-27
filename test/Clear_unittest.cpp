#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <Arduino.h>
#include "LCD812L-B.h"
#include <test_pins.h>
#include <test_base.h>

using ::testing::AnyNumber;
using ::testing::InSequence;
using ::testing::Return;

class ClearTest : public SimpleTest
{
protected:
    void SetUp() override
    {
        SimpleTest::SetUp();
        lcd.begin(CLK_PIN, DATA_PIN, HKS_PIN);
    }
};

static constexpr DataDigitalSequence EXPECTED_DIGITAL_SEQUENCE = STR_TO_SEQ("          ");

TEST_F(ClearTest, CLEAR_SETS_HKS_LOW_WHEN_HKS_IS_HIGH)
{
    // 1. Setup
    ON_CALL(*arduinoMock, digitalRead(HKS_PIN)).WillByDefault(Return(HIGH));

    // 2. EXPECTATIONS (Must come BEFORE the action)
    //  These wildcards catch EVERYTHING
    // TODO: FIND A BETTER GENRIC WAY TO MATCH THE EXPECTED DIGITAL SEQUENCE WITHOUT HAVING TO WRITE EXPECT_CALL FOR EACH CALL
    EXPECT_CALL(*arduinoMock, digitalWrite(_, _)).Times(AnyNumber());
    EXPECT_CALL(*arduinoMock, delay(_)).Times(AnyNumber());
    // EXPECT_CALL(*arduinoMock, delayMicroseconds(_)).Times(AnyNumber());

    {
        InSequence s;
        // 2. EXPECTATIONS (Must come BEFORE the action)
        EXPECT_CALL(*arduinoMock, digitalWrite(HKS_PIN, LOW)).Times(1);
        EXPECT_CALL(*arduinoMock, delay(50)).Times(1);
    }

    // 3. ACTION (The code that triggers the calls)
    lcd.clear();

    // 4. CLEANUP
}

TEST_F(ClearTest, CLEAR_DOES_NOT_SETS_HKS_LOW_WHEN_HKS_IS_LOW)
{
    // 1. Setup
    ON_CALL(*arduinoMock, digitalRead(HKS_PIN)).WillByDefault(Return(LOW));

    // 2. EXPECTATIONS (Must come BEFORE the action)
    //  These wildcards catch EVERYTHING
    // TODO: FIND A BETTER GENRIC WAY TO MATCH THE EXPECTED DIGITAL SEQUENCE WITHOUT HAVING TO WRITE EXPECT_CALL FOR EACH CALL
    EXPECT_CALL(*arduinoMock, digitalWrite(_, _)).Times(AnyNumber());
    EXPECT_CALL(*arduinoMock, delay(_)).Times(AnyNumber());
    // EXPECT_CALL(*arduinoMock, delayMicroseconds(_)).Times(AnyNumber());

    {
        InSequence s;
        // 2. EXPECTATIONS (Must come BEFORE the action)
        EXPECT_CALL(*arduinoMock, digitalWrite(HKS_PIN, LOW)).Times(0);
        EXPECT_CALL(*arduinoMock, delay(50)).Times(0);
    }

    // 3. ACTION (The code that triggers the calls)
    lcd.clear();

    // 4. CLEANUP
}

TEST_F(ClearTest, CLEAR_SHOWS_ALL_CHARACTERS_AS_BLANK)
{
    // 1. Setup
    ON_CALL(*arduinoMock, digitalRead(HKS_PIN)).WillByDefault(Return(LOW));

    // 2. EXPECTATIONS (Must come BEFORE the action)
    //  These wildcards catch EVERYTHING
    // TODO: FIND A BETTER GENRIC WAY TO MATCH THE EXPECTED DIGITAL SEQUENCE WITHOUT HAVING TO WRITE EXPECT_CALL FOR EACH CALL
    EXPECT_CALL(*arduinoMock, digitalWrite(_, _)).Times(AnyNumber());
    EXPECT_CALL(*arduinoMock, delay(_)).Times(AnyNumber());
    EXPECT_CALL(*arduinoMock, delayMicroseconds(_)).Times(AnyNumber());

    {
        InSequence s;
        for (byte i = 0; i < EXPECTED_DIGITAL_SEQUENCE.size(); i++)
        {
            EXPECT_CALL(*arduinoMock, digitalWrite(DATA_PIN, EXPECTED_DIGITAL_SEQUENCE[i]));
            EXPECT_CALL(*arduinoMock, digitalWrite(CLK_PIN, HIGH));
            EXPECT_CALL(*arduinoMock, delayMicroseconds(50));
            EXPECT_CALL(*arduinoMock, digitalWrite(CLK_PIN, LOW));
        }
    }

    // 3. ACTION (The code that triggers the calls)
    lcd.clear();

    // 4. CLEANUP
}