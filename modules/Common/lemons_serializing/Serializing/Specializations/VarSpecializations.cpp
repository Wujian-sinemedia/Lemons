
namespace lemons::serializing
{
template <>
String fromVar (juce::var var)
{
    return var.toString();
}

template < typename Type >
inline String pointToString (const juce::Point< Type >& point)
{
    return String (point.x) + "_" + String (point.y);
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

    juce::Point< float > point;

    point.x = string.upToFirstOccurrenceOf ("_", false, true).getFloatValue();
    point.y = string.fromFirstOccurrenceOf ("_", false, true).getFloatValue();

    return point;
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

    juce::Point< int > point;

    point.x = string.upToFirstOccurrenceOf ("_", false, true).getIntValue();
    point.y = string.fromFirstOccurrenceOf ("_", false, true).getIntValue();

    return point;
}

template <>
juce::var toVar (juce::MemoryBlock& block)
{
    return block.toBase64Encoding();
}

template <>
juce::MemoryBlock fromVar (juce::var var)
{
    juce::MemoryBlock block;
    block.fromBase64Encoding (var.toString());
    return block;
}

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

}  // namespace lemons::serializing
