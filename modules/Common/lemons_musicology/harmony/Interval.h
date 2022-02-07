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

class Interval final
{
public:

	enum class Quality
	{
		Diminished,
		Minor,
		Major,
		Augmented,
		Perfect
	};

	constexpr explicit Interval (int kindToUse, Quality qualityToUse) noexcept;

	[[nodiscard]] static constexpr Interval fromNumSemitones (int semitones) noexcept;

	[[nodiscard]] static Interval fromPitches (const Pitch& pitch1, const Pitch& pitch2) noexcept;

	[[nodiscard]] static constexpr Interval fromPitches (int midiPitch1, int midiPitch2) noexcept;

	constexpr Interval (const Interval& other) noexcept;

	[[nodiscard]] static Interval fromStringDescription (const String& string);

	[[nodiscard]] constexpr bool operator== (const Interval& other) const noexcept;

	[[nodiscard]] constexpr bool isEnharmonicTo (const Interval& other) const noexcept;

	[[nodiscard]] constexpr bool operator!= (const Interval& other) const noexcept;

	[[nodiscard]] constexpr bool operator> (const Interval& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const Interval& other) const noexcept;

	[[nodiscard]] constexpr Interval operator+ (const Interval& other) const noexcept;
	[[nodiscard]] constexpr Interval operator- (const Interval& other) const noexcept;

	[[nodiscard]] constexpr Interval operator+ (int semitonesToAdd) const noexcept;
	[[nodiscard]] constexpr Interval operator- (int semitonesToSubtract) const noexcept;

	constexpr Interval& operator++() noexcept;
	constexpr Interval& operator--() noexcept;

	[[nodiscard]] constexpr int getNumSemitones() const noexcept;

	[[nodiscard]] constexpr int getKind() const noexcept;

	[[nodiscard]] String getKindAsString() const;

	[[nodiscard]] constexpr Quality getQuality() const noexcept;

	[[nodiscard]] String getQualityAsString (bool useShort = false) const;

	[[nodiscard]] String getStringDescription (bool useShort = false) const;

	[[nodiscard]] constexpr int getCents_EqualTemperament() const noexcept;

	[[nodiscard]] juce::Array<double> getCents_JustIntonation() const;

private:

	[[nodiscard]] constexpr bool intervalIsPerfectKind() noexcept;

	Quality quality { Quality::Major };

	int kind { 0 };
};

}  // namespace lemons::music
