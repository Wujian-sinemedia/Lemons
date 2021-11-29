#pragma once

#include <juce_core/juce_core.h>


namespace lemons::tests
{

class Test : public juce::UnitTest
{
public:
	using juce::UnitTest::UnitTest;

protected:
	using String = juce::String;

	void          logImportantMessage (const String& message);
	const String& getLastImportantMessage() const;

private:
	String lastImportantMessage;
};

}  // namespace lemons::tests
