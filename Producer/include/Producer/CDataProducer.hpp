#pragma once

#include <functional>
#include <random>

#include <Utility/IPC/Common.hpp>

namespace Producer
{

class CDataProducer
{
public:
    CDataProducer();
    ~CDataProducer();

    void setSize(std::size_t size);
    std::size_t getSize() const;
    void process(Utility::IPC::Data& data);

private:
    std::default_random_engine m_reng;
    std::uniform_int_distribution<char> m_rdist;
    std::size_t m_seqn;
    std::size_t m_size;
};

}
