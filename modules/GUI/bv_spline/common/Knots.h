#pragma once

#include <juce_graphics/juce_graphics.h>

namespace bav::spline
{
struct Knot
{
    Knot (float xToUse = 0.f, float yToUse = 0.f);
    Knot (const juce::Point< float >& p);

    Knot operator- (const Knot& other) const noexcept;
    Knot operator+ (const Knot& other) const noexcept;
    Knot operator* (const Knot& other) const noexcept;

    template < typename T >
    Knot operator/ (T v) const noexcept
    {
        return {static_cast< float > (static_cast< T > (x) / v),
                static_cast< float > (static_cast< T > (y) / v)};
    }

    template < typename T >
    Knot operator* (T v) const noexcept
    {
        return {static_cast< float > (static_cast< T > (x) * v),
                static_cast< float > (static_cast< T > (y) * v)};
    }

    void select() noexcept;

    bool drag (juce::Point< float > p) noexcept;

    float x, y, dragStartX, dragStartY;
    bool  selected;
};


struct Knots
{
    Knots();

    const Knot&  operator[] (int k) const noexcept;
    Knot&        operator[] (int k) noexcept;
    void         operator+= (const juce::Point< float >& p);
    const size_t size() const noexcept;
    void         sort();

    bool removeOffLimits();
    bool remove (const juce::Range< float >& range);
    void clear() noexcept;

    void select (const juce::Range< float >& range) noexcept;
    void deselect();

    bool drag (const juce::Point< float >& drag) noexcept;

    void makeSpline (std::vector< float >& spline) const;

    std::vector< Knot > knots;
};

}  // namespace bav::spline
