/**
 * @file HfCanFrameRecord.h
 * @brief Frame-record tunnel codec (Vortex CANopen design §7.2), shared by
 *        the host pipe bus (PipePort), the Wi-Fi TLS tunnel and ESP-NOW.
 *
 * @details Datagram, all fields little-endian:
 *
 *              u8  ver = 1
 *              u8  flags        (HF_CAN_RECORD_DGRAM_*; 0 for data)
 *              u16 seq          (sender's datagram counter, wraps)
 *              u32 ack/epoch    (sender epoch; acknowledgement on reliable links)
 *              records...
 *
 *          Record (the transport form of CanFrame):
 *
 *              u32 id | EXT (bit 31) | RTR (bit 30)
 *              u8  flags        (FDF 0x04 | BRS 0x08 | ESI 0x10, same bits as CanFrame)
 *              u8  len          (payload bytes, 0..64)
 *              u8  data[len]
 *
 *          On a byte stream (UNIX socket, TLS) each datagram is preceded by
 *          a u16 little-endian length. ESP-NOW carries one datagram per
 *          action frame. The Python twin is `vortex_canopen.frame_record`.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <cstddef>
#include <cstdint>

#include "HfCanPort.h"

namespace hf::can::record {

inline constexpr uint8_t kVersion = 1U;
inline constexpr std::size_t kHeaderSize = 8U;
inline constexpr std::size_t kRecordHeaderSize = 6U;
inline constexpr std::size_t kMaxRecordSize = kRecordHeaderSize + kFdMaxLen;
/** Largest datagram this codec emits or accepts (stream length prefix is u16). */
inline constexpr std::size_t kMaxDatagram = 4096U;
inline constexpr uint32_t kIdExtBit = 0x80000000U;
inline constexpr uint32_t kIdRtrBit = 0x40000000U;
inline constexpr uint8_t kRecordFlagMask = frame_flag::kFdf | frame_flag::kBrs | frame_flag::kEsi;

/** Datagram flag: keep-alive, no records expected. */
inline constexpr uint8_t kDgramKeepAlive = 0x01U;

/** @brief Datagram header. */
struct Header {
  uint8_t version{kVersion};
  uint8_t flags{0};
  uint16_t seq{0};
  uint32_t epoch{0};
};

/** @brief Why a datagram was rejected. */
enum class DecodeError : uint8_t {
  None = 0,
  Truncated,      ///< shorter than the header, or a record runs past the end
  BadVersion,     ///< ver != 1
  BadLength,      ///< record len > 64
  BadFrame,       ///< record is not a well-formed frame (flags, id range, length)
};

inline void PutU16(uint8_t* p, uint16_t v) noexcept {
  p[0] = static_cast<uint8_t>(v);
  p[1] = static_cast<uint8_t>(v >> 8U);
}
inline void PutU32(uint8_t* p, uint32_t v) noexcept {
  p[0] = static_cast<uint8_t>(v);
  p[1] = static_cast<uint8_t>(v >> 8U);
  p[2] = static_cast<uint8_t>(v >> 16U);
  p[3] = static_cast<uint8_t>(v >> 24U);
}
inline uint16_t GetU16(const uint8_t* p) noexcept {
  return static_cast<uint16_t>(static_cast<uint16_t>(p[0]) | static_cast<uint16_t>(p[1] << 8U));
}
inline uint32_t GetU32(const uint8_t* p) noexcept {
  return static_cast<uint32_t>(p[0]) | (static_cast<uint32_t>(p[1]) << 8U) | (static_cast<uint32_t>(p[2]) << 16U) |
         (static_cast<uint32_t>(p[3]) << 24U);
}

/** @brief Write the header; @p out has kHeaderSize bytes. */
inline std::size_t EncodeHeader(uint8_t* out, const Header& h) noexcept {
  out[0] = h.version;
  out[1] = h.flags;
  PutU16(&out[2], h.seq);
  PutU32(&out[4], h.epoch);
  return kHeaderSize;
}

/** @brief Bytes the record of @p f takes. */
constexpr std::size_t RecordSize(const CanFrame& f) noexcept { return kRecordHeaderSize + f.len; }

/**
 * @brief Append the record of @p f.
 * @return bytes written, or 0 if @p cap is too small or the frame is malformed
 */
inline std::size_t EncodeRecord(uint8_t* out, std::size_t cap, const CanFrame& f) noexcept {
  if (!IsWellFormed(f) || cap < RecordSize(f)) {
    return 0U;
  }
  uint32_t id = f.id;
  if (f.IsExtended()) id |= kIdExtBit;
  if (f.IsRemote()) id |= kIdRtrBit;
  PutU32(&out[0], id);
  out[4] = static_cast<uint8_t>(f.flags & kRecordFlagMask);
  out[5] = f.len;
  for (uint8_t i = 0; i < f.len; ++i) {
    out[kRecordHeaderSize + i] = f.data[i];
  }
  return RecordSize(f);
}

/**
 * @brief Decode a datagram; @p on_frame(const CanFrame&) is called per record.
 * @details The whole datagram is validated before the first callback, so a
 *          malformed datagram delivers nothing.
 */
template <typename OnFrame>
DecodeError DecodeDatagram(const uint8_t* p, std::size_t n, Header* header, OnFrame&& on_frame) {
  if (n < kHeaderSize) return DecodeError::Truncated;
  if (p[0] != kVersion) return DecodeError::BadVersion;
  Header h;
  h.version = p[0];
  h.flags = p[1];
  h.seq = GetU16(&p[2]);
  h.epoch = GetU32(&p[4]);
  for (int pass = 0; pass < 2; ++pass) {
    std::size_t off = kHeaderSize;
    while (off < n) {
      if (n - off < kRecordHeaderSize) return DecodeError::Truncated;
      const uint32_t raw = GetU32(&p[off]);
      CanFrame f;
      f.flags = static_cast<uint8_t>(p[off + 4U] & kRecordFlagMask);
      if ((p[off + 4U] & static_cast<uint8_t>(~kRecordFlagMask)) != 0U) return DecodeError::BadFrame;
      f.len = p[off + 5U];
      if (f.len > kFdMaxLen) return DecodeError::BadLength;
      if (n - off - kRecordHeaderSize < f.len) return DecodeError::Truncated;
      if ((raw & kIdExtBit) != 0U) f.flags |= frame_flag::kExt;
      if ((raw & kIdRtrBit) != 0U) f.flags |= frame_flag::kRtr;
      f.id = raw & ~(kIdExtBit | kIdRtrBit);
      if (!IsWellFormed(f)) return DecodeError::BadFrame;
      if (pass == 1) {
        for (uint8_t i = 0; i < f.len; ++i) {
          f.data[i] = p[off + kRecordHeaderSize + i];
        }
        on_frame(static_cast<const CanFrame&>(f));
      }
      off += kRecordHeaderSize + f.len;
    }
  }
  if (header != nullptr) *header = h;
  return DecodeError::None;
}

}  // namespace hf::can::record
