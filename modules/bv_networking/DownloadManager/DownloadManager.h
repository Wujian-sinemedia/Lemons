
#pragma once

/**
 Downloads files to a memory block and then calls a lambda on the message thread with the results. Does not block the message thread while establishing the HTTP connect like juce::URL::downloadToFile
 */
class DownloadManager
{
public:
    //==============================================================================
    DownloadManager (int connectTimeoutMs  = 30 * 1000,
                     int shutdownTimeoutMs = 30 * 1000)
        : connectTimeout (connectTimeoutMs), shutdownTimeout (shutdownTimeoutMs)
    {
    }

    virtual ~DownloadManager() { cancelAllDownloads(); }

    /** This callback is called every time all downloads are finished */
    void setQueueFinishedCallback (std::function< void() > callback)
    {
        queueFinishedCallback = callback;
    }

    /** How long connecting is given before it times out */
    void setConnectTimeout (int timeout) { connectTimeout = timeout; }

    /** If a download fails, how long many times it should retry */
    void setRetryLimit (int limit) { retryLimit = limit; }

    /** If a download fails, how long to wait until trying again */
    void setRetryDelay (double seconds) { retryDelay = seconds; }

    /** Maximum number of downloads to allow at once */
    void setConcurrentDownloadLimit (int limit)
    {
        maxDownloads = limit;
        triggerNextDownload();
    }

    /** Number of items in download queue */
    int getNumberOfDownloads() { return downloads.size(); }

    /** Set download thread priority. Does not affect priority of
     already running threads */
    void setThreadPriority (int p) { priority = p; }

    /** Sets minimum time between download progress callbacks in milliseconds */
    void setProgressInterval (int ms) { downloadIntervalMS = std::max (1, ms); }

    /** Sets the block size of chunks to download. Progress callbacks and
     cancelling downloads can only happen between these blocks. Max size is 128 KB */
    void setDownloadBlockSize (int bs)
    {
        downloadBlockSize = juce::jlimit (1, 128 * 1000, bs);
    }

    int getNumDownloadsInQueue() { return downloads.size(); }

    /** If enabled, will request the server sends the data compressed
     This only has effect on windows. On macOS it is handled by the system libraries
     and is always on.
     */
    void enableGzipDeflate (bool e) { gzipDeflate = e; }

    /** Pause / resume all downloads. This actually stops any running downloads
     and then restarts them when unpaused. You will loose some downloaded data
     that will need to be redownloaded. */
    void pauseDownloads (bool shouldBePaused) { pause = shouldBePaused; }

    //==============================================================================
    struct DownloadResult
    {
        juce::URL url;
        int       downloadId = 0;
        int       attempts   = 0;

        juce::MemoryBlock data;

        bool                  ok       = false;
        int                   httpCode = 0;
        juce::StringPairArray responseHeaders;
    };

    //==============================================================================
    /** Starts a download and returns the download id which can be used to cancel the download
     progressCallback returns current amount downloaded, total amount to download, and amount
     downloaded since last callback. Note that for http chunk encoding total size is unknown
     and will be maximum int64 value.
     */
    int startAsyncDownload (
        juce::String url,
        juce::String postData,
        std::function< void (DownloadResult) >
            completionCallback,
        std::function< void (juce::int64, juce::int64, juce::int64) >
                     progressCallback = nullptr,
        juce::String extraHeaders     = {})
    {
        return startAsyncDownload (juce::URL (url).withPOSTData (postData),
                                   completionCallback,
                                   progressCallback,
                                   extraHeaders);
    }

    int startAsyncDownload (
        juce::URL url,
        std::function< void (DownloadResult) >
            completionCallback,
        std::function< void (juce::int64, juce::int64, juce::int64) >
                     progressCallback = nullptr,
        juce::String extraHeaders     = {})
    {
        return startAsyncDownloadPriv (
            url, completionCallback, progressCallback, extraHeaders);
    }


    /** Cancels all downloads */
    void cancelAllDownloads()
    {
        runningDownloads = 0;
        downloads.clear();
    }

    /** Cancels a download with a given id */
    void cancelDownload (int downloadId) { cancelDownloadPriv (downloadId); }


    //==============================================================================
    DownloadResult blockingDownload (juce::String url,
                                     juce::String postData,
                                     juce::String extraHeaders = {})
    {
        return blockingDownload (juce::URL (url).withPOSTData (postData),
                                 extraHeaders);
    }

