
/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

namespace lemons::dsp::FX
{
template <typename SampleType>
void Saturator<SampleType>::setHardness (float newHardness)
{
	hardness = static_cast<SampleType> (newHardness);
}

template <typename SampleType>
void Saturator<SampleType>::process (juce::AudioBuffer<SampleType>& audio)
{
	for (int chan = 0; chan < audio.getNumChannels(); ++chan)
	{
		auto* audioData = audio.getWritePointer (chan);

		for (int s = 0; s < audio.getNumSamples(); ++s)
			audioData[s] = processSample (audioData[s]);
	}
}

template <typename SampleType>
inline SampleType saturation_firstPhase (SampleType inputSample, SampleType transferFuncSample)
{
	return std::max (-transferFuncSample,
					 std::min (inputSample, transferFuncSample));
}

template float	saturation_firstPhase (float, float);
template double saturation_firstPhase (double, double);

template <typename SampleType>
inline SampleType saturation_secondPhase (SampleType inputSample, SampleType transferFuncSample, SampleType phaseOneSample)
{
	return std::tanh ((inputSample - phaseOneSample) / (SampleType (1.) - transferFuncSample))
		 * (SampleType (1.) - transferFuncSample);
}

template float	saturation_secondPhase (float, float, float);
template double saturation_secondPhase (double, double, double);

template <typename SampleType>
SampleType Saturator<SampleType>::processSample (SampleType inputSample)
{
	const SampleType amplitude = 1.;

	const auto tf	  = transferFunc (amplitude);
	const auto sample = inputSample * amplitude;

	const auto p1 = saturation_firstPhase (sample, tf);

	return p1 + saturation_secondPhase (sample, tf, p1);
}

template <typename SampleType>
SampleType Saturator<SampleType>::transferFunc (SampleType amplitude)
{
	return std::pow (hardness, amplitude - SampleType (1.));
}

template <typename SampleType>
void Saturator<SampleType>::prepare (double samplerate, int blocksize)
{
}

template <typename SampleType>
void Saturator<SampleType>::bypassedBlock (int numSamples)
{
}

template <typename SampleType>
void Saturator<SampleType>::serialize (TreeReflector& ref)
{
	ref.add ("Hardness", hardness);
}

template class Saturator<float>;
template class Saturator<double>;

}  // namespace lemons::dsp::FX
