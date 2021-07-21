#pragma once

namespace bav::spline
{
struct Knot : SerializableData
{
    Knot (float xToUse = 0.f, float yToUse = 0.f);

    Knot& operator= (const Knot& other);

    void select() noexcept;
    void deselect();
    bool isSelected() const;

    bool drag (const Point& p) noexcept;

    float x, y;
    float dragStartX {0.f};
    float dragStartY {0.f};

private:
    void serialize (TreeReflector& ref) final;

    bool selected {false};
};


struct Knots : SerializableData, std::vector< Knot >
{
    Knots();

    void add (const Point& p);

    void sort();

    void removeOffLimits();
    void remove (const juce::Range< float >& range);

    void select (const juce::Range< float >& range) noexcept;
    void deselect();

    bool drag (const Point& drag) noexcept;

    void makeSpline (std::vector< float >& spline) const;

private:
    void serialize (TreeReflector& ref) final;
};

}  // namespace bav::spline
