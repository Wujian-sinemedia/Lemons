#pragma once

#include <juce_core/juce_core.h>

namespace lemons::console
{

using juce::String;

bool executeIfHasOption (const juce::ArgumentList& args, const String& option,
                         std::function<void(String)> function);

template<typename T>
T conditionalExecuteWithReturn (const juce::ArgumentList& args, const String& option,
                                std::function<T(const String&)> functionIfFound,
                                std::function<T()> functionIfNotFound)
{
    if (args.containsOption (option))
        return functionIfFound (args.getValueForOption (option));
    
    return functionIfNotFound();
}

}
