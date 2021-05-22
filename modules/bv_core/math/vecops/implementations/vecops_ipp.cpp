
namespace bav::vecops
{

static void
    fill (float* BV_R_ vector, const float value, const int count)
{
    ippsSet_32f (value, vector, count);
}

static void
    fill (double* BV_R_ vector, const double value, const int count)
{
    ippsSet_64f (value, vector, count);
}



static void
    convert (double* const BV_R_ dst, const float* const BV_R_ src, const int count)
{
    ippsConvert_32f64f (src, dst, count);
}

static void
    convert (float* const BV_R_ dst, const double* const BV_R_ src, const int count)
{
    ippsConvert_64f32f (src, dst, count);
}



static void
    addC (float* BV_R_ vector, const float value, const int count)
{
    ippsAddC_32f_I (value, vector, count);
}

static void
    addC (double* BV_R_ vector, const double value, const int count)
{
    ippsAddC_64f_I (value, vector, count);
}


static void
    addV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    ippsAdd_32f_I (vecB, vecA, count);
}

static void
    addV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    ippsAdd_64f_I (vecB, vecA, count);
}


static void
    subtractC (float* BV_R_ vector, const float value, const int count)
{
    ippsSubC_32f_I (value, vector, count);
}

static void
    subtractC (double* BV_R_ vector, const double value, const int count)
{
    ippsSubC_64f_I (value, vector, count);
}


static void
    subtractV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    ippsSub_32f_I (vecB, vecA, count);
}

static void
    subtractV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    ippsSub_64f_I (vecB, vecA, count);
}


static void
    multiplyC (float* BV_R_ vector, const float value, const int count)
{
    ippsMulC_32f_I (value, vector, count);
}

static void
    multiplyC (double* BV_R_ vector, const double value, const int count)
{
    ippsMulC_64f_I (value, vector, count);
}


static void
    multiplyV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    ippsMul_32f_I (vecB, vecA, count);
}

static void
    multiplyV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    ippsMul_64f_I (vecB, vecA, count);
}


static void
    divideC (float* BV_R_ vector, const float value, const int count)
{
    ippsDivC_32f_I (value, vector, count);
}

static void
    divideC (double* BV_R_ vector, const double value, const int count)
{
    ippsDivC_64f_I (value, vector, count);
}


static void
    divideV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    ippsDiv_32f_I (vecB, vecA, count);
}

static void
    divideV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    ippsDiv_64f_I (vecB, vecA, count);
}


static void squareRoot (float* BV_R_ data, const int dataSize)
{
    ippsSqrt_32f_I (data, dataSize);
}

static void squareRoot (double* BV_R_ data, const int dataSize)
{
    ippsSqrt_64f_I (data, dataSize);
}


static void square (float* BV_R_ data, const int dataSize)
{
    ippsSqr_32f_I (data, dataSize);
}

static void square (double* BV_R_ data, const int dataSize)
{
    ippsSqr_64f_I (data, dataSize);
}


static void absVal (float* BV_R_ data, const int dataSize)
{
    ippsAbs_32f_I (data, dataSize);
}

static void absVal (double* BV_R_ data, const int dataSize)
{
    ippsAbs_64f_I (data, dataSize);
}


static int findIndexOfMinElement (const float* BV_R_ data,
                                                  const int          dataSize)
{
    float minimum = 0.0f;
    int   index   = 0;
    ippsMinIndx_32f (data, dataSize, &minimum, &index);

    return index;
}

static int findIndexOfMinElement (const double* BV_R_ data,
                                                  const int           dataSize)
{
    double minimum = 0.0;
    int    index   = 0;
    ippsMinIndx_64f (data, dataSize, &minimum, &index);

    return index;
}


static int findIndexOfMaxElement (const float* BV_R_ data,
                                                  const int          dataSize)
{
    float maximum = 0.0f;
    int   index   = 0;
    ippsMaxIndx_32f (data, dataSize, &maximum, &index);

    return index;
}

static int findIndexOfMaxElement (const double* BV_R_ data,
                                                  const int           dataSize)
{
    double maximum = 0.0;
    int    index   = 0;
    ippsMaxIndx_64f (data, dataSize, &maximum, &index);

    return index;
}


static void findMinAndMinIndex (const float* BV_R_ data,
                                                const int          dataSize,
                                                float&             minimum,
                                                int&               minIndex)
{
    ippsMinIndx_32f (data, dataSize, &minimum, &minIndex);
}

