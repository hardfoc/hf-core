/**
 * @file HfResult.h
 * @brief `hf::Result<T>`: a value or an `hf::Status` plus the layer's own error code.
 *
 * Shaped like `std::expected<T, E>` without exceptions or heap:
 *
 * @code
 * hf::Result<float> ReadCelsius() noexcept {
 *     std::uint16_t raw = 0;
 *     if (!port.Read(raw)) {
 *         return hf::Fail(hf::Status::Bus, kMyReadFailed);  // failure: status + detail
 *     }
 *     return raw * 0.0625F;                                 // success: the value
 * }
 *
 * const auto t = ReadCelsius();
 * if (!t.ok()) {
 *     log("%s (detail %lu)", hf::ToString(t.status()), static_cast<unsigned long>(t.detail()));
 * }
 * @endcode
 *
 * - `detail()` is the code of the layer that produced the failure (a driver's `Error` value, an
 *   `hf_*_err_t`), kept for logs and diagnostics. It is 0 on success.
 * - `T` must be default-constructible: a failed `Result<T>` holds `T{}`, so `value()` is always defined
 *   (this is the model of the drivers' `Result<T>`). `value_or()` makes the fallback explicit.
 * - `Result<T>` is trivially copyable whenever `T` is. `Result<void>` is 8 bytes.
 * - Both are `[[nodiscard]]`: ignoring a returned result is a compile warning (an error in hf-core).
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */
#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>

#include "HfStatus.h"

namespace hf {

/// The failure half of a `Result`: what went wrong, coarse (`status`) and layer-native (`detail`).
struct Error {
    Status status{Status::Unknown}; ///< Never `Status::Ok` once inside a `Result`.
    std::uint32_t detail{0};        ///< The producing layer's own error code.
};

/**
 * @brief Build a failure, to return from a function that returns any `Result`.
 * @param status What went wrong. `Status::Ok` is not a failure and is stored as `Status::Unknown`.
 * @param detail The layer-native error code (default 0).
 */
[[nodiscard]] constexpr Error Fail(Status status, std::uint32_t detail = 0) noexcept {
    return Error{status == Status::Ok ? Status::Unknown : status, detail};
}

/**
 * @brief A `T` on success, or an `hf::Error` on failure.
 * @tparam T Value type. Not a reference, not `hf::Error`, default-constructible.
 */
template <typename T>
class [[nodiscard]] Result {
    static_assert(!std::is_reference_v<T>, "hf::Result<T&> is not supported; return a pointer or a value");
    static_assert(!std::is_same_v<std::remove_cv_t<T>, Error>, "hf::Result<hf::Error> is ambiguous");
    static_assert(std::is_default_constructible_v<T>,
                  "hf::Result<T> needs a default-constructible T: a failed result holds T{}");

public:
    using value_type = T;

    /// Success holding @p value.
    constexpr Result(const T& value) noexcept(std::is_nothrow_copy_constructible_v<T>) // NOLINT: implicit
        : value_(value) {}

    /// Success holding @p value.
    constexpr Result(T&& value) noexcept(std::is_nothrow_move_constructible_v<T>) // NOLINT: implicit
        : value_(std::move(value)) {}

    /// Failure; the value is `T{}`.
    constexpr Result(Error error) noexcept(std::is_nothrow_default_constructible_v<T>) // NOLINT: implicit
        : value_{}, status_(error.status == Status::Ok ? Status::Unknown : error.status), detail_(error.detail) {}

    /// True on success.
    [[nodiscard]] constexpr bool ok() const noexcept {
        return status_ == Status::Ok;
    }
    /// Same as `ok()`.
    constexpr explicit operator bool() const noexcept {
        return ok();
    }

    /// `Status::Ok` on success, otherwise what went wrong.
    [[nodiscard]] constexpr Status status() const noexcept {
        return status_;
    }
    /// The producing layer's own error code; 0 on success.
    [[nodiscard]] constexpr std::uint32_t detail() const noexcept {
        return detail_;
    }
    /// Status and detail together, to forward a failure: `return r.error();`.
    [[nodiscard]] constexpr Error error() const noexcept {
        return Error{status_, detail_};
    }

    /// The value; `T{}` after a failure.
    [[nodiscard]] constexpr T& value() & noexcept {
        return value_;
    }
    /// The value; `T{}` after a failure.
    [[nodiscard]] constexpr const T& value() const& noexcept {
        return value_;
    }
    /// The value, moved out; `T{}` after a failure.
    [[nodiscard]] constexpr T&& value() && noexcept {
        return std::move(value_);
    }

    /// The value on success, otherwise @p fallback.
    template <typename U>
    [[nodiscard]] constexpr T value_or(U&& fallback) const& noexcept {
        return ok() ? value_ : static_cast<T>(std::forward<U>(fallback));
    }

    /// Access to the value, as with `std::expected`.
    [[nodiscard]] constexpr T& operator*() & noexcept {
        return value_;
    }
    /// Access to the value, as with `std::expected`.
    [[nodiscard]] constexpr const T& operator*() const& noexcept {
        return value_;
    }
    /// Member access to the value.
    [[nodiscard]] constexpr T* operator->() noexcept {
        return &value_;
    }
    /// Member access to the value.
    [[nodiscard]] constexpr const T* operator->() const noexcept {
        return &value_;
    }

private:
    T value_;
    Status status_{Status::Ok};
    std::uint32_t detail_{0};
};

/// Success or failure, without a value. A default-constructed `Result<void>` is a success.
template <>
class [[nodiscard]] Result<void> {
public:
    using value_type = void;

    /// Success.
    constexpr Result() noexcept = default;

    /// Failure.
    constexpr Result(Error error) noexcept // NOLINT: implicit, as for Result<T>
        : status_(error.status == Status::Ok ? Status::Unknown : error.status), detail_(error.detail) {}

    /// True on success.
    [[nodiscard]] constexpr bool ok() const noexcept {
        return status_ == Status::Ok;
    }
    /// Same as `ok()`.
    constexpr explicit operator bool() const noexcept {
        return ok();
    }
    /// `Status::Ok` on success, otherwise what went wrong.
    [[nodiscard]] constexpr Status status() const noexcept {
        return status_;
    }
    /// The producing layer's own error code; 0 on success.
    [[nodiscard]] constexpr std::uint32_t detail() const noexcept {
        return detail_;
    }
    /// Status and detail together, to forward a failure.
    [[nodiscard]] constexpr Error error() const noexcept {
        return Error{status_, detail_};
    }

private:
    Status status_{Status::Ok};
    std::uint32_t detail_{0};
};

/// A successful `Result<void>`: `return hf::Ok();`.
[[nodiscard]] constexpr Result<void> Ok() noexcept {
    return {};
}

/// True when @p T is an `hf::Result` specialisation.
template <typename T>
inline constexpr bool kIsResult = false;
/// True when @p T is an `hf::Result` specialisation.
template <typename T>
inline constexpr bool kIsResult<Result<T>> = true;

} // namespace hf
