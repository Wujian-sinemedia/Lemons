
#include "Interpolation.h"

namespace bav::spline
{
Knot::Knot (float xToUse, float yToUse)
    : x (xToUse), y (yToUse)
{
}

Knot& Knot::operator= (const Knot& other)
{
    x          = other.x;
    y          = other.y;
    dragStartX = other.dragStartX;
    dragStartY = other.dragStartY;
    selected   = other.selected;

    return *this;
}

void Knot::serialize (TreeReflector& ref)
{
    ref.add ("X", x);
    ref.add ("Y", y);
    ref.add ("DragStartX", dragStartX);
    ref.add ("DragStartY", dragStartY);
    ref.add ("Selected", selected);
}

void Knot::select() noexcept
{
    dragStartX = x;
    dragStartY = y;
    selected   = true;
}

void Knot::deselect()
{
    selected = false;
}

bool Knot::isSelected() const
{
    return selected;
}

bool Knot::drag (const Point& p) noexcept
{
    if (! selected) return false;
    x = dragStartX + p.x;
    y = dragStartY + p.y;
    return true;
}


Knots::Knots()
{
    add ({0, 0});
    add ({1, 1});
}

void Knots::serialize (TreeReflector& ref)
{
    ref.add ("Knot", *(static_cast< std::vector< Knot >* > (this)));
}

void Knots::add (const Point& p)
{
    push_back ({p.x, p.y});
}

void Knots::sort()
{
    struct Sorter
    {
        bool operator() (const Knot& a, const Knot& b) const noexcept { return a.x < b.x; }
    };

    std::sort (begin(), end(), Sorter());
}

void Knots::removeOffLimits()
{
    struct Remover
    {
        bool operator() (const Knot& knot) const noexcept
        {
            if (! knot.isSelected()) return false;
            return knot.x <= 0.f || knot.x >= 1.f || knot.y <= 0.f || knot.y >= 1.f;
        }
    };

    std::remove_if (begin(), end(), Remover());
}

void Knots::remove (const juce::Range< float >& range)
{
    for (auto knot = begin(); knot != end(); ++knot)
    {
        if (range.getEnd() < knot->x) break;

        if (range.getStart() < knot->x)
            erase (knot);
    }
}

void Knots::select (const juce::Range< float >& range) noexcept
{
    for (auto& knot : *this)
    {
        if (knot.x > range.getEnd()) return;

        if (knot.x >= range.getStart())
            knot.select();
    }
}

bool Knots::drag (const Point& drag) noexcept
{
    bool changed = false;

    for (auto& knot : *this)
        if (knot.drag (drag))
            changed = true;

    return changed;
}

void Knots::deselect()
{
    for (auto& knot : *this)
        knot.deselect();
}

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
        if (x >= at (static_cast< std::vector< Knot >::size_type > (kIdx)).x)
        {
            ++kIdx;
            kIdx %= size();
        }

        spline[i] = juce::jlimit (0.f, 1.f,
                                  interpolation::hermitCubic2 (*this, x, smallestDistance, kIdx - 1));
    }
}

}  // namespace bav::spline
