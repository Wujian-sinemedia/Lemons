#pragma once

#include <lemons_core/lemons_core.h>

namespace lemons::midi
{
class MidiFIFO
{
public:
	MidiFIFO (int maxNumMessages = 512);

	void setSize (int maxNumMessages);

	void clear();

	int numStoredEvents() const;

	void pushEvents (const MidiBuffer& source, int numSamples);

	void popEvents (MidiBuffer& output, int numSamples);

private:
	int numStoredSamples { 0 };

	MidiBuffer base;

	MidiBuffer copying;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiFIFO)
};


}  // namespace lemons::midi
