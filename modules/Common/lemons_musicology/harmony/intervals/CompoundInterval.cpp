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

namespace lemons::music
{

juce::Array<double> CompoundInterval::getCents_JustIntonation() const
{
	const auto octaveCents = static_cast<double> (numOctaves) * 1200.;

	auto array = interval.getCents_JustIntonation();

	for (auto i = 0; i < array.size(); ++i)
		array.getReference (i) += octaveCents;

	return array;
}

CompoundInterval CompoundInterval::fromPitches (const Pitch& pitch1, const Pitch& pitch2) noexcept
{
	return CompoundInterval { std::abs (pitch2.getRoundedMidiPitch() - pitch1.getRoundedMidiPitch()) };
}

String CompoundInterval::getStringDescription (bool useShort) const
{
	if (useShort)
		return interval.getQualityAsString (true) + String (getKind());

	return interval.getQualityAsString (false) + " " + String (getKind());
}

Pitch CompoundInterval::applyToPitch (const Pitch& other, bool above) const noexcept
{
	const auto semitones = getNumSemitones();
	const auto starting	 = other.getRoundedMidiPitch();

	if (above)
		return Pitch { starting + semitones };

	return Pitch { starting - semitones };
}

/*-------------------------------------------------------------------------------------------------------------------*/

Pitch operator+ (const Pitch& pitch, const CompoundInterval& interval) noexcept
{
	return Pitch { pitch.getRoundedMidiPitch() + interval.getNumSemitones() };
}

Pitch operator- (const Pitch& pitch, const CompoundInterval& interval) noexcept
{
	return Pitch { pitch.getRoundedMidiPitch() - interval.getNumSemitones() };
}

}  // namespace lemons::music
