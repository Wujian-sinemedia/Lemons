#pragma once

namespace bav
{

struct ParamUpdater : Parameter::Listener
{
    ParamUpdater (Parameter& param,
                  std::function<void()> onValueChange,
                  std::function< void(bool) > onGestureChange = {});
    
private:
    void parameterValueChanged (float) final;
    void parameterGestureStateChanged (bool gestureIsStarting) final;
    
    std::function<void()> valueCallback;
    std::function< void(bool) > gestureCallback;
};

}
