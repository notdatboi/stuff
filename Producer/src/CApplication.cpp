#include <Producer/CApplication.hpp>

#include <iostream>
#include <thread>

#include <Producer/CDataProducer.hpp>
#include <Utility/IPC/CSharedMemorySender.hpp>

namespace Producer
{

CApplication::CApplication(std::chrono::milliseconds throttle)
    : m_throttle{throttle}
    , m_data_send_timeout{3u}
    , m_stop{false}
    , m_payload_size{1u}
{
}

CApplication::~CApplication() = default;

void CApplication::run()
{
    Utility::out() << "Press `x` and Enter to exit, or enter payload size" << std::endl;

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
        else
        {
            std::size_t const size{std::stoul(user_input)};
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

    while (!m_stop.load())
    {
        auto const payload_size = m_payload_size.load();
        producer.setSize(payload_size);
        sender.timed_send(m_data_send_timeout, writer, payload_size);
        if (m_throttle != std::chrono::milliseconds{0u})
        {
            std::this_thread::sleep_for(m_throttle);
        }
    }
}

}
