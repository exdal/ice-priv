#pragma once

#include "Utils/Memory.h"

namespace IceSDK {
    class Log {
    public:
        static void Init();

    private:
    };

} // namespace IceSDK

// Client log macros
#define ICESDK_VERBOSE(str, ...) printf("[VERBOSE]: " str "\n", __VA_ARGS__)
#define ICESDK_INFO(str, ...) printf("[INFO]: " str "\n", __VA_ARGS__)
#define ICESDK_WARN(str, ...) printf("[WARNING]: " str "\n", __VA_ARGS__)
#define ICESDK_ERROR(str, ...) printf("[ERROR]: " str "\n", __VA_ARGS__)
#define ICESDK_CRITICAL(str, ...) printf("[CRITICAL]: " str "\n", __VA_ARGS__)
