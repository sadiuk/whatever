#pragma once
#include "util/ILogger.h"
#include "vulkan/vulkan.h"

template <>
struct fmt::formatter<VkResult> {
    // Parses format specifications (which we don't need in this case)
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    // Formats the VkResult enum
    template <typename FormatContext>
    auto format(const VkResult& result, FormatContext& ctx) {
        std::string name;
        switch (result) {
        case VK_SUCCESS: name = "VK_SUCCESS"; break;
        case VK_NOT_READY: name = "VK_NOT_READY"; break;
        case VK_TIMEOUT: name = "VK_TIMEOUT"; break;
        case VK_EVENT_SET: name = "VK_EVENT_SET"; break;
        case VK_EVENT_RESET: name = "VK_EVENT_RESET"; break;
        case VK_INCOMPLETE: name = "VK_INCOMPLETE"; break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: name = "VK_ERROR_OUT_OF_HOST_MEMORY"; break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: name = "VK_ERROR_OUT_OF_DEVICE_MEMORY"; break;
        case VK_ERROR_INITIALIZATION_FAILED: name = "VK_ERROR_INITIALIZATION_FAILED"; break;
        case VK_ERROR_DEVICE_LOST: name = "VK_ERROR_DEVICE_LOST"; break;
        case VK_ERROR_MEMORY_MAP_FAILED: name = "VK_ERROR_MEMORY_MAP_FAILED"; break;
        case VK_ERROR_LAYER_NOT_PRESENT: name = "VK_ERROR_LAYER_NOT_PRESENT"; break;
        case VK_ERROR_EXTENSION_NOT_PRESENT: name = "VK_ERROR_EXTENSION_NOT_PRESENT"; break;
        case VK_ERROR_FEATURE_NOT_PRESENT: name = "VK_ERROR_FEATURE_NOT_PRESENT"; break;
        case VK_ERROR_INCOMPATIBLE_DRIVER: name = "VK_ERROR_INCOMPATIBLE_DRIVER"; break;
        case VK_ERROR_TOO_MANY_OBJECTS: name = "VK_ERROR_TOO_MANY_OBJECTS"; break;
        case VK_ERROR_FORMAT_NOT_SUPPORTED: name = "VK_ERROR_FORMAT_NOT_SUPPORTED"; break;
        case VK_ERROR_FRAGMENTED_POOL: name = "VK_ERROR_FRAGMENTED_POOL"; break;
        case VK_ERROR_UNKNOWN: name = "VK_ERROR_UNKNOWN"; break;
            // Add more cases as needed
        default: name = "Unknown VkResult"; break;
        }
        return fmt::format_to(ctx.out(), "{}", name);
    }
};

#define TOVKBOOL(x) ((x) ? VK_TRUE : VK_FALSE)


#define WTV_LOG(severity, message, ...)  GetServiceProvider()->GetService<ILogger>()->Log(severity, message, __VA_ARGS__);

#define WTV_LOG_D(message, ...)  GetServiceProvider()->GetService<ILogger>()->Log(ILogger::Severity::Debug, message, __VA_ARGS__);
#define WTV_LOG_I(message, ...)  GetServiceProvider()->GetService<ILogger>()->Log(ILogger::Severity::Info, message, __VA_ARGS__);
#define WTV_LOG_W(message, ...)  GetServiceProvider()->GetService<ILogger>()->Log(ILogger::Severity::Warn, message, __VA_ARGS__);
#define WTV_LOG_E(message, ...)  GetServiceProvider()->GetService<ILogger>()->Log(ILogger::Severity::Error, message, __VA_ARGS__);

#define ASSERT_VK_SUCCESS_ELSE_RET0(x) if((x) != VK_SUCCESS) { WTV_LOG_E("Assertion failed: {}\nFile: {}\nLine:{}\nFunction: {}", x, __FILE__, __LINE__, __func__); assert(false); return 0; }

#define ASSERT_VK_SUCCESS_ELSE_RET(x) if((x) != VK_SUCCESS) {  WTV_LOG_E("Assertion failed: {}\nFile: {}\nLine:{}\nFunction: {}", x, __FILE__, __LINE__, __func__); assert(false); return; }

#define ASSERT_VK_SUCCESS(x) if((x) != VK_SUCCESS) {  WTV_LOG_E("Assertion failed: {}\nFile: {}\nLine:{}\nFunction: {}", x, __FILE__, __LINE__, __func__); assert(false); }