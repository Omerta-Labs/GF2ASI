#pragma once

// C++
#include <stdint.h>

extern bool BitVector_Test(const uint32_t* BitVector, const uint32_t Bit)
{
    const uint32_t WordIdx = static_cast<uint32_t>(Bit) >> 5;  // divide by 32
    const uint32_t BitIdx = static_cast<uint32_t>(Bit) & 31; // mod 32
    return (BitVector[WordIdx] >> BitIdx) & 1u;
}

extern void BitVector_Set(uint32_t* BitVector, const uint32_t Bit)
{
    const uint32_t WordIdx = static_cast<uint32_t>(Bit) >> 5;
    const uint32_t BitIdx = static_cast<uint32_t>(Bit) & 31;
    BitVector[WordIdx] |= (1u << BitIdx);
}

extern void BitVector_Clear(uint32_t* BitVector, const uint32_t Bit)
{
    const uint32_t WordIdx = static_cast<uint32_t>(Bit) >> 5;
    const uint32_t BitIdx = static_cast<uint32_t>(Bit) & 31;
    BitVector[WordIdx] &= ~(1u << BitIdx);
}