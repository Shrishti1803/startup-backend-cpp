#pragma once

#include <memory>
#include <spdlog/logger.h>

class Logger {
public:
    // must be called once at startup
    static void init();

    // accessors
    static std::shared_ptr<spdlog::logger> app();
    static std::shared_ptr<spdlog::logger> auth();
    static std::shared_ptr<spdlog::logger> db();

private:
    // prevent instantiation
    Logger() = delete;
};
