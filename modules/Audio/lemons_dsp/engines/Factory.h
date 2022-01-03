#pragma once


namespace lemons::dsp::factory
{

template <typename SampleType>
using EnginePtr = std::unique_ptr<Engine<SampleType>>;

template <typename SampleType>
using CreationFunc = std::function<EnginePtr<SampleType>()>;


template <typename SampleType>
void registerEngine (CreationFunc<SampleType>, const String& typeID);


struct EngineCreator
{
	explicit EngineCreator (const CreationFunc<float>&  floatFunc,
	                        const CreationFunc<double>& doubleFunc,
	                        const String&               typeID);

	virtual ~EngineCreator() = default;
};


template <template <typename SampleType> class EngineType, LEMONS_MUST_INHERIT_FROM (EngineType<float>, Engine<float>)>
struct DefaultEngineCreator : EngineCreator
{
	explicit DefaultEngineCreator (const String& typeIDtoUse)
	    : EngineCreator ([]()
	                     { return std::make_unique<EngineType<float>>(); },
	                     []()
	                     { return std::make_unique<EngineType<double>>(); },
	                     typeIDtoUse)
	{
	}
};

#define LEMONS_CREATE_DEFAULT_ENGINE_FACTORY(TemplateClassName, typeIDstring)                                     \
  struct TemplateClassName##_creator final : public lemons::dsp::factory::DefaultEngineCreator<TemplateClassName> \
  {                                                                                                               \
	explicit TemplateClassName##_creator()                                                                        \
		: DefaultEngineCreator<TemplateClassName> (typeIDstring)                                                  \
	{                                                                                                             \
	}                                                                                                             \
  };                                                                                                              \
  static const TemplateClassName##_creator TemplateClassName##_creation_object;


template <typename SampleType>
[[nodiscard]] EnginePtr<SampleType> createEngine (const String& typeID) noexcept;


using EngineList = juce::StringArray;

template <typename SampleType>
using EngineSet = juce::OwnedArray<Engine<SampleType>>;

template <typename SampleType>
[[nodiscard]] EngineSet<SampleType> getEngineSet (const EngineList& list);

}  // namespace lemons::dsp::factory
