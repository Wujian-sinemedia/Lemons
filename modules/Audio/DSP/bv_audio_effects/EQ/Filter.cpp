
namespace bav::dsp::FX
{
template < typename SampleType >
Filter< SampleType >::Filter (FilterType filterType, float frequency, float Qfactor, float gainMult)
{
    type = filterType;
    freq = (SampleType) frequency;
    Q = (SampleType) Qfactor;
    gain = (SampleType) gainMult;
    
    filter.prepare();
}

template < typename SampleType >
void Filter< SampleType >::process (juce::AudioBuffer< SampleType >& audio)
{
    switch (type)
    {
        case (LowPass) :
        {
            filter.coefs.makeLowPass (sampleRate, freq, Q);
            break;
        }
        case (HighPass) :
        {
            filter.coefs.makeHighPass (sampleRate, freq, Q);
            break;
        }
        case (LowShelf) :
        {
            filter.coefs.makeLowShelf (sampleRate, freq, Q, gain);
            break;
        }
        case (HighShelf) :
        {
            filter.coefs.makeHighShelf (sampleRate, freq, Q, gain);
            break;
        }
        case (BandPass) :
        {
            filter.coefs.makeBandPass (sampleRate, freq, Q);
            break;
        }
        case (Notch) :
        {
            filter.coefs.makeNotch (sampleRate, freq, Q);
            break;
        }
        case (Peak) :
        {
            filter.coefs.makePeakFilter (sampleRate, freq, Q, gain);
            break;
        }
        case (AllPass) :
        {
            filter.coefs.makeAllPass (sampleRate, freq, Q);
            break;
        }
    }

    filter.process (audio);
}

template < typename SampleType >
void Filter< SampleType >::prepare (double samplerate, int)
{
    sampleRate = samplerate;
    filter.prepare();
}

template < typename SampleType >
void Filter< SampleType >::setFilterType (FilterType newType)
{
    type = newType;
}

template < typename SampleType >
FilterType Filter< SampleType >::getFilterType() const
{
    return type;
}

template < typename SampleType >
void Filter< SampleType >::setFilterFrequency (float newFreqHz)
{
    freq = (SampleType) newFreqHz;
}

template < typename SampleType >
float Filter< SampleType >::getFilterFrequency() const
{
    return (float) freq;
}

template < typename SampleType >
void Filter< SampleType >::setQfactor (float newQ)
{
    jassert (newQ > 0.f);
    Q = (SampleType) newQ;
}

template < typename SampleType >
float Filter< SampleType >::getQfactor() const
{
    return (float) Q;
}

template < typename SampleType >
void Filter< SampleType >::setGain (float newGain)
{
    gain = (SampleType) newGain;
}

template < typename SampleType >
float Filter< SampleType >::getGain() const
{
    return (float) gain;
}

template class Filter< float >;
template class Filter< double >;

}  // namespace bav::dsp::FX
