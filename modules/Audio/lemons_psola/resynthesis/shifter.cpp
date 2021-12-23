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
    : analyzer (analyzerToUse)
{
	analyzer.registerShifter (*this);
}

template <typename SampleType>
Shifter<SampleType>::~Shifter()
{
	analyzer.deregisterShifter (*this);
}

template <typename SampleType>
void Shifter<SampleType>::setPitch (int pitchHz)
{
	const auto samplerate = analyzer.samplerate;

	// Did you call Analyzer::setSamplerate() first?
	jassert (samplerate > 0 && pitchHz > 0);

	targetPeriod  = math::periodInSamples (samplerate, static_cast<float> (pitchHz));
	targetPitchHz = pitchHz;
}

template <typename SampleType>
void Shifter<SampleType>::newBlockStarting()
{
	placeInBlock = 0;
}

template <typename SampleType>
void Shifter<SampleType>::getSamples (AudioBuffer<SampleType>& output)
{
	getSamples (output.getWritePointer (0), output.getNumSamples());
}

template <typename SampleType>
void Shifter<SampleType>::getSamples (SampleType* output, int numSamples)
{
	for (int i = 0; i < numSamples; ++i)
		output[i] = getNextSample();
}

template <typename SampleType>
void Shifter<SampleType>::skipSamples (int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
        getNextSample();
}

template <typename SampleType>
SampleType Shifter<SampleType>::getNextSample()
{
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

	return *grains.add (new Grain);
}

template <typename SampleType>
void Shifter<SampleType>::samplerateChanged()
{
	if (targetPitchHz > 0)
		setPitch (targetPitchHz);
}

template <typename SampleType>
void Shifter<SampleType>::latencyChanged()
{
	const auto latency = analyzer.getLatencySamples();

	while (grains.size() < latency / 2)
		grains.add (new Grain);
}

template <typename SampleType>
void Shifter<SampleType>::releaseResources()
{
	for (auto* grain : grains)
		grain->clearGrain();

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
SampleType Shifter<SampleType>::Grain::getNextSample()
{
	jassert (analysisGrain != nullptr);
	jassert (sampleIdx < analysisGrain->getSize());

	const auto sample = analysisGrain->getSample (sampleIdx++);

	if (sampleIdx >= analysisGrain->getSize())
		clearGrain();

	return sample;
}

template <typename SampleType>
void Shifter<SampleType>::Grain::startNewGrain (AnalysisGrain& analysisGrainToUse)
{
	jassert (analysisGrainToUse.getSize() > 0);

	analysisGrain = &analysisGrainToUse;
	sampleIdx     = 0;

	analysisGrainToUse.incReferenceCount();
}

template <typename SampleType>
void Shifter<SampleType>::Grain::clearGrain()
{
	if (analysisGrain != nullptr)
	{
		analysisGrain->decReferenceCountWithoutDeleting();
		analysisGrain = nullptr;
	}

	sampleIdx = 0;
}

template <typename SampleType>
bool Shifter<SampleType>::Grain::isActive() const
{
	if (analysisGrain == nullptr)
		return false;

	return sampleIdx < analysisGrain->getSize();
}

template class Shifter<float>;
template class Shifter<double>;

}  // namespace lemons::dsp::psola


/*---------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template <typename SampleType>
PsolaTests<SampleType>::PsolaTests()
    : DspTest ("PSOLA tests")
{
}

template <typename SampleType>
void PsolaTests<SampleType>::runTest()
{
	beginTest ("PSOLA tests");

	for (const auto samplerate : getTestingSamplerates())
	{
		analyzer.releaseResources();

		const auto srSubtest = beginSubtest ("Samplerate: " + String (samplerate));

		const auto latency = analyzer.setSamplerate (samplerate);

		const auto detectorLatency = detector.setSamplerate (samplerate);

		expectEquals (detectorLatency, latency);

		origAudio.setSize (1, latency);
		shiftedAudio.setSize (1, latency);

		constexpr auto origFreq = SampleType (440);

		osc.setFrequency (origFreq, static_cast<SampleType> (samplerate));

		{
			const auto st = beginSubtest ("Shifting down");

			for (const auto ratio : { 0.75, 0.8, 0.4, 0.5 })
			{
				const auto subtest = beginSubtest ("Shifting ratio: " + String (ratio));

				const auto targetPitch = origFreq * static_cast<SampleType> (ratio);

				osc.getSamples (origAudio);

				analyzer.analyzeInput (origAudio);

				shifter.setPitch (juce::roundToInt (targetPitch));

				shifter.getSamples (shiftedAudio);

				expectWithinAbsoluteError (detector.detectPitch (shiftedAudio),
				                           static_cast<float> (targetPitch),
				                           5.f);
			}
		}

		{
			const auto st = beginSubtest ("No shifting");

			osc.getSamples (origAudio);

			analyzer.analyzeInput (origAudio);

			shifter.setPitch (juce::roundToInt (origFreq));

			shifter.getSamples (shiftedAudio);

			expectWithinAbsoluteError (detector.detectPitch (shiftedAudio),
			                           static_cast<float> (origFreq),
			                           6.f);
		}

		{
			const auto st = beginSubtest ("Shifting up");

			for (const auto ratio : { 1.3, 1.1 })
			{
				const auto subtest = beginSubtest ("Shifting ratio: " + String (ratio));

				const auto targetPitch = origFreq * static_cast<SampleType> (ratio);

				osc.getSamples (origAudio);

				analyzer.analyzeInput (origAudio);

				shifter.setPitch (juce::roundToInt (targetPitch));

				shifter.getSamples (shiftedAudio);

				expectWithinAbsoluteError (detector.detectPitch (shiftedAudio),
				                           static_cast<float> (targetPitch),
				                           17.f);
			}
		}
	}
}

}  // namespace lemons::tests

#endif
