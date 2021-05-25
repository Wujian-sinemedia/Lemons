
#pragma once


namespace bav
{
/* A basic spin lock where try_lock() is safe to call from the realtime thread and will never result in a system call. */

class RealTimeSpinLock
{
public:
    /* Takes the lock, blocking if necessary. */
    void lock() noexcept
    {
        int numTries        = 10;
        int numBackoffTries = 10;

        while (true)
        {
            for (int i = 0; i < numTries; ++i)
                if (try_lock()) return;

            for (int i = 0; i < numBackoffTries; ++i)
            {
                BV_WAIT_INSTRUCTION;

                if (try_lock()) return;
            }

            numTries -= 2;
            numBackoffTries += 10;
        }
    }


    void unlock() noexcept { flag.clear (std::memory_order_release); }


    bool try_lock() noexcept
    {
        return ! flag.test_and_set (std::memory_order_acquire);
    }

private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

}  // namespace bav
