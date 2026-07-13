#pragma once

#include <Utility/TTimestamp.hpp>

namespace Utility
{

template<class Clock>
typename TTimestamp<Clock>::rep TTimestamp<Clock>::now()
{
    return Clock::now().time_since_epoch().count();
}

}
