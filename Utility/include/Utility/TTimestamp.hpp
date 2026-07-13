#pragma once

namespace Utility
{

template <class Clock>
class TTimestamp
{
public:
    using rep = typename Clock::rep;

public:
    static rep now();
    static bool isInFrameAroundNow(rep ts, std::chrono::milliseconds half_frame);
};

}

#include <Utility/Private/TTimestamp.inl>
