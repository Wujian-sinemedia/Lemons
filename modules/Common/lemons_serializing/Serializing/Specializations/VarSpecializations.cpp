
namespace lemons::serializing
{
template <>
String fromVar (const juce::var& var)
{
    return var.toString();
}

template <>
juce::var toVar (const std::string& string)
{
    String juceString {string};
    return {juceString};
}

template <>
std::string fromVar (const juce::var& var)
{
    return var.toString().toStdString();
}

template <>
juce::var toVar (const size_t& data)
{
    return {static_cast< int > (data)};
}

template <>
size_t fromVar (const juce::var& var)
{
    return static_cast< size_t > ((int) var);
}

template <>
juce::var toVar (const juce::Identifier& ident)
{
    return ident.toString();
}

template <>
juce::Identifier fromVar (const juce::var& var)
{
    return {var.toString()};
}

template <>
juce::var toVar (const juce::BigInteger& bigint)
{
    return {bigint.toInt64()};
}

template <>
juce::BigInteger fromVar (const juce::var& var)
{
    return {(juce::int64) var};
}

/*--------------------------------------- Memory blocks -------------------------------------------*/

static inline String memoryBlockToString (const juce::MemoryBlock& block)
{
    return block.toBase64Encoding();
}

static inline juce::MemoryBlock memoryBlockFromString (const juce::String& string)
{
    juce::MemoryBlock block;
    block.fromBase64Encoding (string);
    return block;
}

template <>
juce::var toVar (const juce::MemoryBlock& block)
{
    return {memoryBlockToString (block)};
}

template <>
juce::MemoryBlock fromVar (const juce::var& var)
{
    return memoryBlockFromString (var.toString());
}

/*-------------------------------------------------------------------------------------------*/

template <>
juce::var toVar (const juce::URL& url)
{
    return url.toString (false);
}

template <>
juce::URL fromVar (const juce::var& var)
{
    return {var.toString()};
}

template <>
juce::var toVar (const juce::IPAddress& address)
{
    return address.toString();
}

template <>
juce::IPAddress fromVar (const juce::var& var)
{
    juce::IPAddress newAddress {var.toString()};
    return newAddress;
}

template <>
juce::var toVar (const juce::Uuid& uuid)
{
    return {uuid.toString()};
}

template <>
juce::Uuid fromVar (const juce::var& var)
{
    return {var.toString()};
}

template <>
juce::var toVar (const juce::Time& time)
{
    return {time.toMilliseconds()};
}

template <>
juce::Time fromVar (const juce::var& var)
{
    juce::Time time {(juce::int64) var};
    return time;
}

template <>
juce::var toVar (const juce::RelativeTime& time)
{
    return {time.inSeconds()};
}

template <>
juce::RelativeTime fromVar (const juce::var& var)
{
    juce::RelativeTime time {(double) var};
    return time;
}

/*--------------------------------------- Files -------------------------------------------*/

template <>
juce::var toVar (const File& file)
{
    return {file.getFullPathName()};
}

template <>
File fromVar (const juce::var& var)
{
    return {var.toString()};
}

FileContents::FileContents (File fileToUse)
    : file (fileToUse)
{
    jassert (file.existsAsFile());
}

FileContents::FileContents (const String& absolutePathToFile)
    : file (absolutePathToFile)
{
    jassert (file.existsAsFile());
}

void FileContents::serialize (TreeReflector& ref)
{
    ref.addLambdaSet< String > (
        "FileContents",
        [&]
        { return file.loadFileAsString(); },
        [&] (String& s)
        { file.replaceWithText (s); });
}

/*--------------------------------------- Images -------------------------------------------*/

template <>
juce::var toVar (const juce::Image& image)
{
    juce::MemoryBlock        block;
    juce::MemoryOutputStream stream {block, false};
    juce::PNGImageFormat     format;

    format.writeImageToStream (image, stream);

    return {memoryBlockToString (block)};
}

template <>
juce::Image fromVar (const juce::var& var)
{
    auto                    block = memoryBlockFromString (var.toString());
    juce::MemoryInputStream stream {block, false};
    juce::PNGImageFormat    format;

    return format.decodeImage (stream);
}


/*--------------------------------------- Points -------------------------------------------*/

constexpr auto POINT_STRING_SEP_CHAR = "_";

template < typename Type >
inline String pointToString (const juce::Point< Type >& point)
{
    return String (point.x) + POINT_STRING_SEP_CHAR + String (point.y);
}

template < typename Type >
inline juce::Point< Type > pointFromString (const String& string)
{
    auto pointTokenToValue = [] (const String& token) -> Type
    {
        if constexpr (std::is_same_v< Type, int >)
            return token.getIntValue();
        else
            return static_cast< Type > (token.getFloatValue());
    };

    return {pointTokenToValue (string.upToFirstOccurrenceOf (POINT_STRING_SEP_CHAR, false, true)),
            pointTokenToValue (string.fromFirstOccurrenceOf (POINT_STRING_SEP_CHAR, false, true))};
}

template <>
juce::var toVar (const juce::Point< float >& point)
{
    return pointToString (point);
}

template <>
juce::Point< float > fromVar (const juce::var& var)
{
    return pointFromString< float > (var.toString());
}

template <>
juce::var toVar (const juce::Point< int >& point)
{
    return pointToString (point);
}

template <>
juce::Point< int > fromVar (const juce::var& var)
{
    return pointFromString< int > (var.toString());
}


/*--------------------------------------- Audio buffers -------------------------------------------*/

static inline juce::MemoryBlock bufferToMemoryBlock (const AudioBuffer< float >& buffer)
{
    juce::MemoryBlock        block;
    juce::MemoryOutputStream stream {block, false};
    juce::FlacAudioFormat    format;

    if (auto* writer = format.createWriterFor (&stream, 48000, (unsigned) buffer.getNumChannels(), 24, {}, 0))
        writer->writeFromAudioSampleBuffer (buffer, 0, buffer.getNumSamples());

    return block;
}

static inline AudioBuffer< float > memoryBlockToBuffer (const juce::MemoryBlock& mem)
{
    AudioBuffer< float > buffer;

    juce::MemoryInputStream in {mem.getData(), mem.getSize(), false};
    juce::FlacAudioFormat   format;

    if (auto* reader = format.createReaderFor (&in, false))
    {
        const auto numChannels = static_cast< int > (reader->numChannels);
        const auto numSamples  = static_cast< int > (reader->lengthInSamples);

        buffer.setSize (numChannels, numSamples);
        reader->read (&buffer, 0, numSamples, 0, true, numChannels > 1);
    }

    return buffer;
}

template <>
juce::var toVar (const AudioBuffer< float >& buffer)
{
    return memoryBlockToString (bufferToMemoryBlock (buffer));
}

template <>
juce::var toVar (const AudioBuffer< double >& buffer)
{
    const auto block = [&]() -> juce::MemoryBlock
    {
        const auto numSamples  = buffer.getNumSamples();
        const auto numChannels = buffer.getNumChannels();

        AudioBuffer< float > temp {numSamples, numChannels};

        for (int chan = 0; chan < numChannels; ++chan)
            vecops::convert (temp.getWritePointer (chan), buffer.getReadPointer (chan), numSamples);

        return bufferToMemoryBlock (temp);
    }();

    return memoryBlockToString (block);
}


template <>
AudioBuffer< float > fromVar (const juce::var& var)
{
    return memoryBlockToBuffer (memoryBlockFromString (var.toString()));
}

template <>
AudioBuffer< double > fromVar (const juce::var& var)
{
    const auto floatBuf = memoryBlockToBuffer (memoryBlockFromString (var.toString()));

    const auto numSamples  = floatBuf.getNumSamples();
    const auto numChannels = floatBuf.getNumChannels();

    juce::AudioBuffer< double > doubleBuf {numSamples, numChannels};

    for (int chan = 0; chan < numChannels; ++chan)
        vecops::convert (doubleBuf.getWritePointer (chan), floatBuf.getReadPointer (chan), numSamples);

    return doubleBuf;
}


/*--------------------------------------- Midi buffers -------------------------------------------*/

template <>
juce::var toVar (const juce::MidiBuffer& buffer)
{
    juce::MidiFile file;

    const auto sequence = [buffer]() -> juce::MidiMessageSequence
    {
        juce::MidiMessageSequence seq;

        for (auto meta : buffer)
            seq.addEvent (meta.getMessage());

        return seq;
    }();

    file.addTrack (sequence);

    juce::MemoryBlock        block;
    juce::MemoryOutputStream stream {block, false};

    file.writeTo (stream);

    return {memoryBlockToString (block)};
}


template <>
juce::MidiBuffer fromVar (const juce::var& var)
{
    auto                    block = memoryBlockFromString (var.toString());
    juce::MemoryInputStream stream {block, false};

    juce::MidiFile file;
    file.readFrom (stream);

    if (auto* track = file.getTrack (0))
    {
        return [sequence = *track]() -> juce::MidiBuffer
        {
            juce::MidiBuffer buffer;

            for (const auto* holder : sequence)
                buffer.addEvent (holder->message,
                                 juce::roundToInt (holder->message.getTimeStamp()));

            return buffer;
        }();
    }

    return {};
}

}  // namespace lemons::serializing
