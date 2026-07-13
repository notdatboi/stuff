#pragma once

#include <functional>
#include <random>

#include <Utility/IPC/Common.hpp>
#include <Consumer/EPacketError.hpp>

namespace Consumer
{

class CDataConsumer
{
public:
    struct Statistics
    {
        std::size_t total_packet_count;
        std::size_t packets_per_second;
        std::size_t bytes_per_second;
        std::unordered_map<EPacketError, std::size_t> errors_by_type;
    };

public:
    CDataConsumer();
    ~CDataConsumer();

    void setSize(std::size_t size);
    std::size_t getSize() const;
    void process(Utility::IPC::Data const& data);
    Statistics getStatistics();

private:
    using Clock = std::chrono::steady_clock;
    struct BPSTrackerData
    {
        Clock::time_point last_update;
        std::size_t accumulated_bytes;
        std::size_t accumulated_packets;
    };

private:
    void validate(Utility::IPC::Data const& data);
    void trackTotalPacketCount();
    void trackBytesAndPacketsPerSecond(std::size_t data_size);
    void trackErrorsByType(EPacketError validation_result);

private:
    std::size_t m_prev_seqn;
    Statistics m_statistics;
    BPSTrackerData m_bps_tracker_data;
};

}
