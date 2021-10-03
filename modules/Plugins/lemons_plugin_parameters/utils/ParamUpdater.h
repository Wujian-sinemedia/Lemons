#pragma once

namespace lemons::plugin
{
struct ParamUpdater : Parameter::Listener
{
    ParamUpdater (
        Parameter& param,
        std::function< void() >
                                     onValueChange,
        std::function< void (bool) > onGestureChange = [] (bool) {});

private:
    void parameterValueChanged (float) final;
    void gestureStateChanged (bool gestureIsStarting) final;

    std::function< void() >      valueCallback;
    std::function< void (bool) > gestureCallback;
};

}  // namespace lemons::plugin
