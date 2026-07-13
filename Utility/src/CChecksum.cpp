#include <Utility/CChecksum.hpp>

#include <algorithm>
#include <execution>
#include <functional>
#include <vector>

namespace Utility
{

CChecksum::CChecksum(std::size_t num_chunks)
    : m_num_chunks{num_chunks}
{
}

CChecksum::~CChecksum() = default;

std::size_t CChecksum::operator()(std::string_view sv)
{
    if (sv.empty())
    {
        return 0;
    }

    if (sv.size() < m_num_chunks * 4u) // skip chunking on small amounts of data
    {
        return ChunkHasher{}(sv);
    }

    std::size_t const chunk_size{sv.size() / m_num_chunks};
    std::vector<std::size_t> chunk_hashes(m_num_chunks);
    std::vector<std::size_t> chunk_indices(m_num_chunks);
    std::iota(chunk_indices.begin(), chunk_indices.end(), 0u);

    std::for_each(std::execution::par, chunk_indices.begin(), chunk_indices.end(), [&](std::size_t const i) {
        std::size_t const offset{i * chunk_size};
        std::size_t const size{(i == m_num_chunks - 1) ? (sv.size() - offset) : chunk_size};
        std::string_view chunk{sv.data() + offset, size};
        chunk_hashes[i] = ChunkHasher{}(chunk);
    });

    // meta-hash, stolen from stackoverflow
    std::size_t hash{chunk_hashes.size()};
    for (auto const h : chunk_hashes)
    {
        hash ^= h + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
}

}