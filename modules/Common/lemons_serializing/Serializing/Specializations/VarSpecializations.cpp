
namespace lemons::serializing
{
template <>
String fromVar (juce::var var)
{
    return var.toString();
}

template <>
juce::var toVar (juce::Point< float >& point)
{
    return String (point.x) + "_" + String (point.y);
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

}  // namespace lemons::serializing
