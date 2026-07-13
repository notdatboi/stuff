#pragma once

#include <functional>

#include <Utility/IPC/Common.hpp>

namespace Utility
{
namespace IPC
{

class IIPCSenderBackend
{
public:
    using WriterFn = std::function<void(Data&)>;

public:
    virtual ~IIPCSenderBackend() = default;

    virtual void send(WriterFn writer, std::size_t payload_size_bytes) = 0;
    virtual bool timed_send(std::chrono::seconds waiting_time, WriterFn writer, std::size_t payload_size_bytes) = 0;
};

}
}
