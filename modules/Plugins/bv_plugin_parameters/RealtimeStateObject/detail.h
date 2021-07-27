#pragma once

namespace bav::plugin
{
template < typename OwnedObjectType >
struct RealtimeStateObject< OwnedObjectType >::ScopedWrite
{
    ScopedWrite (RealtimeStateObject& state)
        : owner (state), object (owner.realtime_beginWrite())
    {
    }

    ~ScopedWrite()
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

/*--- realtime write ---*/

template < typename OwnedObjectType >
OwnedObjectType& RealtimeStateObject< OwnedObjectType >::realtime_beginWrite()
{
}

template < typename OwnedObjectType >
void RealtimeStateObject< OwnedObjectType >::realtime_endWrite()
{
}

template < typename OwnedObjectType >
typename RealtimeStateObject< OwnedObjectType >::ScopedWrite RealtimeStateObject< OwnedObjectType >::realtimeWrite()
{
    return {*this};
}

/*--- non realtime write ---*/

template < typename OwnedObjectType >
OwnedObjectType& RealtimeStateObject< OwnedObjectType >::nonRealtime_beginWrite()
{
}

template < typename OwnedObjectType >
void RealtimeStateObject< OwnedObjectType >::nonRealtime_endWrite()
{
}

template < typename OwnedObjectType >
typename RealtimeStateObject< OwnedObjectType >::ScopedWrite RealtimeStateObject< OwnedObjectType >::nonrealtimeWrite()
{
    return {*this};
}


template < typename OwnedObjectType >
struct RealtimeStateObject< OwnedObjectType >::ScopedRead
{
    ScopedRead (RealtimeStateObject& state)
        : owner (state), object (owner.realtime_beginRead())
    {
    }

    ~ScopedRead()
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

/*--- realtime read ---*/

template < typename OwnedObjectType >
const OwnedObjectType& RealtimeStateObject< OwnedObjectType >::realtime_beginRead()
{
}

template < typename OwnedObjectType >
void RealtimeStateObject< OwnedObjectType >::realtime_endRead()
{
}

template < typename OwnedObjectType >
typename RealtimeStateObject< OwnedObjectType >::ScopedRead RealtimeStateObject< OwnedObjectType >::realtimeRead()
{
    return {*this};
}

/*--- non realtime read ---*/

template < typename OwnedObjectType >
const OwnedObjectType& RealtimeStateObject< OwnedObjectType >::nonrealtime_beginRead()
{
}

template < typename OwnedObjectType >
void RealtimeStateObject< OwnedObjectType >::nonrealtime_endRead()
{
}

template < typename OwnedObjectType >
typename RealtimeStateObject< OwnedObjectType >::ScopedRead RealtimeStateObject< OwnedObjectType >::nonrealtimeRead()
{
    return {*this};
}

}  // namespace bav::plugin
