#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Utils/String.h"

TEST(String, Trim)
{
    std::string input = "    YO!    ";

    ASSERT_EQ(IceSDK::String::Trim(input, " "), "YO!");
}
