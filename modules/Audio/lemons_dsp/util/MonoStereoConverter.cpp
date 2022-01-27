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
void MonoStereoConverter<SampleType>::prepare (int blocksize)
{
	monoStorage.setSize (1, blocksize, true, true, true);
}


template <typename SampleType>
void MonoStereoConverter<SampleType>::setStereoReductionMode (StereoReductionMode newmode)
{
	toMonoMode = newmode;
}


template <typename SampleType>
void MonoStereoConverter<SampleType>::convertStereoToMono (const SampleType* leftIn,
														   const SampleType* rightIn,
														   SampleType*		 monoOut,
														   int				 numSamples)
{
	using FVO = juce::FloatVectorOperations;

	switch (toMonoMode)
	{
		case (StereoReductionMode::leftOnly) :
		{
			FVO::copy (monoOut, leftIn, numSamples);
			return;
		}
		case (StereoReductionMode::rightOnly) :
		{
			FVO::copy (monoOut, rightIn, numSamples);
			return;
		}
		case (StereoReductionMode::mixToMono) :
		{
			monoStorage.copyFrom (0, 0, leftIn, numSamples, SampleType (0.5));
			monoStorage.addFrom (0, 0, rightIn, numSamples, SampleType (0.5));
			FVO::copy (monoOut, monoStorage.getReadPointer (0), numSamples);
			return;
		}
		default : jassertfalse;
	}
}

template <typename SampleType>
void MonoStereoConverter<SampleType>::convertStereoToMono (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output)
{
	jassert (input.getNumSamples() == output.getNumSamples());
	jassert (input.getNumChannels() == 2);
	jassert (output.getNumChannels() == 1);

	convertStereoToMono (input.getReadPointer (0),
						 input.getReadPointer (1),
						 output.getWritePointer (0),
						 output.getNumSamples());
}


template <typename SampleType>
void MonoStereoConverter<SampleType>::convertMonoToStereo (const SampleType* monoIn,
														   SampleType*		 leftOut,
														   SampleType*		 rightOut,
														   int				 numSamples)
{
	using FVO = juce::FloatVectorOperations;

	FVO::copy (leftOut, monoIn, numSamples);
	FVO::copy (rightOut, monoIn, numSamples);
}

template <typename SampleType>
void MonoStereoConverter<SampleType>::convertMonoToStereo (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output)
{
	jassert (input.getNumSamples() == output.getNumSamples());
	jassert (input.getNumChannels() == 1);
	jassert (output.getNumChannels() == 2);

	convertMonoToStereo (input.getReadPointer (0),
						 output.getWritePointer (0),
						 output.getWritePointer (1),
						 output.getNumSamples());
}

template class MonoStereoConverter<float>;
template class MonoStereoConverter<double>;

}  // namespace lemons::dsp
