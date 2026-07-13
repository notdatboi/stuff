#include <Producer/CDataProducer.hpp>

#include <cstdlib>

namespace Producer
{

CDataProducer::CDataProducer()
    : m_reng{std::random_device{}()}
    , m_rdist{32, 127} // ascii
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

    for (std::size_t i = 0; i < m_size; ++i)
    {
        data.payload[i] = m_rdist(m_reng);
    }
    std::string_view payload_sv{data.payload.data(), data.payload.size()}; // can't hash spans easily
    data.metadata.checksum = Utility::CChecksum{}(payload_sv);
    data.metadata.ts = Utility::CTimestamp::now();
    Utility::out()
        << "PACKET METADATA: [" << data.metadata.ts << "|" << data.metadata.seqn << "|" << data.metadata.checksum << "|" << data.metadata.data_size << "] "
        << "DATA: [" << payload_sv << "]" << std::endl;
    ++m_seqn;
}

}
