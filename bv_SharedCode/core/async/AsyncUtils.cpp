
namespace bav 
{

template <typename ArgumentType>
class BackgroundCaller :    private juce::Thread,
                            private juce::AsyncUpdater
{
public:
    BackgroundCaller (std::function< void (ArgumentType) > func,
                      ArgumentType arg)
      : juce::Thread (TRANS ("Function called synchronously on background thread")),
        function (func), argument (arg)
    {
        startThread();
    }
    
    ~BackgroundCaller() override
    {
        stopThread (1000);
        cancelPendingUpdate();
    }
    
    void run() override final
    {
        function (argument);
        triggerAsyncUpdate();
    }
    
    void handleAsyncUpdate() override final
    {
        delete this;
    }
    
private:
    std::function< void (ArgumentType) > function;
    const ArgumentType argument;
};


template <typename ArgumentType = void>
void callInBackground (std::function< void (ArgumentType) > function, ArgumentType argument)
{
    new BackgroundCaller<ArgumentType> (function, argument);
}


template class BackgroundCaller<void>;
template class BackgroundCaller<double>;
template class BackgroundCaller<float>;
template class BackgroundCaller<int>;
template class BackgroundCaller<bool>;
template class BackgroundCaller<juce::String>;

template void callInBackground (std::function< void (void) >, void);
template void callInBackground (std::function< void (double) >, double);
template void callInBackground (std::function< void float >, float);
template void callInBackground (std::function< void (int) >, int);
template void callInBackground (std::function< void (bool) >, bool);
template void callInBackground (std::function< void (juce::String) >, juce::String);


//


class DelayedLambdaHelper : public juce::Timer
{
public:
    DelayedLambdaHelper (std::function<void ()>& cb, int ms)
        : callback (cb)
    {
        startTimer (ms);
    }
    
    void timerCallback() override final
    {
        callback();
        delete this;
    }
    
private:
    std::function< void() > callback;
};


void callDelayed (std::function< void() > callback, int msInFuture)
{
    new DelayedLambdaHelper (callback, msInFuture);
}

    
    
}  // namespace
