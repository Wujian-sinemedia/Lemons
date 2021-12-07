#pragma once


namespace lemons::music
{

class TimeSignature final
{
public:
	constexpr TimeSignature (int numBeats = 4, int beatKind = 4);

	constexpr bool operator== (const TimeSignature& other) const;
	constexpr bool operator!= (const TimeSignature& other) const;

	[[nodiscard]] constexpr bool isDupleMeter() const;
	[[nodiscard]] constexpr bool isTripleMeter() const;

private:
	int numerator { 4 }, denominator { 4 };
};

}  // namespace lemons::music
