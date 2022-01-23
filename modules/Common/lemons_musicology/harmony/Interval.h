#pragma once

namespace lemons::music
{

class Interval final
{
public:

	/*
	 create from
	 - num semitones
	 - quality and interval
	 */

	enum class Quality
	{
		Major,
		Minor,
		Perfect,
		Augmented,
		Diminished
	};

	[[nodiscard]] Quality getQuality() const;

	[[nodiscard]] int getNumSemitones() const { return 0; }

	// get string description

private:
	Quality quality;
};

}  // namespace lemons::music
