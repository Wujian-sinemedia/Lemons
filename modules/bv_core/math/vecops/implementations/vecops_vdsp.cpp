

namespace bav::vecops
{

static void
    fill (float* BV_R_ vector, const float value, const int count)
{
    vDSP_vfill (&value, vector, vDSP_Stride (1), vDSP_Length (count));
}

static void
    fill (double* BV_R_ vector, const double value, const int count)
{
    vDSP_vfillD (&value, vector, vDSP_Stride (1), vDSP_Length (count));
}


static void
    convert (double* const BV_R_ dst, const float* const BV_R_ src, const int count)
{
    vDSP_vspdp (src, vDSP_Stride (1), dst, vDSP_Stride (1), vDSP_Length (count));
}

static void
    convert (float* const BV_R_ dst, const double* const BV_R_ src, const int count)
{
    vDSP_vdpsp (src, vDSP_Stride (1), dst, vDSP_Stride (1), vDSP_Length (count));
}


static void
    addC (float* BV_R_ vector, const float value, const int count)
{
    vDSP_vsadd (vector,
                vDSP_Stride (1),
                &value,
                vector,
                vDSP_Stride (1),
                vDSP_Length (count));
}

static void
    addC (double* BV_R_ vector, const double value, const int count)
{
    vDSP_vsaddD (vector,
                 vDSP_Stride (1),
                 &value,
                 vector,
                 vDSP_Stride (1),
                 vDSP_Length (count));
}


static void
    addV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    vDSP_vadd (vecB,
               vDSP_Stride (1),
               vecA,
               vDSP_Stride (1),
               vecA,
               vDSP_Stride (1),
               vDSP_Length (count));
}

static void
    addV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    vDSP_vaddD (vecB,
                vDSP_Stride (1),
                vecA,
                vDSP_Stride (1),
                vecA,
                vDSP_Stride (1),
                vDSP_Length (count));
}


static void
    subtractC (float* BV_R_ vector, const float value, const int count)
{
    const auto val = -value;
    vDSP_vsadd (
        vector, vDSP_Stride (1), &val, vector, vDSP_Stride (1), vDSP_Length (count));
}

static void
    subtractC (double* BV_R_ vector, const double value, const int count)
{
    const auto val = -value;
    vDSP_vsaddD (
        vector, vDSP_Stride (1), &val, vector, vDSP_Stride (1), vDSP_Length (count));
}


static void
    subtractV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    vDSP_vsub (vecA,
               vDSP_Stride (1),
               vecB,
               vDSP_Stride (1),
               vecA,
               vDSP_Stride (1),
               vDSP_Length (count));
}

static void
    subtractV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    vDSP_vsubD (vecA,
                vDSP_Stride (1),
                vecB,
                vDSP_Stride (1),
                vecA,
                vDSP_Stride (1),
                vDSP_Length (count));
}


static void
    multiplyC (float* BV_R_ vector, const float value, const int count)
{
    vDSP_vsmul (vector,
                vDSP_Stride (1),
                &value,
                vector,
                vDSP_Stride (1),
                vDSP_Length (count));
}

static void
    multiplyC (double* BV_R_ vector, const double value, const int count)
{
    vDSP_vsmulD (vector,
                 vDSP_Stride (1),
                 &value,
                 vector,
                 vDSP_Stride (1),
                 vDSP_Length (count));
}


static void
    multiplyV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    vDSP_vmul (vecA,
               vDSP_Stride (1),
               vecB,
               vDSP_Stride (1),
               vecA,
               vDSP_Stride (1),
               vDSP_Length (count));
}

static void
    multiplyV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    vDSP_vmulD (vecA,
                vDSP_Stride (1),
                vecB,
                vDSP_Stride (1),
                vecA,
                vDSP_Stride (1),
                vDSP_Length (count));
}


