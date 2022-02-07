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

/*
 - pentatonic
 - octatonic
 - whole tone
 - chromatic
 - neapolitan
 - blues scale
 */

struct Scale
{
	constexpr Scale() = default;

	virtual ~Scale() = default;

	[[nodiscard]] bool operator== (const Scale& other) const;
	[[nodiscard]] bool operator!= (const Scale& other) const;

	[[nodiscard]] virtual juce::Array<int> getIntervalsAsSemitones() const = 0;

	[[nodiscard]] juce::Array<Interval> getIntervals() const;

	[[nodiscard]] bool containsPitchClass (int pitchClass) const;

	[[nodiscard]] bool containsPitch (const Pitch& pitch) const;

	[[nodiscard]] bool containsPitch (int midiNoteNumber) const;

	[[nodiscard]] juce::Array<int> getPitchClasses() const;

	[[nodiscard]] juce::Array<Pitch> getPitches (int octaveNumber) const;

	[[nodiscard]] juce::Array<Pitch> getPitches (int lowestMidiNote, int highestMidiNote) const;

	[[nodiscard]] virtual int getNumSharps() const noexcept = 0;

	[[nodiscard]] virtual int getNumFlats() const noexcept = 0;

	[[nodiscard]] virtual int getPitchClassOfRoot() const noexcept = 0;

	[[nodiscard]] Pitch getRoot (int octaveNumber) const noexcept;

	[[nodiscard]] virtual String getStringDescription() const = 0;

	[[nodiscard]] virtual int notesPerOctave() const noexcept = 0;

	[[nodiscard]] virtual String getRootAsString() const noexcept;
};

}  // namespace lemons::music
