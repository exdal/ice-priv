#pragma once

#include "Utils/Memory.h"

namespace IceSDK {
    class Log {
    public:
        static void Init();

       
    private:
    };

} // namespace IceSDK

// Core log macros
#define ICESDK_CORE_TRACE(...) 
#define ICESDK_CORE_INFO(...) 
#define ICESDK_CORE_WARN(...) 
#define ICESDK_CORE_ERROR(...) 
#define ICESDK_CORE_CRITICAL(...) 

// Client log macros
#define ICESDK_TRACE(...) 
#define ICESDK_INFO(...) 
#define ICESDK_WARN(...) 
#define ICESDK_ERROR(...) 
#define ICESDK_CRITICAL(...) 
