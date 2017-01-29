#pragma once

#include <thread>
#include <atomic>
#include <condition_variable>

class MockServer
{
public:
    void start( uint16_t uPort, size_t uTargetPacketCount );
    void stop();

    size_t getPacketCount() const;
    size_t getTotalBytes() const;

private:
    std::thread m_thread;
    std::atomic<bool> m_bExit;
    std::atomic<size_t> m_uBytesRead;
    std::atomic<size_t> m_uPacketCount;
    std::atomic<size_t> m_uTargetPacketCount;

    int m_iSocket;
    std::condition_variable m_cvMessagesRecived;
    std::mutex m_mMutex;
};