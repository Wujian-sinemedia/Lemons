
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

const std::vector< float >& Processor::getSpline() const noexcept { return spline; }

}  // namespace bav::spline
