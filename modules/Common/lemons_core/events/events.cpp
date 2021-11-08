
namespace lemons::events
{

TimerCallback::TimerCallback (std::function<void()> callbackToUse,
                              int                   rateInMs)
    : callback (std::move (callbackToUse))
{
	startTimer (rateInMs);
}

TimerCallback::~TimerCallback()
{
	stopTimer();
}

void TimerCallback::timerCallback()
{
	callback();
}

}  // namespace lemons::events
