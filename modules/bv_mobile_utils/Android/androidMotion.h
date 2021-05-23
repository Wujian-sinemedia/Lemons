
#if ! JUCE_ANDROID
#    error
#endif

#pragma once

#include <android/sensor.h>


namespace bav
{
class MotionManager : public MotionManagerInterface,
                      private juce::Timer
{
public:
    MotionManager();
    virtual ~MotionManager() override;

    virtual void start() override final;

    virtual void stop() override final;

    virtual bool isRunning() override final { return running; }

    void timerCallback() override final;


private:
    bool running;

    const int LOOPER_ID_USER           = 3;
    int       SENSOR_REFRESH_RATE_HZ   = 100;
    int       SENSOR_REFRESH_PERIOD_US = 1000000 / SENSOR_REFRESH_RATE_HZ;

    ASensorManager*    sensorManager;
    ASensorEventQueue* motionEventQueue;
    ALooper*           looper;

    const ASensor* accelerometer;
    const ASensor* rotation;
    const ASensor* gravity;
    const ASensor* attitude;

    void update();
};

}  // namespace bav
