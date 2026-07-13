#include <Utility/Common.hpp>

#include <iostream>
#include <stdexcept>

namespace Utility
{

std::osyncstream out()
{
    return std::osyncstream(std::cout);
}

}