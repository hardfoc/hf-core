/**
 * @file Logger.cpp
 * @brief Implementation of the advanced logging system.
 *
 * This file provides the implementation for the Logger class,
 * which supports various formatting options including colors,
 * styles, and ASCII art display.
 *
 * @author Nebiyu Tadesse
 * @date 2025
 * @copyright HardFOC
 */

#include "Logger.h"

// Include the base logger from the internal interface wrapper
#include "../../hf-core-drivers/internal/hf-internal-interface-wrap/inc/base/BaseLogger.h"
#ifdef HF_MCU_FAMILY_ESP32
#include "../../hf-core-drivers/internal/hf-internal-interface-wrap/inc/mcu/esp32/EspLogger.h"
#endif

#include <cstdarg>
#include <cstring>
#include <algorithm>
#include <cstdio>

//==============================================================================
// CONSTRUCTOR/DESTRUCTOR
//==============================================================================

Logger::Logger() noexcept 
    : initialized_(false)
    , config_()
    , tag_levels_{}
    , base_logger_(nullptr) {
    
    // Set default configuration
    config_.level = LogLevel::INFO;
    config_.color = LogColor::DEFAULT;
    config_.background = LogBackground::DEFAULT;
    config_.style = LogStyle::NORMAL;
    config_.enable_colors = true;
    config_.enable_effects = true;
    config_.max_width = 80;
    config_.center_text = false;
    config_.add_border = false;
    config_.border_char = '*';
    config_.border_padding = 2;
    config_.enable_ascii_art = true;
    config_.format_ascii_art = true;
}

Logger::~Logger() noexcept {
    Deinitialize();
}

//==============================================================================
// SINGLETON ACCESS
//==============================================================================

Logger& Logger::GetInstance() noexcept {
    static Logger instance;
    return instance;
}

//==============================================================================
// INITIALIZATION
//==============================================================================

bool Logger::Initialize(const LogConfig& config) noexcept {
    if (initialized_.load()) {
        return true; // Already initialized
    }

    config_ = config;
    
    // Create base logger
    base_logger_ = CreateBaseLogger();
    if (!base_logger_) {
        return false;
    }

    // Initialize base logger with the fields that hf_logger_config_t actually has
    hf_logger_config_t base_config{};
    base_config.default_level = static_cast<hf_log_level_t>(config.level);
    base_config.enable_thread_safety = true;
    
    if (base_logger_->Initialize(base_config) != hf_logger_err_t::LOGGER_SUCCESS) {
        return false;
    }

    initialized_.store(true);
    return true;
}

void Logger::Deinitialize() noexcept {
    if (!initialized_.load()) {
        return;
    }

    if (base_logger_) {
        base_logger_->Deinitialize();
        base_logger_.reset();
    }

    tag_levels_[0].in_use = false; // Clear sentinel
    for (auto& tl : tag_levels_) { tl.in_use = false; }
    initialized_.store(false);
}

bool Logger::IsInitialized() const noexcept {
    return initialized_.load();
}

//==============================================================================
// LOG LEVEL MANAGEMENT
//==============================================================================

void Logger::SetLogLevel(const char* tag, LogLevel level) noexcept {
    if (!initialized_.load() || tag == nullptr) {
        return;
    }

    // Search for existing entry or first free slot
    int free_slot = -1;
    for (size_t i = 0; i < kMaxTagLevels; ++i) {
        if (tag_levels_[i].in_use && std::strncmp(tag_levels_[i].tag, tag, kMaxTagLength - 1) == 0) {
            tag_levels_[i].level = level;
            if (base_logger_) {
                base_logger_->SetLogLevel(tag, static_cast<hf_log_level_t>(level));
            }
            return;
        }
        if (!tag_levels_[i].in_use && free_slot < 0) {
            free_slot = static_cast<int>(i);
        }
    }

    // Insert into first free slot
    if (free_slot >= 0) {
        std::strncpy(tag_levels_[free_slot].tag, tag, kMaxTagLength - 1);
        tag_levels_[free_slot].tag[kMaxTagLength - 1] = '\0';
        tag_levels_[free_slot].level = level;
        tag_levels_[free_slot].in_use = true;
    }

    if (base_logger_) {
        base_logger_->SetLogLevel(tag, static_cast<hf_log_level_t>(level));
    }
}

