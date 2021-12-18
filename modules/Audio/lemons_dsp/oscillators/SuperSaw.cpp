/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */


#include <lemons_core/lemons_core.h>

namespace lemons::dsp::osc
{
template <typename SampleType>
SuperSaw<SampleType>::SuperSaw()
    : Oscillator<SampleType> ([&]
                              {
    auto sample = SampleType (0);
    
    for (auto* saw : saws)
        sample += saw->getSample();
    
    return sample; })
{
}

template <typename SampleType>
void SuperSaw<SampleType>::resetPhase()
{
	for (auto* saw : saws)
		saw->resetPhase();
}

template <typename SampleType>
void SuperSaw<SampleType>::setFrequency (SampleType frequency, SampleType sampleRate)
{
	lastFrequency = frequency;
	samplerate    = sampleRate;

	const auto spreadSemitones = static_cast<SampleType> (totalSpreadCents) * SampleType (0.01);
	const auto increment       = spreadSemitones / static_cast<SampleType> (saws->size());
	const auto centerPitch     = math::freqToMidi (frequency);

	auto lowBound = centerPitch - (spreadSemitones * SampleType (0.5));

	for (auto* saw : saws)
	{
		saw->setFrequency (static_cast<SampleType> (math::midiToFreq (lowBound)), sampleRate);
		lowBound += increment;
	}
}

template <typename SampleType>
SampleType SuperSaw<SampleType>::getFrequency() const noexcept
{
	return lastFrequency;
}

template <typename SampleType>
void SuperSaw<SampleType>::setDetuneAmount (int totalPitchSpreadInCents)
{
	jassert (totalPitchSpreadInCents >= 0);
	totalSpreadCents = totalPitchSpreadInCents;
	setFrequency (lastFrequency, samplerate);
}

template <typename SampleType>
int SuperSaw<SampleType>::getPitchSpreadCents() const noexcept
{
	return totalSpreadCents;
}

template class SuperSaw<float>;
template class SuperSaw<double>;

}  // namespace lemons::dsp::osc
