#pragma once

namespace bav::plugin
{
template < typename OwnedObjectType >
class RealtimeStateObject
{
public:
    struct ScopedWrite;
    struct ScopedRead;

    ScopedWrite realtimeWrite();
    ScopedRead  realtimeRead();
    ScopedWrite nonrealtimeWrite();
    ScopedRead  nonrealtimeRead();

private:
    friend struct ScopedWrite;
    friend struct ScopedRead;

    OwnedObjectType& realtime_beginWrite();
    void             realtime_endWrite();

    const OwnedObjectType& realtime_beginRead();
    void                   realtime_endRead();

    OwnedObjectType& nonRealtime_beginWrite();
    void             nonRealtime_endWrite();

    const OwnedObjectType& nonrealtime_beginRead();
    void                   nonrealtime_endRead();

    OwnedObjectType object;
};

}  // namespace bav::plugin

#include "detail.h"
