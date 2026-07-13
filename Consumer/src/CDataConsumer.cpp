#include <Consumer/CDataConsumer.hpp>

#include <cstdlib>

namespace Consumer
{
namespace
{

static const std::chrono::milliseconds c_valid_time_frame_half{30000u};
static const std::size_t c_invalid_seqn{std::numeric_limits<std::size_t>::max()};

}

CDataConsumer::CDataConsumer()
    : m_prev_seqn{c_invalid_seqn}
{
}

CDataConsumer::~CDataConsumer() = default;

void CDataConsumer::process(Utility::IPC::Data const& data)
{
    Utility::out()
        << "PACKET METADATA: [" << data.metadata.ts << "|" << data.metadata.seqn << "|" << data.metadata.checksum << "|" << data.metadata.data_size << "]" << std::endl
        << "DATA: [" << std::string_view{data.payload.data(), data.payload.size()} << "] " << std::endl;

    validate(data);
    m_prev_seqn = data.metadata.seqn;
}

void CDataConsumer::validate(Utility::IPC::Data const& data)
{
    EPacketError err{EPacketError::NoError};

    if (!Utility::CTimestamp::isInFrameAroundNow(data.metadata.ts, c_valid_time_frame_half))
    {
        err |= EPacketError::InvalidTimestamp;
    }

    if (m_prev_seqn != c_invalid_seqn && (m_prev_seqn + 1u) != data.metadata.seqn)
    {
        err |= EPacketError::SequenceInconsistency;
    }

    std::string_view payload_sv{data.payload.data(), data.payload.size()}; // can't hash spans easily
    std::size_t checksum{Utility::CChecksum{}(payload_sv)};

    if (checksum != data.metadata.checksum)
    {
        err |= EPacketError::ChecksumMismatch;
    }

    Utility::out() << "PACKET VALIDATION RESULT: 0x" << std::hex << std::fixed << static_cast<std::size_t>(err) << std::endl;
}

}
