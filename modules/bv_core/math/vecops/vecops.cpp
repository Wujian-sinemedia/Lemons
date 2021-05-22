
namespace bav::vecops
{
/* Finds the autocorrelation of a set of samples using a shrinking integration window */
void autocorrelate (const float* BV_R_ inputSamples,
                    int                numSamples,
                    float* BV_R_       outputSamples)
{
    const auto oneOverNumSamples = 1.0f / numSamples;

    for (int i = 0; i < numSamples; i++)
    {
        float sum = 0;

        for (int j = 0; j < numSamples - i; j++)
            sum += inputSamples[j] * inputSamples[j + i];

        outputSamples[i] = sum * oneOverNumSamples;
    }
}

void autocorrelate (const double* BV_R_ inputSamples,
                    int                 numSamples,
                    double* BV_R_       outputSamples)
{
    const auto oneOverNumSamples = 1.0 / numSamples;

    for (int i = 0; i < numSamples; i++)
    {
        double sum = 0;

        for (int j = 0; j < numSamples - i; j++)
            sum += inputSamples[j] * inputSamples[j + i];

        outputSamples[i] = sum * oneOverNumSamples;
    }
}


/* Autocorrelates a signal with itself using a squared difference function. Uses a shrinking integration window. */
void sdfAutocorrelate (const float* BV_R_ inputSamples,
                       int                numSamples,
                       float* BV_R_       outputSamples)
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

void sdfAutocorrelate (const double* BV_R_ inputSamples,
                       int                 numSamples,
                       double* BV_R_       outputSamples)
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


/* copies the contents of one vector to another. */
void copy (const float* const BV_R_ source, float* const BV_R_ dest, const int count)
{
#if BV_USE_IPP
    ippsMove_32f (source, dest, count);
#elif BV_USE_MIPP
    const auto         vecLoopSize = (count / mipp::N< float >()) * mipp::N< float >();
    mipp::Reg< float > rin, rout;
    for (int i = 0; i < vecLoopSize; i += mipp::N< float >())
    {
        rin.load (&source[i]);
        rout = rin;
        rout.store (&dest[i]);
    }
    for (
        int i = vecLoopSize; i < count;
        ++i)  // Scalar tail loop: finish the remaining elements that can't be vectorized.
        dest[i] = source[i];
#else
    memcpy (dest, source, (size_t) count * sizeof (float));
#endif
}

void copy (const double* const BV_R_ source,
           double* const BV_R_       dest,
           const int                 count)
{
#if BV_USE_IPP
    ippsMove_64f (source, dest, count);
#elif BV_USE_MIPP
    const auto          vecLoopSize = (count / mipp::N< double >()) * mipp::N< double >();
    mipp::Reg< double > rin, rout;
    for (int i = 0; i < vecLoopSize; i += mipp::N< double >())
    {
        rin.load (&source[i]);
        rout = rin;
        rout.store (&dest[i]);
    }
    for (int i = vecLoopSize; i < count; ++i)
        dest[i] = source[i];
#else
    memcpy (dest, source, (size_t) count * sizeof (double));
#endif
}


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
void interleave (T* const BV_R_ dst,
                 const T* const BV_R_* const BV_R_ src,
                 const int                         channels,
                 const int                         count)
{
    int idx = 0;
    switch (channels)
    {
        case 2 :
            // common case, may be vectorized by compiler if hardcoded
            for (int i = 0; i < count; ++i)
            {
                for (int j = 0; j < 2; ++j)
                {
                    dst[idx++] = src[j][i];
                }
            }
            return;
        case 1 : copy (src[0], dst, count); return;
        default :
            for (int i = 0; i < count; ++i)
            {
                for (int j = 0; j < channels; ++j)
                {
                    dst[idx++] = src[j][i];
                }
            }
    }
}
#if BV_USE_IPP
#    if (IPP_VERSION_MAJOR <= 7)  // Deprecated in v8, removed in v9
template <>
void interleave (float* const BV_R_ dst,
                 const float* const BV_R_* const BV_R_ src,
                 const int                             channels,
                 const int                             count)
{
    ippsInterleave_32f ((const Ipp32f**) src, channels, count, dst);
}
// IPP does not (currently?) provide double-precision interleave
#    endif
#endif


// deinterleave samples from dst into src
template < typename T >
void deinterleave (T* const BV_R_* const BV_R_ dst,
                   const T* const BV_R_        src,
                   const int                   channels,
                   const int                   count)
{
    int idx = 0;

    switch (channels)
    {
        case 2 :
            // common case, may be vectorized by compiler if hardcoded
            for (int i = 0; i < count; ++i)
            {
                for (int j = 0; j < 2; ++j)
                {
                    dst[j][i] = src[idx++];
                }
            }
            return;

        case 1 : copy (src, dst[0], count); return;

        default :
            for (int i = 0; i < count; ++i)
            {
                for (int j = 0; j < channels; ++j)
                {
                    dst[j][i] = src[idx++];
                }
            }
    }
}
#if BV_USE_IPP
#    if (IPP_VERSION_MAJOR <= 7)  // Deprecated in v8, removed in v9
template <>
void deinterleave (float* const BV_R_* const BV_R_ dst,
                   const float* const BV_R_        src,
                   const int                       channels,
                   const int                       count)
{
    ippsDeinterleave_32f ((const Ipp32f*) src, channels, count, (Ipp32f**) dst);
}
// IPP does not (currently?) provide double-precision deinterleave
#    endif
#endif


void phasor (float* real, float* imag, float phase)
{
#if BV_USE_VDSP
    int one = 1;
    vvsincosf (imag, real, &phase, &one);
#else
    *real                  = cosf (phase);
    *imag                  = sinf (phase);
#endif
}

static void phasor (double* real, double* imag, double phase)
{
#if BV_USE_VDSP
    int one = 1;
    vvsincos (imag, real, &phase, &one);
#else
    *real                  = cos (phase);
    *imag                  = sin (phase);
#endif
}


/* converts cartesian coordinates to frequency bin magnitudes */
void cartesian_to_magnitudes (float* const BV_R_       mag,
                              const float* const BV_R_ real,
                              const float* const BV_R_ imag,
                              const int                count)
{
#if BV_USE_IPP  // IPP is the only one of the auxillery libraries that supports this in one function call
    ippsMagnitude_32f (real, imag, mag, count);
#elif BV_USE_MIPP
    const auto vecLoopSize = (count / mipp::N< float >()) * mipp::N< float >();

    mipp::Reg< float > realIn, imagIn, magOut;

    for (int i = 0; i < vecLoopSize; i += mipp::N< float >())
    {
        realIn.load (&real[i]);
        imagIn.load (&imag[i]);
        magOut = mipp::sqrt ((realIn * realIn) + (imagIn * imagIn));
        magOut.store (&mag[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
    {
        const auto r = real[i];
        const auto c = imag[i];
        mag[i]       = sqrt (r * r + c * c);
    }
#else
    for (int i = 0; i < count; ++i)
    {
        const auto r = real[i];
        const auto c = imag[i];
        mag[i]       = sqrt (r * r + c * c);
    }
#endif
}


void cartesian_to_magnitudes (double* const BV_R_       mag,
                              const double* const BV_R_ real,
                              const double* const BV_R_ imag,
                              const int                 count)
{
#if BV_USE_IPP
    ippsMagnitude_64f (real, imag, mag, count);
#elif BV_USE_MIPP
    const auto vecLoopSize = (count / mipp::N< double >()) * mipp::N< double >();

    mipp::Reg< double > realIn, imagIn, magOut;

    for (int i = 0; i < vecLoopSize; i += mipp::N< double >())
    {
        realIn.load (&real[i]);
        imagIn.load (&imag[i]);
        magOut = mipp::sqrt ((realIn * realIn) + (imagIn * imagIn));
        magOut.store (&mag[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
    {
        const auto r = real[i];
        const auto c = imag[i];
        mag[i]       = sqrt (r * r + c * c);
    }
#else
    for (int i = 0; i < count; ++i)
    {
        const auto r = real[i];
        const auto c = imag[i];
        mag[i]       = sqrt (r * r + c * c);
    }
#endif
}

void cartesian_interleaved_to_magnitudes (
    float* const BV_R_ mag, const float* const BV_R_ src, const int count)
{
#if BV_USE_IPP
    ippsMagnitude_32fc (src, mag, count);
#elif BV_USE_MIPP
    const auto vecLoopSize = (count / mipp::N< float >()) * mipp::N< float >();

    mipp::Reg< float > realIn, imagIn, magOut;

    for (int i = 0; i < vecLoopSize; i += mipp::N< float >())
    {
        realIn.load (&src[i * 2]);
        imagIn.load (&src[i * 2 + 1]);
        magOut = mipp::sqrt ((realIn * realIn) + (imagIn * imagIn));
        magOut.store (&mag[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
    {
        const auto r = src[i * 2];
        const auto c = src[i * 2 + 1];
        mag[i]       = sqrt (r * r + c * c);
    }
#else
    for (int i = 0; i < count; ++i)
    {
        const auto r = src[i * 2];
        const auto c = src[i * 2 + 1];
        mag[i]       = sqrt (r * r + c * c);
    }
#endif
}

void cartesian_interleaved_to_magnitudes (
    double* const BV_R_ mag, const double* const BV_R_ src, const int count)
{
#if BV_USE_IPP
    ippsMagnitude_64fc (src, mag, count);
#elif BV_USE_MIPP
    const auto vecLoopSize = (count / mipp::N< double >()) * mipp::N< double >();

    mipp::Reg< double > realIn, imagIn, magOut;

    for (int i = 0; i < vecLoopSize; i += mipp::N< double >())
    {
        realIn.load (&src[i * 2]);
        imagIn.load (&src[i * 2 + 1]);
        magOut = mipp::sqrt ((realIn * realIn) + (imagIn * imagIn));
        magOut.store (&mag[i]);
    }

    for (int i = vecLoopSize; i < count; ++i)
    {
        const auto r = src[i * 2];
        const auto c = src[i * 2 + 1];
        mag[i]       = sqrt (r * r + c * c);
    }
#else
    for (int i = 0; i < count; ++i)
    {
        const auto r = src[i * 2];
        const auto c = src[i * 2 + 1];
        mag[i]       = sqrt (r * r + c * c);
    }
#endif
}


void cartesian_interleaved_to_polar (double* const BV_R_       mag,
                                     double* const BV_R_       phase,
                                     const double* const BV_R_ src,
                                     const int                 count)
{
#if BV_USE_IPP
    ippsCartToPolar_64fc (src, mag, phase, count);
#else
    for (int i = 0; i < count; ++i)
    {
        const auto real = src[i * 2];
        const auto imag = src[i * 2 + 1];
        *(mag + i)      = sqrt (real * real + imag * imag);
        *(phase + i)    = atan2 (imag, real);
    }
#endif
}

void cartesian_interleaved_to_polar (float* const BV_R_       mag,
                                     float* const BV_R_       phase,
                                     const float* const BV_R_ src,
                                     const int                count)
{
#if BV_USE_IPP
    ippsCartToPolar_32fc (src, mag, phase, count);
#else
    for (int i = 0; i < count; ++i)
    {
        const auto real = src[i * 2];
        const auto imag = src[i * 2 + 1];
        *(mag + i)      = sqrt (real * real + imag * imag);
        *(phase + i)    = atan2 (imag, real);
    }
#endif
}


void polar_to_cartesian_interleaved (float* const BV_R_       dst,
                                     const float* const BV_R_ mag,
                                     const float* const BV_R_ phase,
                                     const int                count)
{
#if BV_USE_IPP
    ippsPolarToCart_32fc (mag, phase, dst, count);
#elif BV_USE_MIPP
    const auto vecLoopSize = (count / mipp::N< float >()) * mipp::N< float >();

    mipp::Reg< float > magIn, phaseIn, realOut, imagOut;

    for (int i = 0; i < vecLoopSize; i += mipp::N< float >())
    {
        magIn.load (&mag[i]);
        phaseIn.load (&phase[i]);
        realOut = mipp::cos (phaseIn) * magIn;
        imagOut = mipp::sin (phaseIn) * magIn;
        realOut.store (&dst[i * 2]);
        imagOut.store (&dst[i * 2 + 1]);
    }

    float real, imag;
    for (int i = vecLoopSize; i < count; ++i)
    {
        phasor (&real, &imag, phase[i]);
        const auto m_mag = mag[i];
        dst[i * 2]       = real * m_mag;
        dst[i * 2 + 1]   = imag * m_mag;
    }
#else
    float real, imag;
    for (int i = 0; i < count; ++i)
    {
        phasor (&real, &imag, phase[i]);
        real *= mag[i];
        imag *= mag[i];
        dst[i * 2]     = real;
        dst[i * 2 + 1] = imag;
    }
#endif
}

void polar_to_cartesian_interleaved (double* const BV_R_       dst,
                                     const double* const BV_R_ mag,
                                     const double* const BV_R_ phase,
                                     const int                 count)
{
#if BV_USE_IPP
    ippsPolarToCart_64fc (mag, phase, dst, count);
#elif BV_USE_MIPP
    const auto vecLoopSize = (count / mipp::N< float >()) * mipp::N< float >();

    mipp::Reg< double > magIn, phaseIn, realOut, imagOut;

    for (int i = 0; i < vecLoopSize; i += mipp::N< float >())
    {
        magIn.load (&mag[i]);
        phaseIn.load (&phase[i]);
        realOut = mipp::cos (phaseIn) * magIn;
        imagOut = mipp::sin (phaseIn) * magIn;
        realOut.store (&dst[i * 2]);
        imagOut.store (&dst[i * 2 + 1]);
    }

    double real, imag;
    for (int i = vecLoopSize; i < count; ++i)
    {
        phasor (&real, &imag, phase[i]);
        const auto m_mag = mag[i];
        dst[i * 2]       = real * m_mag;
        dst[i * 2 + 1]   = imag * m_mag;
    }
#else
    double real, imag;
    for (int i = 0; i < count; ++i)
    {
        phasor (&real, &imag, phase[i]);
        real *= mag[i];
        imag *= mag[i];
        dst[i * 2]     = real;
        dst[i * 2 + 1] = imag;
    }
#endif
}


constexpr bool isUsingVDSP()
{
#if BV_USE_VDSP
    return true;
#else
    return false;
#endif
}

constexpr bool isUsingIPP()
{
#if BV_USE_IPP
    return true;
#else
    return false;
#endif
}

constexpr bool isUsingMIPP()
{
#if BV_USE_MIPP
    return true;
#else
    return false;
#endif
}

constexpr bool isUsingNe10()
{
#if BV_USE_NE10
    return true;
#else
    return false;
#endif
}

constexpr bool isUsingFallback()
{
    return ! (isUsingVDSP() || isUsingIPP() || isUsingMIPP() || isUsingNe10());
}


#if BV_USE_VDSP
#    include "implementations/vecops_vdsp.cpp"
#elif BV_USE_IPP
#    include "implementations/vecops_ipp.cpp"
#elif BV_USE_MIPP
#    include "implementations/vecops_mipp.cpp"
#elif BV_USE_NE10
#    include "implementations/vecops_ne10.cpp"
#else
#    include "implementations/vecops_fallback.cpp"
#endif

}  // namespace bav::vecops
