
namespace bav 
{

class BackgroundCaller :    private juce::Thread,
                            private juce::AsyncUpdater
{
public:
    BackgroundCaller (std::function< void() > func)
      : juce::Thread (TRANS ("Function called synchronously on background thread")),
        function (func)
    {
        startThread();
    }
    
    ~BackgroundCaller() override
    {
        stopThread (1000);
    }
    
    void run() override final
    {
        function();
        triggerAsyncUpdate();
    }
    
    void handleAsyncUpdate() override final
    {
        delete this;
    }
    
private:
    std::function< void() > function;
};

    
void callInBackground (std::function< void() > function)
{
    new BackgroundCaller (function);
}


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


void delayedLambda (std::function< void() > callback, int msInFuture)
{
    new DelayedLambdaHelper (callback, msInFuture);
}

    
    
}  // namespace
