#pragma once

namespace lemons
{

class ArgParser final
{
public:

	void addArgument (const String&                       argOrFlags,
	                  bool                                required     = false,
	                  const String&                       defaultValue = "",
	                  std::function<void (const String&)> action       = nullptr);

	void printHelp();

	juce::StringPairArray parseArguments();

private:
};

}  // namespace lemons
