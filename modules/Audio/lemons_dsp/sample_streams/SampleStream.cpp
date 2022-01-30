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
 *  ======================================================================================
 */

namespace lemons::dsp
{

template <typename SampleType>
SampleStream<SampleType>::SampleStream (std::function<SampleType()>&& sampleFuncToUse)
	: sampleFunc (std::move (sampleFuncToUse))
{
}

template <typename SampleType>
SampleType SampleStream<SampleType>::getSample() const
{
	return sampleFunc();
}

template <typename SampleType>
void SampleStream<SampleType>::getSamples (SampleType* output, int numSamples) const
{
	for (auto i = 0; i < numSamples; ++i)
		output[i] = sampleFunc();
}

template <typename SampleType>
void SampleStream<SampleType>::getSamples (AudioBuffer<SampleType>& output, int channel) const
{
	getSamples (output.getWritePointer (channel), output.getNumSamples());
}

template <typename SampleType>
void SampleStream<SampleType>::skipSamples (int numToSkip) const
{
	for (auto i = 0; i < numToSkip; ++i)
		sampleFunc();
}

template struct SampleStream<float>;
template struct SampleStream<double>;

}  // namespace lemons::dsp
