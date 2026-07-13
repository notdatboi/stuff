#pragma once

#include <functional>
#include <random>

#include <Utility/IPC/Common.hpp>
#include <Consumer/EPacketError.hpp>

namespace Consumer
{

/// @brief Main data consumption class. Checks metadata, gathers statistics
class CDataConsumer
{
public:
    struct Statistics
    {
        std::size_t total_packet_count; ///< total received packets from startup
        std::size_t packets_per_second; ///< packets per second
        std::size_t bytes_per_second; ///< bytes per second
        std::unordered_map<EPacketError, std::size_t> errors_by_type; ///< map of named errors by type
    };

public:
    /// @brief Ctor
    CDataConsumer();

    /// @brief Dtor
    ~CDataConsumer();

    /// @brief Consume data
    /// @param data data to consume
    void process(Utility::IPC::Data const& data);

    /// @brief Get most recent statistics
    /// @return statistics
    Statistics getStatistics();

private:
    using Clock = std::chrono::steady_clock;
    struct BPSTrackerData
    {
        Clock::time_point last_update; ///< time of last statistics update
        std::size_t accumulated_bytes; ///< bytes accumulated since last stat update
        std::size_t accumulated_packets; ///< packets accumulated since last stat update
    };

private:
    /// @brief Validate metadata separately and against data
    /// @param data received data
    void validate(Utility::IPC::Data const& data);

    /// @brief track total packet count
    void trackTotalPacketCount();

    /// @brief Track bytes and packets per second
    /// @param data_size size of recently received packet
    void trackBytesAndPacketsPerSecond(std::size_t data_size);

    /// @brief Track errors by type
    /// @param validation_result recent validation result
    void trackErrorsByType(EPacketError validation_result);

private:
    std::size_t m_prev_seqn; ///< previous seq number, used for sequence metadata validation
    Statistics m_statistics; ///< stat
    BPSTrackerData m_bps_tracker_data; ///< helpful data for stat tracking
};

}
