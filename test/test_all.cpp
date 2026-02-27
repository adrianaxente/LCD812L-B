#include <gtest/gtest.h>
#include <gmock/gmock.h>

int main(int argc, char **argv)
{
    // 1. Initialize GoogleTest
    ::testing::InitGoogleTest(&argc, argv);

    // 2. Initialize GoogleMock (Critical for arduino-mock to work)
    ::testing::InitGoogleMock(&argc, argv);

    // This turns off the "FAIL" for uninteresting calls 
    // and hides the "Long Output" warnings.
    //::testing::GMOCK_FLAG(verbose) = "error"; 

    // 3. Run all tests found in your files
    return RUN_ALL_TESTS();
}