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
	Triangle,
	SuperSaw
};


/** A class that encapsulates together all the kinds of oscillators, and allows you to seamlessly switch between them during playback without pops or clicks.
    @see OscType, OscEngine, Oscillator
 */
template <typename SampleType>
class ChoosableOscillator final : public dsp::Engine<SampleType>
{
public:

	/** Sets the type of oscillator that will be used to generate the output. */
	void setOscType (OscType newType);

	/** Returns the type of oscillator that is currently chosen. */
	[[nodiscard]] OscType getOscType() const noexcept { return type; }

	/** Sets the oscillator's frequency. */
	void setFrequency (float freqHz);

	/** Returns the frequency of the oscillator. */
	[[nodiscard]] float getFrequency() const noexcept { return freq; }

private:
	void renderBlock (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, bool isBypassed) final;

	void prepared (int blocksize, double samplerate, int numChannels) final;

	void released() final;

	OscType type { OscType::Sine };
	float   freq { 440.f };

	Engine<SampleType, Sine>     sine;
	Engine<SampleType, Saw>      saw;
	Engine<SampleType, Square>   square;
	Engine<SampleType, Triangle> triangle;
	Engine<SampleType, SuperSaw> superSaw;

	const std::array<dsp::Engine<SampleType>*, 5> oscillators { &sine, &saw, &square, &triangle, &superSaw };
};

}  // namespace lemons::dsp::osc
