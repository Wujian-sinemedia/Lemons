/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */

namespace lemons::plugin
{
ParamUpdater::ParamUpdater (Parameter& param, std::function<void()> onValueChange, std::function<void (bool)> onGestureChange)
    : Listener (param)
    , valueCallback (onValueChange)
    , gestureCallback (onGestureChange)
{
	jassert (valueCallback != nullptr);
	jassert (gestureCallback != nullptr);
}

void ParamUpdater::parameterValueChanged (float)
{
	valueCallback();
}

void ParamUpdater::gestureStateChanged (bool gestureIsStarting)
{
	gestureCallback (gestureIsStarting);
}

}  // namespace lemons::plugin
