
namespace bav::spline
{
void Processor::serialize (TreeReflector& ref)
{
    ref.add ("SplineKnots", knots);

    if (ref.isLoading())
        updateSpline();
}

void Processor::resizeSpline (int newSize)
{
    jassert (newSize > 0);
    spline.resize (static_cast< size_t > (newSize));
    updateSpline();
}

bool Processor::updateSpline()
{
    if (spline.empty()) return false;
    knots.makeSpline (spline);
    return true;
}

void Processor::replaceKnots (const Knots& otherKnots)
{
    knots = otherKnots;
    updateSpline();
}

const std::vector< float >& Processor::getSpline() const noexcept { return spline; }

void Processor::dbg()
{
#if JUCE_DEBUG
    juce::String str;
    int          i = 0;

    for (auto& knot : knots.knots)
    {
        str += juce::String (i) + ": " + juce::String (knot.x) + ", " + juce::String (knot.y) + "\n";
        ++i;
    }

    DBG (str + "\n");
#endif
}

}  // namespace bav::spline
