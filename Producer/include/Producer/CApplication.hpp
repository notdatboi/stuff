#pragma once

#include <functional>
#include <random>
#include <atomic>
#include <memory>

#include <Utility/IPC/IIPCSenderBackend.hpp>
#include <Producer/CDataProducer.hpp>

namespace Producer
{

/// @brief Application; responsible for running producer logic and handling input
class CApplication
{
public:
    /// @brief Ctor
    /// @param throttle how much to throttle packets
    CApplication(std::chrono::milliseconds throttle = std::chrono::milliseconds::zero());

    /// @brief Dtor
    ~CApplication();

    /// @brief Run application and data producer thread
    void run();

private:
    /// @brief Process data inside producer thread
    void processData();

private:
    std::chrono::milliseconds m_throttle; ///< how much to throttle packets
    std::chrono::seconds m_data_send_timeout; ///< if data was not senbt in this time frame, check atomics and try again
    std::atomic_bool m_stop; ///< stop signal
    std::atomic_bool m_pause; ///< pause signal
    std::atomic_bool m_debug; ///< debug mode signal (packets will be sometimes supplied with incorrect metadata)
    std::atomic_size_t m_payload_size; ///< desired size of payload
};

}
