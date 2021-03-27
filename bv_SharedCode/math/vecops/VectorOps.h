
namespace bav::vecops
{


/*
This namespace contains several floating inlined functions that extend the functionality of JUCE's FloatVectorOperations class.
Apple's vDSP framework and Intel IPP are used if they are available.
*/



/* fills a vector with the specified value. */
static BV_FORCE_INLINE void fill (float* BV_R_ vector, const float value, const int count);

static BV_FORCE_INLINE void fill (double* BV_R_ vector, const double value, const int count);


/* copies the contents of one vector to another. */
static BV_FORCE_INLINE void copy (float* BV_R_ source, float* BV_R_ dest, const int count);

static BV_FORCE_INLINE void copy (double* BV_R_ source, double* BV_R_ dest, const int count);


/* copies each value of src into dst. The vectors may have different value types. If they are the same type, this is the same as using copy */
static BV_FORCE_INLINE void convert (double* const BV_R_ dst, const float* const BV_R_ src, const int count);

static BV_FORCE_INLINE void convert (float* const BV_R_ dst, const double* const BV_R_ src, const int count);


/* adds a single operand to each value in the vector */
static BV_FORCE_INLINE void addC (float* BV_R_ vector, const float value, const int count);

static BV_FORCE_INLINE void addC (double* BV_R_ vector, const double value, const int count);


/* performs element-wise addition of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void addV (float* BV_R_ vecA, float* BV_R_ vecB, const int count);

static BV_FORCE_INLINE void addV (double* BV_R_ vecA, double* BV_R_ vecB, const int count);


/* subtracts a single operand from every element in the vector */
static BV_FORCE_INLINE void subtractC (float* BV_R_ vector, const float value, const int count);

static BV_FORCE_INLINE void subtractC (double* BV_R_ vector, const double value, const int count);


/* performs element-wise subtraction of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void subtractV (float* BV_R_ vecA, float* BV_R_ vecB, const int count);

static BV_FORCE_INLINE void subtractV (double* BV_R_ vecA, double* BV_R_ vecB, const int count);


/* multiplies every element in the vector by a single operand */
static BV_FORCE_INLINE void multiplyC (float* BV_R_ vector, const float value, const int count);

static BV_FORCE_INLINE void multiplyC (double* BV_R_ vector, const double value, const int count);


/* performs element-wise multiplication of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void multiplyV (float* BV_R_ vecA, float* BV_R_ vecB, const int count);

static BV_FORCE_INLINE void multiplyV (double* BV_R_ vecA, double* BV_R_ vecB, const int count);


/* divides every element in the vector by a single operand */
static BV_FORCE_INLINE void divideC (float* BV_R_ vector, const float value, const int count);

static BV_FORCE_INLINE void divideC (double* BV_R_ vector, const double value, const int count);


/* performs element-wise division of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void divideV (float* BV_R_ vecA, float* BV_R_ vecB, const int count);

static BV_FORCE_INLINE void divideV (double* BV_R_ vecA, double* BV_R_ vecB, const int count);


/* replaces every element in the passed vector with its square root */
static BV_FORCE_INLINE void squareRoot (float* BV_R_ data, const int dataSize);

static BV_FORCE_INLINE void squareRoot (double* BV_R_ data, const int dataSize);


/* replaces every element in the passed vector with its square */
static BV_FORCE_INLINE void square (float* BV_R_ data, const int dataSize);

static BV_FORCE_INLINE void square (double* BV_R_ data, const int dataSize);


/* returns the index in the vector of the minimum element */
static BV_FORCE_INLINE int findIndexOfMinElement (const float* BV_R_ data, const int dataSize);

static BV_FORCE_INLINE int findIndexOfMinElement (const double* BV_R_ data, const int dataSize);


/* returns the index in the vector of the maximum element */
static BV_FORCE_INLINE int findIndexOfMaxElement (const float* BV_R_ data, const int dataSize);

static BV_FORCE_INLINE int findIndexOfMaxElement (const double* BV_R_ data, const int dataSize);


/* returns both the minimum element and its index in the vector into the variables minimum and minIndex */
static BV_FORCE_INLINE void findMinAndMinIndex (const float* BV_R_ data, const int dataSize,
                                                float& minimum, int& minIndex);

static BV_FORCE_INLINE void findMinAndMinIndex (const double* BV_R_ data, const int dataSize,
                                                double& minimum, int& minIndex);


/* returns both the maximum element and its index in the vector into the variables maximum and maxIndex */
static BV_FORCE_INLINE void findMaxAndMaxIndex (const float* BV_R_ data, const int dataSize,
                                                float& maximum, int& maxIndex);

static BV_FORCE_INLINE void findMaxAndMaxIndex (const double* BV_R_ data, const int dataSize,
                                                double& maximum, int& maxIndex);


/* locates the element with the highest absolute value and its index in the vector, and returns them into the variables greatestMagnitude and index */
static BV_FORCE_INLINE void locateGreatestAbsMagnitude (const float* BV_R_ data, const int dataSize,
                                                        float& greatestMagnitude, int& index);

static BV_FORCE_INLINE void locateGreatestAbsMagnitude (const double* BV_R_ data, const int dataSize,
                                                        double& greatestMagnitude, int& index);


/* locates the element with the lowest absolute value and its index in the vector, and returns them into the variables leastMagnitude and index */
static BV_FORCE_INLINE void locateLeastAbsMagnitude (const float* BV_R_ data, const int dataSize,
                                                     float& leastMagnitude, int& index);

static BV_FORCE_INLINE void locateLeastAbsMagnitude (const double* BV_R_ data, const int dataSize,
                                                     double& leastMagnitude, int& index);


/* finds both the maximum and minimum elements in the vector and returns them into the variables max and min */
static inline void findExtrema (const float* BV_R_ data, const int dataSize,
                                float& min, float& max);

static BV_FORCE_INLINE void findExtrema (double* BV_R_ data, const int dataSize,
                                         double& min, double& max);


/* returns the distance between the maximum and minimum element of the vector */
static BV_FORCE_INLINE float findRangeOfExtrema (const float* BV_R_ data, const int dataSize);

static BV_FORCE_INLINE double findRangeOfExtrema (const double* BV_R_ data, const int dataSize);


/* converts cartesian to polar coordinates */
static BV_FORCE_INLINE void cartesian_to_polar (float* const BV_R_ mag, float* const BV_R_ phase,
                                                const float* const BV_R_ real, const float* const BV_R_ imag,
                                                const int count);

static BV_FORCE_INLINE void cartesian_to_polar (double* const BV_R_ mag, double* const BV_R_ phase,
                                                const double* const BV_R_ real, const double* const BV_R_ imag,
                                                const int count);


/* converts polar to cartesian coordinates */
static BV_FORCE_INLINE void polar_to_cartesian   (float* const BV_R_ real, float* const BV_R_ imag,
                                                  const float* const BV_R_ mag, const float* const BV_R_ phase,
                                                  const int dataSize);

static BV_FORCE_INLINE void polar_to_cartesian   (double* const BV_R_ real, double* const BV_R_ imag,
                                                  const double* const BV_R_ mag, const double* const BV_R_ phase,
                                                  const int dataSize);


/* converts cartesian coordinates to frequency bin magnitudes */
static BV_FORCE_INLINE void cartesian_to_magnitudes (float* const BV_R_ mag,
                                                     const float* const BV_R_ real, const float* const BV_R_ imag,
                                                     const int count);


static BV_FORCE_INLINE void cartesian_to_magnitudes (double* const BV_R_ mag,
                                                     const double* const BV_R_ real, const double* const BV_R_ imag,
                                                     const int count);

    
// deinterleave samples from dst into src
template<typename T>
static inline void deinterleave (T* dst,
                             const T* src,
                             const int channels,
                             const int count)
{
    int idx = 0;

    switch (channels)
    {
        case 2:
            // common case, may be vectorized by compiler if hardcoded
            for (int i = 0; i < count; ++i) {
                for (int j = 0; j < 2; ++j) {
                    dst[j][i] = src[idx++];
                }
            }
            return;
            
        case 1:
            juce::FloatVectorOperations::copy (dst[0], src, count);
            return;
            
        default:
            for (int i = 0; i < count; ++i) {
                for (int j = 0; j < channels; ++j) {
                    dst[j][i] = src[idx++];
                }
            }
    }
}

static BV_FORCE_INLINE void phasor (float* real, float* imag, float phase)
{
#if BV_USE_VDSP
    int one = 1;
    vvsincosf (imag, real, &phase, &one);

#elif defined LACK_SINCOS
    *real = cosf(phase);
    *imag = sinf(phase);

#elif defined __GNUC__
  #if defined __APPLE__
    #define sincosf __sincosf
  #endif
    sincosf (phase, imag, real);

#else
    *real = cosf(phase);
    *imag = sinf(phase);
#endif
}

static BV_FORCE_INLINE void phasor (double* real, double* imag, double phase)
{
#if BV_USE_VDSP
    int one = 1;
    vvsincos (imag, real, &phase, &one);

#elif defined LACK_SINCOS
    *real = cos(phase);
    *imag = sin(phase);

#elif defined __GNUC__
  #if defined __APPLE__
    #define sincos __sincos
  #endif
    sincos (phase, imag, real);

#else
    *real = cos(phase);
    *imag = sin(phase);
#endif
}


}  // namespace


#if BV_USE_VDSP
  #include "vecops_vdsp.h"
#elif BV_USE_IPP
  #include "vecops_ipp.h"
#else
  #include "vecops_fallback.h"
#endif
