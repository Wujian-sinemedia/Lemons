#include <lemons_core/lemons_core.h>

namespace lemons
{

constexpr TimeSignature::TimeSignature (int numBeats, int beatKind)
: numerator(numBeats), denominator(beatKind)
{ }

constexpr bool TimeSignature::operator== (const TimeSignature& other) const
{
    return numerator == other.numerator && denominator == other.denominator;
}

constexpr bool TimeSignature::operator!= (const TimeSignature& other) const
{
    return ! (*this == other);
}

constexpr bool TimeSignature::isDupleMeter() const
{
    return math::numberIsEven (numerator);
}

constexpr bool TimeSignature::isTripleMeter() const
{
    return math::isDivisibleBy (numerator, 3);
}

}

