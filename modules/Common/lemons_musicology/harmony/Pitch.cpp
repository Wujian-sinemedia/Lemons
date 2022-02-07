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

constexpr Pitch::Pitch (const Pitch& other) noexcept
	: midiPitch (other.midiPitch)
{
}

constexpr bool Pitch::operator== (const Pitch& other) const noexcept
{
	return midiPitch == other.midiPitch;
}

constexpr bool Pitch::operator!= (const Pitch& other) const noexcept
{
	return ! (*this == other);
}

bool Pitch::approximatelyEqual (const Pitch& other) const noexcept
{
	return juce::roundToInt (midiPitch) == juce::roundToInt (other.midiPitch);
}

constexpr bool Pitch::operator> (const Pitch& other) const noexcept
{
	return midiPitch > other.midiPitch;
}

constexpr bool Pitch::operator< (const Pitch& other) const noexcept
{
	return midiPitch < other.midiPitch;
}

double Pitch::getFreqHz() const noexcept
{
	return math::midiToFreq (midiPitch);
}

int Pitch::getRoundedFreqHz() const noexcept
{
	return juce::roundToInt (getFreqHz());
}

constexpr double Pitch::getMidiPitch() const noexcept
{
	return midiPitch;
}

int Pitch::getRoundedMidiPitch() const noexcept
{
	return juce::roundToInt (midiPitch);
}

int Pitch::getPitchClass() const noexcept
{
	return juce::roundToInt (midiPitch) % 11;
}

int Pitch::getOctaveNumber() const noexcept
{
	return juce::roundToInt (midiPitch) / 12 - 1;
}

bool Pitch::isBlackKey() const noexcept
{
	const auto pitchClass = getPitchClass();

	return pitchClass == 1 || pitchClass == 3 || pitchClass == 6 || pitchClass == 8 || pitchClass == 10;
}

bool Pitch::isWhiteKey() const noexcept
{
	return ! isBlackKey();
}

String Pitch::toString() const noexcept
{
	return pitchToString (juce::roundToInt (midiPitch));
}

}  // namespace lemons::music
