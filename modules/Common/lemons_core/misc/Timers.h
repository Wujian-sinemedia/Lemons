/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */

#pragma once

#include <juce_events/juce_events.h>

namespace lemons::events
{

/** @ingroup lemons_core
    A timer callback that calls a lambda function.
    For example:
    @code
    void myFunction();

    TimerCallback t { [&]{ myFunction(); }; };
    @endcode
 */
struct TimerCallback final : public juce::Timer
{
	/** Creates a TimerCallback.
	    @param callbackToUse The function to call as the timer callback.
	    @param rateInMs The number of milliseconds between successive calls of callbackToUse when the timer is running.
	    @param start If true, the timer will be started instantly when this object is created.
	 */
	explicit TimerCallback (std::function<void()>&& callbackToUse,
	                        int                     rateInMs = 100,
	                        bool                    start    = true)
	    : callback (std::move (callbackToUse))
	{
		jassert (callback != nullptr);

		if (start)
			startTimer (rateInMs);
	}

	/** Destructor. */
	~TimerCallback() final
	{
		if (isTimerRunning())
			stopTimer();
	}

private:
	void timerCallback() final
	{
		callback();
	}

	const std::function<void()> callback;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimerCallback)
};

}  // namespace lemons::events
