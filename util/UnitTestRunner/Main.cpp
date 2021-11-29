#include <lemons_unit_testing/lemons_unit_testing.h>


int main (int argc, char **argv)
{
    juce::ArgumentList args (argc, argv);
    
    if (lemons::tests::executeAllTests (args))
        return 0;
    
    return 1;
}
