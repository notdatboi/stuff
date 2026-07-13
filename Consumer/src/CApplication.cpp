#include <Consumer/CApplication.hpp>

#include <iostream>
#include <thread>

#include <Consumer/CDataConsumer.hpp>
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
        std::this_thread::sleep_for(c_ui_sleep_ms);
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
    }
}

}