static void
    divideC (float* BV_R_ vector, const float value, const int count)
{
    vDSP_vsdiv (vector,
                vDSP_Stride (1),
                &value,
                vector,
                vDSP_Stride (1),
                vDSP_Length (count));
}

static void
    divideC (double* BV_R_ vector, const double value, const int count)
{
    vDSP_vsdivD (vector,
                 vDSP_Stride (1),
                 &value,
                 vector,
                 vDSP_Stride (1),
                 vDSP_Length (count));
}


static void
    divideV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    vDSP_vdiv (vecB,
               vDSP_Stride (1),
               vecA,
               vDSP_Stride (1),
               vecA,
               vDSP_Stride (1),
               vDSP_Length (count));
}

static void
    divideV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    vDSP_vdivD (vecB,
                vDSP_Stride (1),
                vecA,
                vDSP_Stride (1),
                vecA,
                vDSP_Stride (1),
                vDSP_Length (count));
}


static void squareRoot (float* BV_R_ data, const int dataSize)
{
    vvsqrtf (data, data, &dataSize);
}

static void squareRoot (double* BV_R_ data, const int dataSize)
{
    vvsqrt (data, data, &dataSize);
}


static void square (float* BV_R_ data, const int dataSize)
{
    vDSP_vsq (data, vDSP_Stride (1), data, vDSP_Stride (1), vDSP_Length (dataSize));
}

static void square (double* BV_R_ data, const int dataSize)
{
    vDSP_vsqD (data, vDSP_Stride (1), data, vDSP_Stride (1), vDSP_Length (dataSize));
}


static void absVal (float* BV_R_ data, const int dataSize)
{
#if (defined(MACOSX_DEPLOYMENT_TARGET) && MACOSX_DEPLOYMENT_TARGET <= 1070 \
     && MAC_OS_X_VERSION_MIN_REQUIRED <= 1070)
    vvfabf (data, data, &dataSize);
#else
    vvfabsf (data, data, &dataSize);
#endif
}

static void absVal (double* BV_R_ data, const int dataSize)
{
#if (defined(MACOSX_DEPLOYMENT_TARGET) && MACOSX_DEPLOYMENT_TARGET <= 1070 \
     && MAC_OS_X_VERSION_MIN_REQUIRED <= 1070)
    vvfab (data, data, &dataSize);
#else
    vvfabs (data, data, &dataSize);
#endif
}


static int findIndexOfMinElement (const float* BV_R_ data,
                                                  const int          dataSize)
{
    unsigned long index   = 0.0;
    float         minimum = 0.0f;
    vDSP_minvi (data, vDSP_Stride (1), &minimum, &index, vDSP_Length (dataSize));
    return int (index);
}

static int findIndexOfMinElement (const double* BV_R_ data
                                                  const int           dataSize)
{
    unsigned long index   = 0.0;
    double        minimum = 0.0;
    vDSP_minviD (data, vDSP_Stride (1), &minimum, &index, vDSP_Length (dataSize));
    return int (index);
}


static int findIndexOfMaxElement (const float* BV_R_ data,
                                                  const int          dataSize)
{
    unsigned long index   = 0.0;
    float         maximum = 0.0f;
    vDSP_maxvi (data, vDSP_Stride (1), &maximum, &index, vDSP_Length (dataSize));
    return int (index);
}

static int findIndexOfMaxElement (const double* BV_R_ data,
                                                  const int           dataSize)
{
    unsigned long index   = 0.0;
    double        maximum = 0.0;
    vDSP_maxviD (data, vDSP_Stride (1), &maximum, &index, vDSP_Length (dataSize));
    return int (index);
}


static void findMinAndMinIndex (const float* BV_R_ data,
                                                const int          dataSize,
                                                float&             minimum,
                                                int&               minIndex)
{
    unsigned long index = 0.0;
    vDSP_minvi (data, vDSP_Stride (1), &minimum, &index, vDSP_Length (dataSize));
    minIndex = int (index);
}

