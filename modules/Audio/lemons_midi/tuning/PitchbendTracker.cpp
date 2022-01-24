
namespace lemons::midi
{

void PitchBendTracker::setRange (int newStUp, int newStDown) noexcept
{
	rangeUp   = static_cast<float> (newStUp);
	rangeDown = static_cast<float> (newStDown);
}

int PitchBendTracker::getRangeUp() const noexcept { return juce::roundToInt (rangeUp); }
int PitchBendTracker::getRangeDown() const noexcept { return juce::roundToInt (rangeDown); }

int PitchBendTracker::getLastReceivedPitchbend() const noexcept
{
	return juce::roundToInt (lastReceivedPitchbend);
}

void PitchBendTracker::newPitchbendReceived (int newPitchbend) noexcept
{
	jassert (newPitchbend >= 0 && newPitchbend <= 127);
	lastReceivedPitchbend = static_cast<float> (newPitchbend);
}

void PitchBendTracker::processMidiMessage (const MidiMessage& m) noexcept
{
	if (m.isPitchWheel())
		newPitchbendReceived (juce::jmap (m.getPitchWheelValue(), 0, 16383, 0, 127));
}

}  // namespace lemons::midi
