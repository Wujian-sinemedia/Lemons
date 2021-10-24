#pragma once

namespace lemons::midi
{

struct VelocityHelper
{
    void setSensitivity (float newSensitivity);
    void setSensitivity (int newSensitivity);
    
    float getGainForVelocity (float midiVelocity);
    float getGainForVelocity (int midiVelocity);
    
    float getSensitivity() const { return sensitivity; }
    
private:
    float sensitivity { 1.0f };
};

}
