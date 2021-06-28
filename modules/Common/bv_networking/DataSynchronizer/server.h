
#pragma once

namespace bav::network
{
class DataSyncClient
{
public:
    DataSyncClient (SerializableData&       dataToUse,
                    std::function< void() > onConnectionLost = {},
                    const String&           hostName         = {},
                    int                     portToUse        = 53100,
                    int                     timeoutMs        = 5000);

    virtual ~DataSyncClient();

    enum Mode
    {
        OSC,
        Socket,
        Pipe
    };

    void setMode (Mode newMode);
    Mode getMode() const;

    void disconnect();

    events::Broadcaster& getBroadcaster() { return broadcaster; }

protected:
    IpcDataSynchronizer ipc;
    OscDataSynchronizer osc;

    String host;

private:
    void disconnected();

    Mode mode {OSC};

    int portNumber;
    int timeout;

    std::function< void() > onDisconnect;
    events::Broadcaster     broadcaster;
};

struct DataSyncServer : DataSyncClient
{
    DataSyncServer (SerializableData&       dataToUse,
                    std::function< void() > onConnectionLost = {},
                    const String&           hostName         = {},
                    int                     portToUse        = 53100,
                    int                     timeoutMs        = 5000);
};

}  // namespace bav::network
