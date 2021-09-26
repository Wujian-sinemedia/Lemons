#pragma once

namespace bav
{
template < class T >
struct WrapperType
{
    using WrappedObjectType = T;

    virtual ~WrapperType() = default;

    T&       operator*() { return object; }
    const T& operator*() const { return object; }

    T*       operator->() { return object; }
    const T* operator->() const { return object; }

    WrapperType& operator= (const T& other)
    {
        object = other;
        return *this;
    }

    WrapperType& operator= (const WrapperType& other)
    {
        object = other.object;
        return *this;
    }

    bool operator== (const T& other)
    {
        return object == other;
    }

    bool operator== (const WrapperType& other)
    {
        return object == other.object;
    }

    T&       getWrappedObject() { return object; }
    const T& getWrappedObject() const { return object; }

private:
    T object;
};

}  // namespace bav
