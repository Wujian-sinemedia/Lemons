
namespace bav
{

ParamUpdater::ParamUpdater (Parameter& param, std::function<void()> onValueChange)
: Listener (param), callback (std::move (onValueChange))
{ }

void ParamUpdater::parameterValueChanged (float)
{
    callback();
}

}
