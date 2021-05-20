
namespace bav
{
class VoidCaller : private juce::Thread, private juce::AsyncUpdater
{
public:
    VoidCaller (std::function< void() > func)
        : juce::Thread (TRANS ("Function called synchronously on background thread")), function (func)
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

    void handleAsyncUpdate() override final { delete this; }

private:
    std::function< void() > function;
};


void callInBackground (std::function< void() > function)
{
    new VoidCaller (function);
}


/*===================================================================================================*/

template < typename ArgumentType >
class BackgroundCaller : private juce::Thread, private juce::AsyncUpdater
{
public:
    BackgroundCaller (std::function< void (ArgumentType) > func, ArgumentType arg)
        : juce::Thread (TRANS ("Function called synchronously on background thread")), function (func), argument (arg)
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

    void handleAsyncUpdate() override final { delete this; }

private:
    std::function< void (ArgumentType) > function;
    const ArgumentType                   argument;
};


template < typename ArgumentType >
void callInBackground (std::function< void (ArgumentType) > function,
                       ArgumentType                         argument)
{
    new BackgroundCaller< ArgumentType > (function, argument);
}


template class BackgroundCaller< double >;
template class BackgroundCaller< float >;
template class BackgroundCaller< int >;
template class BackgroundCaller< bool >;
template class BackgroundCaller< juce::String >;

template void callInBackground (std::function< void (double) >, double);
template void callInBackground (std::function< void (float) >, float);
template void callInBackground (std::function< void (int) >, int);
template void callInBackground (std::function< void (bool) >, bool);
template void callInBackground (std::function< void (juce::String) >, juce::String);


/*===================================================================================================*/
/*===================================================================================================*/

class VoidMessageThreadCaller : private juce::AsyncUpdater
{
public:
    VoidMessageThreadCaller (std::function< void() > func)
        : function (func)
    {
        triggerAsyncUpdate();
    }

    ~VoidMessageThreadCaller() override { cancelPendingUpdate(); }

    void handleAsyncUpdate() override final
    {
        function();
        delete this;
    }

private:
    std::function< void() > function;
};


void callOnMessageThread (std::function< void() > function)
{
    if (juce::MessageManager::getInstance()->isThisTheMessageThread())
        function();
    else
        new VoidMessageThreadCaller (function);
}

/*===================================================================================================*/

template < typename ArgumentType >
class MessageThreadCaller : private juce::AsyncUpdater
{
public:
    MessageThreadCaller (std::function< void (ArgumentType) > func, ArgumentType arg)
        : function (func), argument (arg)
    {
        triggerAsyncUpdate();
    }

    ~MessageThreadCaller() override { cancelPendingUpdate(); }

    void handleAsyncUpdate() override final
    {
        function (argument);
        delete this;
    }

private:
    std::function< void (ArgumentType) > function;
    const ArgumentType                   argument;
};


template < typename ArgumentType >
void callOnMessageThread (std::function< void (ArgumentType) > function,
                          ArgumentType                         argument)
{
    if (juce::MessageManager::getInstance()->isThisTheMessageThread())
        function (argument);
    else
        new MessageThreadCaller< ArgumentType > (function, argument);
}


template class MessageThreadCaller< double >;
template class MessageThreadCaller< float >;
template class MessageThreadCaller< int >;
template class MessageThreadCaller< bool >;
template class MessageThreadCaller< juce::String >;

template void callOnMessageThread (std::function< void (double) >, double);
template void callOnMessageThread (std::function< void (float) >, float);
template void callOnMessageThread (std::function< void (int) >, int);
template void callOnMessageThread (std::function< void (bool) >, bool);
template void callOnMessageThread (std::function< void (juce::String) >,
                                   juce::String);


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

template < typename ArgumentType >
class DelayedLambdaHelper : public juce::Timer
{
public:
    DelayedLambdaHelper (std::function< void (ArgumentType) >& cb,
                         ArgumentType                          arg,
                         int                                   ms)
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
    const ArgumentType                   argument;
};


template < typename ArgumentType >
void callDelayed (std::function< void (ArgumentType) > callback,
                  ArgumentType                         arg,
                  int                                  msInFuture)
{
    new DelayedLambdaHelper< ArgumentType > (callback, arg, msInFuture);
}


template class DelayedLambdaHelper< double >;
template class DelayedLambdaHelper< float >;
template class DelayedLambdaHelper< int >;
template class DelayedLambdaHelper< bool >;
template class DelayedLambdaHelper< juce::String >;

template void callDelayed (std::function< void (double) >, double, int);
template void callDelayed (std::function< void (float) >, float, int);
template void callDelayed (std::function< void (int) >, int, int);
template void callDelayed (std::function< void (bool) >, bool, int);
template void callDelayed (std::function< void (juce::String) >, juce::String, int);


/*===================================================================================================*/
/*===================================================================================================*/

class VoidDelayedBackgroundCaller : private juce::Thread, private juce::AsyncUpdater
{
public:
    VoidDelayedBackgroundCaller (std::function< void() > func, int msInFuture)
        : juce::Thread (
            TRANS (
                "Function called synchronously on background thread with delay of ")
            + juce::String (msInFuture)),
          function (func), delayMs (msInFuture)
    {
        startThread();
    }

    ~VoidDelayedBackgroundCaller() override
    {
        stopThread (1000);
        cancelPendingUpdate();
    }

    void run() override final
    {
        wait (delayMs);

        if (! threadShouldExit()) function();

        triggerAsyncUpdate();
    }

    void handleAsyncUpdate() override final { delete this; }

private:
    std::function< void() > function;
    const int               delayMs;
};


void callInBackgroundDelayed (std::function< void() > function, int msInFuture)
{
    new VoidDelayedBackgroundCaller (function, msInFuture);
}


/*===================================================================================================*/

template < typename ArgumentType >
class DelayedBackgroundCaller : private juce::Thread, private juce::AsyncUpdater
{
public:
    DelayedBackgroundCaller (std::function< void (ArgumentType) > func,
                             ArgumentType                         arg,
                             int                                  msInFuture)
        : juce::Thread (
            TRANS (
                "Function called synchronously on background thread with delay of ")
            + juce::String (msInFuture)),
          function (func), argument (arg), delayMs (msInFuture)
    {
        startThread();
    }

    ~DelayedBackgroundCaller() override
    {
        stopThread (1000);
        cancelPendingUpdate();
    }

    void run() override final
    {
        wait (delayMs);

        if (! threadShouldExit()) function (argument);

        triggerAsyncUpdate();
    }

    void handleAsyncUpdate() override final { delete this; }

private:
    std::function< void (ArgumentType) > function;
    const ArgumentType                   argument;
    const int                            delayMs;
};


template < typename ArgumentType >
void callInBackgroundDelayed (std::function< void (ArgumentType) > function,
                              ArgumentType                         argument,
                              int                                  msInFuture)
{
    new DelayedBackgroundCaller< ArgumentType > (function, argument, msInFuture);
}


template class DelayedBackgroundCaller< double >;
template class DelayedBackgroundCaller< float >;
template class DelayedBackgroundCaller< int >;
template class DelayedBackgroundCaller< bool >;
template class DelayedBackgroundCaller< juce::String >;

template void callInBackgroundDelayed (std::function< void (double) >, double, int);
template void callInBackgroundDelayed (std::function< void (float) >, float, int);
template void callInBackgroundDelayed (std::function< void (int) >, int, int);
template void callInBackgroundDelayed (std::function< void (bool) >, bool, int);
template void callInBackgroundDelayed (std::function< void (juce::String) >,
                                       juce::String,
                                       int);


}  // namespace bav
