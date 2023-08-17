#pragma once

#include "Utils/Memory.h"

namespace IceSDK {


} // namespace IceSDK

// Client log macros
#define ICESDK_VERBOSE(str, ...) printf("[VERBOSE]: " str "\n")
#define ICESDK_INFO(str, ...) printf("[INFO]: " str "\n")
#define ICESDK_WARN(str, ...) printf("[WARNING]: " str "\n")
#define ICESDK_ERROR(str, ...) printf("[ERROR]: " str "\n")
#define ICESDK_CRITICAL(str, ...) printf("[CRITICAL]: " str "\n")

#define ICESDK_VERBOSE_V(str, ...) printf("[VERBOSE]: " str "\n", __VA_ARGS__)
#define ICESDK_INFO_V(str, ...) printf("[INFO]: " str "\n", __VA_ARGS__)
#define ICESDK_WARN_V(str, ...) printf("[WARNING]: " str "\n", __VA_ARGS__)
#define ICESDK_ERROR_V(str, ...) printf("[ERROR]: " str "\n", __VA_ARGS__)
#define ICESDK_CRITICAL_V(str, ...) printf("[CRITICAL]: " str "\n", __VA_ARGS__)
