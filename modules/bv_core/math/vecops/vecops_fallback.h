
/*
    The default implementations of the vecops functions. Uses juce::FloatVectorOperations where possible, and plain C++ where not.
*/


namespace bav::vecops
{
/* fills a vector with the specified value. */
static BV_FORCE_INLINE void
    fill (float* BV_R_ vector, const float value, const int count)
{
    juce::FloatVectorOperations::fill (vector, value, count);
}

static BV_FORCE_INLINE void
    fill (double* BV_R_ vector, const double value, const int count)
{
    juce::FloatVectorOperations::fill (vector, value, count);
}


/* copies each value of src into dst. The vectors may have different value types. If they are the same type, this is the same as using copy */
static BV_FORCE_INLINE void
    convert (double* const BV_R_ dst, const float* const BV_R_ src, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        dst[i] = double (src[i]);
    }
}

static BV_FORCE_INLINE void
    convert (float* const BV_R_ dst, const double* const BV_R_ src, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        dst[i] = float (src[i]);
    }
}


/* adds a single operand to each value in the vector */
static BV_FORCE_INLINE void
    addC (float* BV_R_ vector, const float value, const int count)
{
    juce::FloatVectorOperations::add (vector, value, count);
}

static BV_FORCE_INLINE void
    addC (double* BV_R_ vector, const double value, const int count)
{
    juce::FloatVectorOperations::add (vector, value, count);
}


/* performs element-wise addition of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void
    addV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    juce::FloatVectorOperations::add (vecA, vecB, count);
}

static BV_FORCE_INLINE void
    addV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    juce::FloatVectorOperations::add (vecA, vecB, count);
}


/* subtracts a single operand from every element in the vector */
static BV_FORCE_INLINE void
    subtractC (float* BV_R_ vector, const float value, const int count)
{
    juce::FloatVectorOperations::add (vector, -value, count);
}

static BV_FORCE_INLINE void
    subtractC (double* BV_R_ vector, const double value, const int count)
{
    juce::FloatVectorOperations::add (vector, -value, count);
}


/* performs element-wise subtraction of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void
    subtractV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vecA[i] = vecA[i] - vecB[i];
    }
}

static BV_FORCE_INLINE void
    subtractV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        vecA[i] = vecA[i] - vecB[i];
    }
}


/* multiplies every element in the vector by a single operand */
static BV_FORCE_INLINE void
    multiplyC (float* BV_R_ vector, const float value, const int count)
{
    juce::FloatVectorOperations::multiply (vector, value, count);
}

static BV_FORCE_INLINE void
    multiplyC (double* BV_R_ vector, const double value, const int count)
{
    juce::FloatVectorOperations::multiply (vector, value, count);
}


/* performs element-wise multiplication of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void
    multiplyV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    juce::FloatVectorOperations::multiply (vecA, vecB, count);
}

static BV_FORCE_INLINE void
    multiplyV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    juce::FloatVectorOperations::multiply (vecA, vecB, count);
}


/* divides every element in the vector by a single operand */
static BV_FORCE_INLINE void
    divideC (float* BV_R_ vector, const float value, const int count)
{
    juce::FloatVectorOperations::multiply (vector, 1.0f / value, count);
}

static BV_FORCE_INLINE void
    divideC (double* BV_R_ vector, const double value, const int count)
{
    juce::FloatVectorOperations::multiply (vector, 1.0f / value, count);
}


/* performs element-wise division of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void
    divideV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    for (int i = 0; i < count; ++i)
        *(vecA + i) = vecA[i] / vecB[i];
}

static BV_FORCE_INLINE void
    divideV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    for (int i = 0; i < count; ++i)
        *(vecA + i) = vecA[i] / vecB[i];
}


/* replaces every element in the passed vector with its square root */
static BV_FORCE_INLINE void squareRoot (float* BV_R_ data, const int dataSize)
{
    for (int i = 0; i < dataSize; ++i)
    {
        data[i] = sqrt (data[i]);
    }
}

static BV_FORCE_INLINE void squareRoot (double* BV_R_ data, const int dataSize)
{
    for (int i = 0; i < dataSize; ++i)
    {
        data[i] = sqrt (data[i]);
    }
}


/* replaces every element in the passed vector with its square */
static BV_FORCE_INLINE void square (float* BV_R_ data, const int dataSize)
{
    juce::FloatVectorOperations::multiply (data, data, dataSize);
}

static BV_FORCE_INLINE void square (double* BV_R_ data, const int dataSize)
{
    juce::FloatVectorOperations::multiply (data, data, dataSize);
}


/* replaces every element in the passed vector with its absolute value */
static BV_FORCE_INLINE void absVal (float* BV_R_ data, const int dataSize)
{
    juce::FloatVectorOperations::abs (data, data, dataSize);
}

static BV_FORCE_INLINE void absVal (double* BV_R_ data, const int dataSize)
{
    juce::FloatVectorOperations::abs (data, data, dataSize);
}


