#include <Consumer/CApplication.hpp>

#include <iostream>
#include <thread>

#include <Utility/IPC/CSharedMemoryReceiver.hpp>

namespace Consumer
{
namespace
{

static const std::chrono::milliseconds c_ui_sleep_ms{100u};
static const std::chrono::milliseconds c_worker_pause_sleep_ms{1u};

}

CApplication::CApplication()
    : m_data_receive_timeout{3u}
    , m_stop{false}
    , m_pause{false}
{
}

CApplication::~CApplication() = default;

void CApplication::run()
{
    Utility::out() << "Press Enter after operation to confirm." << std::endl
        << "\tAvailable operations:" << std::endl
        << "\t\t`x` to exit" << std::endl
        << "\t\t`p` to pause" << std::endl
        << "\t\t`u` to unpause" << std::endl;

    std::thread worker{[this]{ processData(); }};
    std::thread statistics_printer{[this]{ printStatistics(); }};
    while (true)
    {
        std::string user_input;
        std::cin >> user_input;
        if (user_input == "x")
        {
            m_stop.store(true);
            statistics_printer.join();
            worker.join();
            break;
        }
        else if (user_input == "p")
        {
            m_pause.store(true);
        }
        else if (user_input == "u")
        {
            m_pause.store(false);
        }
    }
}

void CApplication::processData()
{
    CDataConsumer consumer;
    Utility::IPC::CSharedMemoryReceiver receiver;
    auto reader = [&consumer](Utility::IPC::Data const& data)
    {
        consumer.process(data);
    };

    while (!m_stop.load())
    {
        if (m_pause.load())
        {
            std::this_thread::sleep_for(c_worker_pause_sleep_ms);
        }
        else
        {
            receiver.timed_receive(m_data_receive_timeout, reader);
        }

        // too lazy to optimize
        {
            std::lock_guard<std::mutex> lock{m_statistics_mutex};
            m_statistics = consumer.getStatistics();
        }
    }
}

void CApplication::printStatistics()
{
    Clock::time_point last_statistics_update{Clock::now()};
    CDataConsumer::Statistics statistics;

    while (!m_stop.load())
    {
        if (m_pause.load())
        {
            std::this_thread::sleep_for(c_ui_sleep_ms);
            last_statistics_update = Clock::now();
        }

        auto const now = Clock::now();
        // seconds but double
        auto const elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(now - last_statistics_update).count();
        if (elapsed >= 1.0)
        {
            {
                std::lock_guard<std::mutex> lock{m_statistics_mutex};
                statistics = m_statistics;
            }
            Utility::out() << "Statistics: " << std::endl
                << "\tTotal packet count: " << statistics.total_packet_count << std::endl
                << "\tPackets per second: " << statistics.packets_per_second << std::endl
                << "\tBytes per second: " << statistics.bytes_per_second << std::endl
                << "\tKilobytes per second: " << statistics.bytes_per_second / 1024u << std::endl
                << "\tMegabytes per second: " << statistics.bytes_per_second / 1024u / 1024u << std::endl
                << "\tTotal invalid timestamp errors: " << statistics.errors_by_type[EPacketError::InvalidTimestamp] << std::endl
                << "\tTotal sequence inconsistency errors: " << statistics.errors_by_type[EPacketError::SequenceInconsistency] << std::endl
                << "\tTotal checksum mismatch errors: " << statistics.errors_by_type[EPacketError::ChecksumMismatch] << std::endl << std::endl;
            last_statistics_update = now;
        }

        std::this_thread::sleep_for(c_ui_sleep_ms);
    }
}

}
