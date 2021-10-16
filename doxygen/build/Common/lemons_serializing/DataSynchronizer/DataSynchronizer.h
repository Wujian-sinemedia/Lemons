
/** @ingroup DataSynchronizer
 *  @{
 */

#pragma once

namespace lemons::serializing
{
//==============================================================================
/**
    Keeps two copies of an object inheriting from SerializableData in sync.
    This is akin to juce's ValueTreeSynchronizer, but for working with my SerializableData class. \n
    Because SerializableData does not have any means for detecting changes, this class simply sends a consistent stream of updates on a timer callback. \n
    At first glance this may seem excessive, but I have found it to be the most robust way of ensuring that two remote objects stay in sync, and I haven't encountered any major performance issues.

    @see SerializableData
 */
class DataSynchronizer
{
public:
    /** Creates a DataSynchronizer. Make sure that you don't delete the referenced data object before the synchronizer object! */
    explicit DataSynchronizer (SerializableData& dataToUse);

    virtual ~DataSynchronizer() = default;

    /** When recieving a change message from another data synchronizer, call this function to apply the latest recieved state to the managed object.
        @param recievedData The binary blob you recieved from another DataSynchronizer's sendData function.
        @see sendData
     */
    void applyRecievedData (const juce::MemoryBlock& recievedData);

private:
    /**
        Your subclass of DataSynchroniser should implement this function to send binary blobs of encoded state information to the destination.
        You can use this function to send OSC messages, UDP packets, HTTP requests, MIDI, DMX packets, etc.
        @param outgoingData The encoded binary blob of state data that should be fed to the recieving DataSynchronizer's applyRecievedData function.
        @see applyRecievedData
     */
    virtual void sendData (const juce::MemoryBlock& outgoingData) = 0;

    void callback();

    SerializableData& managedState;

    juce::MemoryBlock cachedData;

    events::TimerCallback t {[&]
                             { callback(); },
                             10};
};

}  // namespace lemons::serializing

/** @}*/
