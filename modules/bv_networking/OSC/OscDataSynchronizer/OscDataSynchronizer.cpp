
namespace bav::network
{
juce::OSCAddressPattern formatAddressPattern (const String& address)
{
    jassert (! address.isEmpty());
    return String ("/") + address.trim().replaceCharacters (" ", "/");
}

OscDataSynchronizerBase::OscDataSynchronizerBase (SerializableData&  dataToUse,
                                                  juce::OSCSender&   s,
                                                  juce::OSCReceiver& r,
                                                  std::function< void() >
                                                      onConnectionLost)
    : DataSynchronizer (dataToUse),
      OscListener (r),
      addressPattern (formatAddressPattern (dataToUse.getDataIdentifier().toString())),
      oscSender (s), connectionChecker (s, r), connectionListener (connectionChecker.getBroadcaster(), onConnectionLost)
{
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

OscDataSynchronizer::OscDataSynchronizer (SerializableData& dataToUse,
                                          std::function< void() >
                                              onConnectionLost)
    : OscManager (dataToUse.getDataIdentifier().toString()),
sync (dataToUse, sender, receiver, onConnectionLost)
{
    dataToUse.addDataChild (*this);
}

}  // namespace bav::network
