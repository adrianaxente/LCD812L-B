#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <Arduino.h>
#include "LCD812L-B.h"
#include <test_pins.h>
#include <test_base.h>

using ::testing::InSequence;
using ::testing::Return;

class BeginTest : public SimpleTest
{
};

TEST_F(BeginTest, BEGIN_SETS_HKS_HIGH_WHEN_HKS_IS_LOW)
{
    // 1. Setup
    ON_CALL(*arduinoMock, digitalRead(HKS_PIN)).WillByDefault(Return(LOW));

    // 2. EXPECTATIONS (Must come BEFORE the action)
    {
        InSequence s; // Ensures they happen in this exact order
        EXPECT_CALL(*arduinoMock, pinMode(CLK_PIN, OUTPUT));
        EXPECT_CALL(*arduinoMock, pinMode(DATA_PIN, OUTPUT));
        EXPECT_CALL(*arduinoMock, pinMode(HKS_PIN, OUTPUT));
        EXPECT_CALL(*arduinoMock, digitalWrite(HKS_PIN, HIGH));
    }

    // 3. ACTION (The code that triggers the calls)
    lcd.begin(CLK_PIN, DATA_PIN, HKS_PIN);

    // 4. CLEANUP
}

TEST_F(BeginTest, BEGIN_DOES_NOT_SET_HKS_HIGH_WHEN_HKS_IS_HIGH)
{
    // 1. Setup
    ON_CALL(*arduinoMock, digitalRead(HKS_PIN)).WillByDefault(Return(HIGH));

    // 2. EXPECTATIONS (Must come BEFORE the action)
    {
        InSequence s; // Ensures they happen in this exact order
        EXPECT_CALL(*arduinoMock, pinMode(CLK_PIN, OUTPUT));
        EXPECT_CALL(*arduinoMock, pinMode(DATA_PIN, OUTPUT));
        EXPECT_CALL(*arduinoMock, pinMode(HKS_PIN, OUTPUT));
        EXPECT_CALL(*arduinoMock, digitalWrite(HKS_PIN, _)).Times(0);
    }

    // 3. ACTION (The code that triggers the calls)
    lcd.begin(CLK_PIN, DATA_PIN, HKS_PIN);

    // 4. CLEANUP
}