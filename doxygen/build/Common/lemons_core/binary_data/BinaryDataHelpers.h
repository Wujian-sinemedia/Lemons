
/** @ingroup binary_data
 *  @{
 */

#pragma once


namespace lemons
{
/** Represents a wrapper around some data contained in a juce BinaryData target.
    Simply provide the filename and this object will load the data contained by the named resource, if it can be located. \n
    Be sure to check isValid() before attempting to use the data! \n
    Example:
    @code
    RawData data {"MyAudioFile.wav"};

    if (data.isValid())
        doSomethingWithData (data.data, data.size);
    @endcode
    If your project doesn't define the LEMONS_HAS_BINARY_DATA macro to 1, every RawData object you create will be invalid.
 */
struct RawData
{
    /** Creates a default, invalid RawData object. */
    RawData() = default;

    /** Creates a RawData object referencing a named resource. */
    explicit RawData (const String& fileToFind);

    /** Creates a RawData object referencing a named resource. */
    explicit RawData (const char* fileToFind);

    /** Returns true if the requested data has been loaded successfully from the binary data target. */
    bool isValid() const;

    /** The raw data. */
    const char* data = nullptr;

    /** The size of the data, in bytes. */
    int size {0};
};


}  // namespace lemons

/** @}*/
