#pragma once

namespace lemons::serializing
{
template < class ObjectType >
class RealtimeMutatable
{
public:
    RealtimeMutatable() = default;

    explicit RealtimeMutatable (const ObjectType& obj)
        : data ({obj, obj}), realtimeCopy (obj)
    {
    }

    ~RealtimeMutatable()
    {
        assert ((control.load() & BIT::BUSY) == 0);  // <- never delete this object while the realtime thread is still using it

        // Spin!
        while ((control.load() & BIT::BUSY) == 1)
            ;

        auto accquired = nonRealtimeLock.try_lock();

        ((void) (accquired));
        assert (accquired);  // <- you didn't call release on one of the non-realtime threads before deleting this object

        nonRealtimeLock.unlock();
    }

    /*------------------------------------------------------------------------------------*/

    struct RealtimeAccess
    {
    public:
        ~RealtimeAccess()
        {
            parent.realtimeRelease();
        }

        ObjectType& object;

        ObjectType* operator->() { return &object; }
        ObjectType& operator()() { return object; }

    private:
        friend class RealtimeMutatable;

        RealtimeAccess (RealtimeMutatable& p)
            : object (p.realtimeAcquire()), parent (p)
        {
        }

        RealtimeMutatable& parent;
    };

    RealtimeAccess access_realtime()
    {
        return {*this};
    }

    /*------------------------------------------------------------------------------------*/

    struct NonrealtimeAccess
    {
    public:
        ~NonrealtimeAccess()
        {
            parent.nonrealtimeRelease();
        }

        const ObjectType& object;

        const ObjectType* operator->() { return &object; }
        const ObjectType& operator()() { return object; }

    private:
        friend class RealtimeMutatable;

        NonrealtimeAccess (RealtimeMutatable& p)
            : object (p.nonrealtimeAcquire()), parent (p)
        {
        }

        RealtimeMutatable& parent;
    };

    NonrealtimeAccess access_nonrealtime()
    {
        return {*this};
    }

    /*------------------------------------------------------------------------------------*/

private:
    ObjectType& realtimeAcquire() noexcept
    {
        return realtimeCopy;
    }

    void realtimeRelease() noexcept
    {
        const auto idx  = acquireIndex();
        data[idx] = realtimeCopy;
        releaseIndex (idx);
    }

    const ObjectType& nonrealtimeAcquire()
    {
        nonRealtimeLock.lock();
        auto current = control.load (std::memory_order_acquire);

        // there is new data so flip the indices around atomically ensuring we are not inside realtimeAssign
        if ((current & BIT::NEWDATA) != 0)
        {
            int newValue;

            do
            {
                // expect the realtime thread not to be inside the realtime-assign
                current &= ~BIT::BUSY;

                // realtime thread should flip index value and clear the newdata bit
                newValue = (current ^ BIT::INDEX) & BIT::INDEX;
            } while (! control.compare_exchange_weak (current, newValue, std::memory_order_acq_rel));

            current = newValue;
        }

        // flip the index bit as we always use the index that the realtime thread is currently NOT using
        const auto nonRealtimeIndex = (current & BIT::INDEX) ^ 1;

        return data[nonRealtimeIndex];
    }

    void nonrealtimeRelease()
    {
        nonRealtimeLock.unlock();
    }

    /*------------------------------------------------------------------------------------*/

    enum class BIT
    {
        INDEX   = (1 << 0),
        BUSY    = (1 << 1),
        NEWDATA = (1 << 2)
    };

    inline int acquireIndex() noexcept
    {
        return control.fetch_or (BIT::BUSY, std::memory_order_acquire) & BIT::INDEX;
    }

    inline void releaseIndex (int idx) noexcept
    {
        control.store ((idx & BIT::INDEX) | BIT::NEWDATA, std::memory_order_release);
    }

    std::atomic< int > control = {0};

    std::array< ObjectType, 2 > data;
    ObjectType                  realtimeCopy;

    std::mutex nonRealtimeLock;
};

}  // namespace lemons::serializing
