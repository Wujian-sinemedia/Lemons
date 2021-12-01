#pragma once

namespace lemons::dsp
{

enum class StereoReductionMode
{
    leftOnly,
    rightOnly,
    mixToMono
};

template <typename SampleType>
class MonoStereoConverter final
{
public:
	MonoStereoConverter();
	virtual ~MonoStereoConverter() = default;

	void prepare (int blocksize);

	void setStereoReductionMode (StereoReductionMode newmode);
    
	StereoReductionMode getStereoReductionMode() const noexcept { return toMonoMode; }

	void convertStereoToMono (const SampleType* leftIn,
	                          const SampleType* rightIn,
	                          SampleType*       monoOut,
	                          int               numSamples);

	void convertStereoToMono (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output);

	void convertMonoToStereo (const SampleType* monoIn,
	                          SampleType*       leftOut,
	                          SampleType*       rightOut,
	                          int               numSamples);

	void convertMonoToStereo (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output);


private:
	StereoReductionMode toMonoMode;

	AudioBuffer<SampleType> monoStorage;
};

}  // namespace lemons::dsp::FX
