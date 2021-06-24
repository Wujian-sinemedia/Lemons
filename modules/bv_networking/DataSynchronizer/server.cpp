
namespace bav::network
{
static inline String getHostName (const String&           inputName,
                                  const SerializableData& data)
{
    if (! inputName.isEmpty()) return inputName;

    return data.getDataIdentifier().toString();
}

DataSyncClient::DataSyncClient (SerializableData&       dataToUse,
                                std::function< void() > onConnectionLost,
                                const String& hostName, int portToUse, int timeoutMs)
    : ipc (dataToUse, [&]
           { disconnected(); }),
      osc (dataToUse, [&]
           { disconnected(); }),
      host (getHostName (hostName, dataToUse)), portNumber (portToUse), timeout (timeoutMs),
      onDisconnect (onConnectionLost)
{
    setMode (Socket);
}

DataSyncClient::~DataSyncClient()
{
    disconnect();
}

void DataSyncClient::disconnected()
{
    broadcaster.trigger();
    onDisconnect();
}

void DataSyncClient::setMode (Mode newMode)
{
    if (mode == newMode)
        return;

    mode = newMode;

    switch (mode)
    {
        case (OSC) :
        {
            osc.connect (host, portNumber);
            ipc.disconnect (-1, juce::InterprocessConnection::Notify::no);
            return;
        }
        case (Socket) :
        {
            ipc.connectToSocket (host, portNumber, timeout);
            osc.disconnect();
            return;
        }
        case (Pipe) :
        {
            ipc.connectToPipe (host, timeout);
            osc.disconnect();
            return;
        }
    }
}

void DataSyncClient::disconnect()
{
    osc.disconnect();
    ipc.disconnect();
}

DataSyncClient::Mode DataSyncClient::getMode() const
{
    return mode;
}


DataSyncServer::DataSyncServer (SerializableData&       dataToUse,
                                std::function< void() > onConnectionLost,
                                const String& hostName, int portToUse, int timeoutMs)
    : DataSyncClient (dataToUse, onConnectionLost, hostName, portToUse, timeoutMs)
{
    ipc.createPipe (host, timeoutMs);
}

}  // namespace bav::network
