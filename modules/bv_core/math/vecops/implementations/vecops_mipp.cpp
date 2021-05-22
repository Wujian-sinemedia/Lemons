

namespace bav::vecops
{
void fill (float* BV_R_ vector, const float value, const int count)
{
    const auto vecLoopSize = (count / mipp::N< float >()) * mipp::N< float >();

    mipp::Reg< float > rout, val;

    val.load (
        &value);  // this vector register stores the value we're setting the vector to. Only need to load it once

    for (int i = 0; i < vecLoopSize; i += mipp::N< float >())
    {
        rout = val;
        rout.store (&vector[i]);
    }

    for (
        int i = vecLoopSize; i < count;
        ++i)  // Scalar tail loop: finish the remaining elements that can't be vectorized.
        vector[i] = value;
}

void fill (double* BV_R_ vector, const double value, const int count)
{
    const auto vecLoopSize = (count / mipp::N< double >()) * mipp::N< double >();

    mipp::Reg< double > rout, val;

    val.load (
        &value);

    for (int i = 0; i < vecLoopSize; i += mipp::N< double >())
    {
        rout = val;
        rout.store (&vector[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
        vector[i] = value;
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
    const auto vecLoopSize = (count / mipp::N< float >()) * mipp::N< float >();

    mipp::Reg< float > rin, rout, val;

    val.load (
        &value);

    for (int i = 0; i < vecLoopSize; i += mipp::N< float >())
    {
        rin.load (&vector[i]);
        rout = rin + val;
        rout.store (&vector[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
        vector[i] = vector[i] + value;
}

void addC (double* BV_R_ vector, const double value, const int count)
{
    const auto vecLoopSize = (count / mipp::N< double >()) * mipp::N< double >();

    mipp::Reg< double > rin, rout, val;

    val.load (
        &value);

    for (int i = 0; i < vecLoopSize; i += mipp::N< double >())
    {
        rin.load (&vector[i]);
        rout = rin + val;
        rout.store (&vector[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
        vector[i] = vector[i] + value;
}


void addV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    const auto vecLoopSize = (count / mipp::N< float >()) * mipp::N< float >();

    mipp::Reg< float > rAin, rBin, rout;

    for (int i = 0; i < vecLoopSize; i += mipp::N< float >())
    {
        rAin.load (&vecA[i]);
        rBin.load (&vecB[i]);
        rout = rAin + rBin;
        rout.store (&vecA[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
        vecA[i] = vecA[i] + vecB[i];
}

void addV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    const auto vecLoopSize = (count / mipp::N< double >()) * mipp::N< double >();

    mipp::Reg< double > rAin, rBin, rout;

    for (int i = 0; i < vecLoopSize; i += mipp::N< double >())
    {
        rAin.load (&vecA[i]);
        rBin.load (&vecB[i]);
        rout = rAin + rBin;
        rout.store (&vecA[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
        vecA[i] = vecA[i] + vecB[i];
}


void subtractC (float* BV_R_ vector, const float value, const int count)
{
    const auto vecLoopSize = (count / mipp::N< float >()) * mipp::N< float >();

    mipp::Reg< float > rin, rout, val;

    val.load (
        &value);

    for (int i = 0; i < vecLoopSize; i += mipp::N< float >())
    {
        rin.load (&vector[i]);
        rout = rin - val;
        rout.store (&vector[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
        vector[i] = vector[i] - value;
}

void subtractC (double* BV_R_ vector, const double value, const int count)
{
    const auto vecLoopSize = (count / mipp::N< double >()) * mipp::N< double >();

    mipp::Reg< double > rin, rout, val;

    val.load (
        &value);

    for (int i = 0; i < vecLoopSize; i += mipp::N< double >())
    {
        rin.load (&vector[i]);
        rout = rin - val;
        rout.store (&vector[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
        vector[i] = vector[i] - value;
}

void subtractV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    const auto vecLoopSize = (count / mipp::N< float >()) * mipp::N< float >();

    mipp::Reg< float > rAin, rBin, rout;

    for (int i = 0; i < vecLoopSize; i += mipp::N< float >())
    {
        rAin.load (&vecA[i]);
        rBin.load (&vecB[i]);
        rout = rAin - rBin;
        rout.store (&vecA[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
        vecA[i] = vecA[i] - vecB[i];
}

void subtractV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    const auto vecLoopSize = (count / mipp::N< double >()) * mipp::N< double >();

    mipp::Reg< double > rAin, rBin, rout;

    for (int i = 0; i < vecLoopSize; i += mipp::N< double >())
    {
        rAin.load (&vecA[i]);
        rBin.load (&vecB[i]);
        rout = rAin - rBin;
        rout.store (&vecA[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
        vecA[i] = vecA[i] - vecB[i];
}


void multiplyC (float* BV_R_ vector, const float value, const int count)
{
    const auto vecLoopSize = (count / mipp::N< float >()) * mipp::N< float >();

    mipp::Reg< float > rin, rout, val;

    val.load (
        &value);

    for (int i = 0; i < vecLoopSize; i += mipp::N< float >())
    {
        rin.load (&vector[i]);
        rout = rin * val;
        rout.store (&vector[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
        vector[i] = vector[i] * value;
}

void multiplyC (double* BV_R_ vector, const double value, const int count)
{
    const auto vecLoopSize = (count / mipp::N< double >()) * mipp::N< double >();

    mipp::Reg< double > rin, rout, val;

    val.load (
        &value);

    for (int i = 0; i < vecLoopSize; i += mipp::N< double >())
    {
        rin.load (&vector[i]);
        rout = rin * val;
        rout.store (&vector[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
        vector[i] = vector[i] * value;
}


void multiplyV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    const auto vecLoopSize = (count / mipp::N< float >()) * mipp::N< float >();

    mipp::Reg< float > rAin, rBin, rout;

    for (int i = 0; i < vecLoopSize; i += mipp::N< float >())
    {
        rAin.load (&vecA[i]);
        rBin.load (&vecB[i]);
        rout = rAin * rBin;
        rout.store (&vecA[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
        vecA[i] = vecA[i] * vecB[i];
}

void multiplyV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    const auto vecLoopSize = (count / mipp::N< double >()) * mipp::N< double >();

    mipp::Reg< double > rAin, rBin, rout;

    for (int i = 0; i < vecLoopSize; i += mipp::N< double >())
    {
        rAin.load (&vecA[i]);
        rBin.load (&vecB[i]);
        rout = rAin * rBin;
        rout.store (&vecA[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
        vecA[i] = vecA[i] * vecB[i];
}


void divideC (float* BV_R_ vector, const float value, const int count)
{
    const auto vecLoopSize = (count / mipp::N< float >()) * mipp::N< float >();

    mipp::Reg< float > rin, rout, val;

    val.load (
        &value);

    for (int i = 0; i < vecLoopSize; i += mipp::N< float >())
    {
        rin.load (&vector[i]);
        rout = rin / val;
        rout.store (&vector[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
        vector[i] = vector[i] / value;
}

void divideC (double* BV_R_ vector, const double value, const int count)
{
    const auto vecLoopSize = (count / mipp::N< double >()) * mipp::N< double >();

    mipp::Reg< double > rin, rout, val;

    val.load (
        &value);

    for (int i = 0; i < vecLoopSize; i += mipp::N< double >())
    {
        rin.load (&vector[i]);
        rout = rin / val;
        rout.store (&vector[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
        vector[i] = vector[i] / value;
}


void divideV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    const auto vecLoopSize = (count / mipp::N< float >()) * mipp::N< float >();

    mipp::Reg< float > rAin, rBin, rout;

    for (int i = 0; i < vecLoopSize; i += mipp::N< float >())
    {
        rAin.load (&vecA[i]);
        rBin.load (&vecB[i]);
        rout = rAin / rBin;
        rout.store (&vecA[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
        vecA[i] = vecA[i] / vecB[i];
}

void divideV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    const auto vecLoopSize = (count / mipp::N< double >()) * mipp::N< double >();

    mipp::Reg< double > rAin, rBin, rout;

    for (int i = 0; i < vecLoopSize; i += mipp::N< double >())
    {
        rAin.load (&vecA[i]);
        rBin.load (&vecB[i]);
        rout = rAin / rBin;
        rout.store (&vecA[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
        vecA[i] = vecA[i] / vecB[i];
}


void squareRoot (float* BV_R_ data, const int dataSize)
{
    const auto vecLoopSize = (dataSize / mipp::N< float >()) * mipp::N< float >();

    mipp::Reg< float > rin, rout;

    for (int i = 0; i < vecLoopSize; i += mipp::N< float >())
    {
        rin.load (&data[i]);
        rout = mipp::sqrt (rin);
        rout.store (&data[i]);
    }

    for (int i = vecLoopSize; i < dataSize; ++i)
        data[i] = sqrt (data[i]);
}

void squareRoot (double* BV_R_ data, const int dataSize)
{
    const auto vecLoopSize = (dataSize / mipp::N< double >()) * mipp::N< double >();

    mipp::Reg< double > rin, rout;

    for (int i = 0; i < vecLoopSize; i += mipp::N< double >())
    {
        rin.load (&data[i]);
        rout = mipp::sqrt (rin);
        rout.store (&data[i]);
    }

    for (int i = vecLoopSize; i < dataSize; ++i)
        data[i] = sqrt (data[i]);
}


void square (float* BV_R_ data, const int dataSize)
{
    const auto vecLoopSize = (dataSize / mipp::N< float >()) * mipp::N< float >();

    mipp::Reg< float > rin, rout;

    for (int i = 0; i < vecLoopSize; i += mipp::N< float >())
    {
        rin.load (&data[i]);
        rout = rin * rin;
        rout.store (&data[i]);
    }

    for (int i = vecLoopSize; i < dataSize; ++i)
        data[i] = data[i] * data[i];
}

void square (double* BV_R_ data, const int dataSize)
{
    const auto vecLoopSize = (dataSize / mipp::N< double >()) * mipp::N< double >();

    mipp::Reg< double > rin, rout;

    for (int i = 0; i < vecLoopSize; i += mipp::N< double >())
    {
        rin.load (&data[i]);
        rout = rin * rin;
        rout.store (&data[i]);
    }

    for (int i = vecLoopSize; i < dataSize; ++i)
        data[i] = data[i] * data[i];
}


void absVal (float* BV_R_ data, const int dataSize)
{
    const auto vecLoopSize = (dataSize / mipp::N< float >()) * mipp::N< float >();

    mipp::Reg< float > rin, rout;

    for (int i = 0; i < vecLoopSize; i += mipp::N< float >())
    {
        rin.load (&data[i]);
        rout = mipp::abs (rin);
        rout.store (&data[i]);
    }

    for (int i = vecLoopSize; i < dataSize; ++i)
        data[i] = abs (data[i]);
}

void absVal (double* BV_R_ data, const int dataSize)
{
    const auto vecLoopSize = (dataSize / mipp::N< double >()) * mipp::N< double >();

    mipp::Reg< double > rin, rout;

    for (int i = 0; i < vecLoopSize; i += mipp::N< double >())
    {
        rin.load (&data[i]);
        rout = mipp::abs (rin);
        rout.store (&data[i]);
    }

    for (int i = vecLoopSize; i < dataSize; ++i)
        data[i] = abs (data[i]);
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
        fill (vector, 0.0f, numSamples);
    }
    else
    {
        multiplyC (vector, 1.0f / max, numSamples);
    }
}

void normalize (double* BV_R_ vector, const int numSamples)
{
    double max = 0.0;
    int    location;

    locateGreatestAbsMagnitude (vector, numSamples, max, location);

    if (max == 0.0)
    {
        fill (vector, 0.0, numSamples);
    }
    else
    {
        multiplyC (vector, 1.0 / max, numSamples);
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

        // AFAIK, MIPP doesn't provide this function, which is the only thing preventing this function frm being vectorized...
        *(phase + i) = atan2 (
            c,
            r);
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
    const auto vecLoopSize = (dataSize / mipp::N< float >()) * mipp::N< float >();

    mipp::Reg< float > magIn, phaseIn, realOut, imagOut;

    for (int i = 0; i < vecLoopSize; i += mipp::N< float >())
    {
        magIn.load (&mag[i]);
        phaseIn.load (&phase[i]);
        realOut = mipp::cos (phaseIn) * magIn;
        imagOut = mipp::sin (phaseIn) * magIn;
        realOut.store (&real[i]);
        imagOut.store (&imag[i]);
    }

    float m_real, m_imag;
    for (int i = vecLoopSize; i < dataSize; ++i)
    {
        phasor (&m_real, &m_imag, phase[i]);
        const auto m_mag = mag[i];
        real[i]          = m_real * m_mag;
        imag[i]          = m_imag * m_mag;
    }
}

void polar_to_cartesian (double* const BV_R_       real,
                         double* const BV_R_       imag,
                         const double* const BV_R_ mag,
                         const double* const BV_R_ phase,
                         const int                 dataSize)
{
    const auto vecLoopSize = (dataSize / mipp::N< double >()) * mipp::N< double >();

    mipp::Reg< double > magIn, phaseIn, realOut, imagOut;

    for (int i = 0; i < vecLoopSize; i += mipp::N< double >())
    {
        magIn.load (&mag[i]);
        phaseIn.load (&phase[i]);
        realOut = mipp::cos (phaseIn) * magIn;
        imagOut = mipp::sin (phaseIn) * magIn;
        realOut.store (&real[i]);
        imagOut.store (&imag[i]);
    }

    double m_real, m_imag;
    for (int i = vecLoopSize; i < dataSize; ++i)
    {
        phasor (&m_real, &m_imag, phase[i]);
        const auto m_mag = mag[i];
        real[i]          = m_real * m_mag;
        imag[i]          = m_imag * m_mag;
    }
}


}  // namespace bav::vecops