    DownloadResult blockingDownload (juce::URL url, juce::String extraHeaders = {})
    {
        return blockingDownloadPriv (url, extraHeaders);
    }


private:
    //==============================================================================
    /** Manages a download on a background thread */
    struct Download : public juce::Thread
    {
        Download (DownloadManager& o)
            : Thread ("DownloadManager::Download"), owner (o)
        {
        }

        ~Download() override
        {
            // Cancel any blocking reads
            if (is != nullptr) is->cancel();

            // Wait a long time before cancelling, WebInputStream could be stuck in connect. Unlikely but possible.
            if (async) stopThread (owner.shutdownTimeout);
        }

        void run() override
        {
            int attemps = owner.retryLimit + 1;

            while (attemps-- > 0 && ! threadShouldExit())
            {
                result.attempts++;

                if (tryDownload()) break;

                if (owner.retryDelay > 0)
                    wait (juce::roundToInt (owner.retryDelay * 1000));

                while (owner.pause.load())
                    wait (500);
            }

            if (async && ! threadShouldExit())
            {
                // Get a weak reference to self, to check if we get deleted before async call happens.
                juce::WeakReference< Download > myself = this;
                juce::MessageManager::callAsync (
                    [myself]
                    {
                        if (myself != nullptr)
                            myself->completionCallback (myself->result);
                        if (myself != nullptr)
                            myself->owner.downloadFinished (myself);
                    });
            }
        }

        bool tryDownload()
        {
            // Use post if we have post data
            const bool post = result.url.getPostData().isNotEmpty();

            if ((is = std::make_unique< juce::WebInputStream > (result.url, post))
                != nullptr)
            {
                if (headers.isNotEmpty()) is->withExtraHeaders (headers);

                is->withConnectionTimeout (owner.connectTimeout);

                if (is->connect (nullptr))
                {
                    // Save headers and http response code
                    result.httpCode        = is->getStatusCode();
                    result.responseHeaders = is->getResponseHeaders();

                    auto keys = result.responseHeaders.getAllKeys();
                    auto vals = result.responseHeaders.getAllValues();

                    juce::MemoryOutputStream os (result.data, false);

                    lastBytesSent           = 0;
                    lastProgress            = juce::Time::getMillisecondCounter();
                    juce::int64 downloaded  = 0;
                    juce::int64 totalLength = is->getTotalLength();

                    // For chunked http encoding, overall length may not be given
                    if (totalLength < 0)
                        totalLength = std::numeric_limits< juce::int64 >::max();

                    // Download all data
                    char buffer[128 * 1000];
                    while (! is->isExhausted() && ! threadShouldExit()
                           && downloaded < totalLength)
                    {
                        juce::int64 toRead =
                            juce::jmin (juce::int64 (sizeof (buffer)),
                                        juce::int64 (owner.downloadBlockSize),
                                        totalLength - downloaded);

                        int read = is->read (buffer, int (toRead));

                        if (owner.pause.load())
                        {
                            result.ok = false;
                            break;
                        }
                        else if (read > 0)
                        {
                            os.write (buffer, size_t (read));
                            downloaded += read;
                            result.ok =
                                (is->isExhausted() || downloaded == totalLength)
                                && result.httpCode == 200;

                            updateProgress (downloaded, totalLength, false);
                        }
                        else if (read == 0 && is->isExhausted())
                        {
                            // For chunked encoding, assume we have it all, otherwise check the length
                            if (totalLength
                                < std::numeric_limits< juce::int64 >::max())
                                result.ok = (totalLength == downloaded)
                                         && result.httpCode == 200;
                            else
                                result.ok = result.httpCode == 200;

                            break;
                        }
                        else
                        {
                            result.ok = false;
                            break;
                        }
                    }

                    updateProgress (downloaded, totalLength, true);
                }
            }

#if JUCE_WINDOWS
            // Decompress the gzip encoded data. This happens automatically on macOS
            if (result.ok && result.responseHeaders["Content-Encoding"] == "gzip")
            {
                juce::MemoryInputStream           mis (result.data, true);
                juce::GZIPDecompressorInputStream gis (
                    &mis, false, juce::GZIPDecompressorInputStream::gzipFormat);

                result.data.reset();

                while (! gis.isExhausted())
                {
                    char buffer[10 * 1024];
                    int  read = gis.read (buffer, sizeof (buffer));
                    if (read > 0) result.data.append (buffer, size_t (read));
                }
            }
#endif

            return result.ok;
        }

