
namespace bav::midi
{
// helper class for outputting weighted midi velocities
class VelocityHelper
{
public:
    VelocityHelper (const int initialSensitivity)
        : sensitivity (initialSensitivity / 100.0f)
    {
    }

    void setSensitivity (int newSensitivity) noexcept
    {
        jassert (newSensitivity >= 0 && newSensitivity <= 100);
        sensitivity.store (newSensitivity * 0.01f);
    }

    void setFloatSensitivity (const float newSensitivity) noexcept
    {
        jassert (newSensitivity >= 0.0f && newSensitivity <= 1.0f);
        sensitivity.store (newSensitivity);
    }

    float getCurrentSensitivity() const noexcept { return sensitivity.load(); }

    float intVelocity (int midiVelocity) const
    {
        jassert (midiVelocity >= 0 && midiVelocity <= 127);
        constexpr auto inv127 = 1.0f / 127.0f;
        return getGainMult (midiVelocity * inv127, sensitivity.load());
    }

    float floatVelocity (float floatVelocity) const
    {
        jassert (floatVelocity >= 0.0f && floatVelocity <= 1.0f);
        return getGainMult (floatVelocity, sensitivity.load());
    }


private:
    std::atomic< float > sensitivity;

    float getGainMult (const float floatVelocity, const float floatSensitivity) const
    {
        const auto gain =
            (1.0f - floatVelocity) * (1.0f - floatSensitivity) + floatVelocity;

        jassert (gain >= 0.0f && gain <= 1.0f);

        return gain;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VelocityHelper)
};

}  // namespace bav::midi
