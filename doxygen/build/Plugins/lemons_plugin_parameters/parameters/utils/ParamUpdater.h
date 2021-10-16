
/** @ingroup parameters
 *  @{
 */

/** @ingroup utils
 *  @{
 */
#pragma once

namespace lemons::plugin
{
/** A listener class that calls lambda functions when a parameter's value or gesture state changes.
    @see ParameterList::Listener, Parameter::Listener, Parameter
 */
struct ParamUpdater : Parameter::Listener
{
    /** Creates an updater.
        @param param The parameter to listen to.
        @param onValueChange Lambda function that will be called when the parameter's value changes.
        @param onGestureChange Lambda function that will be called when the parameter's gesture state changes.
     */
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

/** @}*/

/** @}*/
