#include <Consumer/CDataConsumer.hpp>

#include <cstdlib>

namespace Consumer
{

CDataConsumer::CDataConsumer()
{
}

CDataConsumer::~CDataConsumer() = default;

void CDataConsumer::process(Utility::IPC::Data const& data)
{
    std::string_view payload_sv{data.payload.data(), data.payload.size()}; // can't hash spans easily
    std::size_t checksum{Utility::CChecksum{}(payload_sv)};

    Utility::out()
        << "PACKET METADATA: [" << data.metadata.ts << "|" << data.metadata.seqn << "|" << data.metadata.checksum << "|" << data.metadata.data_size << "] "
        << "DATA: [" << payload_sv << "] CHECKSUM OK: " << (checksum == data.metadata.checksum) << std::endl;
}

}
