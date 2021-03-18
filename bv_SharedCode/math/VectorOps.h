
#include "bv_SharedCode/System.h"


namespace bav::vecops
{
    

    /*
        This namespace contains several floating inlined functions that extend the functionality of JUCE's FloatVectorOperations class.
        Apple's vDSP framework is used where possible.
    */



// returns the index in the vector of the element with the minimum value
template<typename DataType>
inline int findIndexOfMinElement (DataType* data, const int dataSize)
{
    jassert (dataSize > 1);
#if BV_VECTOROPS_USE_VDSP
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
inline int findIndexOfMaxElement (DataType* data, const int dataSize)
{
    jassert (dataSize > 1);
#if BV_VECTOROPS_USE_VDSP
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
inline void findMinAndMinIndex (DataType* data, const int dataSize,
                                DataType& minimum, int& minIndex)
{
    jassert (dataSize > 1);
#if BV_VECTOROPS_USE_VDSP
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
inline void findMaxAndMaxIndex (DataType* data, const int dataSize,
                                DataType& maximum, int& maxIndex)
{
    jassert (dataSize > 1);
#if BV_VECTOROPS_USE_VDSP
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
inline void locateGreatestAbsMagnitude (DataType* data, const int dataSize,
                                        DataType& greatestMagnitude, int& index)
{
    jassert (dataSize > 1);
#if BV_VECTOROPS_USE_VDSP
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
inline void locateLeastAbsMagnitude (DataType* data, const int dataSize,
                                     DataType& leastMagnitude, int& index)
{
    jassert (dataSize > 1);
#if BV_VECTOROPS_USE_VDSP
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
inline void findExtrema (DataType* data, const int dataSize,
                         DataType& min, DataType& max)
{
    auto range = juce::FloatVectorOperations::findMinAndMax (data, dataSize);
    min = range.getStart();
    max = range.getEnd();
}
    
    
// returns the distance between the maximum and minimum element of the vector
template<typename DataType>
inline DataType findRangeOfExtrema (DataType* data, const int dataSize)
{
    return juce::FloatVectorOperations::findMinAndMax (data, dataSize)
           .getLength();
}
    
    
}  // namespace
