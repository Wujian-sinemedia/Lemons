/*
    This file provides functions that attempt to allocate memory in aligned blocks
*/


#ifndef HAVE_POSIX_MEMALIGN
  #ifndef LACK_POSIX_MEMALIGN
    #ifndef _WIN32
      #ifndef __APPLE__
        #define HAVE_POSIX_MEMALIGN 1
      #endif
    #endif
  #endif
#endif

#ifndef HAVE_POSIX_MEMALIGN
  #define HAVE_POSIX_MEMALIGN 0
#endif


#ifndef MALLOC_IS_ALIGNED
  #ifndef MALLOC_IS_NOT_ALIGNED
    #ifdef __APPLE__
      #define MALLOC_IS_ALIGNED 1
    #endif
  #endif
#endif

#ifndef MALLOC_IS_ALIGNED
  #define MALLOC_IS_ALIGNED 0
#endif

#ifndef HAVE__ALIGNED_MALLOC
  #ifndef LACK__ALIGNED_MALLOC
    #ifdef _WIN32
      #define HAVE__ALIGNED_MALLOC 1
    #endif
  #endif
#endif

#ifndef HAVE__ALIGNED_MALLOC
  #define HAVE__ALIGNED_MALLOC 0
#endif


#if HAVE_POSIX_MEMALIGN
  #include <sys/mman.h>
  #include <errno.h>
#endif



namespace bav
{
    
    template <typename T>
    T* aligned_allocate (size_t count)
    {
        void* ptr = nullptr;
        
#if MALLOC_IS_ALIGNED
        ptr = malloc(count * sizeof(T));
#else
        
        // 32-byte alignment is required for at least OpenMAX
        static const int alignment = 32;
        
#if HAVE__ALIGNED_MALLOC
        ptr = _aligned_malloc(count * sizeof(T), alignment);
#else
        
#if HAVE_POSIX_MEMALIGN
        int rv = posix_memalign(&ptr, alignment, count * sizeof(T));
        
        if (rv)
        {
            if (rv == EINVAL) {
                throw std::logic_error("Internal error: invalid alignment");
            } else {
                throw std::bad_alloc();
            }
        }
        
#else /* !HAVE_POSIX_MEMALIGN */
#warning "No aligned malloc implementation is available for your platform."
        
        ptr = malloc(count * sizeof(T));
        
#endif /* !HAVE_POSIX_MEMALIGN */
#endif /* !HAVE__ALIGNED_MALLOC */
#endif /* !MALLOC_IS_ALIGNED */
        
        if (ptr == nullptr)
        {
            throw std::bad_alloc();
        }
        
        T* typed_ptr = static_cast<T*>(ptr);
        
        for (size_t i = 0; i < count; ++i) {
            new (typed_ptr + i) T;
        }
        return typed_ptr;
    }
    
#if BV_USE_IPP
    
    template <>
    float* aligned_allocate (size_t count)
    {
        if (count > INT_MAX)
        {
            throw std::length_error("Size overflow in allocate");
        }
        
        float *ptr = ippsMalloc_32f(int(count));
                      
        if (ptr == nullptr)
        {
            throw (std::bad_alloc());
        }
        
        for (size_t i = 0; i < count; ++i) {
            new (ptr + i) float;
        }
        return ptr;
    }
    
    template <>
    double* aligned_allocate (size_t count)
    {
        if (count > INT_MAX)
        {
            throw std::length_error("Size overflow in allocate");
        }
        
        double *ptr = ippsMalloc_64f(int(count));
        
        if (ptr == nullptr)
        {
            throw (std::bad_alloc());
        }
        
        for (size_t i = 0; i < count; ++i) {
            new (ptr + i) double;
        }
        return ptr;
    }
    
#endif  /* if BV_USE_IPP */
    
    
    
    /*
    */
    
    
    template <typename T>
    void aligned_deallocate (T* ptr)
    {
        if (ptr == nullptr)
            return;
        
#if HAVE__ALIGNED_MALLOC
        _aligned_free((void *)ptr);
#else
        free((void *)ptr);
#endif /* !HAVE__ALIGNED_MALLOC */
    }
    
#if BV_USE_IPP
    
    template <>
    void aligned_deallocate (float* ptr)
    {
        if (ptr != nullptr)
            ippsFree ((void *)ptr);
    }
    
    template <>
    void aligned_deallocate (double* ptr)
    {
        if (ptr != nullptr)
            ippsFree ((void *)ptr);
    }
    
#endif  /* if BV_USE_IPP */
    
    
}  // namespace
