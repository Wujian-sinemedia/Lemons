#pragma once

#include <bv_spline/common/Common.h>


namespace bav::spline
{
class Processor : public SerializableData
{
public:
    void resizeSpline (int newSize);

    bool updateSpline();

    const std::vector< float >& getSpline() const noexcept;

    Knots knots;

private:
    void serialize (TreeReflector& ref) final;

    std::vector< float > spline;
};

}  // namespace bav::spline
