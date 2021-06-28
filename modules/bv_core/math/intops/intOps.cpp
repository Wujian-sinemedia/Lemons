
namespace bav::intops
{
void fill (int* BV_R_ vector, const int value, const int count)
{
    memset (vector, value, (size_t) count * sizeof (int));
}


void copy (const int* const BV_R_ source, int* const BV_R_ dest, const int count)
{
    memcpy (dest, source, (size_t) count * sizeof (int));
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
    for (int i = 0; i < dataSize; ++i)
    {
        data[i] = abs (data[i]);
    }
}


int findMinElement (int* data, int dataSize)
{
    return *(std::min_element (data, data + dataSize));
}


int findMaxElement (int* data, int dataSize)
{
    return *(std::max_element (data, data + dataSize));
}


int findIndexOfMinElement (int* data, int dataSize)
{
    return static_cast< int > (std::min_element (data, data + dataSize) - data);
}


int findIndexOfMaxElement (int* data, int dataSize)
{
    return static_cast< int > (std::max_element (data, data + dataSize) - data);
}


void findMinAndMinIndex (int* data, const int dataSize, int& minimum, int& minIndex)
{
    auto* lowestElement    = std::min_element (data, data + dataSize);
    minimum                = *lowestElement;
    minIndex               = static_cast< int > (lowestElement - data);
}


void findMaxAndMaxIndex (int* data, const int dataSize, int& maximum, int& maxIndex)
{
    auto* highestElement   = std::max_element (data, data + dataSize);
    maximum                = *highestElement;
    maxIndex               = static_cast< int > (highestElement - data);
}


void locateGreatestAbsMagnitude (const int* BV_R_ data,
                                 const int        dataSize,
                                 int&             greatestMagnitude,
                                 int&             index)
{
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
}


void locateLeastAbsMagnitude (const int* BV_R_ data,
                              const int        dataSize,
                              int&             leastMagnitude,
                              int&             index)
{
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
}


void findExtrema (int* data, const int dataSize, int& min, int& max)
{
    min            = *(std::min_element (data, data + dataSize));
    max            = *(std::max_element (data, data + dataSize));
}


int findRangeOfExtrema (int* data, const int dataSize)
{
    return *(std::max_element (data, data + dataSize))
         - *(std::min_element (data, data + dataSize));
}


}  // namespace bav::intops
