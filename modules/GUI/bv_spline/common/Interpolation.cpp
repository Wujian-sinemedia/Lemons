
namespace bav::spline::interpolation
{
Point lin (const Point& a, const Point& b, float x) noexcept
{
    return {
        a.x + x * (b.x - a.x),
        a.y + x * (b.y - a.y)};
}

inline auto nurbs_to_size_k (int idx)
{
    return static_cast< std::vector< Point >::size_type > (idx);
}

inline float nurbs_CalcPoint (float t, float val1, float val2, float val3)
{
    return std::pow (1.f - t, 2.f) * val1 + (1.f - t) * 2.f * t * val2 + t * t * val3;
}

Point nurbs (const std::vector< Point >& knots, float t, const int* k) noexcept
{
    const auto& knot0 = knots[nurbs_to_size_k (k[0])];
    const auto& knot1 = knots[nurbs_to_size_k (k[1])];
    const auto& knot2 = knots[nurbs_to_size_k (k[2])];

    return {
        nurbs_CalcPoint (t, knot0.x, knot1.x, knot2.x),
        nurbs_CalcPoint (t, knot0.y, knot1.y, knot2.y)};
}

float hermitCubic (const Knots& data, float x) noexcept
{
    const auto size = data.size();
    const auto i1   = juce::roundToInt (x);

    auto i0 = i1 - 1;
    while (i0 < 0)
        i0 += size;

    const auto i2 = (i1 + 1) % size;
    const auto i3 = (i1 + 2) % size;

    const auto c1 = .5f * (data[i2].y - data[i0].y);
    const auto c2 = data[i0].y - 2.5f * data[i1].y + 2.f * data[i2].y - .5f * data[i3].y;
    const auto c3 = 1.5f * (data[i1].y - data[i2].y) + .5f * (data[i3].y - data[i0].y);

    const auto frac = x - i1;

    return ((c3 * frac + c2) * frac + c1) * frac + data[i1].y;
}

inline int hermitCubic2_ClipValToSize (int val, int size)
{
    if (val >= size) return size - 1;
    return val;
}

inline float hermitCubic2_StepOne (const Knot& knot1, const Knot& knot2, float x21)
{
    return (knot1.y - knot2.y) / (knot1.x - knot2.x) * x21;
}

float hermitCubic2 (const Knots& data, float x, float tolerance, int i1) noexcept
{
    const auto size = data.size();

    const auto i0 = std::max (0, i1 - 1);
    const auto i2 = hermitCubic2_ClipValToSize (i1 + 1, size);
    const auto i3 = hermitCubic2_ClipValToSize (i1 + 2, size);

    const auto& knoti0 = data[i0];
    const auto& knoti1 = data[i1];
    const auto& knoti2 = data[i2];
    const auto& knoti3 = data[i3];

    const auto x21 = knoti2.x - knoti1.x;

    if (x21 < tolerance)
        return knoti2.y;

    const auto m0 = hermitCubic2_StepOne (knoti2, knoti0, x21);
    const auto m1 = hermitCubic2_StepOne (knoti3, knoti1, x21);

    const auto a = 2.f * (knoti1.y - knoti2.y) + m0 + m1;
    const auto b = 3.f * (knoti2.y - knoti1.y) - 2.f * m0 - m1;

    const auto t = (x - knoti1.x) / x21;
    return knoti1.y + t * (m0 + t * (b + t * a));
}

}  // namespace bav::spline::interpolation
