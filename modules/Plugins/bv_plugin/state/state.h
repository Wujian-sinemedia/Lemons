
#pragma once

namespace bav::plugin
{
struct Dimensions : SerializableData
{
    using SerializableData::SerializableData;

    void serialize (TreeReflector& ref) final;

    int width, height;
};

class State : public SerializableData
{
public:
    State (String pluginName);

    virtual ParameterList& getParameters() = 0;

    virtual void addTo (juce::AudioProcessor& processor);

    void setDimensions (int width, int height);


    ToggleParam mainBypass {"Main bypass", false};

    Dimensions dimensions;

protected:
    void serialize (TreeReflector& ref) override;
};

}  // namespace bav::plugin
