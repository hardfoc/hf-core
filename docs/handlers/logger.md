---
layout: default
title: Logger
parent: Handlers
nav_order: 7
---

# Logger

Singleton logging system with color output, ASCII art, and per-tag filtering.

## Access

```cpp
auto& logger = Logger::GetInstance();
logger.Initialize();  // Optional config: Logger::GetInstance().Initialize(config);
```

## Log Methods

```cpp
logger.Error("MyTag", "Error: code %d", err);
logger.Warn("MyTag", "Warning: value %.2f", val);
logger.Info("MyTag", "Info: initialized");
logger.Debug("MyTag", "Debug: state=%d", state);
logger.Verbose("MyTag", "Verbose: raw=0x%04X", raw);
```

## Formatted Output

```cpp
logger.Info("Tag", LogColor::GREEN, LogStyle::BOLD,
            "Bold green: %d", value);
```

## ASCII Art

```cpp
logger.LogBanner("Tag", R"(
  _   _ _____ _____ _____ 
 | | | |  ___|  _  |  _  |
 | |_| | |__ | | | | |_| |
)");
```

## Per-Tag Filtering

```cpp
logger.SetLogLevel("NoisyModule", LogLevel::WARN);  // Only WARN+ for this tag
```

## Convenience Macros

```cpp
LOG_ERROR("tag", "message %d", val);
LOG_WARN("tag", "message");
LOG_INFO("tag", "message");
LOG_DEBUG("tag", "message");
LOG_VERBOSE("tag", "message");
```

## Thread Safety

The Logger is thread-safe and can be called from any RTOS task or ISR context
(for short messages).

## Test Coverage

See `examples/esp32/main/utils_tests/logger_comprehensive_test.cpp`.