static void findMinAndMinIndex (const double* BV_R_ data,
                                                const int           dataSize,
                                                double&             minimum,
                                                int&                minIndex)
{
    unsigned long index = 0.0;
    vDSP_minviD (data, vDSP_Stride (1), &minimum, &index, vDSP_Length (dataSize));
    minIndex = int (index);
}


static void findMaxAndMaxIndex (const float* BV_R_ data,
                                                const int          dataSize,
                                                float&             maximum,
                                                int&               maxIndex)
{
    unsigned long index = 0.0;
    vDSP_maxvi (data, vDSP_Stride (1), &maximum, &index, vDSP_Length (dataSize));
    maxIndex = int (index);
}

static void findMaxAndMaxIndex (const double* BV_R_ data,
                                                const int           dataSize,
                                                double&             maximum,
                                                int&                maxIndex)
{
    unsigned long index = 0.0;
    vDSP_maxviD (data, vDSP_Stride (1), &maximum, &index, vDSP_Length (dataSize));
    maxIndex = int (index);
}


static void locateGreatestAbsMagnitude (const float* BV_R_ data,
                                                        const int          dataSize,
                                                        float&             greatestMagnitude,
                                                        int&               index)
{
    unsigned long i = 0.0;
    vDSP_maxmgvi (
        data, vDSP_Stride (1), &greatestMagnitude, &i, vDSP_Length (dataSize));
    index = int (i);
}

static void locateGreatestAbsMagnitude (const double* BV_R_ data,
                                                        const int           dataSize,
                                                        double&             greatestMagnitude,
                                                        int&                index)
{
    unsigned long i = 0.0;
    vDSP_maxmgviD (
        data, vDSP_Stride (1), &greatestMagnitude, &i, vDSP_Length (dataSize));
    index = int (i);
}


static void locateLeastAbsMagnitude (const float* BV_R_ data,
                                                     const int          dataSize,
                                                     float&             leastMagnitude,
                                                     int&               index)
{
    unsigned long i = 0.0;
    vDSP_minmgvi (
        data, vDSP_Stride (1), &leastMagnitude, &i, vDSP_Length (dataSize));
    index = int (i);
}

static void locateLeastAbsMagnitude (const double* BV_R_ data,
                                                     const int           dataSize,
                                                     double&             leastMagnitude,
                                                     int&                index)
{
    unsigned long i = 0.0;
    vDSP_minmgviD (
        data, vDSP_Stride (1), &leastMagnitude, &i, vDSP_Length (dataSize));
    index = int (i);
}


static void
    findExtrema (const float* BV_R_ data, const int dataSize, float& min, float& max)
{
    vDSP_minv (data, vDSP_Stride (1), &min, vDSP_Length (dataSize));
    vDSP_maxv (data, vDSP_Stride (1), &max, vDSP_Length (dataSize));
}

static void
    findExtrema (double* BV_R_ data, const int dataSize, double& min, double& max)
{
    vDSP_minvD (data, vDSP_Stride (1), &min, vDSP_Length (dataSize));
    vDSP_maxvD (data, vDSP_Stride (1), &max, vDSP_Length (dataSize));
}


static float findRangeOfExtrema (const float* BV_R_ data,
                                                 const int          dataSize)
{
    float min = 0.0f, max = 0.0f;
    vDSP_minv (data, vDSP_Stride (1), &min, vDSP_Length (dataSize));
    vDSP_maxv (data, vDSP_Stride (1), &max, vDSP_Length (dataSize));
    return max - min;
}

static double findRangeOfExtrema (const double* BV_R_ data,
                                                  const int           dataSize)
{
    double min = 0.0, max = 0.0;
    vDSP_minvD (data, vDSP_Stride (1), &min, vDSP_Length (dataSize));
    vDSP_maxvD (data, vDSP_Stride (1), &max, vDSP_Length (dataSize));
    return max - min;
}


