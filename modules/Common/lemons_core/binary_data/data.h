#pragma once

#include <juce_graphics/juce_graphics.h>


namespace lemons::binary
{
/** Represents a wrapper around some data contained in a juce BinaryData target.
    Simply provide the filename and this object will load the data contained by the named resource, if it can be located. \n
    Be sure to check isValid() before attempting to use the data! \n
    Example:
    @code
    Data data {"MyAudioFile.wav"};

    if (data.isValid())
        doSomethingWithData (data.data, data.size);
    @endcode
    If your project doesn't define the LEMONS_HAS_BINARY_DATA macro to 1, every Data object you create will be invalid.
 
    To easily access binary data converted into higher level juce types, several static methods are provided. These methods all throw assertions if the data cannot be loaded.
 */
struct Data
{
	/** Creates a default, invalid RawData object. */
	Data() = default;

	/** Creates a RawData object referencing a named resource. */
	explicit Data (const String& fileToFind);

	/** Returns true if the requested data has been loaded successfully from the binary data target. */
	bool isValid() const noexcept;
    
    /** Returns the data as a UTF-8 formatted string. */
    String getAsString() const;
    
    /** Returns a memory block representing this data. */
    MemoryBlock getAsMemoryBlock() const;
    
	/** The raw data. This may be null if the data cannot be found. */
	const char* data = nullptr;

	/** The size of the data, in bytes. */
	int size { 0 };
    
    
    /** Returns an image object from an image file in the BinaryData target.
        If the image can't be loaded, an assertion will be thrown.
     */
    static Image getImage (const String& imageFileName);
    
    /** Returns an audio buffer object from an audio file in the BinaryData target.
        If the audio can't be loaded, an assertion will be thrown.
     */
    template<typename SampleType>
    static AudioBuffer<SampleType> getAudio (const String& audioFileName);
    
    /** Returns a MIDI buffer object from a MIDI file in the BinaryData target.
        If the MIDI can't be loaded, an assertion will be thrown.
     */
    static MidiBuffer getMidi (const String& midiFileName);
    
    /** Returns an array of strings, each containing a line of a text file in the BinaryData target.
        This loads the file as a string and parses it into tokens using line break and carriage return characters.
        If the file can't be loaded, an assertion will be thrown.
     */
    static juce::StringArray getStrings (const String& textFileName);
    
    /** Returns an opaque blob of binary data from a file in the BinaryData target.
        If the data can't be loaded, an assertion will be thrown.
     */
    static MemoryBlock getBlob (const String& filename);
};

}  // namespace lemons
