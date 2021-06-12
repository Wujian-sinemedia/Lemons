#pragma once

namespace bav::events
{
struct TimerCallback : private juce::Timer
{
    TimerCallback (std::function< void() > callbackToUse,
                   int                     rateInMs = 100)
        : callback (std::move (callbackToUse))
    {
        startTimer (rateInMs);
    }

    virtual ~TimerCallback() { stopTimer(); }

private:
    void timerCallback() final
    {
        callback();
    }

    std::function< void() > callback;
};

}  // namespace bav::events
