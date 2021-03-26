/*
    This file provides functions that attempt to allocate memory in aligned blocks
*/


namespace bav
{
    
    template <typename T>
    T* aligned_allocate (size_t count)
    {
        void* ptr = nullptr;
        
#if BV_MALLOC_IS_ALIGNED
        ptr = malloc(count * sizeof(T));
#else
        
        // 32-byte alignment is required for at least OpenMAX
        static const int alignment = 32;
        
#if BV_HAVE__ALIGNED_MALLOC
        ptr = _aligned_malloc(count * sizeof(T), alignment);
#else
        
#if BV_POSIX_MEMALIGN
        int rv = posix_memalign(&ptr, alignment, count * sizeof(T));
        
        if (rv != 0)
        {
            if (rv == EINVAL)
                throw std::logic_error("Internal error: invalid size passed to aligned_allocate");
            else
                throw std::bad_alloc();
        }
        
#else /* BV_POSIX_MEMALIGN */
#warning "No aligned malloc implementation is available for your platform."
        
        ptr = malloc(count * sizeof(T)); // all else has failed, so just use regular malloc to at least try and get some memory here...
        
#endif /* BV_POSIX_MEMALIGN */
#endif /* BV_HAVE__ALIGNED_MALLOC */
#endif /* BV_MALLOC_IS_ALIGNED */
        
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
            throw std::length_error("Size overflow in aligned_allocate");
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
            throw std::length_error("Size overflow in aligned_allocate");
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
        
#if BV_HAVE__ALIGNED_MALLOC
        _aligned_free((void *)ptr);
#else
        free((void *)ptr);
#endif
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
