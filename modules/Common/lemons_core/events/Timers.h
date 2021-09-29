#pragma once

namespace lemons::events
{
struct TimerCallback : private juce::Timer
{
    TimerCallback (std::function< void() > callbackToUse,
                   int                     rateInMs = 100);

    virtual ~TimerCallback();

private:
    void timerCallback() final;

    std::function< void() > callback;
};

}  // namespace lemons::events
