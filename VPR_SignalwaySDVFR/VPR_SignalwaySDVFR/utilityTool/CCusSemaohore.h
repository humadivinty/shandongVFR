#ifndef CUS_SEMAPHORE
#define CUS_SEMAPHORE

#include <mutex>
#include <condition_variable>
#include <iostream>

class CCusSemaphore
{
public:
    CCusSemaphore(int cout = 0);
    ~CCusSemaphore();

    inline void notify(int threadID)
    {
        std::unique_lock<std::mutex> lck(m_mtx);
        m_iCount++;
        std::cout << "thread " << threadID << "notify" << std::endl;
        m_cv.notify_one();
    }

    inline void wait(int threadID)
    {
        std::unique_lock<std::mutex> lck(m_mtx);
        while (m_iCount == 0)
        {
            std::cout << "thread " << threadID << " wait" << std::endl;
            m_cv.wait(lck);
            std::cout << "thread " << threadID << " run" << std::endl;
        }
        m_iCount--;
    }

    inline bool tryDecrease(int threadID)
    {
        std::unique_lock<std::mutex> lck(m_mtx);
        if (m_iCount > 0)
        {
            m_iCount--;
            return true;
        }
        return false;
    }

    inline void resetCount(int threadID)
    {
        std::unique_lock<std::mutex> lck(m_mtx);
        m_iCount = 0;
        std::cout << "thread " << threadID << " resetCount" << std::endl;
    }

private:
    int m_iCount;
    std::mutex m_mtx;
    std::condition_variable m_cv;
};

#endif // !CUS_SEMAPHORE
