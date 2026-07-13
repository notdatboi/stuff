#include <Utility/IPC/CSharedMemorySender.hpp>

#include <mutex>
#include <unistd.h>

namespace Utility
{
namespace IPC
{

CSharedMemorySender::CSharedMemorySender()
    : IIPCSenderBackend{}
    , m_produced_sem{c_data_produced_sem_name, true}
    , m_consumed_sem{c_data_consumed_sem_name, false}
    , m_shm{-1}
    , m_payload_size{0u}
{
    m_shm = shm_open(c_data_shm_name, O_CREAT | O_RDWR, c_data_access);
    auto const data_size = sizeof(Metadata) + m_payload_size;
    ftruncate(m_shm, data_size);
}

CSharedMemorySender::~CSharedMemorySender()
{
    close(m_shm);
    shm_unlink(c_data_shm_name);
}

void CSharedMemorySender::send(WriterFn writer, std::size_t payload_size_bytes)
{
    m_consumed_sem.lock();
    doSend(std::move(writer), payload_size_bytes);
}

bool CSharedMemorySender::timed_send(std::chrono::seconds waiting_time, WriterFn writer, std::size_t payload_size_bytes)
{
    if (m_consumed_sem.timed_lock(waiting_time))
    {
        doSend(std::move(writer), payload_size_bytes);
        return true;
    }
    return false;
}

void CSharedMemorySender::doSend(WriterFn writer, std::size_t payload_size_bytes)
{
    if (payload_size_bytes != m_payload_size)
    {
        m_payload_size = payload_size_bytes;
        ftruncate(m_shm, sizeof(Metadata) + m_payload_size);
    }
    auto const data_size = sizeof(Metadata) + m_payload_size;
    void* mapped_data = mmap(nullptr, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_shm, 0);
    auto* mapped_metadata = reinterpret_cast<Metadata*>(mapped_data);
    auto* mapped_payload = reinterpret_cast<char*>(mapped_data) + sizeof(Metadata);
    Data data{*mapped_metadata, Payload{mapped_payload, m_payload_size}};
    writer(data);
    munmap(mapped_data, data_size);
    m_produced_sem.unlock();
}

}
}