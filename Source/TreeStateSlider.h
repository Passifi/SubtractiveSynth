/*
  ==============================================================================

    TreeStateSlider.h
    Created: 7 Apr 2025 2:08:41pm
    Author:  Pascal

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
using juce::Slider, std::unique_ptr, std::make_unique;
typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
class TreeStateSlider {

public:
    TreeStateSlider(std::string name, juce::AudioProcessorValueTreeState& vts,Slider::SliderStyle style);
    Slider slider; 
    unique_ptr<SliderAttachment> sliderAttachment;
    std::string name; 

    void setName(std::string newName);



};