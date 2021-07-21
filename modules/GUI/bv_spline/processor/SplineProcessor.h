#pragma once

#include <bv_spline/common/Common.h>


namespace bav::spline
{
class Processor : public SerializableData
{
public:
    void resizeSpline (int newSize);

    bool updateSpline();

    void replaceKnots (const Knots& otherKnots);

    const std::vector< float >& getSpline() const noexcept;

    Knots knots;

private:
    void serialize (TreeReflector& ref) final;

    void dbg();

    std::vector< float > spline;
};

}  // namespace bav::spline
