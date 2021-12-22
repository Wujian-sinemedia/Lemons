
namespace lemons::dsp::psola
{

template <typename SampleType>
Shifter<SampleType>::Shifter (Analyzer<SampleType>& analyzerToUse)
    : analyzer (analyzerToUse)
{
	analyzer.registerShifter (this);
}

template <typename SampleType>
void Shifter<SampleType>::setPitch (int pitchHz, double samplerate)
{
	jassert (samplerate > 0 && pitchHz > 0);
	targetPeriod = math::periodInSamples (samplerate, static_cast<float> (pitchHz));
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
SampleType Shifter<SampleType>::getNextSample()
{
	jassert (targetPeriod > 0.f);

	if (samplesToNextGrain == 0)
	{
		startNewGrain();
		samplesToNextGrain = juce::roundToInt (targetPeriod);
	}

	SampleType sample { 0 };

	for (auto& grain : grains)
		sample += grain.getNextSample();

	--samplesToNextGrain;
	++placeInBlock;

	return sample;
}

template <typename SampleType>
void Shifter<SampleType>::startNewGrain()
{
	if (const auto* analysisGrain = analyzer.getClosestGrain (placeInBlock))
	{
		Grain newGrain { *analysisGrain, 0 };

		grains.add (newGrain);
	}
	else
	{
		jassertfalse;
	}
}

template <typename SampleType>
void Shifter<SampleType>::releaseResources()
{
	grains.clearQuick();
	samplesToNextGrain = 0;
	targetPeriod       = 0.f;
	placeInBlock       = 0;
}


template <typename SampleType>
Shifter<SampleType>::Grain::Grain (const AnalysisGrain& analysisGrainToUse, int numSilentSamplesFirst)
    : analysisGrain (analysisGrainToUse)
    , zeroesLeft (numSilentSamplesFirst)
{
}

template <typename SampleType>
SampleType Shifter<SampleType>::Grain::getNextSample()
{
	if (zeroesLeft > 0)
	{
		--zeroesLeft;
		return SampleType (0);
	}

	return analysisGrain.samples.getSample (0, sampleIdx++);
}

// template <typename SampleType>
// bool Shifter<SampleType>::Grain::isActive() const
//{
//     if (zeroesLeft > 0)
//         return true;
//
//     return sampleIdx < analysisGrain.grainSize;
// }

template class Shifter<float>;
template class Shifter<double>;

}  // namespace lemons::dsp::psola
