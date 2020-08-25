#pragma once

#include <string>

namespace IceSDK
{
    namespace String
    {
        std::string Trim(std::string input, std::string search)
        {
            input.erase(0, input.find_first_not_of(search));
            input.erase(input.find_last_not_of(search) + 1);

            return input;
        }
    } // namespace String
} // namespace IceSDK
