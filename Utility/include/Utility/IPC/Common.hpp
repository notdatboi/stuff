#pragma once

#include <span>

#include <Utility/Common.hpp>

namespace Utility
{
namespace IPC
{

using Payload = std::span<char>;
struct Metadata
{
    CTimestamp::rep ts;
    std::size_t seqn;
    std::size_t checksum;
    std::size_t data_size;
};
struct Data
{
    Metadata& metadata;
    Payload payload;
};

constexpr char c_data_shm_name[] = "/test_task_data_shm_0";
constexpr char c_data_produced_sem_name[] = "/test_task_sem_prod_14";
constexpr char c_data_consumed_sem_name[] = "/test_task_sem_cons_14";

constexpr int c_data_access = 0666;

}
}
