#pragma once

#include "CASLoop.h"

namespace bav::plugin
{
template < typename OwnedObjectType >
class RealtimeStateObject
{
public:
    template < typename... Args >
    RealtimeStateObject (Args&&... args)
        : realtime (std::forward< Args > (args)...), nonrealtime (std::forward< Args > (args)...)
    {
    }

    virtual ~RealtimeStateObject() = default;

    struct RealtimeScopedWrite;
    struct RealtimeScopedRead;
    struct NonrealtimeScopedWrite;
    struct NonrealtimeScopedRead;

    RealtimeScopedWrite    realtimeWrite();
    RealtimeScopedRead     realtimeRead();
    NonrealtimeScopedWrite nonrealtimeWrite();
    NonrealtimeScopedRead  nonrealtimeRead();

private:
    class ThreadState;

    friend struct RealtimeScopedWrite;
    friend struct RealtimeScopedRead;
    friend struct NonrealtimeScopedWrite;
    friend struct NonrealtimeScopedRead;

    OwnedObjectType& realtime_beginWrite();
    void             realtime_endWrite();

    const OwnedObjectType& realtime_beginRead();
    void                   realtime_endRead();

    OwnedObjectType& nonRealtime_beginWrite();
    void             nonRealtime_endWrite();

    const OwnedObjectType& nonrealtime_beginRead();
    void                   nonrealtime_endRead();

    ThreadState realtime, nonrealtime;
};

}  // namespace bav::plugin

#include "detail.h"
