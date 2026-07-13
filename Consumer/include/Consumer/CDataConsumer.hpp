#pragma once

#include <functional>
#include <random>

#include <Utility/IPC/Common.hpp>
#include <Consumer/EPacketError.hpp>

namespace Consumer
{

class CDataConsumer
{
public:
    CDataConsumer();
    ~CDataConsumer();

    void setSize(std::size_t size);
    std::size_t getSize() const;
    void process(Utility::IPC::Data const& data);

private:
    void validate(Utility::IPC::Data const& data);

private:
    std::size_t m_prev_seqn;
};

}
