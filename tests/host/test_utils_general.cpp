/**
 * @file test_utils_general.cpp
 * @brief hf-utils-general containers, filters and CRC on the host.
 */

#include "AveragingFilter.h"
#include "CircularBuffer.h"
#include "CrcCalculator.h"
#include "HostTest.h"
#include "RingBuffer.h"

#include <vector>

namespace {

template <typename RB>
std::vector<int> Contents(RB& rb) {
    std::vector<int> out;
    for (auto it = rb.begin(); it != rb.end(); ++it) {
        out.push_back(*it);
    }
    return out;
}

}  // namespace

HF_TEST(RingBuffer_FillsThenOverwritesOldest) {
    RingBuffer<int, 4> rb;
    CHECK(rb.IsEmpty());
    CHECK(rb.empty());
    CHECK_EQ(rb.GetSize(), 4u);

    rb.Append(1);
    rb.Append(2);
    rb.Append(3);
    CHECK_EQ(rb.GetCount(), 3u);
    CHECK(!rb.IsFull());
    CHECK(Contents(rb) == (std::vector<int>{1, 2, 3}));

    rb.Append(4);
    CHECK(rb.IsFull());
    CHECK_EQ(rb.GetCount(), 4u);

    rb.Append(5);  // overwrites 1
    rb.Append(6);  // overwrites 2
    CHECK(rb.IsFull());
    CHECK_EQ(rb.GetCount(), 4u);
    CHECK(Contents(rb) == (std::vector<int>{3, 4, 5, 6}));
}

HF_TEST(RingBuffer_EraseResets) {
    RingBuffer<int, 3> rb;
    rb.Append(7);
    rb.Append(8);
    rb.Erase();
    CHECK(rb.IsEmpty());
    CHECK_EQ(rb.GetCount(), 0u);
    rb.Append(9);
    CHECK(Contents(rb) == (std::vector<int>{9}));
}

HF_TEST(CircularBuffer_FifoOrderAndOverwrite) {
    CircularBuffer<uint8_t, 4> cb;
    const uint8_t data[] = {10, 11, 12};
    CHECK(cb.Write(data, 3));
    CHECK_EQ(cb.GetCount(), 3u);

    uint8_t v = 0;
    CHECK(cb.Read(v));
    CHECK_EQ(v, 10u);

    cb.Write(13);
    cb.Write(14);  // now full: 11 12 13 14
    CHECK(cb.IsFull());
    cb.Write(15);  // overwrites 11
    uint8_t out[8] = {};
    const uint16_t n = cb.Read(out, sizeof(out));
    CHECK_EQ(n, 4u);
    CHECK_EQ(out[0], 12u);
    CHECK_EQ(out[3], 15u);
    CHECK(cb.IsEmpty());
    CHECK(!cb.Read(v));
}

HF_TEST(Crc16_CcittFalseCheckValue) {
    // Standard check value for CRC-16/CCITT-FALSE over "123456789".
    const char msg[] = "123456789";
    CHECK_EQ(crc16(msg, 9), 0x29B1u);
    CHECK_EQ(crc16(msg, 0), 0xFFFFu);
    CHECK_EQ(crc16(nullptr, 4), 0xFFFFu);
    // A single flipped bit changes the CRC.
    char bad[] = "123456788";
    CHECK(crc16(bad, 9) != 0x29B1u);
}

HF_TEST(AveragingFilter_AveragesLoadedSamplesThenSlides) {
    AveragingFilter<float, 4> f;
    CHECK_EQ(f.GetCount(), 0u);
    CHECK_NEAR(f.GetValue(), 0.0, 1e-9);
    f.Append(1.0f);
    f.Append(3.0f);
    CHECK(!f.IsFullyLoaded());
    CHECK_NEAR(f.GetValue(), 2.0, 1e-6);
    f.Append(5.0f);
    f.Append(7.0f);
    CHECK(f.IsFullyLoaded());
    CHECK_NEAR(f.GetValue(), 4.0, 1e-6);
    f.Append(9.0f);  // replaces the oldest slot (1.0)
    CHECK_NEAR(f.GetValue(), 6.0, 1e-6);
}
