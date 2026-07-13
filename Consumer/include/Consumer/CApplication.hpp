#pragma once

#include <functional>
#include <random>
#include <atomic>
#include <memory>

#include <Utility/IPC/IIPCSenderBackend.hpp>
#include <Consumer/CDataConsumer.hpp>

namespace Consumer
{

class CApplication
{
public:
    CApplication();
    ~CApplication();

    void run();

private:
    void processData();

private:
    std::chrono::seconds m_data_receive_timeout;
    std::atomic_bool m_stop;
};

}
