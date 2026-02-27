#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <Arduino.h>
#include "LCD812L-B.h"
#include <test_pins.h>
#include <test_base.h>

using ::testing::AnyNumber;
using ::testing::InSequence;
using ::testing::Return;

class ShowClockTest : public SimpleTest
{
protected:
    void SetUp() override
    {
        SimpleTest::SetUp();
        lcd.begin(CLK_PIN, DATA_PIN, HKS_PIN);
    }
};

TEST_F(ShowClockTest, SHOW_CLOCK_SETS_HKS_HIGH_WHEN_HKS_IS_LOW)
{
    // 1. Setup
    ON_CALL(*arduinoMock, digitalRead(HKS_PIN)).WillByDefault(Return(LOW));

    // 2. EXPECTATIONS (Must come BEFORE the action)
    EXPECT_CALL(*arduinoMock, digitalWrite(HKS_PIN, LOW)).Times(0);
    EXPECT_CALL(*arduinoMock, digitalWrite(HKS_PIN, HIGH)).Times(1);

    // 3. ACTION (The code that triggers the calls)
    lcd.showClock();

    // 4. CLEANUP
}

TEST_F(ShowClockTest, SHOW_CLOCK_DOES_NOT_TOCH_HKS_WHEN_HKS_IS_HIGH)
{
    // 1. Setup
    ON_CALL(*arduinoMock, digitalRead(HKS_PIN)).WillByDefault(Return(HIGH));

    // 2. EXPECTATIONS (Must come BEFORE the action)
    EXPECT_CALL(*arduinoMock, digitalWrite(HKS_PIN, _)).Times(0);

    // 3. ACTION (The code that triggers the calls)
    lcd.showClock();

    // 4. CLEANUP
}