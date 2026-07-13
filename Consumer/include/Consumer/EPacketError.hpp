#pragma once

#include <cstdint>

namespace Consumer
{

using PacketErrorType = std::uint8_t;
enum class EPacketError : PacketErrorType
{
    NoError = 0b0000,
    InvalidTimestamp = 0b0001,
    SequenceInconsistency = 0b0010,
    ChecksumMismatch = 0b0100
};

}

#include <Consumer/Private/EPacketError.inl>
