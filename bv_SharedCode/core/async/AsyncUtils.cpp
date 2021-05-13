
namespace bav
{


class VoidCaller :    private juce::Thread,
                      private juce::AsyncUpdater
{
public:
    VoidCaller (std::function< void() > func)
      : juce::Thread (TRANS ("Function called synchronously on background thread")),
        function (func)
    {
        startThread();
    }
    
    ~VoidCaller() override
    {
        stopThread (1000);
        cancelPendingUpdate();
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
    new VoidCaller (function);
}


/*===================================================================================================*/

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


template <typename ArgumentType>
void callInBackground (std::function< void (ArgumentType) > function, ArgumentType argument)
{
    new BackgroundCaller<ArgumentType> (function, argument);
}


template class BackgroundCaller<double>;
template class BackgroundCaller<float>;
template class BackgroundCaller<int>;
template class BackgroundCaller<bool>;
template class BackgroundCaller<juce::String>;

template void callInBackground (std::function< void (double) >, double);
template void callInBackground (std::function< void (float) >, float);
template void callInBackground (std::function< void (int) >, int);
template void callInBackground (std::function< void (bool) >, bool);
template void callInBackground (std::function< void (juce::String) >, juce::String);


/*===================================================================================================*/
/*===================================================================================================*/


class VoidDelayedLambdaHelper : public juce::Timer
{
public:
    VoidDelayedLambdaHelper (std::function< void() >& cb, int ms)
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
    new VoidDelayedLambdaHelper (callback, msInFuture);
}

/*===================================================================================================*/

template<typename ArgumentType>
class DelayedLambdaHelper : public juce::Timer
{
public:
    DelayedLambdaHelper (std::function< void (ArgumentType) >& cb, ArgumentType arg, int ms)
    : callback (cb), argument (arg)
    {
        startTimer (ms);
    }
    
    void timerCallback() override final
    {
        callback (argument);
        delete this;
    }
    
private:
    std::function< void (ArgumentType) > callback;
    const ArgumentType argument;
};


template<typename ArgumentType>
void callDelayed (std::function< void(ArgumentType) > callback, ArgumentType arg, int msInFuture)
{
    new DelayedLambdaHelper<ArgumentType> (callback, arg, msInFuture);
}


template class DelayedLambdaHelper<double>;
template class DelayedLambdaHelper<float>;
template class DelayedLambdaHelper<int>;
template class DelayedLambdaHelper<bool>;
template class DelayedLambdaHelper<juce::String>;

template void callDelayed (std::function< void (double) >, double, int);
template void callDelayed (std::function< void (float) >, float, int);
template void callDelayed (std::function< void (int) >, int, int);
template void callDelayed (std::function< void (bool) >, bool, int);
template void callDelayed (std::function< void (juce::String) >, juce::String, int);

}  // namespace

