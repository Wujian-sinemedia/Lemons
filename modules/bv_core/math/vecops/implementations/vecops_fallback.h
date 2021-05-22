
namespace bav::vecops
{
static void
    fill (float* BV_R_ vector, const float value, const int count)
{
    juce::FloatVectorOperations::fill (vector, value, count);
}

static void
    fill (double* BV_R_ vector, const double value, const int count)
{
    juce::FloatVectorOperations::fill (vector, value, count);
}


static void
    convert (double* const BV_R_ dst, const float* const BV_R_ src, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        dst[i] = double (src[i]);
    }
}

static void
    convert (float* const BV_R_ dst, const double* const BV_R_ src, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        dst[i] = float (src[i]);
    }
}


static void
    addC (float* BV_R_ vector, const float value, const int count)
{
    juce::FloatVectorOperations::add (vector, value, count);
}

static void
    addC (double* BV_R_ vector, const double value, const int count)
{
    juce::FloatVectorOperations::add (vector, value, count);
}


static void
    addV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    juce::FloatVectorOperations::add (vecA, vecB, count);
}

static void
    addV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    juce::FloatVectorOperations::add (vecA, vecB, count);
}


static void
    subtractC (float* BV_R_ vector, const float value, const int count)
{
    juce::FloatVectorOperations::add (vector, -value, count);
}

static void
    subtractC (double* BV_R_ vector, const double value, const int count)
{
    juce::FloatVectorOperations::add (vector, -value, count);
}


static void
    subtractV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vecA[i] = vecA[i] - vecB[i];
    }
}

static void
    subtractV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vecA[i] = vecA[i] - vecB[i];
    }
}


static void
    multiplyC (float* BV_R_ vector, const float value, const int count)
{
    juce::FloatVectorOperations::multiply (vector, value, count);
}

static void
    multiplyC (double* BV_R_ vector, const double value, const int count)
{
    juce::FloatVectorOperations::multiply (vector, value, count);
}


static void
    multiplyV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    juce::FloatVectorOperations::multiply (vecA, vecB, count);
}

static void
    multiplyV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    juce::FloatVectorOperations::multiply (vecA, vecB, count);
}


static void
    divideC (float* BV_R_ vector, const float value, const int count)
{
    juce::FloatVectorOperations::multiply (vector, 1.0f / value, count);
}

static void
    divideC (double* BV_R_ vector, const double value, const int count)
{
    juce::FloatVectorOperations::multiply (vector, 1.0f / value, count);
}


static void
    divideV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    for (int i = 0; i < count; ++i)
        *(vecA + i) = vecA[i] / vecB[i];
}

static void
    divideV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    for (int i = 0; i < count; ++i)
        *(vecA + i) = vecA[i] / vecB[i];
}


static void squareRoot (float* BV_R_ data, const int dataSize)
{
    for (int i = 0; i < dataSize; ++i)
    {
        data[i] = sqrt (data[i]);
    }
}

static void squareRoot (double* BV_R_ data, const int dataSize)
{
    for (int i = 0; i < dataSize; ++i)
    {
        data[i] = sqrt (data[i]);
    }
}


static void square (float* BV_R_ data, const int dataSize)
{
    juce::FloatVectorOperations::multiply (data, data, dataSize);
}

static void square (double* BV_R_ data, const int dataSize)
{
    juce::FloatVectorOperations::multiply (data, data, dataSize);
}


static void absVal (float* BV_R_ data, const int dataSize)
{
    juce::FloatVectorOperations::abs (data, data, dataSize);
}

static void absVal (double* BV_R_ data, const int dataSize)
{
    juce::FloatVectorOperations::abs (data, data, dataSize);
}


static int findIndexOfMinElement (const float* BV_R_ data,
                                  const int          dataSize)
{
    return static_cast< int > (std::min_element (data, data + dataSize) - data);
}

static int findIndexOfMinElement (const double* BV_R_ data,
                                  const int           dataSize)
{
    return static_cast< int > (std::min_element (data, data + dataSize) - data);
}


static int findIndexOfMaxElement (const float* BV_R_ data,
                                  const int          dataSize)
{
    return static_cast< int > (std::max_element (data, data + dataSize) - data);
}

static int findIndexOfMaxElement (const double* BV_R_ data,
                                  const int           dataSize)
{
    return static_cast< int > (std::max_element (data, data + dataSize) - data);
}


static void findMinAndMinIndex (const float* BV_R_ data,
                                const int          dataSize,
                                float&             minimum,
                                int&               minIndex)
{
    auto* lowestElement = std::min_element (data, data + dataSize);
    minimum             = *lowestElement;
    minIndex            = static_cast< int > (lowestElement - data);
}

static void findMinAndMinIndex (const double* BV_R_ data,
                                const int           dataSize,
                                double&             minimum,
                                int&                minIndex)
{
    auto* lowestElement = std::min_element (data, data + dataSize);
    minimum             = *lowestElement;
    minIndex            = static_cast< int > (lowestElement - data);
}


static void findMaxAndMaxIndex (const float* BV_R_ data,
                                const int          dataSize,
                                float&             maximum,
                                int&               maxIndex)
{
    auto* highestElement = std::max_element (data, data + dataSize);
    maximum              = *highestElement;
    maxIndex             = static_cast< int > (highestElement - data);
}

static void findMaxAndMaxIndex (const double* BV_R_ data,
                                const int           dataSize,
                                double&             maximum,
                                int&                maxIndex)
{
    auto* highestElement = std::max_element (data, data + dataSize);
    maximum              = *highestElement;
    maxIndex             = static_cast< int > (highestElement - data);
}


static void locateGreatestAbsMagnitude (const float* BV_R_ data,
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

static void locateGreatestAbsMagnitude (const double* BV_R_ data,
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


static void locateLeastAbsMagnitude (const float* BV_R_ data,
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

static void locateLeastAbsMagnitude (const double* BV_R_ data,
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


static void
    findExtrema (const float* BV_R_ data, const int dataSize, float& min, float& max)
{
    auto range = juce::FloatVectorOperations::findMinAndMax (data, dataSize);
    min        = range.getStart();
    max        = range.getEnd();
}

static void
    findExtrema (double* BV_R_ data, const int dataSize, double& min, double& max)
{
    auto range = juce::FloatVectorOperations::findMinAndMax (data, dataSize);
    min        = range.getStart();
    max        = range.getEnd();
}


static float findRangeOfExtrema (const float* BV_R_ data,
                                 const int          dataSize)
{
    return juce::FloatVectorOperations::findMinAndMax (data, dataSize).getLength();
}

static double findRangeOfExtrema (const double* BV_R_ data,
                                  const int           dataSize)
{
    return juce::FloatVectorOperations::findMinAndMax (data, dataSize).getLength();
}


static void normalize (float* BV_R_ vector, const int numSamples)
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

static void normalize (double* BV_R_ vector, const int numSamples)
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


static void cartesian_to_polar (float* const BV_R_       mag,
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

static void cartesian_to_polar (double* const BV_R_       mag,
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


static void polar_to_cartesian (float* const BV_R_       real,
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

static void polar_to_cartesian (double* const BV_R_       real,
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
