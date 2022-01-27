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


namespace lemons::dsp::osc
{

template <typename SampleType>
void ChoosableOscillator<SampleType>::setOscType (OscType newType)
{
	type = newType;
}

template <typename SampleType>
void ChoosableOscillator<SampleType>::renderBlock (const AudioBuffer<SampleType>& input,
												   AudioBuffer<SampleType>& output, MidiBuffer& midi, bool isBypassed)
{
	output.clear();

	if (isBypassed)
	{
		for (auto* osc : oscillators)
			osc->process (input, output, midi, true);

		return;
	}

	const auto* const chosen = [this]() -> dsp::Engine<SampleType>*
	{
		switch (type)
		{
			case (OscType::Sine) : return &sine;
			case (OscType::Saw) : return &saw;
			case (OscType::Square) : return &square;
			case (OscType::Triangle) : return &triangle;
			case (OscType::SuperSaw) : return &superSaw;
			default : jassertfalse; return nullptr;
		}
	}();

	for (auto* osc : oscillators)
	{
		if (osc == chosen)
			osc->process (input, output, midi, false);
		else
			osc->process (input, output, midi, true);
	}
}

template <typename SampleType>
void ChoosableOscillator<SampleType>::prepared (int blocksize, double samplerate, int numChannels)
{
	for (auto* osc : oscillators)
		osc->prepare (samplerate, blocksize, numChannels);
}

template <typename SampleType>
void ChoosableOscillator<SampleType>::released()
{
	for (auto* osc : oscillators)
		osc->releaseResources();
}

template <typename SampleType>
void ChoosableOscillator<SampleType>::setFrequency (float freqHz)
{
	sine.setFrequency (freqHz);
	saw.setFrequency (freqHz);
	square.setFrequency (freqHz);
	triangle.setFrequency (freqHz);
	superSaw.setFrequency (freqHz);

	freq = freqHz;
}


template class ChoosableOscillator<float>;
template class ChoosableOscillator<double>;


LEMONS_CREATE_DEFAULT_ENGINE_FACTORY (ChoosableOscillator, "ChoosableOscillator")

}  // namespace lemons::dsp::osc
