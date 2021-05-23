
#pragma once

#if JUCE_IOS || JUCE_ANDROID
#error
#endif


/* This dummy Motionmanager class can be instantiated on non-mobile devices */
class MotionManager : public MotionManagerInterface
{
public:
    MotionManager() = default;
    
    void start() override final { }
    void stop() override final { }
    
    bool isRunning() override final { return false; }
};
