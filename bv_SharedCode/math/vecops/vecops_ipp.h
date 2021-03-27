
#include "bv_SharedCode.h"


namespace bav::vecops
{
    

/* fills a vector with the specified value. */
static BV_FORCE_INLINE void fill (float* BV_R_ vector, const float value, const int count)
{
    ippsSet_32f (value, vector, count);
}

static BV_FORCE_INLINE void fill (double* BV_R_ vector, const double value, const int count)
{
    ippsSet_64f (value, vector, count);
}


/* copies the contents of one vector to another. */
static BV_FORCE_INLINE void copy (float* BV_R_ source, float* BV_R_ dest, const int count)
{
    ippsMove_32f (source, dest, count);
}

static BV_FORCE_INLINE void copy (double* BV_R_ source, double* BV_R_ dest, const int count)
{
    ippsMove_64f (source, dest, count);
}


/* copies each value of src into dst. The vectors may have different value types. If they are the same type, this is the same as using copy */
static BV_FORCE_INLINE void convert (double* const BV_R_ dst, const float* const BV_R_ src, const int count)
{
    ippsConvert_32f64f (src, dst, count);
}

static BV_FORCE_INLINE void convert (float* const BV_R_ dst, const double* const BV_R_ src, const int count)
{
    ippsConvert_64f32f (src, dst, count);
}


/* adds a single operand to each value in the vector */
static BV_FORCE_INLINE void addC (float* BV_R_ vector, const float value, const int count)
{
    ippsAddC_32f_I (value, vector, count);
}

static BV_FORCE_INLINE void addC (double* BV_R_ vector, const double value, const int count)
{
    ippsAddC_64f_I (value, vector, count);
}


/* performs element-wise addition of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void addV (float* BV_R_ vecA, float* BV_R_ vecB, const int count)
{
    ippsAdd_32f_I (vecB, vecA, count);
}

static BV_FORCE_INLINE void addV (double* BV_R_ vecA, double* BV_R_ vecB, const int count)
{
    ippsAdd_64f_I (vecB, vecA, count);
}


/* subtracts a single operand from every element in the vector */
static BV_FORCE_INLINE void subtractC (float* BV_R_ vector, const float value, const int count)
{
    ippsSubC_32f_I (value, vector, count);
}

static BV_FORCE_INLINE void subtractC (double* BV_R_ vector, const double value, const int count)
{
    ippsSubC_64f_I (value, vector, count);
}


/* performs element-wise subtraction of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void subtractV (float* BV_R_ vecA, float* BV_R_ vecB, const int count)
{
    ippsSub_32f_I (vecB, vecA, count);
}

static BV_FORCE_INLINE void subtractV (double* BV_R_ vecA, double* BV_R_ vecB, const int count)
{
    ippsSub_64f_I (vecB, vecA, count);
}


/* multiplies every element in the vector by a single operand */
static BV_FORCE_INLINE void multiplyC (float* BV_R_ vector, const float value, const int count)
{
    ippsMulC_32fc_I (value, vector, count);
}

static BV_FORCE_INLINE void multiplyC (double* BV_R_ vector, const double value, const int count)
{
    ippsMulC_64fc_I (value, vector, count);
}


/* performs element-wise multiplication of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void multiplyV (float* BV_R_ vecA, float* BV_R_ vecB, const int count)
{
    ippsMul_32f_I (vecB, vecA, count);
}

static BV_FORCE_INLINE void multiplyV (double* BV_R_ vecA, double* BV_R_ vecB, const int count)
{
    ippsMul_64f_I (vecB, vecA, count);
}


/* divides every element in the vector by a single operand */
static BV_FORCE_INLINE void divideC (float* BV_R_ vector, const float value, const int count)
{
    ippsDivC_32f_I (value, vector, count);
}

static BV_FORCE_INLINE void divideC (double* BV_R_ vector, const double value, const int count)
{
    ippsDivC_64f_I (value, vector, count);
}


/* performs element-wise division of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void divideV (float* BV_R_ vecA, float* BV_R_ vecB, const int count)
{
    ippsDiv_32f_I (vecB, vecA, count);
}

static BV_FORCE_INLINE void divideV (double* BV_R_ vecA, double* BV_R_ vecB, const int count)
{
    ippsDiv_64f_I (vecB, vecA, count);
}


/* replaces every element in the passed vector with its square root */
static BV_FORCE_INLINE void squareRoot (float* BV_R_ data, const int dataSize)
{
    ippsSqrt_32f_I (data, dataSize);
}

static BV_FORCE_INLINE void squareRoot (double* BV_R_ data, const int dataSize)
{
    ippsSqrt_64f_I (data, dataSize);
}


/* replaces every element in the passed vector with its square */
static BV_FORCE_INLINE void square (float* BV_R_ data, const int dataSize)
{
    ippsSqr_32f_I (data, dataSize);
}

static BV_FORCE_INLINE void square (double* BV_R_ data, const int dataSize)
{
    ippsSqr_64f_I (data, dataSize);
}


/* returns the index in the vector of the minimum element */
static BV_FORCE_INLINE int findIndexOfMinElement (const float* BV_R_ data, const int dataSize)
{
    float minimum = 0.0f;
    int index = 0;
    ippsMinIndx_32f (data, dataSize, &minimum, &index);
    
    return index;
}

static BV_FORCE_INLINE int findIndexOfMinElement (const double* BV_R_ data, const int dataSize)
{
    double minimum = 0.0;
    int index = 0;
    ippsMinIndx_64f (data, dataSize, &minimum, &index);
    
    return index;
}


/* returns the index in the vector of the maximum element */
static BV_FORCE_INLINE int findIndexOfMaxElement (const float* BV_R_ data, const int dataSize)
{
    float maximum = 0.0f;
    int index = 0;
    ippsMaxIndx_32f (data, dataSize, &maximum, &index);
    
    return index;
}

static BV_FORCE_INLINE int findIndexOfMaxElement (const double* BV_R_ data, const int dataSize)
{
    double maximum = 0.0;
    int index = 0;
    ippsMaxIndx_64f (data, dataSize, &maximum, &index);
    
    return index;
}


/* returns both the minimum element and its index in the vector into the variables minimum and minIndex */
static BV_FORCE_INLINE void findMinAndMinIndex (const float* BV_R_ data, const int dataSize,
                                                float& minimum, int& minIndex)
{
    ippsMinIndx_32f (data, dataSize, &minimum, &minIndex);
}

static BV_FORCE_INLINE void findMinAndMinIndex (const double* BV_R_ data, const int dataSize,
                                                double& minimum, int& minIndex)
{
    ippsMinIndx_64f (data, dataSize, &minimum, &minIndex);
}


/* returns both the maximum element and its index in the vector into the variables maximum and maxIndex */
static BV_FORCE_INLINE void findMaxAndMaxIndex (const float* BV_R_ data, const int dataSize,
                                                float& maximum, int& maxIndex)
{
    ippsMaxIndx_32f (data, dataSize, &maximum, &maxIndex);
}

static BV_FORCE_INLINE void findMaxAndMaxIndex (const double* BV_R_ data, const int dataSize,
                                                double& maximum, int& maxIndex)
{
    ippsMaxIndx_64f (data, dataSize, &maximum, &maxIndex);
}


/* locates the element with the highest absolute value and its index in the vector, and returns them into the variables greatestMagnitude and index */
static BV_FORCE_INLINE void locateGreatestAbsMagnitude (const float* BV_R_ data, const int dataSize,
                                                        float& greatestMagnitude, int& index)
{
    ippsMaxAbsIndx_32f (data, dataSize, &greatestMagnitude, &index);
}

static BV_FORCE_INLINE void locateGreatestAbsMagnitude (const double* BV_R_ data, const int dataSize,
                                                        double& greatestMagnitude, int& index)
{
    ippsMaxAbsIndx_64f (data, dataSize, &greatestMagnitude, &index);
}


/* locates the element with the lowest absolute value and its index in the vector, and returns them into the variables leastMagnitude and index */
static BV_FORCE_INLINE void locateLeastAbsMagnitude (const float* BV_R_ data, const int dataSize,
                                                     float& leastMagnitude, int& index)
{
    ippsMinAbsIndx_32f (data, dataSize, &leastMagnitude, &index);
}

static BV_FORCE_INLINE void locateLeastAbsMagnitude (const double* BV_R_ data, const int dataSize,
                                                     double& leastMagnitude, int& index)
{
    ippsMinAbsIndx_64f (data, dataSize, &leastMagnitude, &index);
}


/* finds both the maximum and minimum elements in the vector and returns them into the variables max and min */
static inline void findExtrema (const float* BV_R_ data, const int dataSize,
                                float& min, float& max)
{
    ippsMinMax_32f (data, dataSize, &min, &max);
}

static BV_FORCE_INLINE void findExtrema (double* BV_R_ data, const int dataSize,
                                         double& min, double& max)
{
    ippsMinMax_64f (data, dataSize, &min, &max);
}


/* returns the distance between the maximum and minimum element of the vector */
static BV_FORCE_INLINE float findRangeOfExtrema (const float* BV_R_ data, const int dataSize)
{
    float min = 0.0f, max = 0.0f;
    ippsMinMax_32f (data, dataSize, &min, &max);
    return max - min;
}

static BV_FORCE_INLINE double findRangeOfExtrema (const double* BV_R_ data, const int dataSize)
{
    double min = 0.0, max = 0.0;
    ippsMinMax_64f (data, dataSize, &min, &max);
    return max - min;
}


/* converts cartesian to polar coordinates */
static BV_FORCE_INLINE void cartesian_to_polar (float* const BV_R_ mag, float* const BV_R_ phase,
                                                const float* const BV_R_ real, const float* const BV_R_ imag,
                                                const int count)
{
    ippsCartToPolar_32f (real, imag, mag, phase, count);
}

static BV_FORCE_INLINE void cartesian_to_polar (double* const BV_R_ mag, double* const BV_R_ phase,
                                                const double* const BV_R_ real, const double* const BV_R_ imag,
                                                const int count)
{
    ippsCartToPolar_64f (real, imag, mag, phase, count);
}


/* converts polar to cartesian coordinates */
static BV_FORCE_INLINE void polar_to_cartesian   (float* const BV_R_ real, float* const BV_R_ imag,
                                                  const float* const BV_R_ mag, const float* const BV_R_ phase,
                                                  const int dataSize)
{
    ippsPolarToCart_32f (mag, phase, real, imag, dataSize);
}

static BV_FORCE_INLINE void polar_to_cartesian   (double* const BV_R_ real, double* const BV_R_ imag,
                                                  const double* const BV_R_ mag, const double* const BV_R_ phase,
                                                  const int dataSize)
{
    ippsPolarToCart_64f (mag, phase, real, imag, dataSize);
}


/* converts cartesian coordinates to frequency bin magnitudes */
static BV_FORCE_INLINE void cartesian_to_magnitudes (float* const BV_R_ mag,
                                                     const float* const BV_R_ real, const float* const BV_R_ imag,
                                                     const int count)
{
    ippsMagnitude_32f (real, imag, mag, count);
}


static BV_FORCE_INLINE void cartesian_to_magnitudes (double* const BV_R_ mag,
                                                     const double* const BV_R_ real, const double* const BV_R_ imag,
                                                     const int count)
{
    ippsMagnitude_64f (real, imag, mag, count);
}


}  // namespace
