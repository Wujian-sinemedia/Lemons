#pragma once

#include <bv_spline/common/Knots.h>
#include <bv_spline/common/Interpolation.h>


namespace bav::spline
{
class Processor : public SerializableData
{
public:
    void resizeSpline (int newSize);

    bool updateSpline();

    void replaceKnots (const Knots& otherKnots);

    const std::vector< float >& getSpline() const noexcept;

    Knots                knots;
    juce::Atomic< bool > updated {false};

private:
    void serialize (TreeReflector& ref) final;

    void dbg();

    std::vector< float > spline;
};

}  // namespace bav::spline
