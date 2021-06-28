
#pragma once

#if BV_USE_VDSP
#    include <Accelerate/Accelerate.h>
#elif BV_USE_MIPP
#    include <mipp.h>
#endif

namespace bav::vecops
{
/* Finds the autocorrelation of a set of samples using a shrinking integration window */
extern void autocorrelate (const float* BV_R_ inputSamples,
                           int                numSamples,
                           float* BV_R_       outputSamples);

extern void autocorrelate (const double* BV_R_ inputSamples,
                           int                 numSamples,
                           double* BV_R_       outputSamples);


/* Autocorrelates a signal with itself using a squared difference function. Uses a shrinking integration window. */
extern void sdfAutocorrelate (const float* BV_R_ inputSamples,
                              int                numSamples,
                              float* BV_R_       outputSamples);

extern void sdfAutocorrelate (const double* BV_R_ inputSamples,
                              int                 numSamples,
                              double* BV_R_       outputSamples);


/* fills a vector with the specified value. */
extern void
    fill (float* BV_R_ vector, const float value, const int count);

extern void
    fill (double* BV_R_ vector, const double value, const int count);


/* copies the contents of one vector to another. */
extern void copy (const float* const BV_R_ source, float* const BV_R_ dest, const int count);

extern void copy (const double* const BV_R_ source,
                  double* const BV_R_       dest,
                  const int                 count);


/* copies each value of src into dst. The vectors may have different value types. If they are the same type, this is the same as using copy */
extern void
    convert (double* const BV_R_ dst, const float* const BV_R_ src, const int count);

extern void
    convert (float* const BV_R_ dst, const double* const BV_R_ src, const int count);


/* adds a single operand to each value in the vector */
extern void
    addC (float* BV_R_ vector, const float value, const int count);

extern void
    addC (double* BV_R_ vector, const double value, const int count);


/* performs element-wise addition of two vectors and writes the output to vecA */
extern void
    addV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count);

extern void
    addV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count);


/* subtracts a single operand from every element in the vector */
extern void
    subtractC (float* BV_R_ vector, const float value, const int count);

extern void
    subtractC (double* BV_R_ vector, const double value, const int count);


/* performs element-wise subtraction of two vectors and writes the output to vecA */
extern void
    subtractV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count);

extern void
    subtractV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count);


/* multiplies every element in the vector by a single operand */
extern void
    multiplyC (float* BV_R_ vector, const float value, const int count);

extern void
    multiplyC (double* BV_R_ vector, const double value, const int count);


/* performs element-wise multiplication of two vectors and writes the output to vecA */
extern void
    multiplyV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count);

extern void
    multiplyV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count);


/* divides every element in the vector by a single operand */
extern void
    divideC (float* BV_R_ vector, const float value, const int count);

extern void
    divideC (double* BV_R_ vector, const double value, const int count);


/* performs element-wise division of two vectors and writes the output to vecA */
extern void
    divideV (float* BV_R_ vecA, const float* BV_R_ vecB, const int count);

extern void
    divideV (double* BV_R_ vecA, const double* BV_R_ vecB, const int count);


/* replaces every element in the passed vector with its square root */
extern void squareRoot (float* BV_R_ data, const int dataSize);

extern void squareRoot (double* BV_R_ data, const int dataSize);


/* replaces every element in the passed vector with its square */
extern void square (float* BV_R_ data, const int dataSize);

extern void square (double* BV_R_ data, const int dataSize);


/* replaces every element in the passed vector with its absolute value */
extern void absVal (float* BV_R_ data, const int dataSize);

extern void absVal (double* BV_R_ data, const int dataSize);


/* returns the index in the vector of the minimum element */
extern int findIndexOfMinElement (const float* BV_R_ data,
                                  const int          dataSize);

extern int findIndexOfMinElement (const double* BV_R_ data,
                                  const int           dataSize);


/* returns the index in the vector of the maximum element */
extern int findIndexOfMaxElement (const float* BV_R_ data,
                                  const int          dataSize);

extern int findIndexOfMaxElement (const double* BV_R_ data,
                                  const int           dataSize);


/* returns both the minimum element and its index in the vector into the variables minimum and minIndex */
extern void findMinAndMinIndex (const float* BV_R_ data,
                                const int          dataSize,
                                float&             minimum,
                                int&               minIndex);

extern void findMinAndMinIndex (const double* BV_R_ data,
                                const int           dataSize,
                                double&             minimum,
                                int&                minIndex);


/* returns both the maximum element and its index in the vector into the variables maximum and maxIndex */
extern void findMaxAndMaxIndex (const float* BV_R_ data,
                                const int          dataSize,
                                float&             maximum,
                                int&               maxIndex);

