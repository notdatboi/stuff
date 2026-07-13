#pragma once

#include <fcntl.h>
#include <semaphore.h>

#include <Utility/IPC/IIPCReceiverBackend.hpp>

namespace Utility
{
namespace IPC
{

class CSemaphore
{
public:
    CSemaphore(std::string name, bool locked);
    ~CSemaphore();
    bool timed_lock(std::chrono::seconds timeout);
    void lock();
    void unlock();

private:
    sem_t* m_semaphore;
    std::string m_name;
};

}
}
