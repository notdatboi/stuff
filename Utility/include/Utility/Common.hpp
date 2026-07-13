#pragma once

#include <span>
#include <string_view>
#include <chrono>
#include <syncstream>

#include <Utility/TTimestamp.hpp>
#include <Utility/CChecksum.hpp>

namespace Utility
{

using CTimestamp = TTimestamp<std::chrono::steady_clock>;

std::osyncstream out();

}
