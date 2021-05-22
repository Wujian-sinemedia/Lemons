
#pragma once

namespace bav::vecops
{
/*
    This namespace contains some useful vectorized functions optimized for a variety of platforms.
    This header file defines the common interface for the vecops functions, and there is a separate header defining implementations for each individual vectorization library.
    When you compile the bv_SharedCode Juce module, only ONE of the available vecops implementations will be selected and compiled, and all calls to vecops functions will use the same implementation.
 
    Here is a brief, generalized overview of the available implementations:
 
    - vDSP: created by Apple and optimized for their hardware. vDSP ships with the OS on any Apple hardware and easily outperforms any open source library. There isn't really a good reason not to use this if it is available to you, which is why it is the default choice on Apple platforms.
    - IPP: stands for "Intel Integrated Performance Primitives". By far the fastest on supported Intel hardware, but must be specially linked to. Only available on x86/amd64.
    - Ne10: an open-source library of optimized functions for ARM NEON architecture. Very fast for floating point computations, but supports ONLY 32-bit floating point. Any functions with doubles as input or output will either use Juce::FloatVectorOperations or involve some internal conversion overhead.
    - MIPP: an open source library that essentially serves as a wrapper around native SIMD instruction sets. Currently supports NEON, SSE, AVX and AVX-512.
    - Fallback: wraps Juce::FloatVectorOperations where available, and implements operations in pure C/C++ where FVO doesn't provide an implementation.
*/


// make sure that only one of these is set to 1...
// these are evaluated in approximate order of preference, and the first one found in this list is used: vDSP, IPP, MIPP, Ne10, fallback
#if BV_USE_VDSP
#    undef BV_USE_IPP
#    define BV_USE_IPP 0
#    undef BV_USE_MIPP
#    define BV_USE_MIPP 0
#    undef BV_USE_NE10
#    define BV_USE_NE10 0
#elif BV_USE_IPP
#    undef BV_USE_MIPP
#    define BV_USE_MIPP 0
#    undef BV_USE_NE10
#    define BV_USE_NE10 0
#elif BV_USE_MIPP
#    undef BV_USE_NE10
#    define BV_USE_NE10 0
#endif /* if BV_USE_VDSP */


/* Finds the autocorrelation of a set of samples using a shrinking integration window */
static BV_FORCE_INLINE void autocorrelate (const float* BV_R_ inputSamples,
                                           int                numSamples,
                                           float* BV_R_       outputSamples);

static BV_FORCE_INLINE void autocorrelate (const double* BV_R_ inputSamples,
                                           int                 numSamples,
                                           double* BV_R_       outputSamples);


/* Autocorrelates a signal with itself using a squared difference function. Uses a shrinking integration window. */
static BV_FORCE_INLINE void sdfAutocorrelate (const float* BV_R_ inputSamples,
                                              int                numSamples,
                                              float* BV_R_       outputSamples);

static BV_FORCE_INLINE void sdfAutocorrelate (const double* BV_R_ inputSamples,
                                              int                 numSamples,
                                              double* BV_R_       outputSamples);


/* fills a vector with the specified value. */
static BV_FORCE_INLINE void
    fill (float* BV_R_ vector, const float value, const int count);

static BV_FORCE_INLINE void
    fill (double* BV_R_ vector, const double value, const int count);


/* copies the contents of one vector to another. */
static BV_FORCE_INLINE void copy (const float* const BV_R_ source, float* const BV_R_ dest, const int count);

static BV_FORCE_INLINE void copy (const double* const BV_R_ source,
                                  double* const BV_R_       dest,
                                  const int                 count);


/* copies each value of src into dst. The vectors may have different value types. If they are the same type, this is the same as using copy */
static BV_FORCE_INLINE void
    convert (double* const BV_R_ dst, const float* const BV_R_ src, const int count);

static BV_FORCE_INLINE void
    convert (float* const BV_R_ dst, const double* const BV_R_ src, const int count);


/* adds a single operand to each value in the vector */
static BV_FORCE_INLINE void
    addC (float* BV_R_ vector, const float value, const int count);

static BV_FORCE_INLINE void
    addC (double* BV_R_ vector, const double value, const int count);


/* performs element-wise addition of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void
    addV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count);

static BV_FORCE_INLINE void
    addV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count);


/* subtracts a single operand from every element in the vector */
static BV_FORCE_INLINE void
    subtractC (float* BV_R_ vector, const float value, const int count);

static BV_FORCE_INLINE void
    subtractC (double* BV_R_ vector, const double value, const int count);


/* performs element-wise subtraction of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void
    subtractV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count);

static BV_FORCE_INLINE void
    subtractV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count);


/* multiplies every element in the vector by a single operand */
static BV_FORCE_INLINE void
    multiplyC (float* BV_R_ vector, const float value, const int count);

static BV_FORCE_INLINE void
    multiplyC (double* BV_R_ vector, const double value, const int count);


/* performs element-wise multiplication of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void
    multiplyV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count);

static BV_FORCE_INLINE void
    multiplyV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count);


/* divides every element in the vector by a single operand */
static BV_FORCE_INLINE void
    divideC (float* BV_R_ vector, const float value, const int count);

static BV_FORCE_INLINE void
    divideC (double* BV_R_ vector, const double value, const int count);


/* performs element-wise division of two vectors and writes the output to vecA */
static BV_FORCE_INLINE void
    divideV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count);

