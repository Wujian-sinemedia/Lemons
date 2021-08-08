
namespace bav::math
{
bool probability (int percentOfTheTime)
{
    return juce::Random::getSystemRandom().nextInt (100) < percentOfTheTime;
}

constexpr int sampsToMs (double samplerate, int numSamples)
{
    jassert (samplerate > 0.0);
    return juce::roundToInt ((numSamples / samplerate) * 1000.0f);
}

}  // namespace bav::math
