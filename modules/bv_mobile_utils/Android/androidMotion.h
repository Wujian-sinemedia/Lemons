
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
    MotionManager()
    {
        sensorManager = ASensorManager_getInstance();
        accelerometer = ASensorManager_getDefaultSensor (
            sensorManager, ASENSOR_TYPE_LINEAR_ACCELERATION);
        rotation =
            ASensorManager_getDefaultSensor (sensorManager, ASENSOR_TYPE_GYROSCOPE);
        gravity =
            ASensorManager_getDefaultSensor (sensorManager, ASENSOR_TYPE_GRAVITY);
        attitude = ASensorManager_getDefaultSensor (
            sensorManager, ASENSOR_TYPE_GAME_ROTATION_VECTOR);
        looper           = ALooper_prepare (ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
        motionEventQueue = ASensorManager_createEventQueue (
            sensorManager, looper, LOOPER_ID_USER, NULL, NULL);
    }

    virtual ~MotionManager() override
    {
        ASensorManager_destroyEventQueue (sensorManager, motionEventQueue);
    }

    virtual void start() override final
    {
        if (! running)
        {
            running = true;
            ASensorEventQueue_enableSensor (motionEventQueue, accelerometer);
            ASensorEventQueue_setEventRate (
                motionEventQueue, accelerometer, SENSOR_REFRESH_PERIOD_US);
            ASensorEventQueue_enableSensor (motionEventQueue, rotation);
            ASensorEventQueue_setEventRate (
                motionEventQueue, rotation, SENSOR_REFRESH_PERIOD_US);
            ASensorEventQueue_enableSensor (motionEventQueue, gravity);
            ASensorEventQueue_setEventRate (
                motionEventQueue, gravity, SENSOR_REFRESH_PERIOD_US);
            ASensorEventQueue_enableSensor (motionEventQueue, attitude);
            ASensorEventQueue_setEventRate (
                motionEventQueue, attitude, SENSOR_REFRESH_PERIOD_US);
            Timer::startTimerHz (SENSOR_REFRESH_RATE_HZ);
        }
    }

    virtual void stop() override final
    {
        running = false;
        ASensorEventQueue_disableSensor (motionEventQueue, accelerometer);
        ASensorEventQueue_disableSensor (motionEventQueue, rotation);
        ASensorEventQueue_disableSensor (motionEventQueue, gravity);
        ASensorEventQueue_disableSensor (motionEventQueue, attitude);
        Timer::stopTimer();
    }

    virtual bool isRunning() override final { return running; }

    void timerCallback() override final
    {
        if (running) update();
    }


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

    void update()
    {
        ALooper_pollAll (0, NULL, NULL, NULL);
        ASensorEvent event;

        while (ASensorEventQueue_getEvents (motionEventQueue, &event, 1) > 0)
        {
            if (event.type == ASENSOR_TYPE_LINEAR_ACCELERATION)
            {
                accelerationX = event.acceleration.x * -0.08;
                accelerationY = event.acceleration.y * 0.08;
                accelerationZ = event.acceleration.z * 0.08;
            }
            else if (event.type == ASENSOR_TYPE_GYROSCOPE)
            {
                rotationX = event.vector.x * 1;
                rotationY = event.vector.y * 1;
                rotationZ = event.vector.z * 1;
            }
            else if (event.type == ASENSOR_TYPE_GRAVITY)
            {
                gravityX = event.acceleration.x * -0.1;
                gravityY = event.acceleration.y * 0.1;
                gravityZ = event.acceleration.z * 0.1;
            }
            else if (event.type == ASENSOR_TYPE_GAME_ROTATION_VECTOR)
            {
                attitudeX = event.vector.y * 2;
                attitudeY = event.vector.x * 2;
                attitudeZ = event.vector.z * 2;
            }
        }
    }
};

}  // namespace bav
