
#if JUCE_IOS || JUCE_ANDROID

#pragma once


namespace bav
{


class MotionManagerInterface
{
public:
    MotionManagerInterface() { }
    
    virtual ~MotionManagerInterface() = default;
    
    virtual void start() = 0;
    
    virtual void stop() = 0;
    
    virtual bool isRunning() = 0;
    
    double accelerationX = 0;
    double accelerationY = 0;
    double accelerationZ = 0;
    
    double gravityX = 0;
    double gravityY = 0;
    double gravityZ = 0;
    
    double rotationX = 0;
    double rotationY = 0;
    double rotationZ = 0;
    
    double attitudeX = 0;
    double attitudeY = 0;
    double attitudeZ = 0;
};

}  // namespace

#endif
