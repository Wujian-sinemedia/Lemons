
/** @ingroup dummy_desktop_versions
 *  @{
 */

#pragma once

#if JUCE_IOS || JUCE_ANDROID
#    error
#endif


namespace lemons
{
/* This dummy Motionmanager class can be instantiated on non-mobile devices */
class MotionManager : public MotionManagerInterface
{
public:
    MotionManager() = default;

    void start() final { }
    void stop() final { }

    bool isRunning() final { return false; }
};


}  // namespace lemons

/** @}*/
