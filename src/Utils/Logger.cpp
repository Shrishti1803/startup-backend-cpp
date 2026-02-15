#include "Utils/Logger.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

static std::shared_ptr<spdlog::logger> app_logger;
static std::shared_ptr<spdlog::logger> auth_logger;
static std::shared_ptr<spdlog::logger> db_logger;

void Logger::init() {
    // common log format
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v");
    spdlog::set_level(spdlog::level::info);

    // create loggers
    app_logger  = spdlog::basic_logger_mt("APP",  "logs/app.log");
    auth_logger = spdlog::basic_logger_mt("AUTH", "logs/auth.log");
    db_logger   = spdlog::basic_logger_mt("DB",   "logs/db.log");

    app_logger->flush_on(spdlog::level::info);
    auth_logger->flush_on(spdlog::level::info);
    db_logger->flush_on(spdlog::level::info);

    app_logger->info("Application logger initialized");
}

std::shared_ptr<spdlog::logger> Logger::app() {
    return app_logger;
}

std::shared_ptr<spdlog::logger> Logger::auth() {
    return auth_logger;
}

std::shared_ptr<spdlog::logger> Logger::db() {
    return db_logger;
}
