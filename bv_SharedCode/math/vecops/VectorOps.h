
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
    
    
#if defined __GNUC__ && defined _WIN32
    // MinGW doesn't appear to have sincos, so define it -- it's a single x87 instruction anyway
    static BV_FORCE_INLINE void sincos (double x, double* sin, double* cos)
    {
        __asm__ ("fsincos;" : "=t" (*cos), "=u" (*sin) : "0" (x) : "st(7)");
    }
    
    static BV_FORCE_INLINE void sincosf (float fx, float* fsin, float* fcos)
    {
        double sin, cos;
        sincos (fx, &sin, &cos);
        *fsin = sin;
        *fcos = cos;
    }
#endif
    
    
/* Finds the autocorrelation of a set of samples using a shrinking integration window */
static BV_FORCE_INLINE void autocorrelate (const float* BV_R_ inputSamples, int numSamples, float* BV_R_ outputSamples)
{
    const float oneOverNumSamples = 1.0f / numSamples;
    
    for (int i = 0; i < numSamples; i++)
    {
        float sum = 0;
        
        for (int j = 0; j < numSamples - i; j++)
            sum += inputSamples[j] * inputSamples[j + i];
        
        outputSamples[i] = sum * oneOverNumSamples;
    }
}
    
static BV_FORCE_INLINE void autocorrelate (const double* BV_R_ inputSamples, int numSamples, double* BV_R_ outputSamples)
{
    const double oneOverNumSamples = 1.0 / numSamples;
    
    for (int i = 0; i < numSamples; i++)
    {
        double sum = 0;
        
        for (int j = 0; j < numSamples - i; j++)
            sum += inputSamples[j] * inputSamples[j + i];
        
        outputSamples[i] = sum * oneOverNumSamples;
    }
}
    
    
/* Autocorrelates a signal with itself using a squared difference function. Uses a shrinking integration window. */
static BV_FORCE_INLINE void sdfAutocorrelate (const float* BV_R_ inputSamples, int numSamples, float* BV_R_ outputSamples)
{
    for (int i = 0; i < numSamples; i++)
    {
        float sum = 0;
        
        for (int j = 0; j < numSamples - i; j++)
        {
            const auto difference = inputSamples[j] - inputSamples[j + i];
            sum += difference * difference;
        }
        
        outputSamples[i] = sum;
    }
}
    
static BV_FORCE_INLINE void sdfAutocorrelate (const double* BV_R_ inputSamples, int numSamples, double* BV_R_ outputSamples)
{
    for (int i = 0; i < numSamples; i++)
    {
        double sum = 0;
        
        for (int j = 0; j < numSamples - i; j++)
        {
            const auto difference = inputSamples[j] - inputSamples[j + i];
            sum += difference * difference;
        }
        
        outputSamples[i] = sum;
    }
}


/* fills a vector with the specified value. */
static BV_FORCE_INLINE void fill (float* BV_R_ vector, const float value, const int count);

static BV_FORCE_INLINE void fill (double* BV_R_ vector, const double value, const int count);


/* copies the contents of one vector to another. */
static BV_FORCE_INLINE void copy (const float* const BV_R_ source, float* const BV_R_ dest, const int count)
{
#if BV_USE_IPP
    ippsMove_32f (source, dest, count);
#elif BV_USE_MIPP
    const auto vecLoopSize = (count / mipp::N<float>()) * mipp::N<float>();
    mipp::Reg<float> rin, rout;
    for (int i = 0; i < vecLoopSize; i += mipp::N<float>()) {
        rin.load(&source[i]);
        rout = rin;
        rout.store (&dest[i]);
    }
    for (int i = vecLoopSize; i < count; ++i)  // Scalar tail loop: finish the remaining elements that can't be vectorized.
        dest[i] = source[i];
#else
    memcpy (dest, source, (size_t) count * sizeof (float));
#endif
}

