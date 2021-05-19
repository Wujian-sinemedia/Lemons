/*
    This file provides functions that attempt to allocate memory in aligned blocks
*/


/*
 These conditionals declare the following macros:
 
 - BV_MALLOC_IS_ALIGNED
 - BV_HAVE__ALIGNED_MALLOC
 */

#ifndef MALLOC_IS_NOT_ALIGNED
#if JUCE_IOS || JUCE_MAC
#define BV_MALLOC_IS_ALIGNED 1
#endif
#endif

#ifndef BV_MALLOC_IS_ALIGNED
#define BV_MALLOC_IS_ALIGNED 0
#endif

#ifndef LACK__ALIGNED_MALLOC
#if JUCE_WINDOWS
#define BV_HAVE__ALIGNED_MALLOC 1
#endif
#endif

#ifndef BV_HAVE__ALIGNED_MALLOC
#define BV_HAVE__ALIGNED_MALLOC 0
#endif


#if BV_POSIX
#include <sys/mman.h>
#endif


namespace bav
{
template < typename T >
T* aligned_allocate (size_t count)
{
    void* ptr = nullptr;

#if BV_MALLOC_IS_ALIGNED
    ptr = malloc (count * sizeof (T));
#else

    static const int alignment = BV_BIT_DEPTH;

#if BV_HAVE__ALIGNED_MALLOC
    ptr                        = _aligned_malloc (count * sizeof (T), alignment);
#else

#if BV_POSIX
    auto rv = posix_memalign (&ptr, alignment, count * sizeof (T));

    if (rv != 0)
    {
        if (rv == EINVAL)
            throw std::logic_error (
                "Internal error: invalid size passed to aligned_allocate");
        else
            throw std::bad_alloc();
    }

#else /* BV_POSIX */
#warning                                                                            \
    "No aligned malloc implementation is available for your platform. Hacking one together for you now..."

    // Alignment must be a power of two, bigger than the pointer
    // size. Stuff the actual malloc'd pointer in just before the
    // returned value.  This is the least desirable way to do this --
    // the other options are all better  ¯\_(ツ)_/¯
    size_t allocd = count * sizeof (T) + alignment;
    void*  buf    = malloc (allocd);
    if (buf != nullptr)
    {
        char* adj = (char*) buf;
        while ((unsigned long long) adj & (alignment - 1))
            --adj;
        ptr = ((char*) adj) + alignment;
        new (((void**) ptr)[-1]) (void*);
        ((void**) ptr)[-1] = buf;
    }

#endif /* BV_POSIX */
#endif /* BV_HAVE__ALIGNED_MALLOC */
#endif /* BV_MALLOC_IS_ALIGNED */

    if (ptr == nullptr) { throw std::bad_alloc(); }

    T* typed_ptr = static_cast< T* > (ptr);

    for (size_t i = 0; i < count; ++i)
    {
        new (typed_ptr + i) T;
    }

    if (typed_ptr == nullptr) { throw std::bad_alloc(); }

    return typed_ptr;
}


#if BV_USE_IPP

template <>
float* aligned_allocate (size_t count)
{
    if (count > INT_MAX)
    {
        throw std::length_error ("Size overflow in aligned_allocate");
    }

    auto* ptr = ippsMalloc_32f (int (count));

    if (ptr == nullptr) { throw (std::bad_alloc()); }

    for (size_t i = 0; i < count; ++i)
    {
        new (ptr + i) float;
    }
    return ptr;
}

template <>
double* aligned_allocate (size_t count)
{
    if (count > INT_MAX)
    {
        throw std::length_error ("Size overflow in aligned_allocate");
    }

    auto* ptr = ippsMalloc_64f (int (count));

    if (ptr == nullptr) { throw (std::bad_alloc()); }

    for (size_t i = 0; i < count; ++i)
    {
        new (ptr + i) double;
    }
    return ptr;
}

#endif /* if BV_USE_IPP */


/*
        Allocates an aligned block of memory and initializes it to zero
    */

template < typename T >
T* aligned_allocate_zero (size_t count)
{
    auto ptr = aligned_allocate< T > (count);

    for (size_t i = 0; i < count; ++i)
        ptr[i] = T();

    return ptr;
}


/*
    */


template < typename T >
void aligned_deallocate (T* ptr)
{
    if (ptr == nullptr) return;

#if BV_HAVE__ALIGNED_MALLOC
    _aligned_free ((void*) ptr);
#else
    free ((void*) ptr);
#endif
}


#if BV_USE_IPP

template <>
void aligned_deallocate (float* ptr)
{
    if (ptr != nullptr) ippsFree ((void*) ptr);
}

template <>
void aligned_deallocate (double* ptr)
{
    if (ptr != nullptr) ippsFree ((void*) ptr);
}

#endif /* if BV_USE_IPP */


} // namespace bav


#undef BV_MALLOC_IS_ALIGNED
#undef BV_HAVE__ALIGNED_MALLOC
