
/*
    Implementations of vecops functions using Apple vDSP.
*/



namespace bav::vecops
{

    
/* fills a vector with the specified value. */
static BV_FORCE_INLINE void fill (float* BV_R_ vector, const float value, const int count)
{
    vDSP_vfill (&value, vector, vDSP_Stride(1), vDSP_Length(count));
}

static BV_FORCE_INLINE void fill (double* BV_R_ vector, const double value, const int count)
{
    vDSP_vfillD (&value, vector, vDSP_Stride(1), vDSP_Length(count));
}


/* copies each value of src into dst. The vectors may have different value types. If they are the same type, this is the same as using copy */
static BV_FORCE_INLINE void convert (double* const BV_R_ dst, const float* const BV_R_ src, const int count)
{
    vDSP_vspdp (src, vDSP_Stride(1), dst, vDSP_Stride(1), vDSP_Length(count));
}

static BV_FORCE_INLINE void convert (float* const BV_R_ dst, const double* const BV_R_ src, const int count)
{
    vDSP_vdpsp (src, vDSP_Stride(1), dst, vDSP_Stride(1), vDSP_Length(count));
}


/* adds a single operand to each value in the vector */
static BV_FORCE_INLINE void addC (float* BV_R_ vector, const float value, const int count)
{
    vDSP_vsadd (vector, vDSP_Stride(1), &value, vector, vDSP_Stride(1), vDSP_Length(count));
}

static BV_FORCE_INLINE void addC (double* BV_R_ vector, const double value, const int count)
{
    vDSP_vsaddD (vector, vDSP_Stride(1), &value, vector, vDSP_Stride(1), vDSP_Length(count));
}


/* performs element-wise addition of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void addV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    vDSP_vadd (vecB, vDSP_Stride(1), vecA, vDSP_Stride(1), vecA, vDSP_Stride(1), vDSP_Length(count));
}

static BV_FORCE_INLINE void addV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    vDSP_vaddD (vecB, vDSP_Stride(1), vecA, vDSP_Stride(1), vecA, vDSP_Stride(1), vDSP_Length(count));
}


/* subtracts a single operand from every element in the vector */
static BV_FORCE_INLINE void subtractC (float* BV_R_ vector, const float value, const int count)
{
    const auto val = -value;
    vDSP_vsadd (vector, vDSP_Stride(1), &val, vector, vDSP_Stride(1), vDSP_Length(count));
}

static BV_FORCE_INLINE void subtractC (double* BV_R_ vector, const double value, const int count)
{
    const auto val = -value;
    vDSP_vsaddD (vector, vDSP_Stride(1), &val, vector, vDSP_Stride(1), vDSP_Length(count));
}


/* performs element-wise subtraction of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void subtractV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    vDSP_vsub (vecA, vDSP_Stride(1), vecB, vDSP_Stride(1), vecA, vDSP_Stride(1), vDSP_Length(count));
}

static BV_FORCE_INLINE void subtractV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    vDSP_vsubD (vecA, vDSP_Stride(1), vecB, vDSP_Stride(1), vecA, vDSP_Stride(1), vDSP_Length(count));
}


/* multiplies every element in the vector by a single operand */
static BV_FORCE_INLINE void multiplyC (float* BV_R_ vector, const float value, const int count)
{
    vDSP_vsmul (vector, vDSP_Stride(1), &value, vector, vDSP_Stride(1), vDSP_Length(count));
}

static BV_FORCE_INLINE void multiplyC (double* BV_R_ vector, const double value, const int count)
{
    vDSP_vsmulD (vector, vDSP_Stride(1), &value, vector, vDSP_Stride(1), vDSP_Length(count));
}


/* performs element-wise multiplication of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void multiplyV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    vDSP_vmul (vecA, vDSP_Stride(1), vecB, vDSP_Stride(1), vecA, vDSP_Stride(1), vDSP_Length(count));
}

static BV_FORCE_INLINE void multiplyV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    vDSP_vmulD (vecA, vDSP_Stride(1), vecB, vDSP_Stride(1), vecA, vDSP_Stride(1), vDSP_Length(count));
}


/* divides every element in the vector by a single operand */
static BV_FORCE_INLINE void divideC (float* BV_R_ vector, const float value, const int count)
{
    vDSP_vsdiv (vector, vDSP_Stride(1), &value, vector, vDSP_Stride(1), vDSP_Length(count));
}

static BV_FORCE_INLINE void divideC (double* BV_R_ vector, const double value, const int count)
{
    vDSP_vsdivD (vector, vDSP_Stride(1), &value, vector, vDSP_Stride(1), vDSP_Length(count));
}


/* performs element-wise division of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void divideV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count)
{
    vDSP_vdiv (vecB, vDSP_Stride(1), vecA, vDSP_Stride(1), vecA, vDSP_Stride(1), vDSP_Length(count));
}

static BV_FORCE_INLINE void divideV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count)
{
    vDSP_vdivD (vecB, vDSP_Stride(1), vecA, vDSP_Stride(1), vecA, vDSP_Stride(1), vDSP_Length(count));
}


/* replaces every element in the passed vector with its square root */
static BV_FORCE_INLINE void squareRoot (float* BV_R_ data, const int dataSize)
{
    vvsqrtf (data, data, &dataSize);
}

static BV_FORCE_INLINE void squareRoot (double* BV_R_ data, const int dataSize)
{
    vvsqrt (data, data, &dataSize);
}


/* replaces every element in the passed vector with its square */
static BV_FORCE_INLINE void square (float* BV_R_ data, const int dataSize)
{
   vDSP_vsq (data, vDSP_Stride(1), data, vDSP_Stride(1), vDSP_Length(dataSize));
}

static BV_FORCE_INLINE void square (double* BV_R_ data, const int dataSize)
{
   vDSP_vsqD (data, vDSP_Stride(1), data, vDSP_Stride(1), vDSP_Length(dataSize));
}
    
    
/* replaces every element in the passed vector with its absolute value */
static BV_FORCE_INLINE void absVal (float* BV_R_ data, const int dataSize)
{
#if (defined(MACOSX_DEPLOYMENT_TARGET) && MACOSX_DEPLOYMENT_TARGET <= 1070 && MAC_OS_X_VERSION_MIN_REQUIRED <= 1070)
    vvfabf (data, data, &dataSize);
#else
    vvfabsf (data, data, &dataSize);
#endif
}

static BV_FORCE_INLINE void absVal (double* BV_R_ data, const int dataSize)
{
#if (defined(MACOSX_DEPLOYMENT_TARGET) && MACOSX_DEPLOYMENT_TARGET <= 1070 && MAC_OS_X_VERSION_MIN_REQUIRED <= 1070)
    vvfab (data, data, &dataSize);
#else
    vvfabs (data, data, &dataSize);
#endif
}


/* returns the index in the vector of the minimum element */
static BV_FORCE_INLINE int findIndexOfMinElement (const float* BV_R_ data, const int dataSize)
{
    unsigned long index = 0.0;
    float minimum = 0.0f;
    vDSP_minvi (data, vDSP_Stride(1), &minimum, &index, vDSP_Length(dataSize));
    return int(index);
}

static BV_FORCE_INLINE int findIndexOfMinElement (const double* BV_R_ data, const int dataSize)
{
    unsigned long index = 0.0;
    double minimum = 0.0;
    vDSP_minviD (data, vDSP_Stride(1), &minimum, &index, vDSP_Length(dataSize));
    return int(index);
}


/* returns the index in the vector of the maximum element */
static BV_FORCE_INLINE int findIndexOfMaxElement (const float* BV_R_ data, const int dataSize)
{
    unsigned long index = 0.0;
    float maximum = 0.0f;
    vDSP_maxvi (data, vDSP_Stride(1), &maximum, &index, vDSP_Length(dataSize));
    return int(index);
}

static BV_FORCE_INLINE int findIndexOfMaxElement (const double* BV_R_ data, const int dataSize)
{
    unsigned long index = 0.0;
    double maximum = 0.0;
    vDSP_maxviD (data, vDSP_Stride(1), &maximum, &index, vDSP_Length(dataSize));
    return int(index);
}


/* returns both the minimum element and its index in the vector into the variables minimum and minIndex */
static BV_FORCE_INLINE void findMinAndMinIndex (const float* BV_R_ data, const int dataSize,
                                                float& minimum, int& minIndex)
{
    unsigned long index = 0.0;
    vDSP_minvi (data, vDSP_Stride(1), &minimum, &index, vDSP_Length(dataSize));
    minIndex = int (index);
}

static BV_FORCE_INLINE void findMinAndMinIndex (const double* BV_R_ data, const int dataSize,
                                                double& minimum, int& minIndex)
{
    unsigned long index = 0.0;
    vDSP_minviD (data, vDSP_Stride(1), &minimum, &index, vDSP_Length(dataSize));
    minIndex = int (index);
}


/* returns both the maximum element and its index in the vector into the variables maximum and maxIndex */
static BV_FORCE_INLINE void findMaxAndMaxIndex (const float* BV_R_ data, const int dataSize,
                                                float& maximum, int& maxIndex)
{
    unsigned long index = 0.0;
    vDSP_maxvi (data, vDSP_Stride(1), &maximum, &index, vDSP_Length(dataSize));
    maxIndex = int (index);
}

static BV_FORCE_INLINE void findMaxAndMaxIndex (const double* BV_R_ data, const int dataSize,
                                                double& maximum, int& maxIndex)
{
    unsigned long index = 0.0;
    vDSP_maxviD (data, vDSP_Stride(1), &maximum, &index, vDSP_Length(dataSize));
    maxIndex = int (index);
}


/* locates the element with the highest absolute value and its index in the vector, and returns them into the variables greatestMagnitude and index */
static BV_FORCE_INLINE void locateGreatestAbsMagnitude (const float* BV_R_ data, const int dataSize,
                                                        float& greatestMagnitude, int& index)
{
    unsigned long i = 0.0;
    vDSP_maxmgvi (data, vDSP_Stride(1), &greatestMagnitude, &i, vDSP_Length(dataSize));
    index = int(i);
}

static BV_FORCE_INLINE void locateGreatestAbsMagnitude (const double* BV_R_ data, const int dataSize,
                                                        double& greatestMagnitude, int& index)
{
    unsigned long i = 0.0;
    vDSP_maxmgviD (data, vDSP_Stride(1), &greatestMagnitude, &i, vDSP_Length(dataSize));
    index = int(i);
}


/* locates the element with the lowest absolute value and its index in the vector, and returns them into the variables leastMagnitude and index */
static BV_FORCE_INLINE void locateLeastAbsMagnitude (const float* BV_R_ data, const int dataSize,
                                                     float& leastMagnitude, int& index)
{
    unsigned long i = 0.0;
    vDSP_minmgvi (data, vDSP_Stride(1), &leastMagnitude, &i, vDSP_Length(dataSize));
    index = int(i);
}

static BV_FORCE_INLINE void locateLeastAbsMagnitude (const double* BV_R_ data, const int dataSize,
                                                     double& leastMagnitude, int& index)
{
    unsigned long i = 0.0;
    vDSP_minmgviD (data, vDSP_Stride(1), &leastMagnitude, &i, vDSP_Length(dataSize));
    index = int(i);
}


/* finds both the maximum and minimum elements in the vector and returns them into the variables max and min */
static inline void findExtrema (const float* BV_R_ data, const int dataSize,
                                float& min, float& max)
{
    vDSP_minv (data, vDSP_Stride(1), &min, vDSP_Length(dataSize));
    vDSP_maxv (data, vDSP_Stride(1), &max, vDSP_Length(dataSize));
}

static BV_FORCE_INLINE void findExtrema (double* BV_R_ data, const int dataSize,
                                         double& min, double& max)
{
    vDSP_minvD (data, vDSP_Stride(1), &min, vDSP_Length(dataSize));
    vDSP_maxvD (data, vDSP_Stride(1), &max, vDSP_Length(dataSize));
}


/* returns the distance between the maximum and minimum element of the vector */
static BV_FORCE_INLINE float findRangeOfExtrema (const float* BV_R_ data, const int dataSize)
{
    float min = 0.0f, max = 0.0f;
    vDSP_minv (data, vDSP_Stride(1), &min, vDSP_Length(dataSize));
    vDSP_maxv (data, vDSP_Stride(1), &max, vDSP_Length(dataSize));
    return max - min;
}

static BV_FORCE_INLINE double findRangeOfExtrema (const double* BV_R_ data, const int dataSize)
{
    double min = 0.0, max = 0.0;
    vDSP_minvD (data, vDSP_Stride(1), &min, vDSP_Length(dataSize));
    vDSP_maxvD (data, vDSP_Stride(1), &max, vDSP_Length(dataSize));
    return max - min;
}
    
    
/* normalizes the vector to the absolute maximum value contained in the vector. */
static BV_FORCE_INLINE void normalize (float* BV_R_ vector, const int numSamples)
{
    float max = 0.0f;
    unsigned long i = 0.0;
    vDSP_maxmgvi (vector, vDSP_Stride(1), &max, &i, vDSP_Length(numSamples));
    
    if (max == 0.0f)
    {
        vDSP_vfill (&max, vector, vDSP_Stride(1), vDSP_Length(numSamples));
    }
    else
    {
        const auto oneOverMax = 1.0f / max;
        vDSP_vsmul (vector, vDSP_Stride(1), &oneOverMax, vector, vDSP_Stride(1), vDSP_Length(numSamples));
    }
}
 
static BV_FORCE_INLINE void normalize (double* BV_R_ vector, const int numSamples)
{
    double max = 0.0;
    unsigned long i = 0.0;
    vDSP_maxmgviD (vector, vDSP_Stride(1), &max, &i, vDSP_Length(numSamples));
    
    if (max == 0.0)
    {
        vDSP_vfillD (&max, vector, vDSP_Stride(1), vDSP_Length(numSamples));
    }
    else
    {
        const auto oneOverMax = 1.0 / max;
        vDSP_vsmulD (vector, vDSP_Stride(1), &oneOverMax, vector, vDSP_Stride(1), vDSP_Length(numSamples));
    }
}


/* converts cartesian to polar coordinates */
static BV_FORCE_INLINE void cartesian_to_polar (float* const BV_R_ mag, float* const BV_R_ phase,
                                                const float* const BV_R_ real, const float* const BV_R_ imag,
                                                const int count)
{
    DSPSplitComplex c;
    c.realp = const_cast<float*>(real);
    c.imagp = const_cast<float*>(imag);
    vDSP_zvmags (&c, 1, phase, 1, vDSP_Length(count)); // using phase as a temporary dest
    vvsqrtf (mag, phase, &count); // using phase as the source
    vvatan2f (phase, imag, real, &count);
}

static BV_FORCE_INLINE void cartesian_to_polar (double* const BV_R_ mag, double* const BV_R_ phase,
                                                const double* const BV_R_ real, const double* const BV_R_ imag,
                                                const int count)
{
    DSPDoubleSplitComplex c;
    c.realp = const_cast<double*>(real);
    c.imagp = const_cast<double*>(imag);
    vDSP_zvmagsD (&c, 1, phase, 1, vDSP_Length(count)); // using phase as a temporary dest
    vvsqrt (mag, phase, &count); // using phase as the source
    vvatan2 (phase, imag, real, &count);
}
    
    
/* converts polar to cartesian coordinates */
static BV_FORCE_INLINE void polar_to_cartesian   (float* const BV_R_ real, float* const BV_R_ imag,
                                                  const float* const BV_R_ mag, const float* const BV_R_ phase,
                                                  const int dataSize)
{
    vvsincosf (imag, real, phase, &dataSize);
    
    vDSP_vmul (real, vDSP_Stride(1), mag, vDSP_Stride(1), real, vDSP_Stride(1), vDSP_Length(dataSize));
    vDSP_vmul (imag, vDSP_Stride(1), mag, vDSP_Stride(1), imag, vDSP_Stride(1), vDSP_Length(dataSize));
}

static BV_FORCE_INLINE void polar_to_cartesian   (double* const BV_R_ real, double* const BV_R_ imag,
                                                  const double* const BV_R_ mag, const double* const BV_R_ phase,
                                                  const int dataSize)
{
    vvsincos (imag, real, phase, &dataSize);
    
    vDSP_vmulD (real, vDSP_Stride(1), mag, vDSP_Stride(1), real, vDSP_Stride(1), vDSP_Length(dataSize));
    vDSP_vmulD (imag, vDSP_Stride(1), mag, vDSP_Stride(1), imag, vDSP_Stride(1), vDSP_Length(dataSize));
}
    
    
}  // namspace
