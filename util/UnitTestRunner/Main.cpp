#include <lemons_unit_testing/lemons_unit_testing.h>


int main (int argc, char **argv)
{
    juce::ArgumentList args (argc, argv);
    
    if (lemons::tests::executeUnitTests (args))
        return EXIT_SUCCESS;
    
    return EXIT_FAILURE;
}
