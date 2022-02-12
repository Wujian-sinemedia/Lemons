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

#pragma once

namespace lemons::music
{

class Chord final
{
public:

	explicit Chord (const std::initializer_list<int>& midiNotes);

	explicit Chord (const std::initializer_list<Pitch>& midiNotes);

	Chord (const Chord& other);

	Chord& operator= (const Chord& other);

	[[nodiscard]] int getNumPitches() const;

	[[nodiscard]] Pitch getLowestPitch() const;

	[[nodiscard]] Pitch getHighestPitch() const;

	[[nodiscard]] bool contains (const PitchClass& pitchClass) const noexcept;

	[[nodiscard]] bool contains (const Pitch& pitch) const noexcept;

	[[nodiscard]] bool contains (int midiNote) const noexcept;

	[[nodiscard]] bool fitsInScale (const scales::Scale& scale) const noexcept;

	[[nodiscard]] juce::Array<Interval> getIntervals() const;

	[[nodiscard]] juce::Array<PitchClass> getPitchClasses() const;

	[[nodiscard]] int getNumUniquePitchClasses() const;

	[[nodiscard]] Chord applyInterval (const Interval& interval, bool above);

	// isMajor, isMinor, isDiminished, isDomSeventh ...

	// get root note

	// iterator (iterate as Pitch objects)

private:

	juce::Array<Pitch> pitches;
};

}  // namespace lemons::music
