#pragma once

#include <functional>

#include <Utility/IPC/Common.hpp>

namespace Utility
{
namespace IPC
{

/// @brief General interface for IPC senders
class IIPCSenderBackend
{
public:
    using WriterFn = std::function<void(Data&)>;

public:
    /// @brief Dtor
    virtual ~IIPCSenderBackend() = default;

    /// @brief Send data
    /// @param writer actual data writer
    /// @param payload_size_bytes size of the payload
    virtual void send(WriterFn writer, std::size_t payload_size_bytes) = 0;

    /// @brief Send data
    /// @param waiting_time timeout
    /// @param writer actual data writer
    /// @param payload_size_bytes size of the payload
    /// @return whther sending was successful
    virtual bool timed_send(std::chrono::seconds waiting_time, WriterFn writer, std::size_t payload_size_bytes) = 0;
};

}
}