LogLevel Logger::GetLogLevel(const char* tag) const noexcept {
    if (!initialized_.load() || tag == nullptr) {
        return config_.level;
    }

    for (size_t i = 0; i < kMaxTagLevels; ++i) {
        if (tag_levels_[i].in_use && std::strncmp(tag_levels_[i].tag, tag, kMaxTagLength - 1) == 0) {
            return tag_levels_[i].level;
        }
    }

    return config_.level;
}

//==============================================================================
// BASIC LOGGING METHODS
//==============================================================================

void Logger::Error(const char* tag, const char* format, ...) noexcept {
    if (!IsLevelEnabled(LogLevel::ERROR, tag)) {
        return;
    }

    va_list args;
    va_start(args, format);
    LogInternal(LogLevel::ERROR, tag, config_.color, config_.style, format, args);
    va_end(args);
}

void Logger::Warn(const char* tag, const char* format, ...) noexcept {
    if (!IsLevelEnabled(LogLevel::WARN, tag)) {
        return;
    }

    va_list args;
    va_start(args, format);
    LogInternal(LogLevel::WARN, tag, config_.color, config_.style, format, args);
    va_end(args);
}

void Logger::Info(const char* tag, const char* format, ...) noexcept {
    if (!IsLevelEnabled(LogLevel::INFO, tag)) {
        return;
    }

    va_list args;
    va_start(args, format);
    LogInternal(LogLevel::INFO, tag, config_.color, config_.style, format, args);
    va_end(args);
}

void Logger::Debug(const char* tag, const char* format, ...) noexcept {
    if (!IsLevelEnabled(LogLevel::DEBUG, tag)) {
        return;
    }

    va_list args;
    va_start(args, format);
    LogInternal(LogLevel::DEBUG, tag, config_.color, config_.style, format, args);
    va_end(args);
}

void Logger::Verbose(const char* tag, const char* format, ...) noexcept {
    if (!IsLevelEnabled(LogLevel::VERBOSE, tag)) {
        return;
    }

    va_list args;
    va_start(args, format);
    LogInternal(LogLevel::VERBOSE, tag, config_.color, config_.style, format, args);
    va_end(args);
}

//==============================================================================
// FORMATTED LOGGING METHODS
//==============================================================================

void Logger::Error(const char* tag, LogColor color, LogStyle style, const char* format, ...) noexcept {
    if (!IsLevelEnabled(LogLevel::ERROR, tag)) {
        return;
    }

    va_list args;
    va_start(args, format);
    LogInternal(LogLevel::ERROR, tag, color, style, format, args);
    va_end(args);
}

void Logger::Warn(const char* tag, LogColor color, LogStyle style, const char* format, ...) noexcept {
    if (!IsLevelEnabled(LogLevel::WARN, tag)) {
        return;
    }

    va_list args;
    va_start(args, format);
    LogInternal(LogLevel::WARN, tag, color, style, format, args);
    va_end(args);
}

void Logger::Info(const char* tag, LogColor color, LogStyle style, const char* format, ...) noexcept {
    if (!IsLevelEnabled(LogLevel::INFO, tag)) {
        return;
    }

    va_list args;
    va_start(args, format);
    LogInternal(LogLevel::INFO, tag, color, style, format, args);
    va_end(args);
}

void Logger::Debug(const char* tag, LogColor color, LogStyle style, const char* format, ...) noexcept {
    if (!IsLevelEnabled(LogLevel::DEBUG, tag)) {
        return;
    }

    va_list args;
    va_start(args, format);
    LogInternal(LogLevel::DEBUG, tag, color, style, format, args);
    va_end(args);
}

void Logger::Verbose(const char* tag, LogColor color, LogStyle style, const char* format, ...) noexcept {
    if (!IsLevelEnabled(LogLevel::VERBOSE, tag)) {
        return;
    }

    va_list args;
    va_start(args, format);
    LogInternal(LogLevel::VERBOSE, tag, color, style, format, args);
    va_end(args);
}

//==============================================================================
// ASCII ART LOGGING METHODS
//==============================================================================

