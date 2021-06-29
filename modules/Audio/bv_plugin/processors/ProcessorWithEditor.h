#pragma once

namespace bav::dsp
{
template < class ProcessorType, class ComponentType, int width = 450, int height = 300, std::enable_if_t< std::is_base_of< ProcessorBase, ProcessorType >::value >* = nullptr >
struct ProcessorWithEditor : ProcessorType
{
    using ProcessorType::ProcessorType;

    bool hasEditor() const final { return true; }

    juce::AudioProcessorEditor* createEditor() final
    {
        return new gui::PluginEditor< ComponentType > (*this, {width, height}, this->getState());
    }
};

}  // namespace bav::dsp
