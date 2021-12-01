namespace lemons::console
{

bool executeIfHasOption (const juce::ArgumentList& args, const String& option,
                         std::function<void(String)> function)
{
    if (! args.containsOption (option))
        return false;
    
    function (args.getValueForOption (option));
    
    return true;
}

}
