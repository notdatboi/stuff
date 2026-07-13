# Test Task
---
## Prerequisites
1. POSIX system (preferably linux, tested only on wsl Ubuntu 20.04)
2. CMake 3.0 or newer
3. GCC compiler with C++20 support
4. No external libs needed

## How to build and run
1. Download repo
2. chmod +x release.sh
3. Run ./release.sh in repo folder
4. Open separate terminal in repo folder
5. In one terminal, execute ./install/bin/producer
6. In the other terminal, execute ./install/bin/consumer (or vice versa, they can be started in any order)

### Functionality
Producer and consumer both otput the full list of available commands.

#### Producer
Press Enter after operation to confirm.
- Available operations:
    - `x` to exit
    - `<number>` to set payload size (1 to uint32_t max)
    - `p` to pause
    - `u` to unpause
    - `d` to enter debug mode (chance of invalid packets)
    - `r` to exit debug mode

#### Consumer
Press Enter after operation to confirm.
- Available operations:
    - `x` to exit
    - `p` to pause
    - `u` to unpause

#### Notes
When consumer is paused or turned off, producer will write to memory and wait for consumer to read it.
When producer is paused or turned off, consumer will wait for memory write by producer.
Producer produces random ASCII data.

---
## Performance

### System
* Processor: AMD Ryzen 9 9900x 12 cores 24 threads
* RAM: 32GB

### Logs

#### Payload size = 1 byte (default)
Statistics: 
- Total packet count: 130129
- Packets per second: 13129
- Bytes per second: 433263
- Kilobytes per second: 423
- Megabytes per second: 0
- Total invalid timestamp errors: 0
- Total sequence inconsistency errors: 0
- Total checksum mismatch errors: 0

#### Payload size = 1KB
Statistics: 
- Total packet count: 209610
- Packets per second: 12577
- Bytes per second: 13282002
- Kilobytes per second: 12970
- Megabytes per second: 12
- Total invalid timestamp errors: 0
- Total sequence inconsistency errors: 0
- Total checksum mismatch errors: 0

#### Payload size = 1MB
Statistics: 
- Total packet count: 133609
- Packets per second: 217
- Bytes per second: 227928200
- Kilobytes per second: 222586
- Megabytes per second: 217
- Total invalid timestamp errors: 0
- Total sequence inconsistency errors: 0
- Total checksum mismatch errors: 0

#### Payload size = 1GB
Statistics: 
- Total packet count: 28463
- Packets per second: 0
- Bytes per second: 229876926
- Kilobytes per second: 224489
- Megabytes per second: 219
- Total invalid timestamp errors: 0
- Total sequence inconsistency errors: 0
- Total checksum mismatch errors: 0