/* returns the index in the vector of the minimum element */
static BV_FORCE_INLINE int findIndexOfMinElement (const float* BV_R_ data,
                                                  const int          dataSize)
{
    return static_cast< int > (std::min_element (data, data + dataSize) - data);
}

static BV_FORCE_INLINE int findIndexOfMinElement (const double* BV_R_ data,
                                                  const int           dataSize)
{
    return static_cast< int > (std::min_element (data, data + dataSize) - data);
}


/* returns the index in the vector of the maximum element */
static BV_FORCE_INLINE int findIndexOfMaxElement (const float* BV_R_ data,
                                                  const int          dataSize)
{
    return static_cast< int > (std::max_element (data, data + dataSize) - data);
}

static BV_FORCE_INLINE int findIndexOfMaxElement (const double* BV_R_ data,
                                                  const int           dataSize)
{
    return static_cast< int > (std::max_element (data, data + dataSize) - data);
}


/* returns both the minimum element and its index in the vector into the variables minimum and minIndex */
static BV_FORCE_INLINE void findMinAndMinIndex (const float* BV_R_ data,
                                                const int          dataSize,
                                                float&             minimum,
                                                int&               minIndex)
{
    auto* lowestElement = std::min_element (data, data + dataSize);
    minimum             = *lowestElement;
    minIndex            = static_cast< int > (lowestElement - data);
}

static BV_FORCE_INLINE void findMinAndMinIndex (const double* BV_R_ data,
                                                const int           dataSize,
                                                double&             minimum,
                                                int&                minIndex)
{
    auto* lowestElement = std::min_element (data, data + dataSize);
    minimum             = *lowestElement;
    minIndex            = static_cast< int > (lowestElement - data);
}


/* returns both the maximum element and its index in the vector into the variables maximum and maxIndex */
static BV_FORCE_INLINE void findMaxAndMaxIndex (const float* BV_R_ data,
                                                const int          dataSize,
                                                float&             maximum,
                                                int&               maxIndex)
{
    auto* highestElement = std::max_element (data, data + dataSize);
    maximum              = *highestElement;
    maxIndex             = static_cast< int > (highestElement - data);
}

static BV_FORCE_INLINE void findMaxAndMaxIndex (const double* BV_R_ data,
                                                const int           dataSize,
                                                double&             maximum,
                                                int&                maxIndex)
{
    auto* highestElement = std::max_element (data, data + dataSize);
    maximum              = *highestElement;
    maxIndex             = static_cast< int > (highestElement - data);
}


/* locates the element with the highest absolute value and its index in the vector, and returns them into the variables greatestMagnitude and index */
static BV_FORCE_INLINE void locateGreatestAbsMagnitude (const float* BV_R_ data,
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

static BV_FORCE_INLINE void locateGreatestAbsMagnitude (const double* BV_R_ data,
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


/* locates the element with the lowest absolute value and its index in the vector, and returns them into the variables leastMagnitude and index */
static BV_FORCE_INLINE void locateLeastAbsMagnitude (const float* BV_R_ data,
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

static BV_FORCE_INLINE void locateLeastAbsMagnitude (const double* BV_R_ data,
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


/* finds both the maximum and minimum elements in the vector and returns them into the variables max and min */
static inline void
    findExtrema (const float* BV_R_ data, const int dataSize, float& min, float& max)
{
    auto range = juce::FloatVectorOperations::findMinAndMax (data, dataSize);
    min        = range.getStart();
    max        = range.getEnd();
}

static BV_FORCE_INLINE void
    findExtrema (double* BV_R_ data, const int dataSize, double& min, double& max)
{
    auto range = juce::FloatVectorOperations::findMinAndMax (data, dataSize);
    min        = range.getStart();
    max        = range.getEnd();
}


/* returns the distance between the maximum and minimum element of the vector */
static BV_FORCE_INLINE float findRangeOfExtrema (const float* BV_R_ data,
                                                 const int          dataSize)
{
    return juce::FloatVectorOperations::findMinAndMax (data, dataSize).getLength();
}

static BV_FORCE_INLINE double findRangeOfExtrema (const double* BV_R_ data,
                                                  const int           dataSize)
{
    return juce::FloatVectorOperations::findMinAndMax (data, dataSize).getLength();
}


/* normalizes the vector to the absolute maximum value contained in the vector. */
static BV_FORCE_INLINE void normalize (float* BV_R_ vector, const int numSamples)
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

static BV_FORCE_INLINE void normalize (double* BV_R_ vector, const int numSamples)
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


/* converts cartesian to polar coordinates */
static BV_FORCE_INLINE void cartesian_to_polar (float* const BV_R_       mag,
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

static BV_FORCE_INLINE void cartesian_to_polar (double* const BV_R_       mag,
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


/* converts polar to cartesian coordinates */
static BV_FORCE_INLINE void polar_to_cartesian (float* const BV_R_       real,
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

static BV_FORCE_INLINE void polar_to_cartesian (double* const BV_R_       real,
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
