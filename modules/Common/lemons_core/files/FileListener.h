#pragma once

namespace lemons::files
{

/** A class that fires a lambda callback whenever the watched file's modification time changes.
 */
class Listener final
{
public:
	explicit Listener (const File&			   fileToWatch,
					   std::function<void()>&& callback);

private:
	void check();

	const std::function<void()> callbackFunc;

	const File file;

	juce::int64 lastModTime { 0 };

	const events::TimerCallback t { [this]
									{ check(); } };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Listener)
};

}  // namespace lemons::files
