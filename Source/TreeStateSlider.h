/*
  ==============================================================================

    TreeStateSlider.h
    Created: 7 Apr 2025 2:08:41pm
    Author:  Pascal

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
using juce::Slider, std::unique_ptr, std::make_unique, juce::Label;
typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
class TreeStateSlider : public juce::Component {

public:
    TreeStateSlider(std::string name, juce::AudioProcessorValueTreeState& vts,Slider::SliderStyle style);
    Slider slider; 
    Label label;
    unique_ptr<SliderAttachment> sliderAttachment;
    std::string name; 
    void paint(juce::Graphics& g) override;
    void resized() override;


};