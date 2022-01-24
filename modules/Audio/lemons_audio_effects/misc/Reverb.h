
#pragma once

namespace lemons::dsp::FX
{
/**
	Reverb effect class that wraps juce::Reverb with some other useful bells and whistles, like hi- and lo-pass filters for the wet signal, a built-in dry/wet mixer, etc.
	Note that this class is NOT templated, it contains functions for both float and double AudioBuffers. This is because juce::Reverb only processes in floats.
 */
class Reverb
{
public:
	/** Creates a default Reverb object. */
	Reverb();

	/** Creates a Reverb object with some specified initial parameters. */
	Reverb (float roomSizeToUse, float dampingAmountToUse, float widthToUse, int wetPcnt, int duckAmountToUse, float loCutFreq, float hiCutFreq);

	/** Prepares the reverb. */
	void prepare (int blocksize, double samplerate, int numChannels = 2);

	/** Resets the reverb's internal state to a neutral one. */
	void reset();

	/** Sets the room size of the reverb.
		@param newRoomSize The room size to use. Between 0.f and 1.f, where 0 is a small space and 1 is a large space.
	*/
	void setRoomSize (float newRoomSize);

	/** Sets the damping amount for the reverb.
		@param newDampingAmount The damping amount to use. Between 0.f and 1.f, where 0 is no damping and 1 is complete damping.
	*/
	void setDamping (float newDampingAmount);

	/** Sets the stereo width of the reverb.
		@param newWidth The width to use. Between 0.1 and 1.f, where 0 is narrow and 1 is wide.
	*/
	void setWidth (float newWidth);

	/** Sets the dry/wet ratio.
		@param wetMixPercent The percent wet signal that will be present in the output. Between 0 and 100, where 0 is completely dry and 100 is completely wet.
	*/
	void setDryWet (int wetMixPercent);

	/** Sets the ducking amount.
		@param newDuckAmount The amount of ducking to apply to the wet signal. Between 0 and 100, where 0 is no ducking and 100 is complete ducking.
	*/
	void setDuckAmount (int newDuckAmount);

	/** Sets the frequency of the lo cut filter applied to the wet signal. */
	void setLoCutFrequency (float freq);

	/** Sets the frequency of the high cut filter applied to the wet signal. */
	void setHiCutFrequency (float freq);


	/** Processes a stream of double samples with no external sidechain.
		@param input The audio to apply the reverb to.
		@param reverbLevel Pointer to which to report the reverb level. This may be null.
	*/
	void process (juce::AudioBuffer<double>& input, double* reverbLevel = nullptr);


	/** Processes a stream of float samples with no external sidechain.
		@param input The audio to apply the reverb to.
		@param reverbLevel Pointer to which to report the reverb level. This may be null.
	*/
	void process (juce::AudioBuffer<float>& input, float* reverbLevel = nullptr);


	/** Processes a stream of double samples with an external signal sidechaining the reverb signal.
		@param input The signal to which to apply reverb.
		@param compressorSidechain The signal to use as the sidechain for the wet signal's compressor.
		@param reverbLevel Pointer to which to report the reverb level. This may be null.
	*/
	void process (juce::AudioBuffer<double>& input,
				  juce::AudioBuffer<double>& compressorSidechain,
				  double*					 reverbLevel = nullptr);


	/** Processes a stream of float samples with an external signal sidechaining the reverb signal.
		@param input The signal to which to apply reverb.
		@param compressorSidechain The signal to use as the sidechain for the wet signal's compressor.
		@param reverbLevel Pointer to which to report the reverb level. This may be null.
	*/
	void process (juce::AudioBuffer<float>& input,
				  juce::AudioBuffer<float>& compressorSidechain,
				  float*					reverbLevel = nullptr);


private:
	juce::Reverb reverb;

	juce::Reverb::Parameters juceReverbParams;

	AudioBuffer<float> conversionBuffer, workingBuffer, sidechainBuffer;

	bool			  isDucking;
	Compressor<float> compressor;

	dsp::filters::MultiFilter<float> loCut, hiCut;
	float							 loCutFreq { 80.0f }, hiCutFreq { 5500.0f };

	double sampleRate = 0.0;

	SmoothedGain<float, 2> dryGain, wetGain;

	int duck_val { 0 }, dryWet_val { 100 };
};

}  // namespace lemons::dsp::FX
