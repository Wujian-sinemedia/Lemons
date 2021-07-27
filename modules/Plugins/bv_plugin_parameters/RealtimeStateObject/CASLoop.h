#pragma once

namespace bav
{
template < typename OwnedObjectType >
struct CASLoop
{
    CASLoop (OwnedObjectType& object)
        : sharedObject (object)
    {
    }

    OwnedObjectType& hot_acquire()
    {
        localPntr = pointer.exchange (nullptr);
        return *localPntr;
    }

    void hot_release()
    {
        pointer.store (localPntr);
    }

    void cool_swap (OwnedObjectType& newObject)
    {
        for (auto* expected = &sharedObject;
             ! pointer.compare_exchange_strong (expected, &newObject);
             expected = &sharedObject)
            ;

        sharedObject = newObject;
    }

    OwnedObjectType& sharedObject;

    std::atomic< OwnedObjectType* > pointer {&sharedObject};

private:
    OwnedObjectType* localPntr {nullptr};
};


template < typename OwnedObjectType >
class CASManager
{
public:
    CASManager (OwnedObjectType& object)
        : sharedObject (object)
    {
    }

    OwnedObjectType& realtimeAcquire()
    {
        return rtCAS.hot_acquire();
    }

    void realtimeRelease()
    {
        rtCAS.hot_release();

        // not safe here...
        nrtCAS.cool_swap (realtimeCopy);
        sharedObject = realtimeCopy;
    }

    OwnedObjectType& nonrealtimeAcquire()
    {
        // protect access to realtime copy...
        nrtCAS.cool_swap (realtimeCopy);

        return nrtCAS.hot_acquire();
    }

    void nonrealtimeRelease()
    {
        nrtCAS.hot_release();
        rtCAS.cool_swap (nonrealtimeCopy);
        sharedObject = nonrealtimeCopy;
    }

private:
    OwnedObjectType& sharedObject;

    OwnedObjectType realtimeCopy {sharedObject};
    OwnedObjectType nonrealtimeCopy {sharedObject};

    CASLoop< OwnedObjectType > rtCAS {realtimeCopy};
    CASLoop< OwnedObjectType > nrtCAS {nonrealtimeCopy};
};

}  // namespace bav
