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

#pragma once

#include <lemons_dsp/lemons_dsp.h>

namespace lemons::dsp::psola
{

template <typename SampleType>
class Shifter;

template <typename SampleType>
class Analyzer final
{
public:
	explicit Analyzer (int minFreqHz = 60);

	void analyzeInput (const AudioBuffer<SampleType>& inputAudio);

	void analyzeInput (const SampleType* inputAudio, int numSamples);

	[[nodiscard]] int getLatencySamples() const noexcept;

	int setSamplerate (double newSamplerate);

	int setMinInputFreq (int minFreqHz);

	void releaseResources();

private:
	friend class Shifter<SampleType>;
    
    void registerShifter (Shifter<SampleType>& shifter);
    
    void deregisterShifter (Shifter<SampleType>& shifter);

	void newBlockStarting();

	[[nodiscard]] inline int latencyChanged();

	void makeWindow();

	/*-----------------------------------------------------------------------------------*/

	struct Grain final : public juce::ReferenceCountedObject
	{
		int origStartIndex { 0 };

		int grainSize { 0 };

		AudioBuffer<SampleType> samples;
	};

	[[nodiscard]] Grain& getClosestGrain (int placeInBlock);

	[[nodiscard]] Grain& getGrainToStoreIn();

	/*-----------------------------------------------------------------------------------*/

	PitchDetector<SampleType> pitchDetector;
	PeakFinder<SampleType>    peakFinder;

	float currentPeriod { 0.f };

	Array<SampleType> window;

	juce::OwnedArray<Grain> grains;

	int lastBlocksize { 0 };

	Array<Shifter<SampleType>*> shifters;
};

}  // namespace lemons::dsp::psola
