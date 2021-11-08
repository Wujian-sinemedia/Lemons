#include <juce_core/juce_core.h>

//==============================================================================
class ConsoleLogger : public juce::Logger
{
    void logMessage (const juce::String& message) final
    {
        std::cout << message << std::endl;

       #if JUCE_WINDOWS
        juce::Logger::outputDebugString (message);
       #endif
    }
};

//==============================================================================
class ConsoleUnitTestRunner : public juce::UnitTestRunner
{
    void logMessage (const juce::String& message) final
    {
        juce::Logger::writeToLog (message);
    }
};


//==============================================================================

static constexpr auto TEST_EXIT_SUCCESS = 0;
static constexpr auto TEST_EXIT_FAILURE = 1;

int main (int argc, char **argv)
{
    juce::ArgumentList args (argc, argv);

    if (args.containsOption ("--help|-h"))
    {
        std::cout << argv[0] << " [--help|-h] [--list-categories] [--category category] [--seed seed]" << std::endl;
        return TEST_EXIT_SUCCESS;
    }

    if (args.containsOption ("--list-categories"))
    {
        for (auto& category : juce::UnitTest::getAllCategories())
            std::cout << category << std::endl;

        return TEST_EXIT_SUCCESS;
    }

    ConsoleLogger logger;
    juce::Logger::setCurrentLogger (&logger);

    ConsoleUnitTestRunner runner;

    const auto seed = [&args]
    {
        if (args.containsOption ("--seed"))
        {
            const auto seedValueString = args.getValueForOption ("--seed");

            if (seedValueString.startsWith ("0x"))
                return seedValueString.getHexValue64();

            return seedValueString.getLargeIntValue();
        }

        return juce::Random::getSystemRandom().nextInt64();
    }();

    if (args.containsOption ("--category"))
        runner.runTestsInCategory (args.getValueForOption ("--category"), seed);
    else
        runner.runAllTests (seed);

    juce::Logger::setCurrentLogger (nullptr);

    for (int i = 0; i < runner.getNumResults(); ++i)
        if (runner.getResult(i)->failures > 0)
            return TEST_EXIT_FAILURE;

    return TEST_EXIT_SUCCESS;
}
