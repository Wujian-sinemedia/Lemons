#pragma once

namespace bav
{
template < typename OwnedObjectType >
struct CASLoop
{
    CASLoop (OwnedObjectType& objectToReference)
        : pointer (&objectToReference)
    {
    }

    OwnedObjectType& hot_acquire()
    {
        localPntr = pointer.exchange (nullptr);
        jassert (localPntr != nullptr);
        return *localPntr;
    }

    void hot_release()
    {
        jassert (localPntr != nullptr);
        pointer.store (localPntr);
        localPntr = nullptr;
    }

    void cool_swap (OwnedObjectType& newObject)
    {
        for (auto* expected = localPntr;
             ! pointer.compare_exchange_strong (expected, &newObject);
             expected = localPntr)
            ;
    }

private:
    std::atomic< OwnedObjectType* > pointer;

    OwnedObjectType* localPntr {nullptr};
};

}  // namespace bav
