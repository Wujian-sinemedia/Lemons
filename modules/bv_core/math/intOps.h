
namespace bav::intops
{
/*
        This namespace contains several floating inlined functions that provide a lot of similar functionality to the bav::vecops functions, but for integer data.
    */


/* fills a vector with the specified value. */
static BV_FORCE_INLINE void
    fill (int* BV_R_ vector, const int value, const int count)
{
#if BV_USE_IPP
    ippsSet_32s (value, vector, count);
#else
    memset (vector, value, (size_t) count * sizeof (int));
#endif
}


/* copies the contents of one vector to another. */
static BV_FORCE_INLINE void
    copy (const int* const BV_R_ source, int* const BV_R_ dest, const int count)
{
#if BV_USE_IPP
    ippsCopy_32s (source, dest, count);
#else
    memcpy (dest, source, (size_t) count * sizeof (int));
#endif
}


/* adds a single operand to each value in the vector */
static BV_FORCE_INLINE void
    addC (int* BV_R_ vector, const int value, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vector[i] += value;
    }
}


/* performs element-wise addition of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void
    addV (int* BV_R_ vecA, const int* BV_R_ vecB, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vecA[i] += vecB[i];
    }
}


/* subtracts a single operand from every element in the vector */
static BV_FORCE_INLINE void
    subtractC (int* BV_R_ vector, const int value, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vector[i] -= value;
    }
}


/* performs element-wise subtraction of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void
    subtractV (int* BV_R_ vecA, const int* BV_R_ vecB, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vecA[i] -= vecB[i];
    }
}


/* multiplies every element in the vector by a single operand */
static BV_FORCE_INLINE void
    multiplyC (int* BV_R_ vector, const int value, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vector[i] *= value;
    }
}


/* performs element-wise multiplication of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void
    multiplyV (int* BV_R_ vecA, const int* BV_R_ vecB, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vecA[i] *= vecB[i];
    }
}


/* divides every element in the vector by a single operand */
static BV_FORCE_INLINE void
    divideC (int* BV_R_ vector, const int value, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vector[i] = juce::roundToInt (vector[i] / value);
    }
}


/* performs element-wise division of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void
    divideV (int* BV_R_ vecA, const int* BV_R_ vecB, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vecA[i] = juce::roundToInt (vecA[i] / vecB[i]);
    }
}


/* replaces every element in the passed vector with its square */
static BV_FORCE_INLINE void square (int* BV_R_ data, const int dataSize)
{
    for (int i = 0; i < dataSize; ++i)
    {
        data[i] *= data[i];
    }
}


/* replaces every element in the passed vector with its absolute value */
static BV_FORCE_INLINE void absVal (int* BV_R_ data, const int dataSize)
{
#if BV_USE_IPP
    ippsAbs_32s_I (data, dataSize);
#else
    for (int i = 0; i < dataSize; ++i)
    {
        data[i] = abs (data[i]);
    }
#endif
}


// returns the minimum element in the dataset
static BV_FORCE_INLINE int findMinElement (int* data, int dataSize)
{
#if BV_USE_IPP
    int min = 0;
    ippsMin_32s (data, dataSize, &min);
    return min;
#else
    return *(std::min_element (data, data + dataSize));
#endif
}


// returns the maximum element in the dataset
static BV_FORCE_INLINE int findMaxElement (int* data, int dataSize)
{
#if BV_USE_IPP
    int max = 0;
    ippsMax_32s (data, dataSize, &max);
    return max;
#else
    return *(std::max_element (data, data + dataSize));
#endif
}


// returns the index in the dataset of the minimum datum
static BV_FORCE_INLINE int findIndexOfMinElement (int* data, int dataSize)
{
#if BV_USE_IPP
    int idx = 0, min = 0;
    ippsMinIndx_32s (data, dataSize, &min, &idx);
    return idx;
#else
    return static_cast< int > (std::min_element (data, data + dataSize) - data);
#endif
}


