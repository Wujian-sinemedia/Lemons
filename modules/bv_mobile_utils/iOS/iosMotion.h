
#if ! JUCE_IOS
#    error
#endif


#pragma once

#include <CoreMotion/CoreMotion.h>


namespace bav
{
class MotionManager : public MotionManagerInterface
{
    MotionManager();

    virtual ~MotionManager() override;

    virtual void start() override final;

    virtual void stop() override final;

    virtual bool isRunning() override final { return running; }
    

private:
    void* motionManagerWrapper;
    bool  running;
};


}  // namespace bav