void Logger::LogAsciiArt(const char* tag, const char* ascii_art, 
                        const AsciiArtFormat& format) noexcept {
    if (!config_.enable_ascii_art || !IsLevelEnabled(LogLevel::INFO, tag)) {
        return;
    }

    FormatAndLogAsciiArt(tag, LogLevel::INFO, ascii_art, format);
}

void Logger::LogAsciiArt(LogLevel level, const char* tag, const char* ascii_art, 
                        const AsciiArtFormat& format) noexcept {
    if (!config_.enable_ascii_art || !IsLevelEnabled(level, tag)) {
        return;
    }

    FormatAndLogAsciiArt(tag, level, ascii_art, format);
}

void Logger::LogBanner(const char* tag, const char* ascii_art, 
                      const AsciiArtFormat& format) noexcept {
    if (!config_.enable_ascii_art || !IsLevelEnabled(LogLevel::INFO, tag)) {
        return;
    }

    // Create a banner format with default styling
    AsciiArtFormat banner_format = format;
    if (banner_format.color == LogColor::DEFAULT) {
        banner_format.color = LogColor::BRIGHT_CYAN;
    }
    if (banner_format.style == LogStyle::NORMAL) {
        banner_format.style = LogStyle::BOLD;
    }
    if (!banner_format.center_art) {
        banner_format.center_art = true;
    }
    if (!banner_format.add_border) {
        banner_format.add_border = true;
        banner_format.border_char = '=';
        banner_format.border_padding = 1;
    }

    LogAsciiArt(tag, ascii_art, banner_format);
}

//==============================================================================
// UTILITY METHODS
//==============================================================================

void Logger::SetConfig(const LogConfig& config) noexcept {
    config_ = config;
    
    if (base_logger_) {
        hf_logger_config_t base_config{};
        base_config.default_level = static_cast<hf_log_level_t>(config.level);
        base_config.enable_thread_safety = true;
        
        base_logger_->Initialize(base_config);
    }
}

LogConfig Logger::GetConfig() const noexcept {
    return config_;
}

void Logger::EnableColors(bool enable) noexcept {
    config_.enable_colors = enable;
    if (base_logger_) {
        hf_logger_config_t base_config{};
        base_config.default_level = static_cast<hf_log_level_t>(config_.level);
        base_config.enable_thread_safety = true;
        base_logger_->Initialize(base_config);
    }
}

void Logger::EnableEffects(bool enable) noexcept {
    config_.enable_effects = enable;
    if (base_logger_) {
        hf_logger_config_t base_config{};
        base_config.default_level = static_cast<hf_log_level_t>(config_.level);
        base_config.enable_thread_safety = true;
        base_logger_->Initialize(base_config);
    }
}

void Logger::EnableAsciiArt(bool enable) noexcept {
    config_.enable_ascii_art = enable;
}

void Logger::Flush() noexcept {
    if (base_logger_) {
        base_logger_->Flush();
    }
}

//==============================================================================
// PRIVATE METHODS
//==============================================================================

void Logger::LogInternal(LogLevel level, const char* tag, LogColor color, LogStyle style, 
                        const char* format, va_list args) noexcept {
    if (!initialized_.load() || !base_logger_) {
        return;
    }

    // Format the message into a stack buffer
    char msg_buf[1024];
    vsnprintf(msg_buf, sizeof(msg_buf), format, args);

    // Add color codes if enabled and any non-default formatting requested
    if (config_.enable_colors && 
        (color != LogColor::DEFAULT || config_.background != LogBackground::DEFAULT || style != LogStyle::NORMAL)) {
        char formatted[1200]; // msg_buf + room for ANSI escape codes
        size_t pos = WriteColorPrefix(formatted, sizeof(formatted), color, config_.background, style);
        size_t msg_len = std::strlen(msg_buf);
        if (pos + msg_len < sizeof(formatted) - 16) {
            std::memcpy(formatted + pos, msg_buf, msg_len);
            pos += msg_len;
            pos += WriteResetSequence(formatted + pos, sizeof(formatted) - pos);
            formatted[pos] = '\0';
            base_logger_->Log(static_cast<hf_log_level_t>(level), tag, "%s", formatted);
        } else {
            base_logger_->Log(static_cast<hf_log_level_t>(level), tag, "%s", msg_buf);
        }
    } else {
        base_logger_->Log(static_cast<hf_log_level_t>(level), tag, "%s", msg_buf);
    }
}

