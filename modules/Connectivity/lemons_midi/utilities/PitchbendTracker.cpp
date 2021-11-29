
namespace lemons::midi
{

void PitchBendTracker::setRange (int newStUp, int newStDown) noexcept
{
	rangeUp   = static_cast<float> (newStUp);
	rangeDown = static_cast<float> (newStDown);
}

int PitchBendTracker::getRangeUp() const noexcept { return juce::roundToInt (rangeUp); }
int PitchBendTracker::getRangeDown() const noexcept { return juce::roundToInt (rangeDown); }

int PitchBendTracker::getLastRecievedPitchbend() const noexcept
{
	return juce::roundToInt (lastRecievedPitchbend);
}

void PitchBendTracker::newPitchbendRecieved (int newPitchbend) noexcept
{
	jassert (newPitchbend >= 0 && newPitchbend <= 127);
	lastRecievedPitchbend = static_cast<float> (newPitchbend);
}

}  // namespace lemons::midi
