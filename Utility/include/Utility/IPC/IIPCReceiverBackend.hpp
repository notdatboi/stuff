#pragma once

#include <functional>

#include <Utility/IPC/Common.hpp>

namespace Utility
{
namespace IPC
{

/// @brief General interface for IPC receivers
class IIPCReceiverBackend
{
public:
    using ReaderFn = std::function<void(Data const&)>;

public:
    /// @brief Dtor
    virtual ~IIPCReceiverBackend() = default;

    /// @brief Receive data
    /// @param reader data reader
    virtual void receive(ReaderFn reader) = 0;

    /// @brief Receive data or return false if timeout has been reached
    /// @param waiting_time timeout
    /// @param reader actual data reader
    /// @return whether reading happened (or timeout was reached)
    virtual bool timed_receive(std::chrono::seconds waiting_time, ReaderFn reader) = 0;
};

}
}