size_t Logger::WriteColorPrefix(char* buf, size_t buf_size, LogColor color,
                                LogBackground background, LogStyle style) const noexcept {
    size_t pos = 0;
    // Color sequence: \033[XXXm
    if (color != LogColor::DEFAULT && pos + 8 < buf_size) {
        int n = snprintf(buf + pos, buf_size - pos, "\033[%um", static_cast<unsigned>(color));
        if (n > 0) pos += static_cast<size_t>(n);
    }
    // Background sequence
    if (background != LogBackground::DEFAULT && pos + 8 < buf_size) {
        int n = snprintf(buf + pos, buf_size - pos, "\033[%um", static_cast<unsigned>(background));
        if (n > 0) pos += static_cast<size_t>(n);
    }
    // Style sequence
    if (style != LogStyle::NORMAL && pos + 8 < buf_size) {
        uint8_t code = 0;
        switch (style) {
            case LogStyle::BOLD:             code = 1; break;
            case LogStyle::ITALIC:           code = 3; break;
            case LogStyle::UNDERLINE:        code = 4; break;
            case LogStyle::STRIKETHROUGH:    code = 9; break;
            case LogStyle::DOUBLE_UNDERLINE: code = 21; break;
            default: break;
        }
        if (code != 0) {
            int n = snprintf(buf + pos, buf_size - pos, "\033[%um", static_cast<unsigned>(code));
            if (n > 0) pos += static_cast<size_t>(n);
        }
    }
    if (pos < buf_size) buf[pos] = '\0';
    return pos;
}

size_t Logger::WriteResetSequence(char* buf, size_t buf_size) noexcept {
    static constexpr const char kReset[] = "\033[0m";
    static constexpr size_t kLen = sizeof(kReset) - 1;
    if (kLen < buf_size) {
        std::memcpy(buf, kReset, kLen);
        buf[kLen] = '\0';
        return kLen;
    }
    return 0;
}

