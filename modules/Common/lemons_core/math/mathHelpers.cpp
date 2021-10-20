
namespace lemons::math
{
bool probability (int percentOfTheTime) noexcept
{
	return juce::Random::getSystemRandom().nextInt (100) < percentOfTheTime;
}

int sampsToMs (double samplerate, int numSamples) noexcept
{
	jassert (samplerate > 0.0);
	return juce::roundToInt ((numSamples / samplerate) * 1000.0f);
}

}  // namespace lemons::math
