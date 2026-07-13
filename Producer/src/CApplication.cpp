#include <Producer/CApplication.hpp>

#include <iostream>
#include <thread>
#include <numeric>
#include <cmath>

#include <Producer/CDataProducer.hpp>
#include <Utility/IPC/CSharedMemorySender.hpp>

namespace Producer
{
namespace
{

static const std::chrono::milliseconds c_ui_sleep_ms{100u};
static const std::chrono::milliseconds c_worker_pause_sleep_ms{1u};

}

CApplication::CApplication(std::chrono::milliseconds throttle)
    : m_throttle{throttle}
    , m_data_send_timeout{3u}
    , m_stop{false}
    , m_pause{false}
    , m_payload_size{1u}
{
}

CApplication::~CApplication() = default;

void CApplication::run()
{
    Utility::out() << "Press Enter after operation to confirm." << std::endl
        << "\tAvailable operations:" << std::endl
        << "\t\t`x` to exit" << std::endl
        << "\t\t`<number>` to set payload size (1 to uint32_t max)" << std::endl
        << "\t\t`p` to pause" << std::endl
        << "\t\t`u` to unpause" << std::endl
        << "\t\t`d` to enter debug mode (chance of invalid packets)" << std::endl
        << "\t\t`r` to exit debug mode" << std::endl;

    std::thread worker{[this]{ processData(); }};
    while (true)
    {
        std::string user_input;
        std::cin >> user_input;
        if (user_input == "x")
        {
            m_stop.store(true);
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
        else if (user_input == "d")
        {
            m_debug.store(true);
        }
        else if (user_input == "r")
        {
            m_debug.store(false);
        }
        else
        {
            std::size_t size{1u};
            try
            {
                size = std::max(std::min<std::size_t>(std::stoull(user_input), std::numeric_limits<std::uint32_t>::max()), std::size_t{1u});
            }
            catch(std::invalid_argument const& e)
            {
                Utility::out() << "Invalid argument, tried to parse: " << user_input << std::endl;
            }
            catch(std::out_of_range const& e)
            {
                Utility::out() << "Out of range, tried to parse: " << user_input << std::endl;
            }
            Utility::out() << "Accepted size: " << size << std::endl;
            m_payload_size.store(size);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }
}

void CApplication::processData()
{
    CDataProducer producer;
    Utility::IPC::CSharedMemorySender sender;
    auto writer = [&producer](Utility::IPC::Data& data)
    {
        producer.process(data);
    };
    auto debug_writer = [&producer](Utility::IPC::Data& data)
    {
        producer.processDebug(data);
    };

    while (!m_stop.load())
    {
        if (m_pause.load())
        {
            std::this_thread::sleep_for(c_worker_pause_sleep_ms);
        }
        else
        {
            auto const payload_size = m_payload_size.load();
            producer.setSize(payload_size);
            if (m_debug.load())
            {
                sender.timed_send(m_data_send_timeout, debug_writer, payload_size);
            }
            else
            {
                sender.timed_send(m_data_send_timeout, writer, payload_size);
            }
            if (m_throttle != std::chrono::milliseconds::zero())
            {
                std::this_thread::sleep_for(m_throttle);
            }
        }
    }
}

}
