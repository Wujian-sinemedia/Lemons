
namespace lemons::serializing
{
template <>
String fromVar (juce::var var)
{
    return var.toString();
}

template <>
juce::var toVar (std::string& string)
{
    String juceString {string};
    return {juceString};
}

template <>
std::string fromVar (juce::var var)
{
    return var.toString().toStdString();
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
juce::var toVar (juce::MemoryBlock& block)
{
    return {memoryBlockToString (block)};
}

template <>
juce::MemoryBlock fromVar (juce::var var)
{
    return memoryBlockFromString (var.toString());
}

/*-------------------------------------------------------------------------------------------*/

template <>
juce::var toVar (juce::URL& url)
{
    return url.toString (false);
}

template <>
juce::URL fromVar (juce::var var)
{
    return {var.toString()};
}

template <>
juce::var toVar (juce::Uuid& uuid)
{
    return {uuid.toString()};
}

template <>
juce::Uuid fromVar (juce::var var)
{
    return {var.toString()};
}

template <>
juce::var toVar (juce::Time& time)
{
    return {time.toMilliseconds()};
}

template <>
juce::Time fromVar (juce::var var)
{
    juce::Time time {(juce::int64) var};
    return time;
}

template <>
juce::var toVar (juce::RelativeTime& time)
{
    return {time.inSeconds()};
}

template <>
juce::RelativeTime fromVar (juce::var var)
{
    juce::RelativeTime time {(double) var};
    return time;
}


/*--------------------------------------- Images -------------------------------------------*/

template <>
juce::var toVar (juce::Image& image)
{
    juce::MemoryBlock        block;
    juce::MemoryOutputStream stream {block, false};
    juce::PNGImageFormat     format;

    format.writeImageToStream (image, stream);

    return {memoryBlockToString (block)};
}

template <>
juce::Image fromVar (juce::var var)
{
    auto                    block = memoryBlockFromString (var.toString());
    juce::MemoryInputStream stream {block, false};
    juce::PNGImageFormat    format;

    return format.decodeImage (stream);
}


/*--------------------------------------- Points -------------------------------------------*/

template < typename Type >
inline String pointToString (const juce::Point< Type >& point)
{
    return String (point.x) + "_" + String (point.y);
}

template < typename Type >
inline Type pointTokenToValue (const String& token)
{
    if constexpr (std::is_same_v< Type, int >)
        return token.getIntValue();
    else
        return static_cast< Type > (token.getFloatValue());
}

template < typename Type >
inline juce::Point< Type > pointFromString (const String& string)
{
    juce::Point< Type > point;

    point.x = pointTokenToValue< Type > (string.upToFirstOccurrenceOf ("_", false, true));
    point.y = pointTokenToValue< Type > (string.fromFirstOccurrenceOf ("_", false, true));

    return point;
}

template <>
juce::var toVar (juce::Point< float >& point)
{
    return pointToString (point);
}

template <>
juce::Point< float > fromVar (juce::var var)
{
    const auto string = var.toString();

    return pointFromString< float > (string);
}

template <>
juce::var toVar (juce::Point< int >& point)
{
    return pointToString (point);
}

template <>
juce::Point< int > fromVar (juce::var var)
{
    const auto string = var.toString();

    return pointFromString< int > (string);
}


/*--------------------------------------- Midi buffers -------------------------------------------*/

static inline juce::MidiMessageSequence midiBufferToSequence (const juce::MidiBuffer& buffer)
{
    juce::MidiMessageSequence sequence;

    for (auto meta : buffer)
    {
        sequence.addEvent (meta.getMessage());
    }

    return sequence;
}

static inline juce::MidiBuffer midiSequenceToBuffer (const juce::MidiMessageSequence& sequence)
{
    juce::MidiBuffer buffer;

    for (const auto* holder : sequence)
    {
        buffer.addEvent (holder->message,
                         juce::roundToInt (holder->message.getTimeStamp()));
    }

    return buffer;
}

template <>
juce::var toVar (juce::MidiBuffer& buffer)
{
    juce::MidiFile file;

    file.addTrack (midiBufferToSequence (buffer));

    juce::MemoryBlock        block;
    juce::MemoryOutputStream stream {block, false};

    file.writeTo (stream);

    return {memoryBlockToString (block)};
}


template <>
juce::MidiBuffer fromVar (juce::var var)
{
    auto                    block = memoryBlockFromString (var.toString());
    juce::MemoryInputStream stream {block, false};

    juce::MidiFile file;
    file.readFrom (stream);

    if (auto* track = file.getTrack (1))
    {
        return midiSequenceToBuffer (*track);
    }

    return {};
}

}  // namespace lemons::serializing
