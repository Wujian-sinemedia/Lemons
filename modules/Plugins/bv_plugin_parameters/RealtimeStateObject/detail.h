#pragma once

namespace bav::plugin
{
template < typename OwnedObjectType >
class RealtimeStateObject< OwnedObjectType >::ThreadState
{
public:
    template < typename... Args >
    ThreadState (Args&&... args)
    {
        objectCopy = std::make_unique< OwnedObjectType > (std::forward< Args > (args)...);
    }

    OwnedObjectType& acquire()
    {
        casLoop.hot_acquire();
    }

    void release()
    {
        casLoop.hot_release();
    }

    void copyFrom (OwnedObjectType& source)
    {
        copyObject (source, *objectCopy);
    }

private:
    std::unique_ptr< OwnedObjectType > objectCopy;

    CASLoop< OwnedObjectType > casLoop {*objectCopy};
};


/*--- realtime write ---*/

template < typename OwnedObjectType >
OwnedObjectType& RealtimeStateObject< OwnedObjectType >::realtime_beginWrite()
{
    return realtime.acquire();
}

template < typename OwnedObjectType >
void RealtimeStateObject< OwnedObjectType >::realtime_endWrite()
{
    realtime.release();
    nonrealtime.copyFrom (realtime.acquire());
    realtime.release();
}


/*--- non realtime write ---*/

template < typename OwnedObjectType >
OwnedObjectType& RealtimeStateObject< OwnedObjectType >::nonRealtime_beginWrite()
{
    return nonrealtime.acquire();
}

template < typename OwnedObjectType >
void RealtimeStateObject< OwnedObjectType >::nonRealtime_endWrite()
{
    nonrealtime.release();
    realtime.copyFrom (nonrealtime.acquire());
    nonrealtime.release();
}


/*--- realtime read ---*/

template < typename OwnedObjectType >
const OwnedObjectType& RealtimeStateObject< OwnedObjectType >::realtime_beginRead()
{
    return realtime.acquire();
}

template < typename OwnedObjectType >
void RealtimeStateObject< OwnedObjectType >::realtime_endRead()
{
    realtime.release();
}


/*--- non realtime read ---*/

template < typename OwnedObjectType >
const OwnedObjectType& RealtimeStateObject< OwnedObjectType >::nonrealtime_beginRead()
{
    return nonrealtime.acquire();
}

template < typename OwnedObjectType >
void RealtimeStateObject< OwnedObjectType >::nonrealtime_endRead()
{
    nonrealtime.release();
}


/*--------------------------------------------------------------------------
    RAII OBJECTS
 --------------------------------------------------------------------------*/

template < typename OwnedObjectType >
struct RealtimeStateObject< OwnedObjectType >::RealtimeScopedWrite
{
    RealtimeScopedWrite (RealtimeStateObject& state)
        : owner (state), object (owner.realtime_beginWrite())
    {
    }

    ~RealtimeScopedWrite()
    {
        owner.realtime_endWrite();
    }

    OwnedObjectType& get() { return object; }
    OwnedObjectType& operator()() { return object; }
    OwnedObjectType& operator*() { return object; }
    OwnedObjectType* operator->() { return &object; }

private:
    RealtimeStateObject& owner;
    OwnedObjectType&     object;
};

template < typename OwnedObjectType >
typename RealtimeStateObject< OwnedObjectType >::RealtimeScopedWrite RealtimeStateObject< OwnedObjectType >::realtimeWrite()
{
    return {*this};
}


template < typename OwnedObjectType >
struct RealtimeStateObject< OwnedObjectType >::RealtimeScopedRead
{
    RealtimeScopedRead (RealtimeStateObject& state)
        : owner (state), object (owner.realtime_beginRead())
    {
    }

    ~RealtimeScopedRead()
    {
        owner.realtime_endRead();
    }

    const OwnedObjectType& get() const { return object; }
    const OwnedObjectType& operator()() const { return object; }
    const OwnedObjectType& operator*() const { return object; }
    const OwnedObjectType* operator->() const { return &object; }

private:
    RealtimeStateObject&   owner;
    const OwnedObjectType& object;
};

template < typename OwnedObjectType >
typename RealtimeStateObject< OwnedObjectType >::RealtimeScopedRead RealtimeStateObject< OwnedObjectType >::realtimeRead()
{
    return {*this};
}


template < typename OwnedObjectType >
struct RealtimeStateObject< OwnedObjectType >::NonrealtimeScopedWrite
{
    NonrealtimeScopedWrite (RealtimeStateObject& state)
        : owner (state), object (owner.nonRealtime_beginWrite())
    {
    }

    ~NonrealtimeScopedWrite()
    {
        owner.nonRealtime_endWrite();
    }

    OwnedObjectType& get() { return object; }
    OwnedObjectType& operator()() { return object; }
    OwnedObjectType& operator*() { return object; }
    OwnedObjectType* operator->() { return &object; }

private:
    RealtimeStateObject& owner;
    OwnedObjectType&     object;
};

template < typename OwnedObjectType >
typename RealtimeStateObject< OwnedObjectType >::NonrealtimeScopedWrite RealtimeStateObject< OwnedObjectType >::nonrealtimeWrite()
{
    return {*this};
}


template < typename OwnedObjectType >
struct RealtimeStateObject< OwnedObjectType >::NonrealtimeScopedRead
{
    NonrealtimeScopedRead (RealtimeStateObject& state)
        : owner (state), object (owner.nonrealtime_beginRead())
    {
    }

    ~NonrealtimeScopedRead()
    {
        owner.nonrealtime_endRead();
    }

    const OwnedObjectType& get() const { return object; }
    const OwnedObjectType& operator()() const { return object; }
    const OwnedObjectType& operator*() const { return object; }
    const OwnedObjectType* operator->() const { return &object; }

private:
    RealtimeStateObject&   owner;
    const OwnedObjectType& object;
};

template < typename OwnedObjectType >
typename RealtimeStateObject< OwnedObjectType >::NonrealtimeScopedRead RealtimeStateObject< OwnedObjectType >::nonrealtimeRead()
{
    return {*this};
}

}  // namespace bav::plugin
