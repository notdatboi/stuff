#pragma once

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Utility/IPC/IIPCReceiverBackend.hpp>
#include <Utility/IPC/CSemaphore.hpp>

namespace Utility
{
namespace IPC
{

class CSharedMemoryReceiver : public IIPCReceiverBackend
{
public:
    CSharedMemoryReceiver();
    ~CSharedMemoryReceiver() override;

    void receive(ReaderFn reader) override;
    bool timed_receive(std::chrono::seconds waiting_time, ReaderFn reader) override;

private:
    bool tryOpenShm();
    void doReceive(ReaderFn reader);

private:
    CSemaphore m_produced_sem;
    CSemaphore m_consumed_sem;
    int m_shm;
};

}
}