static BV_FORCE_INLINE void
    divideV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count);


/* replaces every element in the passed vector with its square root */
static BV_FORCE_INLINE void squareRoot (float* BV_R_ data, const int dataSize);

static BV_FORCE_INLINE void squareRoot (double* BV_R_ data, const int dataSize);


/* replaces every element in the passed vector with its square */
static BV_FORCE_INLINE void square (float* BV_R_ data, const int dataSize);

static BV_FORCE_INLINE void square (double* BV_R_ data, const int dataSize);


/* replaces every element in the passed vector with its absolute value */
static BV_FORCE_INLINE void absVal (float* BV_R_ data, const int dataSize);

static BV_FORCE_INLINE void absVal (double* BV_R_ data, const int dataSize);


/* returns the index in the vector of the minimum element */
static BV_FORCE_INLINE int findIndexOfMinElement (const float* BV_R_ data,
                                                  const int          dataSize);

static BV_FORCE_INLINE int findIndexOfMinElement (const double* BV_R_ data,
                                                  const int           dataSize);


/* returns the index in the vector of the maximum element */
static BV_FORCE_INLINE int findIndexOfMaxElement (const float* BV_R_ data,
                                                  const int          dataSize);

static BV_FORCE_INLINE int findIndexOfMaxElement (const double* BV_R_ data,
                                                  const int           dataSize);


/* returns both the minimum element and its index in the vector into the variables minimum and minIndex */
static BV_FORCE_INLINE void findMinAndMinIndex (const float* BV_R_ data,
                                                const int          dataSize,
                                                float&             minimum,
                                                int&               minIndex);

static BV_FORCE_INLINE void findMinAndMinIndex (const double* BV_R_ data,
                                                const int           dataSize,
                                                double&             minimum,
                                                int&                minIndex);


/* returns both the maximum element and its index in the vector into the variables maximum and maxIndex */
static BV_FORCE_INLINE void findMaxAndMaxIndex (const float* BV_R_ data,
                                                const int          dataSize,
                                                float&             maximum,
                                                int&               maxIndex);

static BV_FORCE_INLINE void findMaxAndMaxIndex (const double* BV_R_ data,
                                                const int           dataSize,
                                                double&             maximum,
                                                int&                maxIndex);


/* locates the element with the highest absolute value and its index in the vector, and returns them into the variables greatestMagnitude and index */
static BV_FORCE_INLINE void locateGreatestAbsMagnitude (const float* BV_R_ data,
                                                        const int          dataSize,
                                                        float&             greatestMagnitude,
                                                        int&               index);

static BV_FORCE_INLINE void locateGreatestAbsMagnitude (const double* BV_R_ data,
                                                        const int           dataSize,
                                                        double&             greatestMagnitude,
                                                        int&                index);


/* locates the element with the lowest absolute value and its index in the vector, and returns them into the variables leastMagnitude and index */
static BV_FORCE_INLINE void locateLeastAbsMagnitude (const float* BV_R_ data,
                                                     const int          dataSize,
                                                     float&             leastMagnitude,
                                                     int&               index);

static BV_FORCE_INLINE void locateLeastAbsMagnitude (const double* BV_R_ data,
                                                     const int           dataSize,
                                                     double&             leastMagnitude,
                                                     int&                index);


/* finds both the maximum and minimum elements in the vector and returns them into the variables max and min */
static inline void findExtrema (const float* BV_R_ data,
                                const int          dataSize,
                                float&             min,
                                float&             max);

static BV_FORCE_INLINE void
    findExtrema (double* BV_R_ data, const int dataSize, double& min, double& max);


