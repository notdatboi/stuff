#include <Utility/IPC/CSemaphore.hpp>

#include <stdexcept>
#include <ctime>

namespace Utility
{
namespace IPC
{

CSemaphore::CSemaphore(std::string name, bool locked)
    : m_semaphore{SEM_FAILED}
    , m_name{std::move(name)}
{
    m_semaphore = sem_open(m_name.c_str(), O_CREAT, c_data_access, locked ? 0 : 1);
    if (m_semaphore == SEM_FAILED)
    {
        throw std::runtime_error{"Can't open semaphore. Please ensure that you have correct access rights."};
    }
}

CSemaphore::~CSemaphore()
{
    if (m_semaphore != SEM_FAILED)
    {
        sem_close(m_semaphore);
        sem_unlink(m_name.c_str());
    }
}

bool CSemaphore::timed_lock(std::chrono::seconds timeout)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    ts.tv_sec += timeout.count();
    return sem_clockwait(m_semaphore, CLOCK_MONOTONIC, &ts) != -1;
}

void CSemaphore::lock()
{
    sem_wait(m_semaphore);
}

void CSemaphore::unlock()
{
    sem_post(m_semaphore);
}

}
}