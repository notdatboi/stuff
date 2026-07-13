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
};

}

#include <Utility/Private/TTimestamp.inl>
