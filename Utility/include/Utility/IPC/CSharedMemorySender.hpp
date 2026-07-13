#pragma once

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Utility/IPC/IIPCSenderBackend.hpp>
#include <Utility/IPC/CSemaphore.hpp>

namespace Utility
{
namespace IPC
{

class CSharedMemorySender : public IIPCSenderBackend
{
public:
    CSharedMemorySender();
    ~CSharedMemorySender() override;

    void send(WriterFn writer, std::size_t payload_size_bytes) override;
    bool timed_send(std::chrono::seconds waiting_time, WriterFn writer, std::size_t payload_size_bytes) override;

private:
    void doSend(WriterFn writer, std::size_t payload_size_bytes);

private:
    CSemaphore m_produced_sem;
    CSemaphore m_consumed_sem;
    int m_shm;
    std::size_t m_payload_size;
};

}
}
