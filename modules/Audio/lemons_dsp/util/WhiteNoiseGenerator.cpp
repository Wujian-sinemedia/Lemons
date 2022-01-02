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


namespace lemons::dsp
{

template <typename SampleType>
SampleType WhiteNoiseGenerator<SampleType>::getNextSample()
{
	return rand.nextFloat() * 0.25f - 0.125f;
}

template <typename SampleType>
void WhiteNoiseGenerator<SampleType>::getSamples (SampleType* output, int numSamples)
{
	for (int i = 0; i < numSamples; ++i)
		output[i] = getNextSample();
}

template <typename SampleType>
void WhiteNoiseGenerator<SampleType>::getSamples (AudioBuffer<SampleType>& output)
{
	const auto numSamples = output.getNumSamples();

	for (int chan = 0; chan < output.getNumChannels(); ++chan)
		getSamples (output.getWritePointer (chan), numSamples);
}

template class WhiteNoiseGenerator<float>;
template class WhiteNoiseGenerator<double>;

}  // namespace lemons::dsp
