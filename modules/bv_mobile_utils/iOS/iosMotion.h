
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


    void accelerationChanged (double x, double y, double z);

    void gravityChanged (double x, double y, double z);

    void rotationChanged (double x, double y, double z);

    void attitudeChanged (double x, double y, double z);

private:
    void* motionManagerWrapper;
    bool  running;
};


}  // namespace bav