/* returns the distance between the maximum and minimum element of the vector */
static BV_FORCE_INLINE float findRangeOfExtrema (const float* BV_R_ data,
                                                 const int          dataSize);

static BV_FORCE_INLINE double findRangeOfExtrema (const double* BV_R_ data,
                                                  const int           dataSize);


/* Normalises a set of samples to the absolute maximum contained within the buffer. */
static BV_FORCE_INLINE void normalize (float* BV_R_ vector, const int numSamples);

static BV_FORCE_INLINE void normalize (double* BV_R_ vector, const int numSamples);


/* converts cartesian to polar coordinates */
static BV_FORCE_INLINE void cartesian_to_polar (float* const BV_R_       mag,
                                                float* const BV_R_       phase,
                                                const float* const BV_R_ real,
                                                const float* const BV_R_ imag,
                                                const int                count);

static BV_FORCE_INLINE void cartesian_to_polar (double* const BV_R_       mag,
                                                double* const BV_R_       phase,
                                                const double* const BV_R_ real,
                                                const double* const BV_R_ imag,
                                                const int                 count);


/* converts polar to cartesian coordinates */
static BV_FORCE_INLINE void polar_to_cartesian (float* const BV_R_       real,
                                                float* const BV_R_       imag,
                                                const float* const BV_R_ mag,
                                                const float* const BV_R_ phase,
                                                const int                dataSize);

static BV_FORCE_INLINE void polar_to_cartesian (double* const BV_R_       real,
                                                double* const BV_R_       imag,
                                                const double* const BV_R_ mag,
                                                const double* const BV_R_ phase,
                                                const int                 dataSize);


/**
 * interleave
 *
 * Interleave (zip) the \arg channels vectors in \arg src, each of
 * length \arg count, into the single vector \arg dst of length \arg
 * channels * \arg count.
 *
 * Caller guarantees that the \arg src and \arg dst vectors are
 * non-overlapping.
 */
template < typename T >
static inline void interleave (T* const BV_R_ dst,
                               const T* const BV_R_* const BV_R_ src,
                               const int                         channels,
                               const int                         count);


// deinterleave samples from dst into src
template < typename T >
static inline void deinterleave (T* const BV_R_* const BV_R_ dst,
                                 const T* const BV_R_        src,
                                 const int                   channels,
                                 const int                   count);


static BV_FORCE_INLINE void phasor (float* real, float* imag, float phase);

static BV_FORCE_INLINE void phasor (double* real, double* imag, double phase);


/* converts cartesian coordinates to frequency bin magnitudes */
static BV_FORCE_INLINE void cartesian_to_magnitudes (float* const BV_R_       mag,
                                                     const float* const BV_R_ real,
                                                     const float* const BV_R_ imag,
                                                     const int                count);

static BV_FORCE_INLINE void cartesian_to_magnitudes (double* const BV_R_       mag,
                                                     const double* const BV_R_ real,
                                                     const double* const BV_R_ imag,
                                                     const int                 count);

static BV_FORCE_INLINE void cartesian_interleaved_to_magnitudes (
                                                                 float* const BV_R_ mag, const float* const BV_R_ src, const int count);

static BV_FORCE_INLINE void cartesian_interleaved_to_magnitudes (
                                                                 double* const BV_R_ mag, const double* const BV_R_ src, const int count);


static BV_FORCE_INLINE void
    cartesian_interleaved_to_polar (double* const BV_R_       mag,
                                    double* const BV_R_       phase,
                                    const double* const BV_R_ src,
                                    const int                 count);

static BV_FORCE_INLINE void
    cartesian_interleaved_to_polar (float* const BV_R_       mag,
                                    float* const BV_R_       phase,
                                    const float* const BV_R_ src,
                                    const int                count);


static BV_FORCE_INLINE void
    polar_to_cartesian_interleaved (float* const BV_R_       dst,
                                    const float* const BV_R_ mag,
                                    const float* const BV_R_ phase,
                                    const int                count);

static BV_FORCE_INLINE void
    polar_to_cartesian_interleaved (double* const BV_R_       dst,
                                    const double* const BV_R_ mag,
                                    const double* const BV_R_ phase,
                                    const int                 count);


static constexpr bool isUsingVDSP();

static constexpr bool isUsingIPP();

static constexpr bool isUsingMIPP();

static constexpr bool isUsingNe10();

static constexpr bool isUsingFallback();


}  // namespace bav::vecops

