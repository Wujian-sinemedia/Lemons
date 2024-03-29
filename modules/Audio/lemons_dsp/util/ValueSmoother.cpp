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

namespace lemons
{
template <typename SampleType>
void ValueSmoother<SampleType>::set (SampleType newGain, bool snapImmediately)
{
	using SV = juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Multiplicative>;

	const auto newTarget = std::max (newGain, (SampleType) 0.0001);

	if (snapImmediately)
		SV::setCurrentAndTargetValue (newTarget);
	else
		SV::setTargetValue (newTarget);
}

template struct ValueSmoother<float>;
template struct ValueSmoother<double>;

}  // namespace lemons
