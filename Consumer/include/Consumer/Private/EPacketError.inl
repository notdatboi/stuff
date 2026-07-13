#pragma once

#include <Consumer/EPacketError.hpp>

namespace Consumer
{

inline EPacketError operator|(EPacketError l, EPacketError r)
{
    return static_cast<EPacketError>(static_cast<PacketErrorType>(l) | static_cast<PacketErrorType>(r));
}

inline EPacketError& operator|=(EPacketError& l, EPacketError r)
{
    l = l | r;
    return l;
}

inline EPacketError operator&(EPacketError l, EPacketError r)
{
    return static_cast<EPacketError>(static_cast<PacketErrorType>(l) & static_cast<PacketErrorType>(r));
}

}
