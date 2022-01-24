#pragma once

namespace lemons::dsp
{

/** Base class for any object that can output a stream of audio samples, with no audio or MIDI input.
	The constructor takes a lambda that must produce the next sample value when called with no arguments.
 */
template <typename SampleType>
struct SampleStream
{
public:

	/** Creates a sample stream with a specified lambda function for producing the next sample. */
	explicit SampleStream (std::function<SampleType()>&& sampleFuncToUse);

	/** Destructor. */
	virtual ~SampleStream() = default;

	/** Returns the next output sample from the stream. */
	[[nodiscard]] SampleType getSample() const;

	/** Returns a stream of samples. */
	void getSamples (SampleType* output, int numSamples) const;

	/** Returns a stream of samples. */
	void getSamples (AudioBuffer<SampleType>& output, int channel = 0) const;

	/** Skips a number of samples in the stream. */
	void skipSamples (int numToSkip) const;

private:
	std::function<SampleType()> sampleFunc;
};

}  // namespace lemons::dsp
