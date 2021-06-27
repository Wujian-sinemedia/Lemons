#pragma once

namespace bav
{

struct ParamUpdater : Parameter::Listener
{
    ParamUpdater (Parameter& param, std::function<void()> onValueChange);
    
    void parameterValueChanged (float) final;
    
private:
    std::function<void()> callback;
};

}
