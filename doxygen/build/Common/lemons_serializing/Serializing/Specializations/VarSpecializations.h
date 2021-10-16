
/** @ingroup Serializing
 *  @{
 */

/** @ingroup Specializations
 *  @{
 */
#pragma once

namespace lemons::serializing
{
/** Explicitly calls juce::var::toString(). */
template <>
String fromVar (const juce::var& var);

/** Serializes an Identifier. */
template <>
juce::var toVar (const juce::Identifier& ident);

/** Serializes an Identifier. */
template <>
juce::Identifier fromVar (const juce::var& var);

/** Serializes a size_t. */
template <>
juce::var toVar (const size_t& data);

/** Serializes a size_t. */
template <>
size_t fromVar (const juce::var& var);

/** Serializes a juce BigInteger. */
template <>
juce::var toVar (const juce::BigInteger& bigint);

/** Serializes a juce BigInteger. */
template <>
juce::BigInteger fromVar (const juce::var& var);

/** Serializes a float Point. */
template <>
juce::var toVar (const juce::Point< float >& point);

/** Serializes a float Point. */
template <>
juce::Point< float > fromVar (const juce::var& var);

/** Serializes an integer Point. */
template <>
juce::var toVar (const juce::Point< int >& point);

/** Serializes an integer Point. */
template <>
juce::Point< int > fromVar (const juce::var& var);

/** Serializes a MemoryBlock.
    This works by writing the contents of the memory block to a String use Base64 encoding.
 */
template <>
juce::var toVar (const juce::MemoryBlock& block);

/** Serializes a MemoryBlock.
    This works by reading the contents of a String into binary form, using Base64 encoding.
 */
template <>
juce::MemoryBlock fromVar (const juce::var& var);

/** Serializes a URL as a String. */
template <>
juce::var toVar (const juce::URL& url);

/** Serializes a URL as a String. */
template <>
juce::URL fromVar (const juce::var& var);

/** Serializes an IP address as a String. */
template <>
juce::var toVar (const juce::IPAddress& address);

/** Serializes an IP address as a String. */
template <>
juce::IPAddress fromVar (const juce::var& var);

/** Serializes a std::string. */
template <>
juce::var toVar (const std::string& string);

/** Serializes a std::string. */
template <>
std::string fromVar (const juce::var& var);

/** Serializes a juce::Uuid as a String. */
template <>
juce::var toVar (const juce::Uuid& uuid);

/** Serializes a juce::Uuid as a String. */
template <>
juce::Uuid fromVar (const juce::var& var);

/** Serializes a juce::Time object by saving it as the number of milliseconds since the computing epoch. */
template <>
juce::var toVar (const juce::Time& time);

/** Serializes a juce::Time object by loading it as the number of milliseconds since the computing epoch. */
template <>
juce::Time fromVar (const juce::var& var);

/** Serializes a juce::RelativeTime object. */
template <>
juce::var toVar (const juce::RelativeTime& time);

/** Serializes a juce::RelativeTime object. */
template <>
juce::RelativeTime fromVar (const juce::var& var);

/** Serializes a juce::Image object using the PNG image format. */
template <>
juce::var toVar (const juce::Image& image);

/** Serializes a juce::Image object using the PNG image format. */
template <>
juce::Image fromVar (const juce::var& var);

/** Serializes a juce::MidiBuffer. */
template <>
juce::var toVar (const juce::MidiBuffer& buffer);

/** Serializes a juce::MidiBuffer. */
template <>
juce::MidiBuffer fromVar (const juce::var& var);

/** Serializes an AudioBuffer to a var.
    Internally, this uses juce's FlacAudioFormat to write the buffer's contents into a String.
 */
template <>
juce::var toVar (const AudioBuffer< float >& buffer);

/** Serializes an AudioBuffer to a var.
    Since juce's FlacAudioFormat is only available for float samples, this first converts the buffer to floats and then uses juce::FlacAudioFormat to write the buffer's contents to a String.
 */
template <>
juce::var toVar (const AudioBuffer< double >& buffer);

/** Deserializes an AudioBuffer from a var.
    Internally, this converts the var to a String, then uses juce's FlacAudioFormat to decode the String back into audio samples.
 */
template <>
juce::AudioBuffer< float > fromVar (const juce::var& var);

/** Deserializes an AudioBuffer from a var.
    Internally, this converts the var to a String, then uses juce's FlacAudioFormat to decode the String back into audio samples. \n
    Since juce's FlacAudioFormat is only available for float samples, the buffer is first decoded with float samples and then converted to double samples for the final result.
 */
template <>
juce::AudioBuffer< double > fromVar (const juce::var& var);

/** Serializes a file as its path's String representation.
    This method may be platform-dependant. I recommend using the FileContents struct to reliably serialize files' contents instead of their paths.
 */
template <>
juce::var toVar (const File& file);

/** Serializes a file as its path's String representation.
 This method may be platform-dependant. I recommend using the FileContents struct to reliably serialize files' contents instead of their paths.
 */
template <>
File fromVar (const juce::var& var);

/** A wrapper around a File that, when serializing, loads and saves the file's entire contents.
 */
struct FileContents : SerializableData
{
    /** Must be passed a file or filepath on construction. */
    explicit FileContents (File fileToUse);

    /** Must be passed a file or filepath on construction. */
    explicit FileContents (const String& absolutePathToFile);

    /** The managed file object. */
    File file;

private:
    void serialize (TreeReflector& ref) final;
};

}  // namespace lemons::serializing

/** @}*/

/** @}*/
