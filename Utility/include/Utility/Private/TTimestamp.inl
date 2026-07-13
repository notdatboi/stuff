#pragma once

#include <Utility/TTimestamp.hpp>

namespace Utility
{

template<class Clock>
typename TTimestamp<Clock>::rep TTimestamp<Clock>::now()
{
    return Clock::now().time_since_epoch().count();
}

template<class Clock>
bool TTimestamp<Clock>::isInFrameAroundNow(rep ts, std::chrono::milliseconds half_frame)
{
    auto const now = Clock::now();
    auto frame_start = now;
    frame_start -= half_frame;
    auto frame_end = now;
    frame_end += half_frame;
    auto const frame_start_tse = frame_start.time_since_epoch().count();
    auto const frame_end_tse = frame_end.time_since_epoch().count();
    return ts >= frame_start_tse && ts <= frame_end_tse;
}

}
