
namespace bav::vecops
{
void fill (float* BV_R_ vector, const float value, const int count)
{
    juce::FloatVectorOperations::fill (vector, value, count);
}

void fill (double* BV_R_ vector, const double value, const int count)
{
    juce::FloatVectorOperations::fill (vector, value, count);
}


void convert (double* const BV_R_ dst, const float* const BV_R_ src, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        dst[i] = double (src[i]);
    }
}

void convert (float* const BV_R_ dst, const double* const BV_R_ src, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        dst[i] = float (src[i]);
    }
}


void addC (float* BV_R_ vector, const float value, const int count)
{
    juce::FloatVectorOperations::add (vector, value, count);
}

void addC (double* BV_R_ vector, const double value, const int count)
{
    juce::FloatVectorOperations::add (vector, value, count);
}


void addV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    juce::FloatVectorOperations::add (vecA, vecB, count);
}

void addV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    juce::FloatVectorOperations::add (vecA, vecB, count);
}


void subtractC (float* BV_R_ vector, const float value, const int count)
{
    juce::FloatVectorOperations::add (vector, -value, count);
}

void subtractC (double* BV_R_ vector, const double value, const int count)
{
    juce::FloatVectorOperations::add (vector, -value, count);
}


void subtractV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vecA[i] = vecA[i] - vecB[i];
    }
}

void subtractV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vecA[i] = vecA[i] - vecB[i];
    }
}


void multiplyC (float* BV_R_ vector, const float value, const int count)
{
    juce::FloatVectorOperations::multiply (vector, value, count);
}

void multiplyC (double* BV_R_ vector, const double value, const int count)
{
    juce::FloatVectorOperations::multiply (vector, value, count);
}


void multiplyV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    juce::FloatVectorOperations::multiply (vecA, vecB, count);
}

void multiplyV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    juce::FloatVectorOperations::multiply (vecA, vecB, count);
}


void divideC (float* BV_R_ vector, const float value, const int count)
{
    juce::FloatVectorOperations::multiply (vector, 1.0f / value, count);
}

void divideC (double* BV_R_ vector, const double value, const int count)
{
    juce::FloatVectorOperations::multiply (vector, 1.0f / value, count);
}


void divideV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    for (int i = 0; i < count; ++i)
        *(vecA + i) = vecA[i] / vecB[i];
}

void divideV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    for (int i = 0; i < count; ++i)
        *(vecA + i) = vecA[i] / vecB[i];
}


void squareRoot (float* BV_R_ data, const int dataSize)
{
    for (int i = 0; i < dataSize; ++i)
    {
        data[i] = sqrt (data[i]);
    }
}

void squareRoot (double* BV_R_ data, const int dataSize)
{
    for (int i = 0; i < dataSize; ++i)
    {
        data[i] = sqrt (data[i]);
    }
}


void square (float* BV_R_ data, const int dataSize)
{
    juce::FloatVectorOperations::multiply (data, data, dataSize);
}

void square (double* BV_R_ data, const int dataSize)
{
    juce::FloatVectorOperations::multiply (data, data, dataSize);
}


void absVal (float* BV_R_ data, const int dataSize)
{
    juce::FloatVectorOperations::abs (data, data, dataSize);
}

void absVal (double* BV_R_ data, const int dataSize)
{
    juce::FloatVectorOperations::abs (data, data, dataSize);
}


int findIndexOfMinElement (const float* BV_R_ data,
                           const int          dataSize)
{
    return static_cast< int > (std::min_element (data, data + dataSize) - data);
}

int findIndexOfMinElement (const double* BV_R_ data,
                           const int           dataSize)
{
    return static_cast< int > (std::min_element (data, data + dataSize) - data);
}


int findIndexOfMaxElement (const float* BV_R_ data,
                           const int          dataSize)
{
    return static_cast< int > (std::max_element (data, data + dataSize) - data);
}

int findIndexOfMaxElement (const double* BV_R_ data,
                           const int           dataSize)
{
    return static_cast< int > (std::max_element (data, data + dataSize) - data);
}


void findMinAndMinIndex (const float* BV_R_ data,
                         const int          dataSize,
                         float&             minimum,
                         int&               minIndex)
{
    auto* lowestElement = std::min_element (data, data + dataSize);
    minimum             = *lowestElement;
    minIndex            = static_cast< int > (lowestElement - data);
}

void findMinAndMinIndex (const double* BV_R_ data,
                         const int           dataSize,
                         double&             minimum,
                         int&                minIndex)
{
    auto* lowestElement = std::min_element (data, data + dataSize);
    minimum             = *lowestElement;
    minIndex            = static_cast< int > (lowestElement - data);
}


void findMaxAndMaxIndex (const float* BV_R_ data,
                         const int          dataSize,
                         float&             maximum,
                         int&               maxIndex)
{
    auto* highestElement = std::max_element (data, data + dataSize);
    maximum              = *highestElement;
    maxIndex             = static_cast< int > (highestElement - data);
}

