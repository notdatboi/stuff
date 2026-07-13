#pragma once

#include <functional>
#include <random>
#include <atomic>
#include <memory>

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
    using Clock = std::chrono::steady_clock;

private:
    void processData();
    void printStatistics();

private:
    std::chrono::seconds m_data_receive_timeout;
    std::atomic_bool m_stop;
    std::atomic_bool m_pause;
    CDataConsumer::Statistics m_statistics;
    std::mutex m_statistics_mutex;
};

}