// returns the index in the dataset of the maximum datum
static BV_FORCE_INLINE int findIndexOfMaxElement (int* data, int dataSize)
{
#if BV_USE_IPP
    int idx = 0, max = 0;
    ippsMaxIndx_32s (data, dataSize, &max, &idx);
    return idx;
#else
    return static_cast< int > (std::max_element (data, data + dataSize) - data);
#endif
}


// returns both the minimum element and its index in the dataset into the variables minimum and minIndex
static BV_FORCE_INLINE void
    findMinAndMinIndex (int* data, const int dataSize, int& minimum, int& minIndex)
{
#if BV_USE_IPP
    ippsMinIndx_32s (data, dataSize, &minimum, &minIndex);
#else
    auto* lowestElement    = std::min_element (data, data + dataSize);
    minimum                = *lowestElement;
    minIndex               = static_cast< int > (lowestElement - data);
#endif
}


// returns both the maximum element and its index in the dataset into the variables maximum and maxIndex
static BV_FORCE_INLINE void
    findMaxAndMaxIndex (int* data, const int dataSize, int& maximum, int& maxIndex)
{
#if BV_USE_IPP
    ippsMaxIndx_32s (data, dataSize, &maximum, &maxIndex);
#else
    auto* highestElement   = std::max_element (data, data + dataSize);
    maximum                = *highestElement;
    maxIndex               = static_cast< int > (highestElement - data);
#endif
}


/* locates the element with the highest absolute value and its index in the vector, and returns them into the variables greatestMagnitude and index */
static BV_FORCE_INLINE void locateGreatestAbsMagnitude (const int* BV_R_ data,
                                                        const int        dataSize,
                                                        int&             greatestMagnitude,
                                                        int&             index)
{
#if BV_USE_IPP
    ippsMaxAbsIndx_32s (data, dataSize, &greatestMagnitude, &index);
#else
    int  strongestMagIndex = 0;
    auto strongestMag      = abs (data[0]);

    for (int i = 1; i < dataSize; ++i)
    {
        const auto current = abs (data[i]);

        if (current > strongestMag)
        {
            strongestMag      = current;
            strongestMagIndex = i;
        }
    }

    greatestMagnitude    = strongestMag;
    index                = strongestMagIndex;
#endif
}


/* locates the element with the lowest absolute value and its index in the vector, and returns them into the variables leastMagnitude and index */
static BV_FORCE_INLINE void locateLeastAbsMagnitude (const int* BV_R_ data,
                                                     const int        dataSize,
                                                     int&             leastMagnitude,
                                                     int&             index)
{
#if BV_USE_IPP
    ippsMinAbsIndx_32s (data, dataSize, &leastMagnitude, &index);
#else
    int  weakestMagIndex = 0;
    auto weakestMag      = abs (data[0]);

    for (int i = 1; i < dataSize; ++i)
    {
        const auto current = abs (data[i]);

        if (current < weakestMag)
        {
            weakestMag      = current;
            weakestMagIndex = i;
        }
    }

    leastMagnitude = weakestMag;
    index          = weakestMagIndex;
#endif
}


// finds both the maximum and minimum elements in the dataset and returns them into the variables max and min.
static BV_FORCE_INLINE void
    findExtrema (int* data, const int dataSize, int& min, int& max)
{
#if BV_USE_IPP
    ippsMax_32s (data, dataSize, &max);
    ippsMin_32s (data, dataSize, &min);
#else
    min            = *(std::min_element (data, data + dataSize));
    max            = *(std::max_element (data, data + dataSize));
#endif
}


//  returns the distance between the maximum and minimum elements in the dataset
static BV_FORCE_INLINE int findRangeOfExtrema (int* data, const int dataSize)
{
#if BV_USE_IPP
    int max = 0, min = 0;
    ippsMax_32s (data, dataSize, &max);
    ippsMin_32s (data, dataSize, &min);
    return max - min;
#else
    return *(std::max_element (data, data + dataSize))
         - *(std::min_element (data, data + dataSize));
#endif
}


}  // namespace bav::intops
