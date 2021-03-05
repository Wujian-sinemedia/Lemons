
#pragma once


#undef BV_VECTOROPS_USE_VDSP

#ifdef JUCE_USE_VDSP_FRAMEWORK
  #if JUCE_USE_VDSP_FRAMEWORK
    #define BV_VECTOROPS_USE_VDSP 1
  #else
    #define BV_VECTOROPS_USE_VDSP 0
  #endif
#else
  #if (JUCE_MAC || JUCE_IOS)
    #define BV_VECTOROPS_USE_VDSP 1
  #else
    #define BV_VECTOROPS_USE_VDSP 0
  #endif
#endif

#if BV_VECTOROPS_USE_VDSP
  #include <Accelerate/Accelerate.h>
#endif


namespace bav
{
    

namespace vecops
{
    
    
template<typename SampleType>
inline void makeHannWindow (SampleType* output, const int numSamples)
{
    jassert (numSamples > 1);
#if BV_VECTOROPS_USE_VDSP
    if constexpr (std::is_same_v <SampleType, float>)
        vDSP_hann_window (output, vDSP_Length(numSamples), 2);
    else if constexpr (std::is_same_v <SampleType, double>)
        vDSP_hann_windowD (output, vDSP_Length(numSamples), 2);
#else
    const SampleType samplemultiplier = static_cast<SampleType>( (juce::MathConstants<SampleType>::pi * 2.0) / (numSamples - 1) );
    constexpr SampleType one = SampleType(1.0);
    constexpr SampleType pointFive = SampleType(0.5);
    
    for (int i = 0; i < numSamples; ++i)
        output[i] = static_cast<SampleType>( (one - (std::cos (i * samplemultiplier))) * pointFive );
#endif
}

    
template<typename DataType>
inline int findIndexOfMinElement (DataType* data, const int dataSize)
{
    jassert (dataSize > 1);
#if BV_VECTOROPS_USE_VDSP
    constexpr vDSP_Stride strideOfOne = vDSP_Stride(1);
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
    
    
template<typename DataType>
inline int findIndexOfMaxElement (DataType* data, const int dataSize)
{
    jassert (dataSize > 1);
#if BV_VECTOROPS_USE_VDSP
    constexpr vDSP_Stride strideOfOne = vDSP_Stride(1);
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
    

template<typename DataType>
inline void findMinAndMinIndex (DataType* data, const int dataSize,
                                DataType& minimum, int& minIndex)
{
    jassert (dataSize > 1);
#if BV_VECTOROPS_USE_VDSP
    constexpr vDSP_Stride strideOfOne = vDSP_Stride(1);
    unsigned long index = 0.0;
    
    if constexpr (std::is_same_v <DataType, float>)
        vDSP_minvi (data, strideOfOne, &minimum, &index, vDSP_Length(dataSize));
    else if constexpr (std::is_same_v <DataType, double>)
        vDSP_minviD (data, strideOfOne, &minimum, &index, vDSP_Length(dataSize));
    
    minIndex = int (index);
#else
    DataType* lowestElement = std::min_element (data, data + dataSize);
    minimum = *lowestElement;
    minIndex = static_cast<int> (std::distance (data, lowestElement));
#endif
}
    
    
template<typename DataType>
inline void findMaxAndMaxIndex (DataType* data, const int dataSize,
                                DataType& maximum, int& maxIndex)
{
    jassert (dataSize > 1);
#if BV_VECTOROPS_USE_VDSP
    constexpr vDSP_Stride strideOfOne = vDSP_Stride(1);
    unsigned long index = 0.0;
    
    if constexpr (std::is_same_v <DataType, float>)
        vDSP_maxvi (data, strideOfOne, &maximum, &index, vDSP_Length(dataSize));
    else if constexpr (std::is_same_v <DataType, double>)
        vDSP_maxviD (data, strideOfOne, &maximum, &index, vDSP_Length(dataSize));
    
    maxIndex = int (index);
#else
    DataType* highestElement = std::max_element (data, data + dataSize);
    maximum = *highestElement;
    maxIndex = static_cast<int> (std::distance (data, highestElement));
#endif
}
    
    
template<typename DataType>
inline void locateGreatestAbsMagnitude (DataType* data, const int dataSize,
                                        DataType& greatestMagnitude, int& index)
{
    jassert (dataSize > 1);
#if BV_VECTOROPS_USE_VDSP
    constexpr vDSP_Stride strideOfOne = vDSP_Stride(1);
    unsigned long i = 0.0;
    
    if constexpr (std::is_same_v <DataType, float>)
        vDSP_maxmgvi (data, strideOfOne, &greatestMagnitude, &i, vDSP_Length(dataSize));
    else if constexpr (std::is_same_v <DataType, double>)
        vDSP_maxmgviD (data, strideOfOne, &greatestMagnitude, &i, vDSP_Length(dataSize));
    
    index = int(i);
#else
    
#endif
}
    
    
template<typename DataType>
inline DataType findRangeOfExtrema (DataType* data, const int dataSize)
{
    return juce::FloatVectorOperations::findMinAndMax (data, dataSize).getLength();
}
    
    
    
}  // namespace vecops
    
}  // namespace bav
