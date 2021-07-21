
#include "Interpolation.h"

namespace bav::spline
{
Knot::Knot (float xToUse, float yToUse)
    : x (xToUse), y (yToUse),
      dragStartX (0), dragStartY (0),
      selected (false)
{
}

Knot::Knot (const juce::Point< float >& p)
    : x (p.x), y (p.y),
      dragStartX (0), dragStartY (0),
      selected (false)
{
}

Knot Knot::operator- (const Knot& other) const noexcept
{
    return {x - other.x, y - other.y};
}

Knot Knot::operator+ (const Knot& other) const noexcept
{
    return {x + other.x, y + other.y};
}

Knot Knot::operator* (const Knot& other) const noexcept
{
    return {x * other.x, y * other.y};
}

void Knot::select() noexcept
{
    dragStartX = x;
    dragStartY = y;
    selected   = true;
}

bool Knot::drag (juce::Point< float > p) noexcept
{
    if (! selected) return false;
    x = dragStartX + p.x;
    y = dragStartY + p.y;
    return true;
}


Knots::Knots()
    : knots ({
        {0, 0},
        {1, 1},
    })
{
}

const Knot& Knots::operator[] (int k) const noexcept { return knots[static_cast< size_t > (k)]; }

Knot& Knots::operator[] (int k) noexcept { return knots[static_cast< size_t > (k)]; }

void Knots::operator+= (const juce::Point< float >& p) { knots.push_back (p); }

const size_t Knots::size() const noexcept { return knots.size(); }

void Knots::sort()
{
    struct Sorter
    {
        bool operator() (const Knot& a, const Knot& b) const noexcept { return a.x < b.x; }
    };

    std::sort (knots.begin(), knots.end(), Sorter());
}

bool Knots::removeOffLimits()
{
    const auto sizeBefore = size();

    struct Remover
    {
        bool operator() (const Knot& knot) const noexcept
        {
            if (! knot.selected) return false;
            return knot.x <= 0.f || knot.x >= 1.f || knot.y <= 0.f || knot.y >= 1.f;
        }
    };

    std::remove_if (knots.begin(), knots.end(), Remover());

    return size() != sizeBefore;
}

bool Knots::remove (const juce::Range< float >& range)
{
    bool changed = false;

    for (auto knot = knots.begin(); knot != knots.end(); ++knot)
    {
        if (range.getEnd() < knot->x) break;

        if (range.getStart() < knot->x)
        {
            knots.erase (knot);
            changed = true;
        }
    }

    return changed;
}

void Knots::select (const juce::Range< float >& range) noexcept
{
    for (auto& knot : knots)
    {
        if (knot.x > range.getEnd()) return;

        if (knot.x >= range.getStart())
            knot.select();
    }
}

bool Knots::drag (const juce::Point< float >& drag) noexcept
{
    bool changed = false;

    for (auto& knot : knots)
        if (knot.drag (drag))
            changed = true;

    return changed;
}

void Knots::deselect()
{
    for (auto& knot : knots)
        knot.selected = false;
}

void Knots::clear() noexcept { knots.clear(); }

void Knots::makeSpline (std::vector< float >& spline) const
{
    auto       x                = 0.f;
    const auto inc              = 1.f / static_cast< float > (spline.size());
    const auto smallestDistance = inc * 2.f;
    int        kIdx             = 1;

    for (size_t i = 0;
         i < spline.size();
         ++i, x += inc)
    {
        if (x >= knots[static_cast< std::vector< Knot >::size_type > (kIdx)].x)
        {
            ++kIdx;
            kIdx %= knots.size();
        }

        spline[i] = juce::jlimit (0.f, 1.f,
                                  interpolation::hermitCubic2 (*this, x, smallestDistance, kIdx - 1));
    }
}

}  // namespace bav::spline
