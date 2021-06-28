
#pragma once

namespace bav
{
class DataSynchronizer
{
public:
    explicit DataSynchronizer (SerializableData& dataToUse);

    void applyChangeData (const void* data, size_t dataSize);

private:
    virtual void sendChangeData (const void* data, size_t dataSize) = 0;
    void         callback();

    SerializableData& sData;

    events::TimerCallback t {[&]
                             { callback(); },
                             10};
};

}  // namespace bav
