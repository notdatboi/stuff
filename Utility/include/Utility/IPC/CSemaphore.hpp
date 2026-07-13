#pragma once

#include <fcntl.h>
#include <semaphore.h>

#include <Utility/IPC/IIPCReceiverBackend.hpp>

namespace Utility
{
namespace IPC
{

/// @brief Simple wrapper around semaphore
class CSemaphore
{
public:
    /// @brief Ctor
    /// @param name semaphore name
    /// @param locked whether to create it locked
    CSemaphore(std::string name, bool locked);

    /// @brief Dtor
    ~CSemaphore();

    /// @brief Try locking in specified time
    /// @param timeout time to try to lock
    /// @return whether locking succeeded
    bool timed_lock(std::chrono::seconds timeout);

    /// @brief lock semaphore
    void lock();

    /// @brief Unlock semaphore
    void unlock();

private:
    sem_t* m_semaphore; ///< internal system semaphore
    std::string m_name; ///< semaphore name
};

}
}
