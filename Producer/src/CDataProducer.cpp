#include <Producer/CDataProducer.hpp>

#include <cstdlib>
#include <execution>

namespace Producer
{

CDataProducer::CDataProducer()
    : m_reng{std::random_device{}()}
    , m_rdist{0, 100}
    , m_seqn{0u}
    , m_size{1u}
{
}

CDataProducer::~CDataProducer() = default;

void CDataProducer::setSize(std::size_t size)
{
    m_size = size;
}

std::size_t CDataProducer::getSize() const
{
    return m_size;
}

void CDataProducer::process(Utility::IPC::Data& data)
{
    data.metadata.seqn = m_seqn;
    data.metadata.data_size = m_size;

    std::for_each(std::execution::par, data.payload.begin(), data.payload.end(), [](char& e) {
        thread_local std::random_device rd;
        thread_local std::default_random_engine reng{rd()};
        thread_local std::uniform_int_distribution rdist{32, 127}; // ascii
        e = rdist(reng);
    });
    std::string_view payload_sv{data.payload.data(), data.payload.size()}; // can't hash spans easily
    data.metadata.checksum = Utility::CChecksum{}(payload_sv);
    data.metadata.ts = Utility::CTimestamp::now();
    ++m_seqn;
}

void CDataProducer::processDebug(Utility::IPC::Data& data)
{
    process(data);
    auto const random_n = m_rdist(m_reng);
    if (random_n % 3 == 0)
    {
        data.metadata.checksum = random_n;
    }
    else if (random_n % 5 == 0)
    {
        data.metadata.seqn = random_n;
    }
    else if (random_n % 8 == 0)
    {
        data.metadata.ts = random_n;
    }
}

}
