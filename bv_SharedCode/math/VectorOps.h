
namespace bav::vecops
{
    

    /*
        This namespace contains several floating inlined functions that extend the functionality of JUCE's FloatVectorOperations class.
        Apple's vDSP framework and Intel IPP are used where possible.
    */
    
    
// replaces each element in the passed vector with its square root
static inline void squareRoot (float* BV_R_ data, const int dataSize)
{
#if BV_USE_VDSP
    vvsqrtf (data, data, &dataSize);
#elif BV_USE_IPP
    ippsSqrt_32f_I (data, dataSize);
#else
    for (int i = 0; i < dataSize; ++i) {
        data[i] = sqrt (data[i]);
    }
#endif
}
    
static inline void squareRoot (double* BV_R_ data, const int dataSize)
{
#if BV_USE_VDSP
    vvsqrt (data, data, &dataSize);
#elif BV_USE_IPP
    ippsSqrt_64f_I (data, dataSize);
#else
    for (int i = 0; i < dataSize; ++i) {
        data[i] = sqrt (data[i]);
    }
#endif
}
    
    
// replaces each element in the passed vector with its square.
static inline void square (float* BV_R_ data, const int dataSize)
{
#if BV_USE_VDSP
    vDSP_vsq (data, vDSP_Stride(1), data, vDSP_Stride(1), vDSP_Length(dataSize));
#elif BV_USE_IPP
    ippsSqr_32f_I (data, dataSize);
#else
    for (int i = 0; i < dataSize; ++i) {
        const auto datum = data[i];
        data[i] = datum * datum;
    }
#endif
}
    
static inline void square (double* BV_R_ data, const int dataSize)
{
#if BV_USE_VDSP
    vDSP_vsqD (data, vDSP_Stride(1), data, vDSP_Stride(1), vDSP_Length(dataSize));
#elif BV_USE_IPP
    ippsSqr_64f_I (data, dataSize);
#else
    for (int i = 0; i < dataSize; ++i) {
        const auto datum = data[i];
        data[i] = datum * datum;
    }
#endif
}


// returns the index in the vector of the element with the minimum value
static inline int findIndexOfMinElement (const float* BV_R_ data, const int dataSize)
{
    jassert (dataSize > 1);
#if BV_USE_VDSP
    unsigned long index = 0.0;
    float minimum = 0.0f;
    vDSP_minvi (data, vDSP_Stride(1), &minimum, &index, vDSP_Length(dataSize));
    return int(index);
    
#elif BV_USE_IPP
    float minimum = 0.0f;
    int index = 0;
    ippsMinIndx_32f (data, dataSize, &minimum, &index);
    return index;
    
#else
    return static_cast<int> (std::distance (data,
                                            std::min_element (data, data + dataSize)));
#endif
}
    
static inline int findIndexOfMinElement (const double* BV_R_ data, const int dataSize)
{
    jassert (dataSize > 1);
#if BV_USE_VDSP
    unsigned long index = 0.0;
    double minimum = 0.0;
    vDSP_minviD (data, vDSP_Stride(1), &minimum, &index, vDSP_Length(dataSize));
    return int(index);
    
#elif BV_USE_IPP
    double minimum = 0.0;
    int index = 0;
    ippsMinIndx_64f (data, dataSize, &minimum, &index);
    return index;
    
#else
    return static_cast<int> (std::distance (data,
                                            std::min_element (data, data + dataSize)));
#endif
}
    
    
    
// returns the index in the vector of the element with the maximum value
static inline int findIndexOfMaxElement (const float* BV_R_ data, const int dataSize)
{
    jassert (dataSize > 1);
#if BV_USE_VDSP
    unsigned long index = 0.0;
    float maximum = 0.0f;
    vDSP_maxvi (data, vDSP_Stride(1), &maximum, &index, vDSP_Length(dataSize));
    return int(index);
    
#elif BV_USE_IPP
    float maximum = 0.0f;
    int index = 0;
    ippsMaxIndx_32f (data, dataSize, &maximum, &index);
    return index;
    
#else
    return static_cast<int> (std::distance (data,
                                            std::max_element (data, data + dataSize)));
#endif
}
    
static inline int findIndexOfMaxElement (const double* BV_R_ data, const int dataSize)
{
    jassert (dataSize > 1);
#if BV_USE_VDSP
    unsigned long index = 0.0;
    double maximum = 0.0;
    vDSP_maxviD (data, vDSP_Stride(1), &maximum, &index, vDSP_Length(dataSize));
    return int(index);
    
#elif BV_USE_IPP
    double maximum = 0.0;
    int index = 0;
    ippsMaxIndx_64f (data, dataSize, &maximum, &index);
    return index;
    
#else
    return static_cast<int> (std::distance (data,
                                            std::max_element (data, data + dataSize)));
#endif
}
    
    
    

// returns both the minimum element and its index in the vector into the variables minimum and minIndex
static inline void findMinAndMinIndex (const float* BV_R_ data, const int dataSize,
                                       float& minimum, int& minIndex)
{
    jassert (dataSize > 1);
#if BV_USE_VDSP
    unsigned long index = 0.0;
    vDSP_minvi (data, vDSP_Stride(1), &minimum, &index, vDSP_Length(dataSize));
    minIndex = int (index);
    
#elif BV_USE_IPP
    ippsMinIndx_32f (data, dataSize, &minimum, &minIndex);
    
#else
    auto* lowestElement = std::min_element (data, data + dataSize);
    minimum = *lowestElement;
    minIndex = static_cast<int> (std::distance (data, lowestElement));
#endif
}
    
static inline void findMinAndMinIndex (const double* BV_R_ data, const int dataSize,
                                       double& minimum, int& minIndex)
{
    jassert (dataSize > 1);
#if BV_USE_VDSP
    unsigned long index = 0.0;
    vDSP_minviD (data, vDSP_Stride(1), &minimum, &index, vDSP_Length(dataSize));
    minIndex = int (index);
    
#elif BV_USE_IPP
    ippsMinIndx_64f (data, dataSize, &minimum, &minIndex);
    
#else
    auto* lowestElement = std::min_element (data, data + dataSize);
    minimum = *lowestElement;
    minIndex = static_cast<int> (std::distance (data, lowestElement));
#endif
}
    

// returns both the maximum element and its index in the vector into the variables maximum and maxIndex
static inline void findMaxAndMaxIndex (const float* BV_R_ data, const int dataSize,
                                       float& maximum, int& maxIndex)
{
    jassert (dataSize > 1);
#if BV_USE_VDSP
    unsigned long index = 0.0;
    vDSP_maxvi (data, vDSP_Stride(1), &maximum, &index, vDSP_Length(dataSize));
    maxIndex = int (index);
    
#elif BV_USE_IPP
    ippsMaxIndx_32f (data, dataSize, &maximum, &maxIndex);
    
#else
    auto* highestElement = std::max_element (data, data + dataSize);
    maximum = *highestElement;
    maxIndex = static_cast<int> (std::distance (data, highestElement));
#endif
}
    
static inline void findMaxAndMaxIndex (const double* BV_R_ data, const int dataSize,
                                       double& maximum, int& maxIndex)
{
    jassert (dataSize > 1);
#if BV_USE_VDSP
    unsigned long index = 0.0;
    vDSP_maxviD (data, vDSP_Stride(1), &maximum, &index, vDSP_Length(dataSize));
    maxIndex = int (index);
    
#elif BV_USE_IPP
    ippsMaxIndx_64f (data, dataSize, &maximum, &maxIndex);
    
#else
    auto* highestElement = std::max_element (data, data + dataSize);
    maximum = *highestElement;
    maxIndex = static_cast<int> (std::distance (data, highestElement));
#endif
}
    

// locates the element with the highest absolute value and its index in the vector, and returns them into the variables greatestMagnitude and index
static inline void locateGreatestAbsMagnitude (const float* BV_R_ data, const int dataSize,
                                               float& greatestMagnitude, int& index)
{
    jassert (dataSize > 1);
#if BV_USE_VDSP
    unsigned long i = 0.0;
    vDSP_maxmgvi (data, vDSP_Stride(1), &greatestMagnitude, &i, vDSP_Length(dataSize));
    index = int(i);
    
#elif BV_USE_IPP
    ippsMaxAbsIndx_32f (data, dataSize, &greatestMagnitude, &index);
    
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
    
static inline void locateGreatestAbsMagnitude (const double* BV_R_ data, const int dataSize,
                                               double& greatestMagnitude, int& index)
{
    jassert (dataSize > 1);
#if BV_USE_VDSP
    unsigned long i = 0.0;
    vDSP_maxmgviD (data, vDSP_Stride(1), &greatestMagnitude, &i, vDSP_Length(dataSize));
    index = int(i);
    
#elif BV_USE_IPP
    ippsMaxAbsIndx_64f (data, dataSize, &greatestMagnitude, &index);
    
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
static inline void locateLeastAbsMagnitude (const float* BV_R_ data, const int dataSize,
                                            float& leastMagnitude, int& index)
{
    jassert (dataSize > 1);
#if BV_USE_VDSP
    unsigned long i = 0.0;
    vDSP_minmgvi (data, vDSP_Stride(1), &leastMagnitude, &i, vDSP_Length(dataSize));
    index = int(i);
    
#elif BV_USE_IPP
    ippsMinAbsIndx_32f (data, dataSize, &leastMagnitude, &index);
    
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
    
static inline void locateLeastAbsMagnitude (const double* BV_R_ data, const int dataSize,
                                            double& leastMagnitude, int& index)
{
    jassert (dataSize > 1);
#if BV_USE_VDSP
    unsigned long i = 0.0;
    vDSP_minmgviD (data, vDSP_Stride(1), &leastMagnitude, &i, vDSP_Length(dataSize));
    index = int(i);
    
#elif BV_USE_IPP
    ippsMinAbsIndx_64f (data, dataSize, &leastMagnitude, &index);
    
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
static inline void findExtrema (const float* BV_R_ data, const int dataSize,
                                float& min, float& max)
{
#if BV_USE_VDSP
    vDSP_minv (data, vDSP_Stride(1), &min, vDSP_Length(dataSize));
    vDSP_maxv (data, vDSP_Stride(1), &max, vDSP_Length(dataSize));
    
#elif BV_USE_IPP
    ippsMinMax_32f (data, dataSize, &min, &max);
#else
    auto range = juce::FloatVectorOperations::findMinAndMax (data, dataSize);
    min = range.getStart();
    max = range.getEnd();
#endif
}
    
static inline void findExtrema (double* BV_R_ data, const int dataSize,
                                double& min, double& max)
{
#if BV_USE_VDSP
    vDSP_minvD (data, vDSP_Stride(1), &min, vDSP_Length(dataSize));
    vDSP_maxvD (data, vDSP_Stride(1), &max, vDSP_Length(dataSize));
    
#elif BV_USE_IPP
    ippsMinMax_64f (data, dataSize, &min, &max);
#else
    auto range = juce::FloatVectorOperations::findMinAndMax (data, dataSize);
    min = range.getStart();
    max = range.getEnd();
#endif
}
    
    
// returns the distance between the maximum and minimum element of the vector
static inline float findRangeOfExtrema (const float* BV_R_ data, const int dataSize)
{
#if BV_USE_VDSP
    float min = 0.0f, max = 0.0f;
    vDSP_minv (data, vDSP_Stride(1), &min, vDSP_Length(dataSize));
    vDSP_maxv (data, vDSP_Stride(1), &max, vDSP_Length(dataSize));
    return max - min;
    
#elif BV_USE_IPP
    float min = 0.0f, max = 0.0f;
    ippsMinMax_32f (data, dataSize, &min, &max);
    return max - min;
#else
    return juce::FloatVectorOperations::findMinAndMax (data, dataSize)
                .getLength();
#endif
}

static inline double findRangeOfExtrema (const double* BV_R_ data, const int dataSize)
{
#if BV_USE_VDSP
    double min = 0.0, max = 0.0;
    vDSP_minvD (data, vDSP_Stride(1), &min, vDSP_Length(dataSize));
    vDSP_maxvD (data, vDSP_Stride(1), &max, vDSP_Length(dataSize));
    return max - min;
    
#elif BV_USE_IPP
    double min = 0.0, max = 0.0;
    ippsMinMax_64f (data, dataSize, &min, &max);
    return max - min;
#else
    return juce::FloatVectorOperations::findMinAndMax (data, dataSize)
                .getLength();
#endif
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
    
    
static inline void cartesian_to_polar (float* const BV_R_ mag,
                                       float* const BV_R_ phase,
                                       const float* const BV_R_ real,
                                       const float* const BV_R_ imag,
                                       const int count)
{
#if BV_USE_VDSP
    DSPSplitComplex c;
    c.realp = const_cast<float*>(real);
    c.imagp = const_cast<float*>(imag);
    vDSP_zvmags (&c, 1, phase, 1, vDSP_Length(count)); // using phase as a temporary dest
    vvsqrtf (mag, phase, &count); // using phase as the source
    vvatan2f (phase, imag, real, &count);
    
#elif BV_USE_IPP
    ippsCartToPolar_32f (real, imag, mag, phase, count);
    
#else
    for (int i = 0; i < count; ++i)
    {
        const auto r = real[i];
        const auto c = imag[i];
        *(mag + i)   = sqrt (r * r + c * c);
        *(phase + i) = atan2 (c, r);
    }
#endif
}
    
static inline void cartesian_to_polar (double* const BV_R_ mag,
                                       double* const BV_R_ phase,
                                       const double* const BV_R_ real,
                                       const double* const BV_R_ imag,
                                       const int count)
{
#if BV_USE_VDSP
    DSPDoubleSplitComplex c;
    c.realp = const_cast<double*>(real);
    c.imagp = const_cast<double*>(imag);
    vDSP_zvmagsD (&c, 1, phase, 1, vDSP_Length(count)); // using phase as a temporary dest
    vvsqrt (mag, phase, &count); // using phase as the source
    vvatan2 (phase, imag, real, &count);
    
#elif BV_USE_IPP
    ippsCartToPolar_64f (real, imag, mag, phase, count);
    
#else
    for (int i = 0; i < count; ++i)
    {
        const auto r = real[i];
        const auto c = imag[i];
        *(mag + i)   = sqrt (r * r + c * c);
        *(phase + i) = atan2 (c, r);
    }
#endif
}
    

static inline void phasor (float* real, float* imag, float phase)
{
#if defined HAVE_VDSP
    int one = 1;
    vvsincosf (imag, real, (const float *)&phase, &one);
    
#elif defined LACK_SINCOS
    *real = cosf(phase);
    *imag = sinf(phase);
    
#elif defined __GNUC__
  #if defined __APPLE__
    #define sincosf __sincosf
  #endif
    sincosf (phase, imag, real);
    
#else
    *real = cosf(phase);
    *imag = sinf(phase);
#endif
}

static inline void phasor (double* real, double* imag, double phase)
{
#if defined HAVE_VDSP
    int one = 1;
    
    vvsincos (imag, real, (const double *)&phase, &one);
#elif defined LACK_SINCOS
    *real = cos(phase);
    *imag = sin(phase);
    
#elif defined __GNUC__
  #if defined __APPLE__
    #define sincos __sincos
  #endif
    sincos (phase, imag, real);
    
#else
    *real = cos(phase);
    *imag = sin(phase);
#endif
}
    
    
static inline void polar_to_cartesian   (float* const BV_R_ real,
                                         float* const BV_R_ imag,
                                         const float* const BV_R_ mag,
                                         const float* const BV_R_ phase,
                                         const int dataSize)
{
#if BV_USE_IPP
    ippsPolarToCart_32f (mag, phase, real, imag, dataSize);
    
#else
    for (int i = 0; i < dataSize; ++i) {
        phasor (real + i, imag + i, phase[i]);
    }
    
    juce::FloatVectorOperations::multiply (real, real, mag, dataSize);
    juce::FloatVectorOperations::multiply (imag, imag, mag, dataSize);
#endif
}
    
static inline void polar_to_cartesian   (double* const BV_R_ real,
                                         double* const BV_R_ imag,
                                         const double* const BV_R_ mag,
                                         const double* const BV_R_ phase,
                                         const int dataSize)
{
#if BV_USE_IPP
    ippsPolarToCart_64f (mag, phase, real, imag, dataSize);
    
#else
    for (int i = 0; i < dataSize; ++i) {
        phasor (real + i, imag + i, phase[i]);
    }
    
    juce::FloatVectorOperations::multiply (real, real, mag, dataSize);
    juce::FloatVectorOperations::multiply (imag, imag, mag, dataSize);
#endif
}
    
    

    
    
    
    
}  // namespace
