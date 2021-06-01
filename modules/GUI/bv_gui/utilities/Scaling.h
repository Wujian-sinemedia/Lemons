#pragma once

namespace bav::gui
{

static inline juce::Rectangle<float> scaleRect (const juce::Rectangle<float>& source,
                                                const juce::Rectangle<float>& ratio)
{
    auto newWidth = source.getWidth() * ratio.getWidth();
    auto newHeight = source.getHeight() * ratio.getHeight();
    auto newX = source.getX() + (ratio.getX() * source.getWidth());
    auto newY = source.getY() + (ratio.getY() * source.getHeight());
    
    return {newX, newY, newWidth, newHeight};
}

static inline juce::AffineTransform getRectTransformFor (const juce::Rectangle<float>& source,
                                                         const juce::Rectangle<float>& target)
{
    auto newX = target.getX();
    auto newY = target.getY();
    
    auto scaleX = target.getWidth() / source.getWidth();
    auto scaleY = target.getHeight() / source.getHeight();
    
    return juce::AffineTransform::translation(-source.getX(), -source.getY())
    .scaled(scaleX, scaleY)
    .translated(newX, newY);
}

static inline void scale (juce::Component& component, const juce::Rectangle<float>& boundsRatio)
{
    if (boundsRatio.isEmpty())
        return;
    
    if (auto* parent = component.getParentComponent())
    {
        auto scaledBounds = scaleRect (parent->getLocalBounds().toFloat(),
                                       boundsRatio);
        
        auto compBounds = scaledBounds.getSmallestIntegerContainer();
        auto transform = getRectTransformFor (compBounds.toFloat(), scaledBounds);
        
        component.setTransform (juce::AffineTransform());
        component.setBounds (compBounds);
        component.setTransform (transform);
    }
}

}  // namespace
