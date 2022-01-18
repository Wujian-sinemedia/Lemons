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

namespace lemons::dsp::psola
{

template <typename SampleType>
Shifter<SampleType>::Shifter (Analyzer<SampleType>& analyzerToUse)
    : SampleStream<SampleType> ([this]()
                                { return getNextSample(); })
    , analyzer (analyzerToUse)
{
	analyzer.registerShifter (*this);
}

template <typename SampleType>
Shifter<SampleType>::~Shifter()
{
	analyzer.deregisterShifter (*this);
}

template <typename SampleType>
void Shifter<SampleType>::setPitchHz (int pitchHz) noexcept
{
	// Did you call Analyzer::setSamplerate() first?
	jassert (analyzer.samplerate > 0 && pitchHz > 0);

	targetPeriod  = math::periodInSamples (analyzer.samplerate, static_cast<decltype (targetPeriod)> (pitchHz));
	targetPitchHz = pitchHz;

    if (const auto intTargetPeriod = juce::roundToInt (targetPeriod);
        samplesToNextGrain > intTargetPeriod)
	{
		// ???
		samplesToNextGrain = intTargetPeriod;
	}

	pitchHzChanged (pitchHz);
}

template <typename SampleType>
float Shifter<SampleType>::getPitchHz() const noexcept
{
	// Did you call Analyzer::setSamplerate() first?
	jassert (analyzer.samplerate > 0);

	return math::freqFromPeriod (analyzer.samplerate, static_cast<float> (targetPeriod));
}

template <typename SampleType>
void Shifter<SampleType>::newBlockStarting() noexcept
{
	placeInBlock = 0;
	onNewBlock();
}

template <typename SampleType>
SampleType Shifter<SampleType>::getNextSample()
{
	// did you call setPitch() first?
	jassert (targetPeriod > 0.f);

	if (samplesToNextGrain == 0)
	{
		getGrainToStart().startNewGrain (analyzer.getClosestGrain (placeInBlock));
		samplesToNextGrain = juce::roundToInt (targetPeriod);
	}

	SampleType sample { 0 };

	for (auto* grain : grains)
		if (grain->isActive())
			sample += grain->getNextSample();

	--samplesToNextGrain;
	++placeInBlock;

	return sample;
}

template <typename SampleType>
typename Shifter<SampleType>::Grain& Shifter<SampleType>::getGrainToStart()
{
	for (auto* grain : grains)
		if (! grain->isActive())
			return *grain;
    
    jassertfalse;

	return *grains.add (new Grain);
}

template <typename SampleType>
void Shifter<SampleType>::samplerateChanged() noexcept
{
	if (targetPitchHz > 0)
		setPitchHz (targetPitchHz);
}

template <typename SampleType>
void Shifter<SampleType>::latencyChanged (int newLatency)
{
	while (grains.size() < newLatency / 2)
		grains.add (new Grain);
}

template <typename SampleType>
void Shifter<SampleType>::reset() noexcept
{
	for (auto* grain : grains)
		grain->clearGrain();

	samplesToNextGrain = 0;
	targetPeriod       = 0.f;
	placeInBlock       = 0;
}

template <typename SampleType>
void Shifter<SampleType>::releaseResources()
{
	grains.clear();

	samplesToNextGrain = 0;
	targetPeriod       = 0.f;
	placeInBlock       = 0;
}

/*---------------------------------------------------------------------------------------------------------------------------*/

template <typename SampleType>
Shifter<SampleType>::Grain::~Grain()
{
	if (analysisGrain != nullptr)
		analysisGrain->decReferenceCountWithoutDeleting();
}

template <typename SampleType>
SampleType Shifter<SampleType>::Grain::getNextSample() noexcept
{
	jassert (analysisGrain != nullptr);

	const auto sample = analysisGrain->getSample (sampleIdx++);

	if (sampleIdx >= analysisGrain->getSize())
		clearGrain();

	return sample;
}

template <typename SampleType>
void Shifter<SampleType>::Grain::startNewGrain (AnalysisGrain& analysisGrainToUse) noexcept
{
	jassert (analysisGrain == nullptr);
	jassert (analysisGrainToUse.getSize() > 0);

	analysisGrain = &analysisGrainToUse;
	sampleIdx     = 0;

	analysisGrainToUse.incReferenceCount();
}

template <typename SampleType>
void Shifter<SampleType>::Grain::clearGrain() noexcept
{
	if (analysisGrain != nullptr)
	{
		analysisGrain->decReferenceCountWithoutDeleting();
		analysisGrain = nullptr;
	}

	sampleIdx = 0;
}

template <typename SampleType>
bool Shifter<SampleType>::Grain::isActive() const noexcept
{
	return analysisGrain != nullptr;
}

template class Shifter<float>;
template class Shifter<double>;

}  // namespace lemons::dsp::psola
