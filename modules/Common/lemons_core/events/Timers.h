#pragma once

namespace lemons::events
{

/** A timer callback that calls a lambda function.
    e. g.
    @code
    TimerCallback t { [data = getSomeData()]{ callMyFunction (data); }; };
    @endcode
 */
template <typename FunctionType>
struct TimerCallback final : private juce::Timer
{
	static_assert (std::is_invocable_v<FunctionType()>,
	               "FunctionType must be invocable with no arguments!");

	/** Creates a TimerCallback.
	    @param callbackToUse The function to call as the timer callback.
	    @param rateInMs The number of milliseconds between successive calls of callbackToUse when the timer is running.
	    @param start If true, the timer will be started istantly when this object is created.
	 */
	explicit TimerCallback (FunctionType&& callbackToUse,
	                        int            rateInMs = 100,
	                        bool           start    = true)
	    : callback (std::move (callbackToUse))
	{
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

	FunctionType callback;
};

}  // namespace lemons::events