static void findMinAndMinIndex (const double* BV_R_ data,
                                                const int           dataSize,
                                                double&             minimum,
                                                int&                minIndex)
{
    ippsMinIndx_64f (data, dataSize, &minimum, &minIndex);
}


static void findMaxAndMaxIndex (const float* BV_R_ data,
                                                const int          dataSize,
                                                float&             maximum,
                                                int&               maxIndex)
{
    ippsMaxIndx_32f (data, dataSize, &maximum, &maxIndex);
}

static void findMaxAndMaxIndex (const double* BV_R_ data,
                                                const int           dataSize,
                                                double&             maximum,
                                                int&                maxIndex)
{
    ippsMaxIndx_64f (data, dataSize, &maximum, &maxIndex);
}


static void locateGreatestAbsMagnitude (const float* BV_R_ data,
                                                        const int          dataSize,
                                                        float&             greatestMagnitude,
                                                        int&               index)
{
    ippsMaxAbsIndx_32f (data, dataSize, &greatestMagnitude, &index);
}

static void locateGreatestAbsMagnitude (const double* BV_R_ data,
                                                        const int           dataSize,
                                                        double&             greatestMagnitude,
                                                        int&                index)
{
    ippsMaxAbsIndx_64f (data, dataSize, &greatestMagnitude, &index);
}


static void locateLeastAbsMagnitude (const float* BV_R_ data,
                                                     const int          dataSize,
                                                     float&             leastMagnitude,
                                                     int&               index)
{
    ippsMinAbsIndx_32f (data, dataSize, &leastMagnitude, &index);
}

static void locateLeastAbsMagnitude (const double* BV_R_ data,
                                                     const int           dataSize,
                                                     double&             leastMagnitude,
                                                     int&                index)
{
    ippsMinAbsIndx_64f (data, dataSize, &leastMagnitude, &index);
}


static void
    findExtrema (const float* BV_R_ data, const int dataSize, float& min, float& max)
{
    ippsMinMax_32f (data, dataSize, &min, &max);
}

static void
    findExtrema (double* BV_R_ data, const int dataSize, double& min, double& max)
{
    ippsMinMax_64f (data, dataSize, &min, &max);
}


static float findRangeOfExtrema (const float* BV_R_ data,
                                                 const int          dataSize)
{
    float min = 0.0f, max = 0.0f;
    ippsMinMax_32f (data, dataSize, &min, &max);
    return max - min;
}

static double findRangeOfExtrema (const double* BV_R_ data,
                                                  const int           dataSize)
{
    double min = 0.0, max = 0.0;
    ippsMinMax_64f (data, dataSize, &min, &max);
    return max - min;
}


static void normalize (float* BV_R_ vector, const int numSamples)
{
    float max = 0.0;
    int   location;

    ippsMaxAbsIndx_32f (vector, numSamples, &max, &location);

    if (max == 0.0)
    {
        ippsSet_32f (0.0, vector, numSamples);
    }
    else
    {
        ippsMulC_32f_I (1.0 / max, vector, numSamples);
    }
}

static void normalize (double* BV_R_ vector, const int numSamples)
{
    double max = 0.0;
    int    location;

    ippsMaxAbsIndx_64f (vector, numSamples, &max, &location);

    if (max == 0.0)
    {
        ippsSet_64f (0.0, vector, numSamples);
    }
    else
    {
        ippsMulC_64f_I (1.0 / max, vector, numSamples);
    }
}


static void cartesian_to_polar (float* const BV_R_       mag,
                                                float* const BV_R_       phase,
                                                const float* const BV_R_ real,
                                                const float* const BV_R_ imag,
                                                const int                count)
{
    ippsCartToPolar_32f (real, imag, mag, phase, count);
}

static void cartesian_to_polar (double* const BV_R_       mag,
                                                double* const BV_R_       phase,
                                                const double* const BV_R_ real,
                                                const double* const BV_R_ imag,
                                                const int                 count)
{
    ippsCartToPolar_64f (real, imag, mag, phase, count);
}


static void polar_to_cartesian (float* const BV_R_       real,
                                                float* const BV_R_       imag,
                                                const float* const BV_R_ mag,
                                                const float* const BV_R_ phase,
                                                const int                dataSize)
{
    ippsPolarToCart_32f (mag, phase, real, imag, dataSize);
}

static void polar_to_cartesian (double* const BV_R_       real,
                                                double* const BV_R_       imag,
                                                const double* const BV_R_ mag,
                                                const double* const BV_R_ phase,
                                                const int                 dataSize)
{
    ippsPolarToCart_64f (mag, phase, real, imag, dataSize);
}


}  // namespace bav::vecops
