#pragma once

#include <functional>

#include <Utility/IPC/Common.hpp>

namespace Utility
{
namespace IPC
{

class IIPCReceiverBackend
{
public:
    using ReaderFn = std::function<void(Data const&)>;

public:
    virtual ~IIPCReceiverBackend() = default;

    virtual void receive(ReaderFn reader) = 0;
    virtual bool timed_receive(std::chrono::seconds waiting_time, ReaderFn reader) = 0;
};

}
}
