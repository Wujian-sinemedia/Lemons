
namespace bav::spline::interpolation
{
juce::Point< float > lin (const juce::Point< float >& a, const juce::Point< float >& b, float x) noexcept
{
    return {
        a.x + x * (b.x - a.x),
        a.y + x * (b.y - a.y)};
}

inline auto to_size_k (int idx)
{
    return static_cast< std::vector< juce::Point< float > >::size_type > (idx);
}

juce::Point< float > nurbs (const std::vector< juce::Point< float > >& knots, float t, const int* k) noexcept
{
    const auto& knot0 = knots[to_size_k (k[0])];
    const auto& knot1 = knots[to_size_k (k[1])];
    const auto& knot2 = knots[to_size_k (k[2])];

    return {
        std::pow (1.f - t, 2.f) * knot0.x + (1.f - t) * 2.f * t * knot1.x + t * t * knot2.x,
        std::pow (1.f - t, 2.f) * knot0.y + (1.f - t) * 2.f * t * knot1.y + t * t * knot2.y};
}

float hermitCubic (const Knots& data, float x) noexcept
{
    const auto size = static_cast< int > (data.size());
    const auto i1   = static_cast< int > (x);

    auto i0 = i1 - 1;
    auto i2 = i1 + 1;
    auto i3 = i1 + 2;

    while (i3 >= size)
        i3 -= size;
    while (i2 >= size)
        i2 -= size;
    while (i0 < 0)
        i0 += size;

    const auto frac = x - i1;
    const auto v0   = data[i0].y;
    const auto v1   = data[i1].y;
    const auto v2   = data[i2].y;
    const auto v3   = data[i3].y;

    const auto c0 = v1;
    const auto c1 = .5f * (v2 - v0);
    const auto c2 = v0 - 2.5f * v1 + 2.f * v2 - .5f * v3;
    const auto c3 = 1.5f * (v1 - v2) + .5f * (v3 - v0);

    return ((c3 * frac + c2) * frac + c1) * frac + c0;
}

float hermitCubic2 (const Knots& data, float x, float tolerance, int i1) noexcept
{
    const auto size = static_cast< int > (data.size());

    auto i0 = i1 - 1;
    auto i2 = i1 + 1;
    auto i3 = i1 + 2;

    if (i0 < 0) i0 = 0;
    if (i2 >= size) i2 = size - 1;
    if (i3 >= size) i3 = size - 1;

    const auto& knoti0 = data[i0];
    const auto& knoti1 = data[i1];
    const auto& knoti2 = data[i2];
    const auto& knoti3 = data[i3];

    const auto x21 = knoti2.x - knoti1.x;
    const auto x20 = knoti2.x - knoti0.x;
    const auto x31 = knoti3.x - knoti1.x;

    if (x21 < tolerance)
        return knoti2.y;

    const auto m0 = (knoti2.y - knoti0.y) / x20 * x21;
    const auto m1 = (knoti3.y - knoti1.y) / x31 * x21;

    const auto y0 = knoti1.y;
    const auto y1 = knoti2.y;
    const auto a  = 2.f * (y0 - y1) + m0 + m1;
    const auto b  = 3.f * (y1 - y0) - 2.f * m0 - m1;
    const auto c  = m0;
    const auto d  = y0;

    const auto t = (x - knoti1.x) / x21;
    return d + t * (c + t * (b + t * a));
}

}  // namespace bav::spline::interpolation