void Logger::FormatAndLogAsciiArt(const char* tag, LogLevel level,
                                  const char* ascii_art,
                                  const AsciiArtFormat& format) noexcept {
    if (!ascii_art || !base_logger_) return;
    const hf_log_level_t base_level = static_cast<hf_log_level_t>(level);

    // First pass: scan lines and find max length
    static constexpr size_t kMaxLines = 64;
    struct LineInfo { const char* start; size_t length; };
    LineInfo lines[kMaxLines];
    size_t line_count = 0;
    size_t max_length = 0;

    const char* p = ascii_art;
    const char* line_start = p;
    while (*p && line_count < kMaxLines) {
        if (*p == '\n') {
            size_t len = static_cast<size_t>(p - line_start);
            lines[line_count++] = {line_start, len};
            if (len > max_length) max_length = len;
            line_start = p + 1;
        }
        ++p;
    }
    if (line_start < p && line_count < kMaxLines) {
        size_t len = static_cast<size_t>(p - line_start);
        lines[line_count++] = {line_start, len};
        if (len > max_length) max_length = len;
    }

    // Prepare ANSI prefix/suffix if formatting enabled
    char prefix[64] = {};
    char suffix[16] = {};
    size_t prefix_len = 0;
    size_t suffix_len = 0;
    if (config_.enable_colors && config_.format_ascii_art) {
        prefix_len = WriteColorPrefix(prefix, sizeof(prefix), format.color, format.background, format.style);
        suffix_len = WriteResetSequence(suffix, sizeof(suffix));
    }

    // Centering padding
    size_t center_pad = 0;
    if (format.center_art && format.max_width > max_length) {
        center_pad = (format.max_width - max_length) / 2;
    }

    // Helper: assemble and log one output line
    auto emit_line = [&](const char* content, size_t content_len) {
        char out[512];
        size_t pos = 0;
        if (prefix_len > 0 && pos + prefix_len < sizeof(out)) {
            std::memcpy(out + pos, prefix, prefix_len);
            pos += prefix_len;
        }
        if (content_len > 0 && pos + content_len < sizeof(out)) {
            std::memcpy(out + pos, content, content_len);
            pos += content_len;
        }
        if (suffix_len > 0 && pos + suffix_len < sizeof(out)) {
            std::memcpy(out + pos, suffix, suffix_len);
            pos += suffix_len;
        }
        out[pos] = '\0';
        base_logger_->Log(base_level, tag, "%s", out);
    };

    if (format.add_border) {
        size_t border_content_w = max_length + 2;
        size_t total_w = border_content_w + 2 * format.border_padding;
        char border_line[512];
        size_t bw = (total_w < sizeof(border_line) - 1) ? total_w : sizeof(border_line) - 1;
        std::memset(border_line, format.border_char, bw);
        border_line[bw] = '\0';

        emit_line(border_line, bw);
        for (size_t i = 0; i < format.border_padding; ++i) emit_line(border_line, bw);

        for (size_t i = 0; i < line_count; ++i) {
            char row[512];
            size_t rp = 0;
            for (size_t j = 0; j < format.border_padding && rp < sizeof(row) - 1; ++j)
                row[rp++] = format.border_char;
            if (rp < sizeof(row) - 1) row[rp++] = ' ';
            for (size_t j = 0; j < center_pad && rp < sizeof(row) - 1; ++j)
                row[rp++] = ' ';
            size_t clen = lines[i].length;
            if (rp + clen >= sizeof(row) - 1) clen = sizeof(row) - 1 - rp;
            std::memcpy(row + rp, lines[i].start, clen);
            rp += clen;
            for (size_t j = lines[i].length; j < max_length && rp < sizeof(row) - 1; ++j)
                row[rp++] = ' ';
            if (rp < sizeof(row) - 1) row[rp++] = ' ';
            for (size_t j = 0; j < format.border_padding && rp < sizeof(row) - 1; ++j)
                row[rp++] = format.border_char;
            row[rp] = '\0';
            emit_line(row, rp);
        }

        for (size_t i = 0; i < format.border_padding; ++i) emit_line(border_line, bw);
        emit_line(border_line, bw);
    } else {
        for (size_t i = 0; i < line_count; ++i) {
            if (lines[i].length == 0) continue;
            char row[512];
            size_t rp = 0;
            for (size_t j = 0; j < center_pad && rp < sizeof(row) - 1; ++j)
                row[rp++] = ' ';
            size_t clen = lines[i].length;
            if (rp + clen >= sizeof(row) - 1) clen = sizeof(row) - 1 - rp;
            std::memcpy(row + rp, lines[i].start, clen);
            rp += clen;
            row[rp] = '\0';
            emit_line(row, rp);
        }
    }
}

bool Logger::IsLevelEnabled(LogLevel level, const char* tag) const noexcept {
    if (!initialized_.load()) {
        return false;
    }

    LogLevel tag_level = GetLogLevel(tag);
    return static_cast<uint8_t>(level) <= static_cast<uint8_t>(tag_level);
}

std::unique_ptr<BaseLogger> Logger::CreateBaseLogger() noexcept {
    // Create an EspLogger directly
#ifdef HF_MCU_FAMILY_ESP32
    return std::make_unique<EspLogger>();
#else
    return nullptr; // Fallback for other platforms
#endif
}

