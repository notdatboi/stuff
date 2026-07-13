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

/// @brief Specialization of IPC receiver (shared mem backend)
class CSharedMemoryReceiver : public IIPCReceiverBackend
{
public:
    /// @brief Ctor
    CSharedMemoryReceiver();

    /// @brief Dtor
    ~CSharedMemoryReceiver() override;

    /// @brief Wait for, receive and handle data
    /// @param reader data handler
    void receive(ReaderFn reader) override;

    /// @brief Wait for, receive and handle data (or return if waiting took too long)
    /// @param waiting_time tiem to wait
    /// @param reader data handler, called after data has been received
    /// @return whether locking succeeded
    bool timed_receive(std::chrono::seconds waiting_time, ReaderFn reader) override;

private:
    /// @brief Try to open shared mem
    /// @return whether succeeded
    bool tryOpenShm();

    /// @brief Handle receival after successful locking
    /// @param reader data handler
    void doReceive(ReaderFn reader);

private:
    CSemaphore m_produced_sem; ///< data produced semaphore
    CSemaphore m_consumed_sem; ///< data consumed semapore
    int m_shm; ///< shared mem handle
};

}
}
