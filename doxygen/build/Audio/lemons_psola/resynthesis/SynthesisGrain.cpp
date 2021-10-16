
/** @ingroup resynthesis
 *  @{
 */

namespace lemons::dsp::psola
{
template < typename SampleType >
Shifter< SampleType >::Grain::Grain (const Storage& storageToUse)
    : storage (storageToUse)
{
}

template < typename SampleType >
bool Shifter< SampleType >::Grain::isActive() const
{
    return active;
}

template < typename SampleType >
void Shifter< SampleType >::Grain::startNewGrain (int start, int length)
{
    active      = true;
    startIndex  = start;
    grainLength = length;
    currentTick = 0;
}

/*-------------------------------------------------------------*/
/* Hann window function */

template < typename FloatType >
static inline FloatType ncos (int order, int i, int size) noexcept
{
    return std::cos (static_cast< FloatType > (order * i)
                     * juce::MathConstants< FloatType >::pi / static_cast< FloatType > (size - 1));
}
template float  ncos (int, int, int) noexcept;
template double ncos (int, int, int) noexcept;

template < typename SampleType >
static inline SampleType getWindowValue (int index, int windowLength) noexcept
{
    return static_cast< SampleType > (0.5 - 0.5 * ncos< SampleType > (2, index, windowLength));
}
template float  getWindowValue (int, int) noexcept;
template double getWindowValue (int, int) noexcept;

/*-------------------------------------------------------------*/

template < typename SampleType >
SampleType Shifter< SampleType >::Grain::getNextSample()
{
    jassert (grainLength > 0);

    const auto sample = storage.getSample (startIndex, currentTick) * getWindowValue< SampleType > (currentTick, grainLength);

    currentTick++;

    if (currentTick > grainLength)
        active = false;

    return sample;
}

}  // namespace lemons::dsp::psola

/** @}*/
