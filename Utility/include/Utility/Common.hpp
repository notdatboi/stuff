#pragma once

#include <span>
#include <string_view>
#include <chrono>
#include <syncstream>

#include <Utility/TTimestamp.hpp>

namespace Utility
{

using CChecksum = std::hash<std::string_view>;
using CTimestamp = TTimestamp<std::chrono::steady_clock>;

std::osyncstream out();

}