static BV_FORCE_INLINE void copy (const double* const BV_R_ source, double* const BV_R_ dest, const int count)
{
#if BV_USE_IPP
    ippsMove_64f (source, dest, count);
#elif BV_USE_MIPP
    const auto vecLoopSize = (count / mipp::N<double>()) * mipp::N<double>();
    mipp::Reg<float> rin, rout;
    for (int i = 0; i < vecLoopSize; i += mipp::N<double>()) {
        rin.load(&source[i]);
        rout = rin;
        rout.store (&dest[i]);
    }
    for (int i = vecLoopSize; i < count; ++i)
        dest[i] = source[i];
#else
    memcpy (dest, source, (size_t) count * sizeof (double));
#endif
}


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
    
    
/* replaces every element in the passed vector with its absolute value */
static BV_FORCE_INLINE void absVal (float* BV_R_ data, const int dataSize);

static BV_FORCE_INLINE void absVal (double* BV_R_ data, const int dataSize);


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
    
    
/* Normalises a set of samples to the absolute maximum contained within the buffer. */
static BV_FORCE_INLINE void normalize (float* BV_R_ vector, const int numSamples);

static BV_FORCE_INLINE void normalize (double* BV_R_ vector, const int numSamples);


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
template<typename T>
inline void interleave (T *const BV_R_ dst,
                        const T* const BV_R_ *const BV_R_ src,
                        const int channels,
                        const int count)
{
    int idx = 0;
    switch (channels) {
        case 2:
            // common case, may be vectorized by compiler if hardcoded
            for (int i = 0; i < count; ++i) {
                for (int j = 0; j < 2; ++j) {
                    dst[idx++] = src[j][i];
                }
            }
            return;
        case 1:
            copy (src[0], dst, count);
            return;
        default:
            for (int i = 0; i < count; ++i) {
                for (int j = 0; j < channels; ++j) {
                    dst[idx++] = src[j][i];
                }
            }
    }
}
#if BV_USE_IPP
  #if (IPP_VERSION_MAJOR <= 7) // Deprecated in v8, removed in v9
    template<>
    inline void interleave (float* const BV_R_ dst,
                            const float* const BV_R_ *const BV_R_ src,
                            const int channels,
                            const int count)
    {
        ippsInterleave_32f ((const Ipp32f **)src, channels, count, dst);
    }
    // IPP does not (currently?) provide double-precision interleave
  #endif
#endif
    
    
// deinterleave samples from dst into src
template<typename T>
static inline void deinterleave (T* const BV_R_ *const BV_R_ dst,
                                 const T* const BV_R_ src,
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
            copy (src, dst[0], count);
            return;
            
        default:
            for (int i = 0; i < count; ++i) {
                for (int j = 0; j < channels; ++j) {
                    dst[j][i] = src[idx++];
                }
            }
    }
}
#if BV_USE_IPP
  #if (IPP_VERSION_MAJOR <= 7) // Deprecated in v8, removed in v9
    template<>
    inline void v_deinterleave(float* const BV_R_ *const BV_R_ dst,
                               const float* const BV_R_ src,
                               const int channels,
                               const int count)
    {
        ippsDeinterleave_32f((const Ipp32f *)src, channels, count, (Ipp32f **)dst);
    }
    // IPP does not (currently?) provide double-precision deinterleave
  #endif
#endif

    
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
    
    
/* converts cartesian coordinates to frequency bin magnitudes */
static BV_FORCE_INLINE void cartesian_to_magnitudes (float* const BV_R_ mag,
                                                     const float* const BV_R_ real, const float* const BV_R_ imag,
                                                     const int count)
{
#if BV_USE_IPP  // IPP is the only one of the auxillery libraries that supports this in one function call
    ippsMagnitude_32f (real, imag, mag, count);
#else
    for (int i = 0; i < count; ++i) {
        const auto r = real[i];
        const auto c = imag[i];
        mag[i] = sqrt (r * r + c * c);
    }
#endif
}


