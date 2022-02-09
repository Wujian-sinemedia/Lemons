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

Pitch::Pitch (const String& pitchString) noexcept
	: midiPitch (static_cast<double> (stringToPitch (pitchString)))
{
}

bool Pitch::approximatelyEqual (const Pitch& other) const noexcept
{
	return juce::roundToInt (midiPitch) == juce::roundToInt (other.midiPitch);
}

double Pitch::getFreqHz() const noexcept
{
	return math::midiToFreq (midiPitch);
}

int Pitch::getRoundedFreqHz() const noexcept
{
	return juce::roundToInt (getFreqHz());
}

int Pitch::getRoundedMidiPitch() const noexcept
{
	return juce::roundToInt (midiPitch);
}

PitchClass Pitch::getPitchClass() const noexcept
{
	return PitchClass { juce::roundToInt (midiPitch) };
}

int Pitch::getOctaveNumber() const noexcept
{
	return octaveNumberOfMidiNote (juce::roundToInt (midiPitch));
}

bool Pitch::isBlackKey() const noexcept
{
	return getPitchClass().isBlackKey();
}

bool Pitch::isWhiteKey() const noexcept
{
	return ! isBlackKey();
}

String Pitch::toString (bool asSharps) const noexcept
{
	return pitchToString (juce::roundToInt (midiPitch), asSharps);
}

}  // namespace lemons::music
