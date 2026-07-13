#pragma once

#include <functional>
#include <random>
#include <atomic>
#include <memory>

#include <Utility/IPC/IIPCSenderBackend.hpp>
#include <Producer/CDataProducer.hpp>

namespace Producer
{

class CApplication
{
public:
    CApplication(std::chrono::milliseconds throttle = std::chrono::milliseconds{1000u});
    ~CApplication();

    void run();

private:
    void processData();

private:
    std::chrono::milliseconds m_throttle;
    std::chrono::seconds m_data_send_timeout;
    std::atomic_bool m_stop;
    std::atomic_bool m_pause;
    std::atomic_bool m_debug;
    std::atomic_size_t m_payload_size;
};

}