void findMaxAndMaxIndex (const double* BV_R_ data,
                         const int           dataSize,
                         double&             maximum,
                         int&                maxIndex)
{
    auto* highestElement = std::max_element (data, data + dataSize);
    maximum              = *highestElement;
    maxIndex             = static_cast< int > (highestElement - data);
}


void locateGreatestAbsMagnitude (const float* BV_R_ data,
                                 const int          dataSize,
                                 float&             greatestMagnitude,
                                 int&               index)
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

    greatestMagnitude = strongestMag;
    index             = strongestMagIndex;
}

void locateGreatestAbsMagnitude (const double* BV_R_ data,
                                 const int           dataSize,
                                 double&             greatestMagnitude,
                                 int&                index)
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

    greatestMagnitude = strongestMag;
    index             = strongestMagIndex;
}


void locateLeastAbsMagnitude (const float* BV_R_ data,
                              const int          dataSize,
                              float&             leastMagnitude,
                              int&               index)
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

void locateLeastAbsMagnitude (const double* BV_R_ data,
                              const int           dataSize,
                              double&             leastMagnitude,
                              int&                index)
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


void findExtrema (const float* BV_R_ data, const int dataSize, float& min, float& max)
{
    auto range = juce::FloatVectorOperations::findMinAndMax (data, dataSize);
    min        = range.getStart();
    max        = range.getEnd();
}

void findExtrema (double* BV_R_ data, const int dataSize, double& min, double& max)
{
    auto range = juce::FloatVectorOperations::findMinAndMax (data, dataSize);
    min        = range.getStart();
    max        = range.getEnd();
}


float findRangeOfExtrema (const float* BV_R_ data,
                          const int          dataSize)
{
    return juce::FloatVectorOperations::findMinAndMax (data, dataSize).getLength();
}

double findRangeOfExtrema (const double* BV_R_ data,
                           const int           dataSize)
{
    return juce::FloatVectorOperations::findMinAndMax (data, dataSize).getLength();
}


void normalize (float* BV_R_ vector, const int numSamples)
{
    float max = 0.0f;
    int   location;

    locateGreatestAbsMagnitude (vector, numSamples, max, location);

    if (max == 0.0f)
    {
        juce::FloatVectorOperations::fill (vector, 0.0f, numSamples);
    }
    else
    {
        juce::FloatVectorOperations::multiply (vector, 1.0f / max, numSamples);
    }
}

void normalize (double* BV_R_ vector, const int numSamples)
{
    double max = 0.0;
    int    location;

    locateGreatestAbsMagnitude (vector, numSamples, max, location);

    if (max == 0.0)
    {
        juce::FloatVectorOperations::fill (vector, 0.0, numSamples);
    }
    else
    {
        juce::FloatVectorOperations::multiply (vector, 1.0 / max, numSamples);
    }
}


void cartesian_to_polar (float* const BV_R_       mag,
                         float* const BV_R_       phase,
                         const float* const BV_R_ real,
                         const float* const BV_R_ imag,
                         const int                count)
{
    for (int i = 0; i < count; ++i)
    {
        const auto r = real[i];
        const auto c = imag[i];
        *(mag + i)   = sqrt (r * r + c * c);
        *(phase + i) = atan2 (c, r);
    }
}

void cartesian_to_polar (double* const BV_R_       mag,
                         double* const BV_R_       phase,
                         const double* const BV_R_ real,
                         const double* const BV_R_ imag,
                         const int                 count)
{
    for (int i = 0; i < count; ++i)
    {
        const auto r = real[i];
        const auto c = imag[i];
        *(mag + i)   = sqrt (r * r + c * c);
        *(phase + i) = atan2 (c, r);
    }
}


void polar_to_cartesian (float* const BV_R_       real,
                         float* const BV_R_       imag,
                         const float* const BV_R_ mag,
                         const float* const BV_R_ phase,
                         const int                dataSize)
{
    for (int i = 0; i < dataSize; ++i)
    {
        phasor (real + i, imag + i, phase[i]);
    }

    juce::FloatVectorOperations::multiply (real, real, mag, dataSize);
    juce::FloatVectorOperations::multiply (imag, imag, mag, dataSize);
}

void polar_to_cartesian (double* const BV_R_       real,
                         double* const BV_R_       imag,
                         const double* const BV_R_ mag,
                         const double* const BV_R_ phase,
                         const int                 dataSize)
{
    for (int i = 0; i < dataSize; ++i)
    {
        phasor (real + i, imag + i, phase[i]);
    }

    juce::FloatVectorOperations::multiply (real, real, mag, dataSize);
    juce::FloatVectorOperations::multiply (imag, imag, mag, dataSize);
}


}  // namespace bav::vecops
