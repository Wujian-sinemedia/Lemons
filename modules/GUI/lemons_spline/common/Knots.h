#pragma once

namespace lemons::spline
{
struct Knot
{
	Knot (float xToUse = 0.f, float yToUse = 0.f);

	Knot& operator= (const Knot& other);

	void select() noexcept;
	void deselect();
	bool isSelected() const;

	bool drag (const Point& p) noexcept;

	Point getDenormalizedPoint (const juce::Rectangle<float>& bounds) const;

	Point location;
	Point dragStart;

private:
	bool selected { false };
};


struct Knots
    : std::vector<Knot>
{
	Knots();

	const Knot& getKnot (int index) const;

	void add (const Point& p);

	void sort();

	void removeOffLimits();
	void remove (const juce::Range<float>& range);

	void select (const juce::Range<float>& range) noexcept;
	void deselect();

	bool drag (const Point& drag) noexcept;

	void makeSpline (Points& spline) const;
};

}  // namespace lemons::spline
