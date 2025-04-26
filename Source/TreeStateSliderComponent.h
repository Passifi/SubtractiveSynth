/*
  ==============================================================================

    TreeStateSliderComponent.h
    Created: 20 Apr 2025 11:40:29am
    Author:  Pascal

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class TreeStateSliderComponent  : public juce::Component
{
public:
    TreeStateSliderComponent();
    ~TreeStateSliderComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TreeStateSliderComponent)
};