        void updateProgress (juce::int64 current,
                             juce::int64 total,
                             bool        forceNotification)
        {
            if (progressCallback)
            {
                // Update progress no more than once per second
                auto now = juce::Time::getMillisecondCounter();
                if ((now >= lastProgress + uint32_t (owner.downloadIntervalMS))
                    || forceNotification)
                {
                    juce::int64 delta = current - lastBytesSent;
                    lastBytesSent     = current;
                    lastProgress      = now;

                    if (delta > 0)
                    {
                        // Get a weak reference to self, to check if we get deleted before async call happens.
                        juce::WeakReference< Download > myself = this;
                        juce::MessageManager::callAsync (
                            [myself, current, total, delta]
                            {
                                if (myself != nullptr)
                                    myself->progressCallback (current, total, delta);
                            });
                    }
                }
            }
        }

        //==============================================================================
        DownloadResult                         result;
        std::function< void (DownloadResult) > completionCallback;
        std::function< void (juce::int64, juce::int64, juce::int64) >
            progressCallback;

        std::unique_ptr< juce::WebInputStream > is;

        DownloadManager& owner;

        juce::String headers;
        bool         started = false, async = true;
        juce::uint32 lastProgress  = 0;
        juce::int64  lastBytesSent = 0;

        //==============================================================================
        JUCE_DECLARE_WEAK_REFERENCEABLE (Download)
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Download)
    };

    void downloadFinished (Download* download)
    {
        runningDownloads--;
        downloads.removeObject (download);

        triggerNextDownload();

        if (downloads.size() == 0 && queueFinishedCallback) queueFinishedCallback();
    }

    void triggerNextDownload()
    {
        for (int i = 0; i < downloads.size() && runningDownloads < maxDownloads; i++)
        {
            auto d = downloads[i];
            if (! d->started)
            {
                runningDownloads++;
                d->started = true;
                d->startThread (priority);
            }
        }
    }

    //==============================================================================
    int nextId          = 0;
    int connectTimeout  = 30 * 1000;
    int shutdownTimeout = 30 * 1000;
    int retryLimit = 0, priority = 5, downloadIntervalMS = 1000,
        downloadBlockSize = 128 * 1000;

    double                       retryDelay       = 0.0;
    int                          runningDownloads = 0, maxDownloads = 100;
    juce::OwnedArray< Download > downloads;
    std::function< void() >      queueFinishedCallback;
    bool                         gzipDeflate = true;
    std::atomic< bool >          pause;

    //==============================================================================

    DownloadResult blockingDownloadPriv (juce::URL url, juce::String extraHeaders)
    {
#if JUCE_WINDOWS
        auto headerList = juce::StringArray::fromTokens (extraHeaders, "\n", "");
        headerList.add ("Accept-Encoding: gzip");
        extraHeaders = headerList.joinIntoString ("\n");
#endif

        Download download (*this);
        download.async              = false;
        download.result.url         = url;
        download.headers            = extraHeaders;
        download.result.downloadId  = 0;
        download.completionCallback = nullptr;
        download.progressCallback   = nullptr;

        download.run();

        return download.result;
    }


    int startAsyncDownloadPriv (
        juce::URL url,
        std::function< void (DownloadResult) >
            completionCallback,
        std::function< void (juce::int64, juce::int64, juce::int64) >
                     progressCallback,
        juce::String extraHeaders)
    {
#if JUCE_WINDOWS
        // macOS does this automatically
        if (gzipDeflate)
        {
            auto headerList = juce::StringArray::fromTokens (extraHeaders, "\n", "");
            headerList.add ("Accept-Encoding: gzip");
            extraHeaders = headerList.joinIntoString ("\n");
        }
#endif

        auto download                = new Download (*this);
        download->result.url         = url;
        download->headers            = extraHeaders;
        download->result.downloadId  = ++nextId;
        download->completionCallback = completionCallback;
        download->progressCallback   = progressCallback;

        downloads.add (download);

        triggerNextDownload();

        return download->result.downloadId;
    }


    void cancelDownloadPriv (int downloadId)
    {
        for (int i = downloads.size(); --i >= 0;)
        {
            if (downloads[i]->result.downloadId == downloadId)
            {
                if (downloads[i]->isThreadRunning()) runningDownloads--;

                downloads.remove (i);
                triggerNextDownload();

                if (downloads.size() == 0 && queueFinishedCallback)
                    queueFinishedCallback();

                break;
            }
        }
    }


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DownloadManager)
};
