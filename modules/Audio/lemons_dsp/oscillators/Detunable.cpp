namespace lemons::dsp::osc
{

template<typename SampleType>
DetunableBase<SampleType>::DetunableBase (ConstructedArray<Oscillator<SampleType>>& arrayToUse)
: Oscillator<SampleType> ([&]
                          {
    auto sample = SampleType (0);
    
    for (auto* osc : oscillators)
        sample += osc->getSample();
    
    return sample; }),
oscillators(arrayToUse)
{
    
}

template<typename SampleType>
void DetunableBase<SampleType>::resetPhase()
{
    for (auto* osc : oscillators)
        osc->resetPhase();
}

template<typename SampleType>
void DetunableBase<SampleType>::setFrequency (SampleType frequency, SampleType sampleRate)
{
    jassert (! oscillators->isEmpty());
    
    lastFrequency = frequency;
    samplerate    = sampleRate;
    
    const auto spreadSemitones = static_cast<SampleType> (totalSpreadCents) * SampleType (0.01);
    const auto increment       = spreadSemitones / static_cast<SampleType> (oscillators->size());
    const auto centerPitch     = math::freqToMidi (frequency);
    
    auto lowBound = centerPitch - (spreadSemitones * SampleType (0.5));
    
    for (auto* osc : oscillators)
    {
        osc->setFrequency (static_cast<SampleType> (math::midiToFreq (lowBound)), sampleRate);
        lowBound += increment;
    }
}

template<typename SampleType>
SampleType DetunableBase<SampleType>::getFrequency() const noexcept
{
    return lastFrequency;
}

template<typename SampleType>
void DetunableBase<SampleType>::setDetuneAmount (int totalPitchSpreadInCents)
{
    jassert (totalPitchSpreadInCents >= 0);
    totalSpreadCents = totalPitchSpreadInCents;
    setFrequency (lastFrequency, samplerate);
}

template<typename SampleType>
void DetunableBase<SampleType>::changeNumVoices (int newNumVoices)
{
    oscillators.resize (newNumVoices);
    setFrequency (lastFrequency, samplerate);
}

template<typename SampleType>
int DetunableBase<SampleType>::getNumVoices() const noexcept
{
    return oscillators->size();
}

template<typename SampleType>
int DetunableBase<SampleType>::getPitchSpreadCents() const noexcept
{
    return totalSpreadCents;
}

template class DetunableBase<float>;
template class DetunableBase<double>;

}

