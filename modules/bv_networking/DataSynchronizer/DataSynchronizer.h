
#pragma once

namespace bav
{
class DataSynchronizer : private juce::Timer
{
public:
    explicit DataSynchronizer (SerializableData& dataToUse);
    virtual ~DataSynchronizer() override;

    void applyChangeData (const void* data, size_t dataSize);
    
private:
    void timerCallback() override final;

    virtual void sendChangeData (const void* data, size_t dataSize) = 0;

    SerializableData& sData;
};

}  // namespace bav
