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

/// @brief Specialization of IPC sender (shared mem backend)
class CSharedMemorySender : public IIPCSenderBackend
{
public:
    /// @brief Ctor
    CSharedMemorySender();

    /// @brief Dtor
    ~CSharedMemorySender() override;

    /// @brief Wait for read signal and write new data
    /// @param writer data handler, called after data has been locked
    /// @param payload_size_bytes size of the payload to be written
    void send(WriterFn writer, std::size_t payload_size_bytes) override;

    /// @brief Wait for read signal and write new data
    /// @param waiting_time max time to wait
    /// @param writer data handler, called after data has been locked
    /// @param payload_size_bytes size of the payload to be written
    bool timed_send(std::chrono::seconds waiting_time, WriterFn writer, std::size_t payload_size_bytes) override;

private:
    /// @brief Write data, after lock was obtained
    /// @param writer actual data writer
    /// @param payload_size_bytes size of the payload to be written
    void doSend(WriterFn writer, std::size_t payload_size_bytes);

private:
    CSemaphore m_produced_sem; ///< produced semaphore
    CSemaphore m_consumed_sem; ///< consumed semaphore
    int m_shm; ///< shared mem hadle
    std::size_t m_payload_size; ///< current payload size
};

}
}
