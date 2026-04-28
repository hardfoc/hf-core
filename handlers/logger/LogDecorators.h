#pragma once

#include "Logger.h"

namespace logger_decor {

inline void BannerColored(const char* tag, const char* line1,
                          LogColor color) noexcept {
    auto& logger = Logger::GetInstance();
    logger.Info(tag, "");
    logger.Info(tag, color, LogStyle::BOLD,
                "╔══════════════════════════════════════════════════════════╗");
    logger.Info(tag, color, LogStyle::BOLD,
                "║  %-54s  ║", line1);
    logger.Info(tag, color, LogStyle::BOLD,
                "╚══════════════════════════════════════════════════════════╝");
    logger.Info(tag, "");
}

inline void Banner(const char* tag, const char* line1) noexcept {
    BannerColored(tag, line1, LogColor::CYAN);
}

inline void Section(const char* tag, const char* title) noexcept {
    Logger::GetInstance().Info(tag, LogColor::BRIGHT_BLUE, LogStyle::BOLD,
                               "── %s ──", title);
}

inline void Rule(const char* tag) noexcept {
    Logger::GetInstance().Info(tag, "============================================================");
}

}  // namespace logger_decor