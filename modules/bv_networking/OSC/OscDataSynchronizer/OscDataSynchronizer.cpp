
namespace bav::network
{
juce::OSCAddressPattern formatAddressPattern (const String& address)
{
    jassert (! address.isEmpty());
    
    address.replaceCharacters (" ", "/");
    
    if (address.startsWith("/"))
        return address;
    
    return String("/") + address;
}

OscDataSynchronizerBase::OscDataSynchronizerBase (SerializableData& dataToUse, juce::OSCSender& s, juce::OSCReceiver& r)
    : DataSynchronizer (dataToUse),
      addressPattern (formatAddressPattern (dataToUse.getDataIdentifier().toString())),
      oscSender (s),
      oscReceiver (r)
{
    oscReceiver.addListener (this);
}

OscDataSynchronizerBase::~OscDataSynchronizerBase()
{
    oscReceiver.removeListener (this);
}

void OscDataSynchronizerBase::sendChangeData (const void* data, size_t dataSize)
{
    outgoingData.replaceWith (data, dataSize);
    oscSender.send (addressPattern, outgoingData);
}

void OscDataSynchronizerBase::oscMessageReceived (const juce::OSCMessage& message)
{
    if (message.getAddressPattern() != addressPattern || message.isEmpty())
        return;

    auto& arg = message[0];

    if (arg.isBlob())
    {
        auto& block = arg.getBlob();
        applyChangeData (block.getData(), block.getSize());
    }
}


OscDataSynchronizer::OscDataSynchronizer (SerializableData&   dataToUse,
                                                            const String& targetHostName,
                                                            int                 portNumber)
    : OscManager (targetHostName, portNumber),
      sync (dataToUse, sender, receiver)
{
    dataToUse.addDataChild (*this);
}

}  // namespace bav::network
