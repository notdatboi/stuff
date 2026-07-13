#pragma once

#include <string_view>

#include <Utility/Common.hpp>

namespace Utility
{

/// @brief Parallelized checksum calculator
class CChecksum
{
public:
    /// @brief Ctor
    /// @param num_chunks how many chunks to use
    CChecksum(std::size_t num_chunks = 16u);

    /// @brief Dtor
    ~CChecksum();

    /// @brief Hash function
    /// @param sv string view to hash
    /// @return hash
    std::size_t operator()(std::string_view sv);

private:
    using ChunkHasher = std::hash<std::string_view>;

private:
    std::size_t m_num_chunks; ///< how many chunks to use
};

}
