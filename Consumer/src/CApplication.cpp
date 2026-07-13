#include <Consumer/CApplication.hpp>

#include <iostream>
#include <thread>

#include <Consumer/CDataConsumer.hpp>
#include <Utility/IPC/CSharedMemoryReceiver.hpp>

namespace Consumer
{

CApplication::CApplication()
    : m_data_receive_timeout{3u}
    , m_stop{false}
{
}

CApplication::~CApplication() = default;

void CApplication::run()
{
    Utility::out() << "Press `x` and Enter to exit" << std::endl;

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
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
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
        receiver.timed_receive(m_data_receive_timeout, reader);
    }
}

}
