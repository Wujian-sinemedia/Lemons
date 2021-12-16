/*
 ======================================================================================
 
 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 
 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 
 ======================================================================================
 */


#pragma once

#include <juce_graphics/juce_graphics.h>


namespace lemons::binary
{

using juce::String;
using juce::Image;
using juce::MemoryBlock;
using juce::MidiBuffer;

template <typename T>
using AudioBuffer = juce::AudioBuffer<T>;

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
struct Data final
{
	/** Creates a default, invalid Data object. */
	explicit Data() = default;

	/** Creates a Data object referencing a named resource. */
	explicit Data (const String& fileToFind);

	/** Destructor. */
	~Data() = default;

	/** Returns true if the requested data has been loaded successfully from the binary data target. */
	bool isValid() const noexcept;

	/** Returns the data as a UTF-8 formatted string. */
	[[nodiscard]] String getAsString() const;

	/** Returns a memory block representing this data. */
	[[nodiscard]] MemoryBlock getAsMemoryBlock() const;

	/** The raw data. This may be null if the data cannot be found. */
    const char* data { nullptr };

	/** The size of the data, in bytes. */
	int size { 0 };

	/** Returns true if this module was compiled with the LEMONS_HAS_BINARY_DATA flag set to 1.
	    If this returns false, every Data object you create will be invalid.
	 */
    [[nodiscard]] static constexpr bool hasBinaryData() noexcept;
    
    /** Returns an opaque blob of binary data from a file in the BinaryData target.
        If the data can't be loaded, an assertion will be thrown.
     */
    [[nodiscard]] static MemoryBlock getBlob (const String& filename);

	/** Returns an image object from an image file in the BinaryData target.
	    If the image can't be loaded, an assertion will be thrown.
	 */
	[[nodiscard]] static Image getImage (const String& imageFileName);

	/** Returns an audio file from the BinaryData target. If the audio can't be loaded, an assertion will be thrown.
	    @see AudioFile
	 */
	[[nodiscard]] static AudioFile getAudio (const String& audioFileName);

	/** Returns a MIDI buffer object from a MIDI file in the BinaryData target.
	    If the MIDI can't be loaded, an assertion will be thrown.
	 */
	[[nodiscard]] static MidiBuffer getMidi (const String& midiFileName);

	/** Returns a text file in the BinaryData target as one string.
	    If the data can't be loaded, an assertion will be thrown.
	    @see getStrings()
	 */
	[[nodiscard]] static String getString (const String& textFileName);

	/** Returns an array of strings, each containing a line of a text file in the BinaryData target.
	    This loads the file as a string and parses it into tokens using line break and carriage return characters.
	    If the file can't be loaded, an assertion will be thrown.
	    @see getString()
	 */
	[[nodiscard]] static juce::StringArray getStrings (const String& textFileName);
    
    /** Returns a font that has been previously serialized with the gui::serializeFont() method.
        If the font can't be loaded, an assertion will be thrown.
        @see gui::serializeFont()
     */
    [[nodiscard]] static std::unique_ptr<juce::CustomTypeface> getFont (const String& filename);
};

}  // namespace lemons::binary


/*---------------------------------------------------------------------------------------------------------------------------------*/

#if 0

namespace lemons::tests
{

struct BinaryDataTests : public Test
{
    BinaryDataTests();

private:
    void runTest() final;
};

}

#  if LEMONS_BINARIES_UNIT_TESTS
#	if ! LEMONS_HAS_BINARY_DATA
#	  error "LEMONS_BINARIES_UNIT_TESTS is enabled, but LEMONS_HAS_BINARY_DATA is false!"
#	endif
#  endif

#endif
