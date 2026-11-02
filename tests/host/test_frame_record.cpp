/**
 * @file test_frame_record.cpp
 * @brief §7.2 frame-record codec (HfCanFrameRecord.h): wire format, round
 *        trips and rejection of malformed datagrams.
 *
 * kGolden is the same byte string the Python twin
 * (hardfoc-vortex-v1 tools/vortex_canopen/vortex_canopen/frame_record.py)
 * is tested against, so both sides agree on the wire format.
 */

#include <cstring>
#include <vector>

#include "HfCanFrameRecord.h"
#include "HostTest.h"

using hf::can::CanFrame;
namespace rec = hf::can::record;
namespace ff = hf::can::frame_flag;

namespace {

CanFrame Make(uint32_t id, uint8_t flags, const std::vector<uint8_t>& data) {
  CanFrame f;
  f.id = id;
  f.flags = flags;
  f.len = static_cast<uint8_t>(data.size());
  std::memcpy(f.data, data.data(), data.size());
  return f;
}

std::vector<uint8_t> Encode(const std::vector<CanFrame>& frames, uint16_t seq, uint32_t epoch) {
  std::vector<uint8_t> out(rec::kHeaderSize + frames.size() * rec::kMaxRecordSize);
  rec::Header h;
  h.seq = seq;
  h.epoch = epoch;
  std::size_t n = rec::EncodeHeader(out.data(), h);
  for (const auto& f : frames) {
    const std::size_t w = rec::EncodeRecord(&out[n], out.size() - n, f);
    REQUIRE(w != 0U);
    n += w;
  }
  out.resize(n);
  return out;
}

// ver 1, flags 0, seq 0x0102, epoch 0x0A0B0C0D,
// record 0x605 [0x40 0x00 0x10 0x00], record ext 0x1ABCDEF RTR len 0,
// record FD|BRS 0x123 len 12 [0..11].
const std::vector<uint8_t> kGolden = {
    0x01, 0x00, 0x02, 0x01, 0x0D, 0x0C, 0x0B, 0x0A,                                      // header
    0x05, 0x06, 0x00, 0x00, 0x00, 0x04, 0x40, 0x00, 0x10, 0x00,                          // 0x605
    0xEF, 0xCD, 0xAB, 0xC1, 0x00, 0x00,                                                  // ext+rtr
    0x23, 0x01, 0x00, 0x00, 0x0C, 0x0C, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};          // FD

}  // namespace

HF_TEST(Record_GoldenBytes) {
  const auto bytes = Encode({Make(0x605, 0, {0x40, 0x00, 0x10, 0x00}), Make(0x1ABCDEF, ff::kExt | ff::kRtr, {}),
                             Make(0x123, ff::kFdf | ff::kBrs, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11})},
                            0x0102, 0x0A0B0C0D);
  CHECK(bytes == kGolden);
}

HF_TEST(Record_DecodeGolden) {
  std::vector<CanFrame> got;
  rec::Header h;
  CHECK_EQ(rec::DecodeDatagram(kGolden.data(), kGolden.size(), &h, [&](const CanFrame& f) { got.push_back(f); }),
           rec::DecodeError::None);
  CHECK_EQ(h.seq, 0x0102U);
  CHECK_EQ(h.epoch, 0x0A0B0C0DU);
  REQUIRE(got.size() == 3U);
  CHECK_EQ(got[0].id, 0x605U);
  CHECK_EQ(got[0].len, 4U);
  CHECK_EQ(got[0].data[0], 0x40U);
  CHECK_EQ(got[1].id, 0x1ABCDEFU);
  CHECK_EQ(got[1].flags, static_cast<uint8_t>(ff::kExt | ff::kRtr));
  CHECK_EQ(got[2].flags, static_cast<uint8_t>(ff::kFdf | ff::kBrs));
  CHECK_EQ(got[2].len, 12U);
  CHECK_EQ(got[2].data[11], 11U);
}

HF_TEST(Record_RoundTripEveryClassicLength) {
  for (uint8_t len = 0; len <= 8; ++len) {
    std::vector<uint8_t> d(len);
    for (uint8_t i = 0; i < len; ++i) {
      d[i] = static_cast<uint8_t>(0xA0 + i);
    }
    const auto bytes = Encode({Make(0x7FF, 0, d)}, len, 0);
    std::vector<CanFrame> got;
    CHECK_EQ(rec::DecodeDatagram(bytes.data(), bytes.size(), nullptr, [&](const CanFrame& f) { got.push_back(f); }),
             rec::DecodeError::None);
    REQUIRE(got.size() == 1U);
    CHECK_EQ(got[0].len, len);
    CHECK(std::memcmp(got[0].data, d.data(), len) == 0);
  }
}

HF_TEST(Record_EncodeRefusesMalformedFrames) {
  uint8_t buf[rec::kMaxRecordSize];
  CHECK_EQ(rec::EncodeRecord(buf, sizeof(buf), Make(0x800, 0, {})), 0U);              // id range
  CHECK_EQ(rec::EncodeRecord(buf, sizeof(buf), Make(0x100, ff::kBrs, {})), 0U);       // BRS without FDF
  CHECK_EQ(rec::EncodeRecord(buf, sizeof(buf), Make(0x100, ff::kFdf, std::vector<uint8_t>(9))), 0U);  // no DLC
  CanFrame big = Make(0x100, 0, {});
  big.len = 9;
  CHECK_EQ(rec::EncodeRecord(buf, sizeof(buf), big), 0U);                             // classic > 8
  CHECK_EQ(rec::EncodeRecord(buf, 5, Make(0x100, 0, {1})), 0U);                       // no room
}

HF_TEST(Record_DecodeRejectsWithoutDeliveringAnything) {
  int calls = 0;
  auto count = [&](const CanFrame&) { ++calls; };
  auto bad = kGolden;
  CHECK_EQ(rec::DecodeDatagram(bad.data(), 7, nullptr, count), rec::DecodeError::Truncated);
  bad[0] = 2;
  CHECK_EQ(rec::DecodeDatagram(bad.data(), bad.size(), nullptr, count), rec::DecodeError::BadVersion);
  bad = kGolden;
  CHECK_EQ(rec::DecodeDatagram(bad.data(), bad.size() - 1U, nullptr, count), rec::DecodeError::Truncated);
  bad = kGolden;
  bad[13] = 65;  // first record len
  CHECK_EQ(rec::DecodeDatagram(bad.data(), bad.size(), nullptr, count), rec::DecodeError::BadLength);
  bad = kGolden;
  bad[12] = 0x01;  // EXT is carried in the id, not the record flags
  CHECK_EQ(rec::DecodeDatagram(bad.data(), bad.size(), nullptr, count), rec::DecodeError::BadFrame);
  bad = kGolden;
  bad[13] = 9;     // classic record with 9 bytes (and the rest misaligned)
  CHECK(rec::DecodeDatagram(bad.data(), bad.size(), nullptr, count) != rec::DecodeError::None);
  CHECK_EQ(calls, 0);
}

HF_TEST(Record_HeaderOnlyDatagramIsValid) {
  uint8_t buf[rec::kHeaderSize];
  rec::Header h;
  h.flags = rec::kDgramKeepAlive;
  rec::EncodeHeader(buf, h);
  rec::Header out;
  int calls = 0;
  CHECK_EQ(rec::DecodeDatagram(buf, sizeof(buf), &out, [&](const CanFrame&) { ++calls; }), rec::DecodeError::None);
  CHECK_EQ(out.flags, rec::kDgramKeepAlive);
  CHECK_EQ(calls, 0);
}
