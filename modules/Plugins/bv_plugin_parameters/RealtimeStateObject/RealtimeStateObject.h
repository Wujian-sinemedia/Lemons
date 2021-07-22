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

    RealtimeScopedWrite realtimeWrite() { return {*this}; }


    /*--- realtime read ---*/

    const OwnedObjectType& realtime_beginRead();
    void                   realtime_endRead();

    struct RealtimeScopedRead;

    RealtimeScopedRead realtimeRead() { return {*this}; }


    /*--- non realtime write ---*/

    OwnedObjectType& nonRealtime_beginWrite();
    void             nonRealtime_endWrite();

    struct NonrealtimeScopedWrite;

    NonrealtimeScopedWrite nonrealtimeWrite() { return {*this}; }


    /*--- non realtime read ---*/

    const OwnedObjectType& nonrealtime_beginRead();
    void                   nonrealtime_endRead();

    struct NonrealtimeScopedRead;

    NonrealtimeScopedRead nonrealtimeRead() { return {*this}; }

private:
    OwnedObjectType object;
};

}  // namespace bav::plugin

#include "detail.h"