static BV_FORCE_INLINE void cartesian_to_magnitudes (double* const BV_R_ mag,
                                                     const double* const BV_R_ real, const double* const BV_R_ imag,
                                                     const int count)
{
#if BV_USE_IPP
    ippsMagnitude_64f (real, imag, mag, count);
#else
    for (int i = 0; i < count; ++i) {
        const auto r = real[i];
        const auto c = imag[i];
        mag[i] = sqrt (r * r + c * c);
    }
#endif
}
    
static BV_FORCE_INLINE void cartesian_interleaved_to_magnitudes (float* const BV_R_ mag,
                                                                 const float* const BV_R_ src,
                                                                 const int count)
{
#if BV_USE_IPP
    ippsMagnitude_32fc (src, mag, count);
#else
    for (int i = 0; i < count; ++i)
    {
        const auto ip = src[i*2];
        const auto tn = src[i*2+1];
        mag[i] = sqrt (ip * ip + tn * tn);
    }
#endif
}

static BV_FORCE_INLINE void cartesian_interleaved_to_magnitudes (double* const BV_R_ mag,
                                                                 const double* const BV_R_ src,
                                                                 const int count)
{
#if BV_USE_IPP
    ippsMagnitude_64fc (src, mag, count);
#else
    for (int i = 0; i < count; ++i)
    {
        const auto ip = src[i*2];
        const auto tn = src[i*2+1];
        mag[i] = sqrt (ip * ip + tn * tn);
    }
#endif
}
    
    
static BV_FORCE_INLINE void cartesian_interleaved_to_polar (double* const BV_R_ mag,
                                                            double* const BV_R_ phase,
                                                            const double* const BV_R_ src,
                                                            const int count)
{
#if BV_USE_IPP
    ippsCartToPolar_64fc (src, mag, phase, count);
#else
    for (int i = 0; i < count; ++i)
    {
        const auto real = src[i * 2];
        const auto imag = src[i * 2 + 1];
        *(mag + i) = sqrt (real * real + imag * imag);
        *(phase + i) = atan2 (imag, real);
    }
#endif
}

static BV_FORCE_INLINE void cartesian_interleaved_to_polar (float* const BV_R_ mag,
                                                            float* const BV_R_ phase,
                                                            const float* const BV_R_ src,
                                                            const int count)
{
#if BV_USE_IPP
    ippsCartToPolar_32fc (src, mag, phase, count);
#else
    for (int i = 0; i < count; ++i)
    {
        const auto real = src[i * 2];
        const auto imag = src[i * 2 + 1];
        *(mag + i) = sqrt (real * real + imag * imag);
        *(phase + i) = atan2 (imag, real);
    }
#endif
}
    
    
static BV_FORCE_INLINE void polar_to_cartesian_interleaved (float* const BV_R_ dst,
                                                            const float* const BV_R_ mag,
                                                            const float* const BV_R_ phase,
                                                            const int count)
{
#if BV_USE_IPP
    ippsPolarToCart_32fc (mag, phase, dst, count);
#else
    float real, imag;
    for (int i = 0; i < count; ++i)
    {
        phasor (&real, &imag, phase[i]);
        real *= mag[i];
        imag *= mag[i];
        dst[i*2] = real;
        dst[i*2+1] = imag;
    }
#endif
}

static BV_FORCE_INLINE void polar_to_cartesian_interleaved (double* const BV_R_ dst,
                                                            const double* const BV_R_ mag,
                                                            const double* const BV_R_ phase,
                                                            const int count)
{
#if BV_USE_IPP
    ippsPolarToCart_64fc (mag, phase, dst, count);
#else
    double real, imag;
    for (int i = 0; i < count; ++i)
    {
        phasor (&real, &imag, phase[i]);
        real *= mag[i];
        imag *= mag[i];
        dst[i*2] = real;
        dst[i*2+1] = imag;
    }
#endif
}


}  // namespace


#if BV_USE_VDSP
  #include "vecops_vdsp.h"
#elif BV_USE_IPP
  #include "vecops_ipp.h"
#elif BV_USE_MIPP
  #include "vecops/vecops_mipp.h"
#elif BV_USE_NE10
  #include "vecops_ne10.h"
#else
  #include "vecops_fallback.h"
#endif

