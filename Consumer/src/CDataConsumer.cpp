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
    , m_statistics{0u, 0u, 0u, {}}
    , m_bps_tracker_data{Clock::now(), 0u}
{
}

CDataConsumer::~CDataConsumer() = default;

void CDataConsumer::process(Utility::IPC::Data const& data)
{
    // process stuff + calc error statistics
    validate(data);
    m_prev_seqn = data.metadata.seqn;

    // calc statistics
    trackTotalPacketCount();
    trackBytesAndPacketsPerSecond(sizeof(Utility::IPC::Metadata) + data.payload.size());
}

CDataConsumer::Statistics CDataConsumer::getStatistics()
{
    return m_statistics;
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

    trackErrorsByType(err);
}

void CDataConsumer::trackTotalPacketCount()
{
    ++m_statistics.total_packet_count;
}

void CDataConsumer::trackBytesAndPacketsPerSecond(std::size_t data_size)
{
    m_bps_tracker_data.accumulated_bytes += data_size;
    ++m_bps_tracker_data.accumulated_packets;
    auto const now = Clock::now();
    // seconds but double
    auto const elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(now - m_bps_tracker_data.last_update).count();
    if (elapsed >= 1.0)
    {
        m_statistics.bytes_per_second = static_cast<std::size_t>(m_bps_tracker_data.accumulated_bytes / elapsed);
        m_statistics.packets_per_second = static_cast<std::size_t>(m_bps_tracker_data.accumulated_packets / elapsed);
        m_bps_tracker_data.last_update = now;
        m_bps_tracker_data.accumulated_bytes = 0u;
        m_bps_tracker_data.accumulated_packets = 0u;
    }
}

void CDataConsumer::trackErrorsByType(EPacketError validation_result)
{
    if (validation_result == EPacketError::NoError)
    {
        return;
    }
    auto check_and_populate_error = [this, validation_result](EPacketError error_to_check)
    {
        if (static_cast<bool>(validation_result & error_to_check))
        {
            ++m_statistics.errors_by_type[error_to_check];
        }
    };
    check_and_populate_error(EPacketError::InvalidTimestamp);
    check_and_populate_error(EPacketError::SequenceInconsistency);
    check_and_populate_error(EPacketError::ChecksumMismatch);
}

}
