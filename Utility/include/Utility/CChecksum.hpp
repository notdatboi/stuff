#pragma once

#include <string_view>

#include <Utility/Common.hpp>

namespace Utility
{

class CChecksum
{
public:
    CChecksum(std::size_t num_chunks = 16u);
    ~CChecksum();

    std::size_t operator()(std::string_view sv);

private:
    using ChunkHasher = std::hash<std::string_view>;

private:
    std::size_t m_num_chunks;
};

}
