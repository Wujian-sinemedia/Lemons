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


namespace lemons::dsp::osc
{
/** An enum class that represents the kinds of oscillator that ChoosableOscillator encapsulates.
    @see ChoosableOscillator
 */
enum class OscType
{
	Sine,
	Saw,
	Square,
	Triangle
};


/** A class that encapsulates together all the kinds of oscillators, and allows you to seamlessly switch between them during playback.
    @see OscType, OscEngine, Oscillator
 */
template <typename SampleType>
class ChoosableOscillator final
{
public:
    
    explicit ChoosableOscillator() = default;
    
	/** Sets the type of oscillator that will be used to generate the output.
	    @see OscType
	 */
	void setOscType (OscType newType);

	/** Returns the type of oscillator that is currently chosen. */
	OscType getOscType() const noexcept { return type; }

	/** Sets the oscillator's frequency. */
	void setFrequency (float freqHz);

	/** Returns the frequency of the oscillator. */
	float getFrequency() const noexcept { return freq; }

	/** Processes a stream of audio. */
	void process (AudioBuffer<SampleType>& output);

	/** Prepares the choosable oscillator for playback. */
	void prepare (int blocksize, double samplerate);


private:
	/** If you're implementing a subclass of ChoosableOscillator, this will be called after the main call to prepare() is made. */
	virtual void prepared (int /*blocksize*/) { }

	OscType type { OscType::Sine };
	float   freq { 440.f };

	Engine<SampleType, Sine>     sine;
	Engine<SampleType, Saw>      saw;
	Engine<SampleType, Square>   square;
	Engine<SampleType, Triangle> triangle;
};

}  // namespace lemons::dsp::osc