void Logger::DumpStatistics() const noexcept {
    static constexpr const char* TAG = "Logger";
    
    // Use direct output since we're logging the logger itself
    if (!base_logger_) {
        printf("[%s] ERROR: Logger not initialized, cannot dump statistics\n", TAG);
        return;
    }
    
    printf("[%s] INFO: === LOGGER STATISTICS ===\n", TAG);
    
    // System Health
    printf("[%s] INFO: System Health:\n", TAG);
    printf("[%s] INFO:   Initialized: %s\n", TAG, initialized_ ? "YES" : "NO");
    printf("[%s] INFO:   Base Logger: %s\n", TAG, base_logger_ ? "ACTIVE" : "INACTIVE");
    
    // Configuration
    printf("[%s] INFO: Configuration:\n", TAG);
    printf("[%s] INFO:   Default Log Level: %s\n", TAG, 
        config_.level == LogLevel::ERROR ? "ERROR" :
        config_.level == LogLevel::WARN ? "WARN" :
        config_.level == LogLevel::INFO ? "INFO" :
        config_.level == LogLevel::DEBUG ? "DEBUG" :
        config_.level == LogLevel::VERBOSE ? "VERBOSE" : "UNKNOWN");
    
    printf("[%s] INFO:   Colors Enabled: %s\n", TAG, config_.enable_colors ? "YES" : "NO");
    printf("[%s] INFO:   Effects Enabled: %s\n", TAG, config_.enable_effects ? "YES" : "NO");
    printf("[%s] INFO:   ASCII Art Enabled: %s\n", TAG, config_.enable_ascii_art ? "YES" : "NO");
    printf("[%s] INFO:   Max Width: %u\n", TAG, static_cast<unsigned>(config_.max_width));
    printf("[%s] INFO:   Border Character: '%c'\n", TAG, config_.border_char);
    
    // Tag-specific levels
    printf("[%s] INFO: Tag-specific Log Levels:\n", TAG);
    size_t tag_count = 0;
    for (size_t i = 0; i < kMaxTagLevels; ++i) {
        if (tag_levels_[i].in_use) ++tag_count;
    }
    if (tag_count == 0) {
        printf("[%s] INFO:   No tag-specific levels configured\n", TAG);
    } else {
        printf("[%s] INFO:   %d tag-specific levels configured:\n", TAG, static_cast<int>(tag_count));
        int printed = 0;
        for (size_t i = 0; i < kMaxTagLevels; ++i) {
            if (!tag_levels_[i].in_use) continue;
            if (printed >= 10) {
                printf("[%s] INFO:   ... and %d more\n", TAG, static_cast<int>(tag_count) - printed);
                break;
            }
            const char* level_str = 
                tag_levels_[i].level == LogLevel::ERROR ? "ERROR" :
                tag_levels_[i].level == LogLevel::WARN ? "WARN" :
                tag_levels_[i].level == LogLevel::INFO ? "INFO" :
                tag_levels_[i].level == LogLevel::DEBUG ? "DEBUG" :
                tag_levels_[i].level == LogLevel::VERBOSE ? "VERBOSE" : "UNKNOWN";
            printf("[%s] INFO:     %s: %s\n", TAG, tag_levels_[i].tag, level_str);
            printed++;
        }
    }
    
    // Memory Usage
    printf("[%s] INFO: Memory Usage:\n", TAG);
    size_t config_memory = sizeof(config_);
    size_t tag_levels_memory = sizeof(tag_levels_);
    size_t total_memory = sizeof(*this);
    
    printf("[%s] INFO:   Logger Instance: %d bytes\n", TAG, static_cast<int>(sizeof(*this)));
    printf("[%s] INFO:   Configuration: %d bytes\n", TAG, static_cast<int>(config_memory));
    printf("[%s] INFO:   Tag Levels Array: %d bytes\n", TAG, static_cast<int>(tag_levels_memory));
    printf("[%s] INFO:   Total Estimated: %d bytes\n", TAG, static_cast<int>(total_memory));
    
    // Platform Information
    printf("[%s] INFO: Platform Information:\n", TAG);
#ifdef HF_MCU_FAMILY_ESP32
    printf("[%s] INFO:   Platform: ESP32\n", TAG);
    printf("[%s] INFO:   Base Logger Type: EspLogger\n", TAG);
#else
    printf("[%s] INFO:   Platform: Other\n", TAG);
    printf("[%s] INFO:   Base Logger Type: Generic\n", TAG);
#endif
    
    // Feature Support
    printf("[%s] INFO: Feature Support:\n", TAG);
    printf("[%s] INFO:   ANSI Colors: %s\n", TAG, "SUPPORTED");
    printf("[%s] INFO:   Text Styles: %s\n", TAG, "SUPPORTED");
    printf("[%s] INFO:   ASCII Art: %s\n", TAG, "SUPPORTED");
    printf("[%s] INFO:   Formatted Output: %s\n", TAG, "SUPPORTED");
    printf("[%s] INFO:   Thread Safety: %s\n", TAG, "SUPPORTED");
    
    printf("[%s] INFO: === END LOGGER STATISTICS ===\n", TAG);
} 