static void normalize (float* BV_R_ vector, const int numSamples)
{
    float         max = 0.0f;
    unsigned long i   = 0.0;
    vDSP_maxmgvi (vector, vDSP_Stride (1), &max, &i, vDSP_Length (numSamples));

    if (max == 0.0f)
    {
        vDSP_vfill (&max, vector, vDSP_Stride (1), vDSP_Length (numSamples));
    }
    else
    {
        const auto oneOverMax = 1.0f / max;
        vDSP_vsmul (vector,
                    vDSP_Stride (1),
                    &oneOverMax,
                    vector,
                    vDSP_Stride (1),
                    vDSP_Length (numSamples));
    }
}

static void normalize (double* BV_R_ vector, const int numSamples)
{
    double        max = 0.0;
    unsigned long i   = 0.0;
    vDSP_maxmgviD (vector, vDSP_Stride (1), &max, &i, vDSP_Length (numSamples));

    if (max == 0.0)
    {
        vDSP_vfillD (&max, vector, vDSP_Stride (1), vDSP_Length (numSamples));
    }
    else
    {
        const auto oneOverMax = 1.0 / max;
        vDSP_vsmulD (vector,
                     vDSP_Stride (1),
                     &oneOverMax,
                     vector,
                     vDSP_Stride (1),
                     vDSP_Length (numSamples));
    }
}


static void cartesian_to_polar (float* const BV_R_       mag,
                                                float* const BV_R_       phase,
                                                const float* const BV_R_ real,
                                                const float* const BV_R_ imag,
                                                const int                count)
{
    DSPSplitComplex c;
    c.realp = const_cast< float* > (real);
    c.imagp = const_cast< float* > (imag);
    vDSP_zvmags (
        &c, 1, phase, 1, vDSP_Length (count));  // using phase as a temporary dest
    vvsqrtf (mag, phase, &count);               // using phase as the source
    vvatan2f (phase, imag, real, &count);
}

static void cartesian_to_polar (double* const BV_R_       mag,
                                                double* const BV_R_       phase,
                                                const double* const BV_R_ real,
                                                const double* const BV_R_ imag,
                                                const int                 count)
{
    DSPDoubleSplitComplex c;
    c.realp = const_cast< double* > (real);
    c.imagp = const_cast< double* > (imag);
    vDSP_zvmagsD (
        &c, 1, phase, 1, vDSP_Length (count));  // using phase as a temporary dest
    vvsqrt (mag, phase, &count);                // using phase as the source
    vvatan2 (phase, imag, real, &count);
}


static void polar_to_cartesian (float* const BV_R_       real,
                                                float* const BV_R_       imag,
                                                const float* const BV_R_ mag,
                                                const float* const BV_R_ phase,
                                                const int                dataSize)
{
    vvsincosf (imag, real, phase, &dataSize);

    vDSP_vmul (real,
               vDSP_Stride (1),
               mag,
               vDSP_Stride (1),
               real,
               vDSP_Stride (1),
               vDSP_Length (dataSize));
    vDSP_vmul (imag,
               vDSP_Stride (1),
               mag,
               vDSP_Stride (1),
               imag,
               vDSP_Stride (1),
               vDSP_Length (dataSize));
}

static void polar_to_cartesian (double* const BV_R_       real,
                                                double* const BV_R_       imag,
                                                const double* const BV_R_ mag,
                                                const double* const BV_R_ phase,
                                                const int                 dataSize)
{
    vvsincos (imag, real, phase, &dataSize);

    vDSP_vmulD (real,
                vDSP_Stride (1),
                mag,
                vDSP_Stride (1),
                real,
                vDSP_Stride (1),
                vDSP_Length (dataSize));
    vDSP_vmulD (imag,
                vDSP_Stride (1),
                mag,
                vDSP_Stride (1),
                imag,
                vDSP_Stride (1),
                vDSP_Length (dataSize));
}


}  // namespace bav::vecops
