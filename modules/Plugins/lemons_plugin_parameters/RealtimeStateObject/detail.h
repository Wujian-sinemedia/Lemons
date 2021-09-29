#pragma once

namespace lemons::plugin
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


/*--------------------------------------------------------------------------
    RAII OBJECTS
 --------------------------------------------------------------------------*/

#define BV_RTSO_IMPLEMENT_RAII_HELPER(Class) \
    Class& get() { return object; }          \
    Class& operator()() { return object; }   \
    Class& operator*() { return object; }    \
    Class* operator->() { return &object; }

template < typename OwnedObjectType >
struct RealtimeStateObject< OwnedObjectType >::RealtimeScopedWrite
{
    RealtimeScopedWrite (RealtimeStateObject& state)
        : owner (state), object (owner.realtime.acquire())
    {
    }

    ~RealtimeScopedWrite()
    {
        owner.realtime.release();
        owner.nonrealtime.copyFrom (owner.realtime.acquire());
        owner.realtime.release();
    }

    BV_RTSO_IMPLEMENT_RAII_HELPER (OwnedObjectType)

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
        : owner (state), object (owner.realtime.acquire())
    {
    }

    ~RealtimeScopedRead()
    {
        owner.realtime.release();
    }

    BV_RTSO_IMPLEMENT_RAII_HELPER (const OwnedObjectType)

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
        : owner (state), object (owner.nonrealtime.acquire())
    {
    }

    ~NonrealtimeScopedWrite()
    {
        owner.nonrealtime.release();
        owner.realtime.copyFrom (owner.nonrealtime.acquire());
        owner.nonrealtime.release();
    }

    BV_RTSO_IMPLEMENT_RAII_HELPER (OwnedObjectType)

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
        : owner (state), object (owner.nonrealtime.acquire())
    {
    }

    ~NonrealtimeScopedRead()
    {
        owner.nonrealtime.release();
    }

    BV_RTSO_IMPLEMENT_RAII_HELPER (const OwnedObjectType)

private:
    RealtimeStateObject&   owner;
    const OwnedObjectType& object;
};

template < typename OwnedObjectType >
typename RealtimeStateObject< OwnedObjectType >::NonrealtimeScopedRead RealtimeStateObject< OwnedObjectType >::nonrealtimeRead()
{
    return {*this};
}

}  // namespace lemons::plugin

#undef BV_RTSO_IMPLEMENT_RAII_HELPER
