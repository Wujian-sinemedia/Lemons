
namespace bav::vecops
{
    

    /*
        This namespace contains several floating inlined functions that extend the functionality of JUCE's FloatVectorOperations class.
        Apple's vDSP framework is used where possible.
    */



// returns the index in the vector of the element with the minimum value
template<typename DataType>
static inline int findIndexOfMinElement (DataType* data, const int dataSize)
{
    jassert (dataSize > 1);
#if BV_USE_VDSP
    constexpr auto strideOfOne = vDSP_Stride(1);
    unsigned long index = 0.0;
    DataType minimum = 0;
    
    if constexpr (std::is_same_v <DataType, float>)
        vDSP_minvi (data, strideOfOne, &minimum, &index, vDSP_Length(dataSize));
    else if constexpr (std::is_same_v <DataType, double>)
        vDSP_minviD (data, strideOfOne, &minimum, &index, vDSP_Length(dataSize));
    
    return int(index);
#else
    return static_cast<int> (std::distance (data,
                                            std::min_element (data, data + dataSize)));
#endif
}
    
    
// returns the index in the vector of the element with the maximum value
template<typename DataType>
static inline int findIndexOfMaxElement (DataType* data, const int dataSize)
{
    jassert (dataSize > 1);
#if BV_USE_VDSP
    constexpr auto strideOfOne = vDSP_Stride(1);
    unsigned long index = 0.0;
    DataType maximum = 0;
    
    if constexpr (std::is_same_v <DataType, float>)
        vDSP_maxvi (data, strideOfOne, &maximum, &index, vDSP_Length(dataSize));
    else if constexpr (std::is_same_v <DataType, double>)
        vDSP_maxviD (data, strideOfOne, &maximum, &index, vDSP_Length(dataSize));
    
    return int(index);
#else
    return static_cast<int> (std::distance (data,
                                            std::max_element (data, data + dataSize)));
#endif
}
    

// returns both the minimum element and its index in the vector into the variables minimum and minIndex
template<typename DataType>
static inline void findMinAndMinIndex (DataType* data, const int dataSize,
                                       DataType& minimum, int& minIndex)
{
    jassert (dataSize > 1);
#if BV_USE_VDSP
    constexpr auto strideOfOne = vDSP_Stride(1);
    unsigned long index = 0.0;
    
    if constexpr (std::is_same_v <DataType, float>)
        vDSP_minvi (data, strideOfOne, &minimum, &index, vDSP_Length(dataSize));
    else if constexpr (std::is_same_v <DataType, double>)
        vDSP_minviD (data, strideOfOne, &minimum, &index, vDSP_Length(dataSize));
    
    minIndex = int (index);
#else
    auto* lowestElement = std::min_element (data, data + dataSize);
    minimum = *lowestElement;
    minIndex = static_cast<int> (std::distance (data, lowestElement));
#endif
}
    

// returns both the maximum element and its index in the vector into the variables maximum and maxIndex
template<typename DataType>
static inline void findMaxAndMaxIndex (DataType* data, const int dataSize,
                                       DataType& maximum, int& maxIndex)
{
    jassert (dataSize > 1);
#if BV_USE_VDSP
    constexpr auto strideOfOne = vDSP_Stride(1);
    unsigned long index = 0.0;
    
    if constexpr (std::is_same_v <DataType, float>)
        vDSP_maxvi (data, strideOfOne, &maximum, &index, vDSP_Length(dataSize));
    else if constexpr (std::is_same_v <DataType, double>)
        vDSP_maxviD (data, strideOfOne, &maximum, &index, vDSP_Length(dataSize));
    
    maxIndex = int (index);
#else
    auto* highestElement = std::max_element (data, data + dataSize);
    maximum = *highestElement;
    maxIndex = static_cast<int> (std::distance (data, highestElement));
#endif
}
    

// locates the element with the highest absolute value and its index in the vector, and returns them into the variables greatestMagnitude and index
template<typename DataType>
static inline void locateGreatestAbsMagnitude (DataType* data, const int dataSize,
                                               DataType& greatestMagnitude, int& index)
{
    jassert (dataSize > 1);
#if BV_USE_VDSP
    constexpr auto strideOfOne = vDSP_Stride(1);
    unsigned long i = 0.0;
    
    if constexpr (std::is_same_v <DataType, float>)
        vDSP_maxmgvi (data, strideOfOne, &greatestMagnitude, &i, vDSP_Length(dataSize));
    else if constexpr (std::is_same_v <DataType, double>)
        vDSP_maxmgviD (data, strideOfOne, &greatestMagnitude, &i, vDSP_Length(dataSize));
    
    index = int(i);
#else
    int strongestMagIndex = 0;
    auto strongestMag = abs(data[0]);
    
    for (int i = 1; i < dataSize; ++i)
    {
        const auto current = abs(data[i]);
        
        if (current > strongestMag)
        {
            strongestMag = current;
            strongestMagIndex = i;
        }
    }
    
    greatestMagnitude = strongestMag;
    index = strongestMagIndex;
#endif
}
    

// locates the element with the lowest absolute value and its index in the vector, and returns them into the variables leastMagnitude and index
template<typename DataType>
static inline void locateLeastAbsMagnitude (DataType* data, const int dataSize,
                                            DataType& leastMagnitude, int& index)
{
    jassert (dataSize > 1);
#if BV_USE_VDSP
    constexpr auto strideOfOne = vDSP_Stride(1);
    unsigned long i = 0.0;
    
    if constexpr (std::is_same_v <DataType, float>)
        vDSP_minmgvi (data, strideOfOne, &leastMagnitude, &i, vDSP_Length(dataSize));
    else if constexpr (std::is_same_v <DataType, double>)
        vDSP_minmgviD (data, strideOfOne, &leastMagnitude, &i, vDSP_Length(dataSize));
    
    index = int(i);
#else
    int weakestMagIndex = 0;
    auto weakestMag = abs(data[0]);
    
    for (int i = 1; i < dataSize; ++i)
    {
        const auto current = abs(data[i]);
        
        if (current < weakestMag)
        {
            weakestMag = current;
            weakestMagIndex = i;
        }
    }
    
    leastMagnitude = weakestMag;
    index = weakestMagIndex;
#endif
}
    

// finds both the maximum and minimum elements in the vector and returns them into the variables max and min.
template<typename DataType>
static inline void findExtrema (DataType* data, const int dataSize,
                                DataType& min, DataType& max)
{
    auto range = juce::FloatVectorOperations::findMinAndMax (data, dataSize);
    min = range.getStart();
    max = range.getEnd();
}
    
    
// returns the distance between the maximum and minimum element of the vector
template<typename DataType>
static inline DataType findRangeOfExtrema (DataType* data, const int dataSize)
{
    return juce::FloatVectorOperations::findMinAndMax (data, dataSize)
                .getLength();
}
    
    
// deinterleave samples from dst into src
template<typename T>
static inline void deinterleave (T* dst,
                                 const T* src,
                                 const int channels,
                                 const int count)
{
    int idx = 0;
    
    switch (channels)
    {
        case 2:
            // common case, may be vectorized by compiler if hardcoded
            for (int i = 0; i < count; ++i) {
                for (int j = 0; j < 2; ++j) {
                    dst[j][i] = src[idx++];
                }
            }
            return;
            
        case 1:
            juce::FloatVectorOperations::copy (dst[0], src, count);
            return;
            
        default:
            for (int i = 0; i < count; ++i) {
                for (int j = 0; j < channels; ++j) {
                    dst[j][i] = src[idx++];
                }
            }
    }
}
    
    
template<typename SampleType>
static inline void cartesian_to_polar (SampleType* const mag,
                                       SampleType* const phase,
                                       const SampleType* const real,
                                       const SampleType* const imag,
                                       const int count)
{
#if BV_USE_VDSP
    if constexpr (std::is_same_v <SampleType, float>)
    {
        DSPSplitComplex c;
        c.realp = const_cast<float*>(real);
        c.imagp = const_cast<float*>(imag);
        vDSP_zvmags (&c, 1, phase, 1, count); // using phase as a temporary dest
        vvsqrtf (mag, phase, &count); // using phase as the source
        vvatan2f (phase, imag, real, &count);
    }
    else if constexpr (std::is_same_v <SampleType, double>)
    {
        DSPDoubleSplitComplex c;
        c.realp = const_cast<double*>(real);
        c.imagp = const_cast<double*>(imag);
        vDSP_zvmagsD (&c, 1, phase, 1, count); // using phase as a temporary dest
        vvsqrt (mag, phase, &count); // using phase as the source
        vvatan2 (phase, imag, real, &count);
    }
#elif BV_USE_IPP
    if constexpr (std::is_same_v <SampleType, float>)
    {
        ippsCartToPolar_32f (real, imag, mag, phase, count);
    }
    else if constexpr (std::is_same_v <SampleType, double>)
    {
        ippsCartToPolar_64f (real, imag, mag, phase, count);
    }
#else
    for (int i = 0; i < count; ++i)
    {
        c_magphase<T>(mag + i, phase + i, real[i], imag[i]);
    }
#endif
}
    
    
template<typename T>
static inline void c_magphase (T* mag, T* phase, T real, T imag)
{
    *mag = sqrt (real * real + imag * imag);
    *phase = atan2 (imag, real);
}
    
    
}  // namespace
