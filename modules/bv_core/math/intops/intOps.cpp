
namespace bav::intops
{
void fill (int* BV_R_ vector, const int value, const int count)
{
#if BV_USE_IPP
    ippsSet_32s (value, vector, count);
#else
    memset (vector, value, (size_t) count * sizeof (int));
#endif
}


void copy (const int* const BV_R_ source, int* const BV_R_ dest, const int count)
{
#if BV_USE_IPP
    ippsCopy_32s (source, dest, count);
#else
    memcpy (dest, source, (size_t) count * sizeof (int));
#endif
}


void addC (int* BV_R_ vector, const int value, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vector[i] += value;
    }
}


void addV (int* BV_R_ vecA, const int* BV_R_ vecB, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vecA[i] += vecB[i];
    }
}


void subtractC (int* BV_R_ vector, const int value, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vector[i] -= value;
    }
}


void subtractV (int* BV_R_ vecA, const int* BV_R_ vecB, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vecA[i] -= vecB[i];
    }
}


void multiplyC (int* BV_R_ vector, const int value, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vector[i] *= value;
    }
}


void multiplyV (int* BV_R_ vecA, const int* BV_R_ vecB, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vecA[i] *= vecB[i];
    }
}


void divideC (int* BV_R_ vector, const int value, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vector[i] = juce::roundToInt (vector[i] / value);
    }
}


void divideV (int* BV_R_ vecA, const int* BV_R_ vecB, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vecA[i] = juce::roundToInt (vecA[i] / vecB[i]);
    }
}


void square (int* BV_R_ data, const int dataSize)
{
    for (int i = 0; i < dataSize; ++i)
    {
        data[i] *= data[i];
    }
}


void absVal (int* BV_R_ data, const int dataSize)
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


int findMinElement (int* data, int dataSize)
{
#if BV_USE_IPP
    int min = 0;
    ippsMin_32s (data, dataSize, &min);
    return min;
#else
    return *(std::min_element (data, data + dataSize));
#endif
}


int findMaxElement (int* data, int dataSize)
{
#if BV_USE_IPP
    int max = 0;
    ippsMax_32s (data, dataSize, &max);
    return max;
#else
    return *(std::max_element (data, data + dataSize));
#endif
}


int findIndexOfMinElement (int* data, int dataSize)
{
#if BV_USE_IPP
    int idx = 0, min = 0;
    ippsMinIndx_32s (data, dataSize, &min, &idx);
    return idx;
#else
    return static_cast< int > (std::min_element (data, data + dataSize) - data);
#endif
}


int findIndexOfMaxElement (int* data, int dataSize)
{
#if BV_USE_IPP
    int idx = 0, max = 0;
    ippsMaxIndx_32s (data, dataSize, &max, &idx);
    return idx;
#else
    return static_cast< int > (std::max_element (data, data + dataSize) - data);
#endif
}


void findMinAndMinIndex (int* data, const int dataSize, int& minimum, int& minIndex)
{
#if BV_USE_IPP
    ippsMinIndx_32s (data, dataSize, &minimum, &minIndex);
#else
    auto* lowestElement    = std::min_element (data, data + dataSize);
    minimum                = *lowestElement;
    minIndex               = static_cast< int > (lowestElement - data);
#endif
}


void findMaxAndMaxIndex (int* data, const int dataSize, int& maximum, int& maxIndex)
{
#if BV_USE_IPP
    ippsMaxIndx_32s (data, dataSize, &maximum, &maxIndex);
#else
    auto* highestElement   = std::max_element (data, data + dataSize);
    maximum                = *highestElement;
    maxIndex               = static_cast< int > (highestElement - data);
#endif
}


void locateGreatestAbsMagnitude (const int* BV_R_ data,
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


void locateLeastAbsMagnitude (const int* BV_R_ data,
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


void findExtrema (int* data, const int dataSize, int& min, int& max)
{
#if BV_USE_IPP
    ippsMax_32s (data, dataSize, &max);
    ippsMin_32s (data, dataSize, &min);
#else
    min            = *(std::min_element (data, data + dataSize));
    max            = *(std::max_element (data, data + dataSize));
#endif
}


int findRangeOfExtrema (int* data, const int dataSize)
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
