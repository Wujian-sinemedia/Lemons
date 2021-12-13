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
	explicit MonoStereoConverter();
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

}  // namespace lemons::dsp


/*---------------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template <typename FloatType>
struct MonoStereoConverterTests : public DspTest
{
public:
	MonoStereoConverterTests();

private:
	void runTest() final;

	AudioBuffer<FloatType> monoBuffer, stereoBuffer;

	dsp::MonoStereoConverter<FloatType> converter;
};

LEMONS_CREATE_DSP_TEST (MonoStereoConverterTests)

}  // namespace lemons::tests

#endif
