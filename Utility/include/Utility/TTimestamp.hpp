#pragma once

namespace Utility
{

/// @brief Time operation handler/wrapper
/// @tparam Clock clock
template <class Clock>
class TTimestamp
{
public:
    using rep = typename Clock::rep;

public:
    /// @brief Get current time point
    /// @return current time point
    static rep now();

    /// @brief Check if time point is in specified frame [half_frame---now---half_frame]
    /// @param ts time point to check
    /// @param half_frame half of the time frame
    /// @return whether it is in
    static bool isInFrameAroundNow(rep ts, std::chrono::milliseconds half_frame);
};

}

#include <Utility/Private/TTimestamp.inl>
