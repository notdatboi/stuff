#include <Utility/IPC/CSharedMemoryReceiver.hpp>

#include <mutex>
#include <unistd.h>

namespace Utility
{
namespace IPC
{

CSharedMemoryReceiver::CSharedMemoryReceiver()
    : IIPCReceiverBackend{}
    , m_produced_sem{c_data_produced_sem_name, true}
    , m_consumed_sem{c_data_consumed_sem_name, false}
    , m_shm{-1}
{
    std::ignore = tryOpenShm();
}

CSharedMemoryReceiver::~CSharedMemoryReceiver()
{
    close(m_shm);
}

void CSharedMemoryReceiver::receive(ReaderFn reader)
{
    m_produced_sem.lock();
    doReceive(std::move(reader));
} 

bool CSharedMemoryReceiver::timed_receive(std::chrono::seconds waiting_time, ReaderFn reader)
{
    if (m_produced_sem.timed_lock(waiting_time))
    {
        doReceive(std::move(reader));
        return true;
    }
    return false;
} 

void CSharedMemoryReceiver::doReceive(ReaderFn reader)
{
    if (tryOpenShm())
    {
        auto* mapped_metadata = reinterpret_cast<Metadata*>(mmap(nullptr, sizeof(Metadata), PROT_READ, MAP_SHARED, m_shm, 0));
        auto const payload_size = mapped_metadata->data_size;
        munmap(mapped_metadata, sizeof(Metadata)); // can't just mmap again payload-only due to paging

        auto const data_size = sizeof(Metadata) + payload_size;
        void* mapped_data = mmap(nullptr, data_size, PROT_READ, MAP_SHARED, m_shm, 0);
        mapped_metadata = reinterpret_cast<Metadata*>(mapped_data);
        auto* mapped_payload = reinterpret_cast<char*>(mapped_data) + sizeof(Metadata);
        Data data{*mapped_metadata, Payload{mapped_payload, payload_size}};
        reader(data);
        
        munmap(mapped_data, data_size);
        m_consumed_sem.unlock();
    }
    else
    {
        m_produced_sem.unlock(); // try to read again after some time
    }
} 

bool CSharedMemoryReceiver::tryOpenShm()
{
    if (m_shm == -1)
    {
        m_shm = shm_open(c_data_shm_name, O_RDONLY, c_data_access);
    }
    return m_shm != -1;
}

}
}