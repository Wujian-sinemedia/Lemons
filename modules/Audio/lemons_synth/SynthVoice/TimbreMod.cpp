
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

namespace lemons::dsp::synth
{
template <typename SampleType>
TimbreMod<SampleType>::TimbreMod (const typename SynthBase<SampleType>::TimbreModParams& modSource)
	: params (modSource)
{
}

template <typename SampleType>
void TimbreMod<SampleType>::prepare (int blocksize, double samplerate)
{
	// filter.prepare (samplerate, blocksize, 1);
	gain.prepare (samplerate, blocksize);
}

template <typename SampleType>
void TimbreMod<SampleType>::process (AudioBuffer<SampleType>& audio)
{
	//    if (filterToggle)
	//    {
	//        filter->setParams (filterParams);
	//        filter.process (audio, toggle);
	//    }

	gain.process (audio);
}

template <typename SampleType>
void TimbreMod<SampleType>::skipSamples (int numSamples)
{
	gain.bypassedBlock (numSamples);
}

template <typename SampleType>
void TimbreMod<SampleType>::reset()
{
	gain.reset();
}

template <typename SampleType>
void TimbreMod<SampleType>::setToggle (bool shouldModBeOn)
{
	toggle = shouldModBeOn;

	if (toggle)
		gain.setGain (params.gain);
	else
		gain.setGain (1.f);
}

template class TimbreMod<float>;
template class TimbreMod<double>;

}  // namespace lemons::dsp::synth
