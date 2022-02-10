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

constexpr CompoundInterval::CompoundInterval (int octaves, const Interval& simpleInterval) noexcept
	: interval (simpleInterval), numOctaves (octaves)
{
	if (interval.getKind() == 8)
	{
		const auto quality = interval.getQuality();

		if (quality != Interval::Quality::Diminished)
		{
			++numOctaves;
			interval = Interval { 0, quality };
		}
	}
}

constexpr CompoundInterval::CompoundInterval (int kind, Interval::Quality quality) noexcept
{
	if (kind <= 8)
	{
		numOctaves = 0;
		interval   = Interval { kind, quality };
		return;
	}

	numOctaves = (kind - 1) / 7;

	const auto intervalKind = kind - (7 * numOctaves);

	if (intervalKind <= 0)
		interval = Interval();
	else
		interval = Interval { intervalKind, quality };
}

constexpr CompoundInterval::CompoundInterval (int numSemitones) noexcept
{
	numOctaves = numSemitones / 12;
	interval   = Interval::fromNumSemitones (numSemitones - (numOctaves * 12));
}

constexpr CompoundInterval::CompoundInterval (const CompoundInterval& other) noexcept
	: interval (other.interval), numOctaves (other.numOctaves)
{
}

constexpr CompoundInterval& CompoundInterval::operator= (const CompoundInterval& other) noexcept
{
	interval   = other.interval;
	numOctaves = other.numOctaves;
	return *this;
}

constexpr CompoundInterval CompoundInterval::fromPitches (int midiPitch1, int midiPitch2) noexcept
{
	return CompoundInterval { math::abs (midiPitch2 - midiPitch1) };
}

constexpr bool CompoundInterval::operator== (const CompoundInterval& other) const noexcept
{
	return numOctaves == other.numOctaves && interval == other.interval;
}

constexpr bool CompoundInterval::isEnharmonicTo (const CompoundInterval& other) const noexcept
{
	return getNumSemitones() == other.getNumSemitones();
}

constexpr bool CompoundInterval::operator!= (const CompoundInterval& other) const noexcept
{
	return ! (*this == other);
}

constexpr bool CompoundInterval::operator> (const CompoundInterval& other) const noexcept
{
	if (numOctaves < other.numOctaves)
		return false;

	return interval > other.interval;
}

constexpr bool CompoundInterval::operator< (const CompoundInterval& other) const noexcept
{
	if (numOctaves > other.numOctaves)
		return false;

	return interval < other.interval;
}

constexpr int CompoundInterval::getNumOctaves() const noexcept
{
	return numOctaves;
}

constexpr Interval CompoundInterval::getSimpleInterval() const noexcept
{
	return interval;
}

constexpr int CompoundInterval::getNumSemitones() const noexcept
{
	return interval.getNumSemitones() + (numOctaves * 12);
}

constexpr bool CompoundInterval::hasSameSimpleInterval (const CompoundInterval& other) const noexcept
{
	return interval == other.interval;
}

constexpr Interval::Quality CompoundInterval::getQuality() const noexcept
{
	return interval.getQuality();
}

constexpr int CompoundInterval::getKind() const noexcept
{
	const auto simple = [kind = interval.getKind()]
	{
		if (kind > 0)
			return kind - 1;

		return kind;
	}();

	return (numOctaves * 7) + simple + 1;
}

constexpr int CompoundInterval::getCents_EqualTemperament() const noexcept
{
	return getNumSemitones() * 100;
}

}  // namespace lemons::music