extern void findMaxAndMaxIndex (const double* BV_R_ data,
                                const int           dataSize,
                                double&             maximum,
                                int&                maxIndex);


/* locates the element with the highest absolute value and its index in the vector, and returns them into the variables greatestMagnitude and index */
extern void locateGreatestAbsMagnitude (const float* BV_R_ data,
                                        const int          dataSize,
                                        float&             greatestMagnitude,
                                        int&               index);

extern void locateGreatestAbsMagnitude (const double* BV_R_ data,
                                        const int           dataSize,
                                        double&             greatestMagnitude,
                                        int&                index);


/* locates the element with the lowest absolute value and its index in the vector, and returns them into the variables leastMagnitude and index */
extern void locateLeastAbsMagnitude (const float* BV_R_ data,
                                     const int          dataSize,
                                     float&             leastMagnitude,
                                     int&               index);

extern void locateLeastAbsMagnitude (const double* BV_R_ data,
                                     const int           dataSize,
                                     double&             leastMagnitude,
                                     int&                index);


/* finds both the maximum and minimum elements in the vector and returns them into the variables max and min */
extern void findExtrema (const float* BV_R_ data,
                         const int          dataSize,
                         float&             min,
                         float&             max);

extern void
    findExtrema (double* BV_R_ data, const int dataSize, double& min, double& max);


/* returns the distance between the maximum and minimum element of the vector */
extern float findRangeOfExtrema (const float* BV_R_ data,
                                 const int          dataSize);

extern double findRangeOfExtrema (const double* BV_R_ data,
                                  const int           dataSize);


/* Normalises a set of samples to the absolute maximum contained within the buffer. */
extern void normalize (float* BV_R_ vector, const int numSamples);

extern void normalize (double* BV_R_ vector, const int numSamples);


/* converts cartesian to polar coordinates */
extern void cartesian_to_polar (float* const BV_R_       mag,
                                float* const BV_R_       phase,
                                const float* const BV_R_ real,
                                const float* const BV_R_ imag,
                                const int                count);

extern void cartesian_to_polar (double* const BV_R_       mag,
                                double* const BV_R_       phase,
                                const double* const BV_R_ real,
                                const double* const BV_R_ imag,
                                const int                 count);


/* converts polar to cartesian coordinates */
extern void polar_to_cartesian (float* const BV_R_       real,
                                float* const BV_R_       imag,
                                const float* const BV_R_ mag,
                                const float* const BV_R_ phase,
                                const int                dataSize);

extern void polar_to_cartesian (double* const BV_R_       real,
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
extern void interleave (T* const BV_R_ dst,
                        const T* const BV_R_* const BV_R_ src,
                        const int                         channels,
                        const int                         count);


// deinterleave samples from dst into src
template < typename T >
extern void deinterleave (T* const BV_R_* const BV_R_ dst,
                          const T* const BV_R_        src,
                          const int                   channels,
                          const int                   count);


extern void phasor (float* real, float* imag, float phase);

extern void phasor (double* real, double* imag, double phase);


/* converts cartesian coordinates to frequency bin magnitudes */
extern void cartesian_to_magnitudes (float* const BV_R_       mag,
                                     const float* const BV_R_ real,
                                     const float* const BV_R_ imag,
                                     const int                count);

extern void cartesian_to_magnitudes (double* const BV_R_       mag,
                                     const double* const BV_R_ real,
                                     const double* const BV_R_ imag,
                                     const int                 count);

extern void cartesian_interleaved_to_magnitudes (
    float* const BV_R_ mag, const float* const BV_R_ src, const int count);

extern void cartesian_interleaved_to_magnitudes (
    double* const BV_R_ mag, const double* const BV_R_ src, const int count);


extern void
    cartesian_interleaved_to_polar (double* const BV_R_       mag,
                                    double* const BV_R_       phase,
                                    const double* const BV_R_ src,
                                    const int                 count);

extern void
    cartesian_interleaved_to_polar (float* const BV_R_       mag,
                                    float* const BV_R_       phase,
                                    const float* const BV_R_ src,
                                    const int                count);


extern void
    polar_to_cartesian_interleaved (float* const BV_R_       dst,
                                    const float* const BV_R_ mag,
                                    const float* const BV_R_ phase,
                                    const int                count);

extern void
    polar_to_cartesian_interleaved (double* const BV_R_       dst,
                                    const double* const BV_R_ mag,
                                    const double* const BV_R_ phase,
                                    const int                 count);


extern constexpr bool isUsingVDSP();

extern constexpr bool isUsingIPP();

extern constexpr bool isUsingMIPP();

extern constexpr bool isUsingNe10();

extern constexpr bool isUsingFallback();


}  // namespace bav::vecops
