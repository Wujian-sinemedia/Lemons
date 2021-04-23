/** Async Download. Doesn't have the main thread pause the URL::downloadToFile has
 */
class AsyncDownload : private juce::Thread,
                      private juce::AsyncUpdater
{
public:
    AsyncDownload (juce::String url_, std::function<void (AsyncDownload*, juce::MemoryBlock, bool)> cb_, int timeoutMS_ = 0)
    : Thread ("AsyncDownload"), url (url_), cb (cb_), timeoutMS (timeoutMS_)
    {
        startThread();
    }
    
    AsyncDownload (juce::URL url_, std::function<void (AsyncDownload*, juce::MemoryBlock, bool)> cb_, int timeoutMS_ = 0)
    : Thread ("AsyncDownload"), url (url_), cb (cb_), timeoutMS (timeoutMS_)
    {
        startThread();
    }
    
    ~AsyncDownload() override
    {
        stopThread (100);
    }
    
    void run() override
    {
        ok = readEntireBinaryStream (data);
        handleAsyncUpdate();
    }

    bool readEntireBinaryStream (juce::MemoryBlock& destData, bool usePostCommand = false)
    {
        const std::unique_ptr<juce::InputStream> input (url.isLocalFile() ? url.getLocalFile().createInputStream() : url.createInputStream (usePostCommand, nullptr, nullptr, {}, timeoutMS));
        
        if (input != nullptr)
        {
            input->readIntoMemoryBlock (destData);
            return true;
        }
        
        return false;
    }
    
    void handleAsyncUpdate() override
    {
        if (cb)
            cb (this, data, ok);
    }
    
    juce::URL url;
    std::function<void (AsyncDownload*, juce::MemoryBlock, bool)> cb;
    int timeoutMS = 0;
    bool ok = false;
    juce::MemoryBlock data;
};

