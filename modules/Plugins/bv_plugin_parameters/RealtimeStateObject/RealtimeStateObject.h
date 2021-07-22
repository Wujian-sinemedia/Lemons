#pragma once

namespace bav::plugin
{
template < typename OwnedObjectType >
class RealtimeStateObject
{
public:
    /*--- realtime write ---*/

    OwnedObjectType& realtime_beginWrite();
    void             realtime_endWrite();

    struct RealtimeScopedWrite;

    RealtimeScopedWrite realtimeWrite();


    /*--- realtime read ---*/

    const OwnedObjectType& realtime_beginRead();
    void                   realtime_endRead();

    struct RealtimeScopedRead;

    RealtimeScopedRead realtimeRead();


    /*--- non realtime write ---*/

    OwnedObjectType& nonRealtime_beginWrite();
    void             nonRealtime_endWrite();

    struct NonrealtimeScopedWrite;

    NonrealtimeScopedWrite nonrealtimeWrite();


    /*--- non realtime read ---*/

    const OwnedObjectType& nonrealtime_beginRead();
    void                   nonrealtime_endRead();

    struct NonrealtimeScopedRead;

    NonrealtimeScopedRead nonrealtimeRead();

private:
    OwnedObjectType object;
};

}  // namespace bav::plugin

#include "detail.h"
