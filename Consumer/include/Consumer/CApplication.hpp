#pragma once

#include <functional>
#include <random>
#include <atomic>
#include <memory>

#include <Consumer/CDataConsumer.hpp>

namespace Consumer
{

/// @brief Application; responsible for running consumer logic and handling input
class CApplication
{
public:
    /// @brief Ctor
    CApplication();

    /// @brief Dtor
    ~CApplication();

    /// @brief Start the application
    void run();

private:
    using Clock = std::chrono::steady_clock;

private:
    /// @brief Execute data consuming (called from separate thread)
    void processData();

    /// @brief Execute stat printing (called from separate thread)
    void printStatistics();

private:
    std::chrono::seconds m_data_receive_timeout; ///< if data was not received in this time frame, check atomics and try again
    std::atomic_bool m_stop; ///< full stop signal
    std::atomic_bool m_pause; ///< temporary pause signal
    CDataConsumer::Statistics m_statistics; ///< statistics temporary storage, used by worker and printer threads
    std::mutex m_statistics_mutex; ///< statistics storage mutex
};

}
